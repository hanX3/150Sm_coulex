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

  coin_width = TimeWindow;

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

  if(!ReadTSOffsetData()){
    throw std::invalid_argument("can not read ts offset data.");
  }
  // PrintTSOffsetData();

  if(!ReadCaliData()){
    throw std::invalid_argument("can not read cali data.");
  }
  // PrintCaliData();

#ifdef OPENS3COR  
  if(!ReadS3CorData()){
    throw std::invalid_argument("can not read ts s3 cor data.");
  }
#else
  for(int i=1;i<=32;i++){
    map_s3_sector_cor[i] = {0, 1};
  }
  for(int i=1;i<=24;i++){
    map_s3_ring_cor[i] = {0, 1};
  }
#endif
  // PrintS3CorData();

#ifdef OPENS3ATT 
  if(!ReadS3AttData()){
    throw std::invalid_argument("can not read ts s3 att data.");
  }
#else
  for(int i=1;i<=32;i++){
    map_s3_sector_att[i] = 1.;
  }
  for(int i=1;i<=24;i++){
    map_s3_ring_att[i] = 1.;
  }
#endif
  // PrintS3AttData();

  if(!InitMapSectorRingID()){
    throw std::invalid_argument("can not init map for sector and ring id");
  }
  // PrintMapSectorRingID();

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
  GetS3FrontBackDataPrompt();
  
  benchmark->Show("build");
}

//
void build::GetS3FrontBackDataPrompt()
{
  std::cout << "start get s3 front and back data prompt" << std::endl;

  double cut_si_energy = CutSiEnergy;

  int n_max_s3_ring = nMaxS3Ring;
  int n_max_s3_sector = nMaxS3Sector;

  double t_win = coin_width;

  //
  std::map<Double_t, std::tuple<Short_t, Short_t, Short_t, Long64_t>> m_sector_hit_info;
  std::map<Double_t, std::tuple<Short_t, Short_t, Short_t, Long64_t>> m_ring_hit_info;

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
  ss << "coincidence window [-" << t_win << ", " << t_win << "] ns";
  TTree *tr = new TTree("tr_event", ss.str().c_str());

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
    energy = GetEnergy();
    ts_ns = GetTSns();
    id = GetID();
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
    energy = GetEnergy();
    ts_ns = GetTSns();
    id = GetID();
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

    // std::cout << std::endl;
    // std::cout << "info s3 " << sid1 << " " << ch1 << " " << evte1 << " " << ts1 << std::endl;

    while(true){//search backward
      if(i_current--==0) break;
      tr_in->GetEntry(i_current);
      energy = GetEnergy();
      ts_ns = GetTSns();
      id = GetID();
      if(cid==0 || (cid==1 && !(std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end())) || (cid==1 && (std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end()) && energy<=cut_si_energy)){
        continue;
      }

      id2 = id;
      sid2 = sid;
      ch2 = ch;
      evte2 = energy;
      ts2 = ts_ns;
    
      if(abs(ts2-ts1)<=t_win){//this coincidence
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
    } // while search backward

    while(true){//search forward
      if(i>=tr_in->GetEntries()) break;
      tr_in->GetEntry(i);
      energy = GetEnergy();
      ts_ns = GetTSns();
      id = GetID();
      if(cid==0 || (cid==1 && !(std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end())) || (cid==1 && (std::find(v_s3_sid.begin(), v_s3_sid.end(), sid)!=v_s3_sid.end()) && energy<=cut_si_energy)){
        i++;
        continue;
      }

      id2 = id;
      sid2 = sid;
      ch2 = ch;
      evte2 = energy;
      ts2 = ts_ns;
    
      if(abs(ts2-ts1)<=t_win){//this coincidence
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

    // reject some events
    // n_s3_ring==2 && n_s3_sector==1
    if(n_s3_ring==2 && n_s3_sector==1){
      if(std::abs(s3_ring_id[0]-s3_ring_id[1]) == 1){
        n_s3_ring = 1;
        if(s3_ring_energy[0]<s3_ring_energy[1]){
          s3_ring_id[0] = s3_ring_id[1];
          s3_ring_sid[0] = s3_ring_sid[1];
          s3_ring_ch[0] = s3_ring_ch[1];
        }
 
        s3_ring_energy[0] += s3_ring_energy[1];
      }
    }

    // n_s3_ring==1 && n_s3_sector==2
    if(n_s3_ring==1 && n_s3_sector==2){
      if(std::abs(s3_sector_id[0]-s3_sector_id[1])==1 || std::abs(s3_sector_id[0]-s3_sector_id[1])==31){
        n_s3_sector = 1;
        if(s3_sector_energy[0]<s3_sector_energy[1]){
          s3_sector_id[0] = s3_sector_id[1];
          s3_sector_sid[0] = s3_sector_sid[1];
          s3_sector_ch[0] = s3_sector_ch[1];
        }
 
        s3_sector_energy[0] += s3_sector_energy[1];
      }
    }
    
    // n_s3_ring==2 && n_s3_sector==2
    if(n_s3_ring==2 && n_s3_sector==2){
      for(int j=0;j<n_s3_sector;j++){
        m_sector_hit_info[s3_sector_energy[j]] = std::make_tuple(s3_sector_id[j], s3_sector_sid[j], s3_sector_ch[j], s3_sector_ts[j]);
      }
      for(int j=0;j<n_s3_ring;j++){
        m_ring_hit_info[s3_ring_energy[j]] = std::make_tuple(s3_ring_id[j], s3_ring_sid[j], s3_ring_ch[j], s3_ring_ts[j]);
      }
      
      auto it_sector = m_sector_hit_info.begin();
      s3_sector_energy[0] = it_sector->first;
      s3_sector_id[0] = std::get<0>(it_sector->second);
      s3_sector_sid[0] = std::get<1>(it_sector->second);
      s3_sector_ch[0] = std::get<2>(it_sector->second);
      s3_sector_ts[0] = std::get<3>(it_sector->second);
      it_sector++;
      s3_sector_energy[1] = it_sector->first;
      s3_sector_id[1] = std::get<0>(it_sector->second);
      s3_sector_sid[1] = std::get<1>(it_sector->second);
      s3_sector_ch[1] = std::get<2>(it_sector->second);
      s3_sector_ts[1] = std::get<3>(it_sector->second);
      
      auto it_ring = m_ring_hit_info.begin();
      s3_ring_energy[0] = it_ring->first;
      s3_ring_id[0] = std::get<0>(it_ring->second);
      s3_ring_sid[0] = std::get<1>(it_ring->second);
      s3_ring_ch[0] = std::get<2>(it_ring->second);
      s3_ring_ts[0] = std::get<3>(it_ring->second);
      it_ring++;
      s3_ring_energy[1] = it_ring->first;
      s3_ring_id[1] = std::get<0>(it_ring->second);
      s3_ring_sid[1] = std::get<1>(it_ring->second);
      s3_ring_ch[1] = std::get<2>(it_ring->second);
      s3_ring_ts[1] = std::get<3>(it_ring->second);
      
      m_sector_hit_info.clear();
      m_ring_hit_info.clear();
    }

    if((n_s3_ring==1 && n_s3_sector==1 && std::abs(s3_ring_energy[0]-s3_sector_energy[0])<CutS3EnergyDiff) || (n_s3_ring==2 && n_s3_sector==2 && std::abs(s3_ring_energy[0]-s3_sector_energy[0])<CutS3EnergyDiff && std::abs(s3_ring_energy[1]-s3_sector_energy[1])<CutS3EnergyDiff)){
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
      energy = GetEnergy();
      ts_ns = GetTSns();
      id = GetID();
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
Short_t build::GetID()
{
  Short_t id = -1;

  int key = 10000*cid+100*sid+ch;
  if(map_s3_sector_id.find(key) != map_s3_sector_id.end()){
    id = map_s3_sector_id[key];  
  }

  if(map_s3_ring_id.find(key) != map_s3_ring_id.end()){
    id = map_s3_ring_id[key];
  }

  return id;
}

//
Long64_t build::GetTSns()
{
  if(sr==250){
    return 8*ts + map_ts_offset[10000*cid+100*sid+ch];
  }

  if(sr==100){
    return 10*ts + map_ts_offset[10000*cid+100*sid+ch];
  }

  return -1;
}

//
double build::GetEnergy()
{
  int key = 10000*cid+100*sid+ch;

  auto it = map_cali.find(key);
  if(it==map_cali.end()){
    return 0;
  }

  double e = map_cali[key][0]+evte*map_cali[key][1]+evte*evte*map_cali[key][2]+rndm->Uniform(-5.,5.);

  // modified
  if(cid==1 && sid==11) e *= 10.;
  if((run>475&&run<616) || (run>=680&&run<=718)){
    if(sid==2){
      e *= 4.;
    }
  }
  if((run>=616&&run<=674)){
    if(sid==9){
      e *= 4.;
    }
  }

  // s3att
  if(map_s3_sector_id.find(key)!=map_s3_sector_id.end()){
    e *= map_s3_sector_att[map_s3_sector_id[key]];
  }
  if(map_s3_ring_id.find(key)!=map_s3_ring_id.end()){
    e *= map_s3_ring_att[map_s3_ring_id[key]];
  }

  // s3cor
  if(map_s3_sector_id.find(key)!=map_s3_sector_id.end()){
    e = map_s3_sector_cor[map_s3_sector_id[key]][0]+e*map_s3_sector_cor[map_s3_sector_id[key]][1];
  }
  if(map_s3_ring_id.find(key)!=map_s3_ring_id.end()){
    e = map_s3_ring_cor[map_s3_ring_id[key]][0]+e*map_s3_ring_cor[map_s3_ring_id[key]][1];
  }

  return e;
}

//
bool build::ReadCaliData()
{
  std::cout << "start read cali data" << std::endl;

  //
  std::ifstream fi;
  
  if(run==460 || (run>=472 && run<=605) || run==723){
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

    map_cali.insert(std::pair<int, std::vector<double>>(key, value));
  }

  fi.close();

  return 1;
}

//
void build::PrintCaliData()
{
  std::cout << "start print cali data" << std::endl;

  std::map<int, std::vector<double>>::iterator it = map_cali.begin();
  for(it=map_cali.begin();it!=map_cali.end();it++){
    std::cout << it->first << " => " << it->second[0] << " " << it->second[1] << " " << it->second[2] << '\n';
  }
}

//
bool build::ReadS3CorData() 
{
  int id;
  double par0, par1;

  std::string line;

  std::ifstream fi_fb_cor_sector(TString::Format("../pars/run_fb_cor/correction_sector_%04d.txt",run).Data());
  if(!fi_fb_cor_sector){
    std::cout << "can not open sector correction file." << std::endl;
    return 0;
  }else{
    std::getline(fi_fb_cor_sector, line);
    
    while(1){
      fi_fb_cor_sector >> id >> par0 >> par1;
      if(!fi_fb_cor_sector.good()) break;

      map_s3_sector_cor[id] = {par0, par1};
    }
    fi_fb_cor_sector.close();
  }

  std::ifstream fi_fb_cor_ring(TString::Format("../pars/run_fb_cor/correction_ring_%04d.txt",run).Data());
  if(!fi_fb_cor_ring){
    std::cout << "can not open ring correction file." << std::endl;
    return 0;
  }else{
    std::getline(fi_fb_cor_ring, line);
    
    while(1){
      fi_fb_cor_ring >> id >> par0 >> par1;
      if(!fi_fb_cor_ring.good()) break;

      map_s3_ring_cor[id] = {par0, par1};
    }
    fi_fb_cor_ring.close();
  }

  return 1;
}

//
void build::PrintS3CorData()
{
  std::cout << "\nstart print s3 cor data" << std::endl;

  std::cout << "\nfor sector ..." << std::endl;
  std::map<int, std::vector<double>>::iterator it_sector = map_s3_sector_cor.begin();
  for(;it_sector!=map_s3_sector_cor.end();it_sector++){
    std::cout << it_sector->first << " => " << it_sector->second[0] << " " << it_sector->second[1] << '\n';
  }

  std::cout << "\nfor ring ..." << std::endl;
  std::map<int, std::vector<double>>::iterator it_ring = map_s3_ring_cor.begin();
  for(;it_ring!=map_s3_ring_cor.end();it_ring++){
    std::cout << it_ring->first << " => " << it_ring->second[0] << " " << it_ring->second[1] << '\n';
  }
}

//
bool build::ReadS3AttData() 
{
  int r;
  double par;

  bool flag = 0;

  std::ifstream fi_sector;
  for(int i=0;i<32;i++){
    flag = 0;
    fi_sector.open(Form("../pars/run_s3_att/sector%02d.txt",i+1));
    if(!fi_sector){
      std::cout << "can not open sector att file." << std::endl;
      return 0;
    }else{
      while(fi_sector >> r >> par){
        if(r==run){
          map_s3_sector_att[i+1] = par;
          flag = 1;
          break;
        }else continue;
      }
      fi_sector.close();
    }
  }
  if(!flag){
    std::cout << "can not find run " << run << " s3 att data." << std::endl;
    return 0;
  }

  std::ifstream fi_ring;
  for(int i=0;i<24;i++){
    flag = 0;
    fi_ring.open(Form("../pars/run_s3_att/ring%02d.txt",i+1));
    if(!fi_ring){
      std::cout << "can not open ring att file." << std::endl;
      return 0;
    }else{
      while(fi_ring >> r >> par){
        if(r==run){
          map_s3_ring_att[i+1] = par;
          flag = 1;
          break;
        }else continue;
      }
      fi_ring.close();
    }
  }
  if(!flag){
    std::cout << "can not find run " << run << " s3 att data." << std::endl;
    return 0;
  }


  return 1;
}

//
void build::PrintS3AttData()
{
  std::cout << "\nstart print s3 att data" << std::endl;

  std::cout << "\n run number " << run << " for sector ..." << std::endl;
  std::map<int, double>::iterator it_sector = map_s3_sector_att.begin();
  for(;it_sector!=map_s3_sector_att.end();it_sector++){
    std::cout << it_sector->first << " => " << it_sector->second << '\n';
  }

  std::cout << "\n run number " << run << " for ring ..." << std::endl;
  std::map<int, double>::iterator it_ring = map_s3_ring_att.begin();
  for(;it_ring!=map_s3_ring_att.end();it_ring++){
    std::cout << it_ring->first << " => " << it_ring->second << '\n';
  }
}

//
bool build::ReadTSOffsetData()
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
void build::PrintTSOffsetData()
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
    map_s3_sector_id = {
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
    map_s3_sector_id = {
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
 
  map_s3_ring_id = {
    {11102, 1},
    {11103, 3},
    {11104, 5},
    {11105, 7},
    {11106, 9},
    {11107, 11},
    {11108, 13},
    {11109, 15},
    {11110, 17},
    {11111, 19},
    {11112, 21},
    {11113, 23},
    {11200, 2},
    {11201, 4},
    {11202, 6},
    {11203, 8},
    {11204, 10},
    {11205, 12},
    {11206, 14},
    {11207, 16},
    {11208, 18},
    {11209, 20},
    {11210, 22},
    {11211, 24}
  };

  return 1;
}

//
void build::PrintMapSectorRingID()
{
  std::cout << "start print ring and sector id." << std::endl;

  for(auto &it:map_s3_sector_id){
    std::cout << it.first << " => " << it.second << std::endl;
  }
  std::cout << std::endl;

  for(auto &it:map_s3_ring_id){
    std::cout << it.first << " => " << it.second << std::endl;
  }
}
