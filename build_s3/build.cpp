#include "build.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>

//
build::build(const std::string &filename_in, const std::string &filename_out, int r)
{
  benchmark = new TBenchmark;

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "open file " << filename_in << " error!" << std::endl;
    delete file_in;

    return ;
  }

  tr_in = (TTree*)file_in->Get("tree");
  tr_in->SetBranchAddress("sr", &sr);
  tr_in->SetBranchAddress("cid", &cid);
  tr_in->SetBranchAddress("sid", &sid);
  tr_in->SetBranchAddress("ch", &ch);
  tr_in->SetBranchAddress("evte", &evte);
  tr_in->SetBranchAddress("ts", &ts);

  //
  run = r;
  if((run>=457 && run<=462) || (run>=472 && run<=605) || (run>=680 && run<=718)){
    v_s3_sid = {9, 10, 11, 12};
  }else if((run>=616 && run<=674) || (run>=721 && run<=723)){
    v_s3_sid = {2, 3, 11, 12};
  }else{
    throw std::invalid_argument("wrong run number.");
  }

  if(!ReadTSOffset()){
    throw std::invalid_argument("can not read ts offset data.");
  }
  PrintTSOffset();

  if(!ReadCaliData()){
    throw std::invalid_argument("can not read cali data.");
  }
  PrintCaliData();

  if(!InitMapSectorRingID()){
    throw std::invalid_argument("can not init map for sector and ring id");
  }
  PrintMapSectorRingID();

  rndm = new TRandom3((Long64_t)time(0));
  file_out = TFile::Open(filename_out.c_str(), "recreate");
}

//
build::~build()
{
  delete file_in;
  delete file_out;
}

//
void build::Process()
{
  benchmark->Start("build");

  //
  double time_window = TimeWindow;
  //
  GetSi3FrontBackData("tr_s3", 0, time_window);
  
  benchmark->Show("build");
}

//
void build::GetSi3FrontBackData(TString tr_name, double abs_time1, double abs_time2)
{
  std::cout << "start get s3 front and back data" << std::endl;

  double cut_si_energy = CutSiEnergy;

  int n_max_s3_ring = nMaxS3Ring;
  int n_max_s3_sector = nMaxS3Sector;

  Int_t n_s3_sector = 0;
  Short_t s3_sector_sid[n_max_s3_sector];
  Short_t s3_sector_ch[n_max_s3_sector];
  Short_t s3_sector_id[n_max_s3_sector];
  Double_t s3_sector_energy[n_max_s3_sector];
  Long64_t s3_sector_ts[n_max_s3_sector];
  Int_t n_s3_ring = 0;
  Short_t s3_ring_sid[n_max_s3_ring];
  Short_t s3_ring_ch[n_max_s3_ring];
  Short_t s3_ring_id[n_max_s3_ring];
  Double_t s3_ring_energy[n_max_s3_ring];
  Long64_t s3_ring_ts[n_max_s3_ring];

  std::stringstream ss;
  ss.str("");
  ss << "timewindow_" << abs_time2-abs_time1;
  TTree *tr = new TTree(tr_name.Data(), ss.str().c_str());

  memset(s3_sector_sid, 0, sizeof(s3_sector_sid));
  memset(s3_sector_ch, 0, sizeof(s3_sector_ch));
  memset(s3_sector_id, 0, sizeof(s3_sector_id));
  memset(s3_sector_energy, 0, sizeof(s3_sector_energy));
  memset(s3_sector_ts, 0, sizeof(s3_sector_ts));

  memset(s3_ring_sid, 0, sizeof(s3_ring_sid));
  memset(s3_ring_ch, 0, sizeof(s3_ring_ch));
  memset(s3_ring_id, 0, sizeof(s3_ring_id));
  memset(s3_ring_energy, 0, sizeof(s3_ring_energy));
  memset(s3_ring_ts, 0, sizeof(s3_ring_ts));

  tr->Branch("n_s3_sector", &n_s3_sector, "n_s3_sector/I");
  tr->Branch("s3_sector_sid", s3_sector_sid, "s3_sector_sid[n_s3_sector]/S");
  tr->Branch("s3_sector_ch", s3_sector_ch, "s3_sector_ch[n_s3_sector]/S");
  tr->Branch("s3_sector_id", s3_sector_id, "s3_sector_id[n_s3_sector]/S");
  tr->Branch("s3_sector_energy", s3_sector_energy, "s3_sector_energy[n_s3_sector]/D");
  tr->Branch("s3_sector_ts", s3_sector_ts, "s3_sector_ts[n_s3_sector]/L");

  tr->Branch("n_s3_ring", &n_s3_ring, "n_s3_ring/I");
  tr->Branch("s3_ring_sid", s3_ring_sid, "s3_ring_sid[n_s3_ring]/S");
  tr->Branch("s3_ring_ch", s3_ring_ch, "s3_ring_ch[n_s3_ring]/S");
  tr->Branch("s3_ring_id", s3_ring_id, "s3_ring_id[n_s3_ring]/S");
  tr->Branch("s3_ring_energy", s3_ring_energy, "s3_ring_energy[n_s3_ring]/D");
  tr->Branch("s3_ring_ts", s3_ring_ts, "s3_ring_ts[n_s3_ring]/L");
  
  Long64_t i_start = 0;

  Short_t sid1 = 0;
  Short_t ch1 = 0;
  Short_t id1 = 0;
  Double_t evte1 = 0;
  Long64_t ts1 = 0;
  
  Short_t sid2 = 0;
  Short_t ch2 = 0;
  Short_t id2 = 0;
  Double_t evte2 = 0;
  Long64_t ts2 = 0;

  Short_t id;
  Double_t energy;
  Long64_t ts_ns;

  while(true){//get first s3 data
    tr_in->GetEntry(i_start);
    energy = CaliEnergy(evte, cid, sid, ch);
    ts_ns = GetTSns(ts, sr, cid, sid, ch);
    id = GetID(cid, sid, ch);
    i_start++;
    if(cid==1 && (std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end()) && energy>cut_si_energy){
      break;
    }
  }

  Long64_t n_evt = 0;
  Long64_t i = i_start;
  Long64_t i_current = i-1;
  while(true){
    if(i==tr_in->GetEntries()) break;

    tr_in->GetEntry(i_current);
    energy = CaliEnergy(evte, cid, sid, ch);
    ts_ns = GetTSns(ts, sr, cid, sid, ch);
    id = GetID(cid, sid, ch);
    if(cid==0 || (cid==1 && !(std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end())) || (cid==1 && (std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end()) && energy<=cut_si_energy)){
      i++;
      continue;
    }

    id1 = id;
    sid1 = sid;
    ch1 = ch;
    evte1 = energy;
    ts1 = ts_ns;

    if(sid==v_s3_sid[0] || sid==v_s3_sid[1]){ // s3 sector
      s3_sector_id[n_s3_sector] = id1; 
      s3_sector_sid[n_s3_sector] = sid1; 
      s3_sector_ch[n_s3_sector] = ch1; 
      s3_sector_energy[n_s3_sector] = evte1; 
      s3_sector_ts[n_s3_sector] = ts1; 
      n_s3_sector++;
    }

    if(sid==v_s3_sid[2] || sid==v_s3_sid[3]){ // s3 ring
      s3_ring_id[n_s3_ring] = id1; 
      s3_ring_sid[n_s3_ring] = sid1; 
      s3_ring_ch[n_s3_ring] = ch1; 
      s3_ring_energy[n_s3_ring] = evte1; 
      s3_ring_ts[n_s3_ring] = ts1; 
      n_s3_ring++;
    }

    while(true){//search forward
      if(i>=tr_in->GetEntries()) break;
      tr_in->GetEntry(i);
      energy = CaliEnergy(evte, cid, sid, ch);
      ts_ns = GetTSns(ts, sr, cid, sid, ch);
      id = GetID(cid, sid, ch);
      if(cid==0 || (cid==1 && !(std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end())) || (cid==1 && (std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end()) && energy<=cut_si_energy)){
        i++;
        continue;
      }

      id2 = id;
      sid2 = sid;
      ch2 = ch;
      evte2 = energy;
      ts2 = ts_ns;
    
      if((abs(ts2-ts1))>abs_time1 && abs(ts2-ts1)<abs_time2){//this coincidence
        i++;
        if(sid==v_s3_sid[0] || sid==v_s3_sid[1]){ // s3 sector
          s3_sector_id[n_s3_sector] = id2; 
          s3_sector_sid[n_s3_sector] = sid2; 
          s3_sector_ch[n_s3_sector] = ch2; 
          s3_sector_energy[n_s3_sector] = evte2; 
          s3_sector_ts[n_s3_sector] = ts2; 
          n_s3_sector++;
        }

        if(sid==v_s3_sid[2] || sid==v_s3_sid[3]){ // s3 ring
          s3_ring_id[n_s3_ring] = id2; 
          s3_ring_sid[n_s3_ring] = sid2; 
          s3_ring_ch[n_s3_ring] = ch2; 
          s3_ring_energy[n_s3_ring] = evte2; 
          s3_ring_ts[n_s3_ring] = ts2; 
          n_s3_ring++;
        }

      }else break;
    } // while search forward

    // std::cout << "n_s3_ring " << n_s3_ring << " n_s3_sector " << n_s3_sector << std::endl;
    if(n_s3_ring+n_s3_sector > 0){
      n_evt++;

      file_out->cd();
      tr->Fill();

      if(n_evt%1000==0){
        std::cout << "\r" << n_evt << "  " << i << "/" << tr_in->GetEntries();
        std::cout << std::flush;
      }
    }

    n_s3_sector = 0;
    memset(s3_sector_id, 0, sizeof(s3_sector_id));
    memset(s3_sector_sid, 0, sizeof(s3_sector_sid));
    memset(s3_sector_ch, 0, sizeof(s3_sector_ch));
    memset(s3_sector_energy, 0, sizeof(s3_sector_energy));
    memset(s3_sector_ts, 0, sizeof(s3_sector_ts));

    n_s3_ring = 0;
    memset(s3_ring_id, 0, sizeof(s3_ring_id));
    memset(s3_ring_sid, 0, sizeof(s3_ring_sid));
    memset(s3_ring_ch, 0, sizeof(s3_ring_ch));
    memset(s3_ring_energy, 0, sizeof(s3_ring_energy));
    memset(s3_ring_ts, 0, sizeof(s3_ring_ts));

    while(true){//get next s3 data
      if(i>=tr_in->GetEntries()) break;
      tr_in->GetEntry(i);
      energy = CaliEnergy(evte, cid, sid, ch);
      ts_ns = GetTSns(ts, sr, cid, sid, ch);
      id = GetID(cid, sid, ch);
      i++;
      if(cid==1 && (std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end()) && energy>cut_si_energy){
        break;
      }
    }
    i_current = i-1;
  }//while

  std::cout << std::endl;

  file_out->cd();
  tr->Write();
}

//
Short_t build::GetID(int crate, int slot, int channel)
{
  Short_t id = -1;

  int key = 10000*crate+100*slot+channel;
  if(map_sector_id.find(key) != map_sector_id.end()){
    id = map_sector_id[key];  
  }

  if(map_ring_id.find(key) != map_ring_id.end()){
    id = map_ring_id[key];
  }

  return id;
}

//
Long64_t build::GetTSns(Long64_t ts, int sr, int crate, int slot, int channel)
{
  if(sr==250){
    return 8*ts + map_ts_offset[10000*crate+100*slot+channel];
  }

  if(sr==100){
    return 10*ts + map_ts_offset[10000*crate+100*slot+channel];
  }

  return -1;
}

//
double build::CaliEnergy(int adc, int crate, int slot, int channel)
{
  int key = 10000*crate + 100*slot + channel;

  auto it = map_cali_data.find(key);
  if(it==map_cali_data.end()){
    return 0;
  }

  double adcc = map_cali_data[key][0] + adc*map_cali_data[key][1] + adc*adc*map_cali_data[key][2] + rndm->Uniform(-0.5, 0.5);
  return adcc;
}

//
bool build::ReadCaliData()
{
  std::cout << "start read cali data" << std::endl;

  //
  std::ifstream fi;
  
  if(run==460 || (run>=472 && run<=605)){
    fi.open("../pars/cali_si_r460.txt");
  }else if((run>=616 && run<=674) || run==722){
    fi.open("../pars/cali_si_r457_low_current.txt");
  }else if(run==457 || (run>=680 && run<=718)){
    fi.open("../pars/cali_si_r457_high_current.txt");
  }else{
    std::cout << "wrong run number." << std::endl;
    return 0;
  }
  
  if(!fi){
    std::cout << "can not open s3 cali data" << std::endl;
    return 0;
  }

  std::string line;
  std::getline(fi, line);

  int cid, sid, ch;
  double par0, par1, par2, chi2;
  int key = 0;

  while(1){
    fi >> cid >> sid >> ch >> par0 >> par1 >> par2 >> chi2;
    if(!fi.good()) break;

    key = 10000*cid+100*sid+ch; // ch from 0 to 15
    std::vector<double> value;
    value.push_back(par0);
    value.push_back(par1);
    value.push_back(par2);

    map_cali_data.insert(std::pair<int, std::vector<double>>(key, value));
  }

  fi.close();

  return 1;
}

//
void build::PrintCaliData()
{
  std::cout << "start print cali data" << std::endl;

  std::map<int, std::vector<double>>::iterator it = map_cali_data.begin();
  for(it=map_cali_data.begin();it!=map_cali_data.end();it++){
    std::cout << it->first << " => " << it->second[0] << " " << it->second[1] << " " << it->second[2] << '\n';
  }
}

//
bool build::ReadTSOffset()
{
  std::cout << "start read ts offset data" << std::endl;

  std::ifstream fi;
  if((run>=457 && run<=462) || (run>=472 && run<=605)){
    fi.open("../pars/ts_offset_r472_r605.txt");
  }else if((run>=616 && run<=674) || (run>=721 && run<=723)){
    fi.open("../pars/ts_offset_r616_r674.txt");
  }else if(run>=680 && run<=718){
    fi.open("../pars/ts_offset_r680_r718.txt");
  }else{
    std::cout << "wrong run number." << std::endl;
    return 0;
  }
  
  if(!fi){
    std::cout << "can not open ts offset data" << std::endl;
    return 0;
  }

  std::string line;
  std::getline(fi, line);

  int cid, sid, ch;
  double ts_offset, chi2;
  int key = 0;

  while(1){
    fi >> cid >> sid >> ch >> ts_offset >> chi2;
    if(!fi.good()) break;

    key = 10000*cid+100*sid+ch; // ch from 0 to 15

    map_ts_offset.insert(std::pair<int, double>(key, ts_offset));
  }

  fi.close();

  return 1;
}

//
void build::PrintTSOffset()
{
  std::cout << "start print ts offset data" << std::endl;

  std::map<int, double>::iterator it = map_ts_offset.begin();
  for(it=map_ts_offset.begin();it!=map_ts_offset.end();it++){
    std::cout << it->first << " => " << it->second << '\n';
  }
}

//
void build::SaveFile()
{
  file_out->cd();
  file_out->Close();
}

//
bool build::InitMapSectorRingID()
{
  std::cout << "start init ring and sector id." << std::endl;
  
  if((run>=457 && run<=462) || (run>=472 && run<=605) || (run>=680 && run<=718)){
    map_sector_id = {
      {10900, 10},
      {10901, 12},
      {10902, 14},
      {10903, 16},
      {10904, 18},
      {10905, 20},
      {10906, 22},
      {10907, 24},
      {10908, 26},
      {10909, 28},
      {10910, 30},
      {10911, 32},
      {10912, 2},
      {10913, 4},
      {10914, 6},
      {10915, 8},
      {11000, 11},
      {11001, 13},
      {11002, 15},
      {11003, 17},
      {11004, 19},
      {11005, 21},
      {11006, 23},
      {11007, 25},
      {11008, 27},
      {11009, 29},
      {11010, 31},
      {11011, 1},
      {11012, 3},
      {11013, 5},
      {11014, 7},
      {11015, 9}
    };
  }else if((run>=616 && run<=674) || (run>=721 && run<=723)){
    map_sector_id = {
      {10200, 10},
      {10201, 12},
      {10202, 14},
      {10203, 16},
      {10204, 18},
      {10205, 20},
      {10206, 22},
      {10207, 24},
      {10208, 26},
      {10209, 28},
      {10210, 30},
      {10211, 32},
      {10212, 2},
      {10213, 4},
      {10214, 6},
      {10215, 8},
      {10300, 11},
      {10301, 13},
      {10302, 15},
      {10303, 17},
      {10304, 19},
      {10305, 21},
      {10306, 23},
      {10307, 25},
      {10308, 27},
      {10309, 29},
      {10310, 31},
      {10311, 1},
      {10312, 3},
      {10313, 5},
      {10314, 7},
      {10315, 9}
    };
  }else{
  std::cout << "wrong run number." << std::endl;
    return 0;
  }
 
  map_ring_id = {
    {11102, 2},
    {11103, 4},
    {11104, 6},
    {11105, 8},
    {11106, 10},
    {11107, 12},
    {11108, 14},
    {11109, 16},
    {11110, 18},
    {11111, 20},
    {11112, 22},
    {11113, 24},
    {11200, 1},
    {11201, 3},
    {11202, 5},
    {11203, 7},
    {11204, 9},
    {11205, 11},
    {11206, 13},
    {11207, 15},
    {11208, 17},
    {11209, 19},
    {11210, 21},
    {11211, 23}
  };

  return 1;
}

//
void build::PrintMapSectorRingID()
{
  std::cout << "start print ring and sector id." << std::endl;

  for(auto &it:map_sector_id){
    std::cout << it.first << " => " << it.second << std::endl;
  }
  std::cout << std::endl;

  for(auto &it:map_ring_id){
    std::cout << it.first << " => " << it.second << std::endl;
  }
}
