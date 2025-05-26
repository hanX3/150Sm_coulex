#include "sort.h"

#include <vector>
#include "math.h"

//
sort::sort(const std::string &filename_in, const std::string &filename_out, const int r)
{
  benchmark = new TBenchmark;

  run = r;

  if(!InitMapSectorRingID()){
    throw std::invalid_argument("can not init ring and sector id data.");
  }
  PrintMapSectorRingID();

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "open file " << filename_in.c_str() << " error!" << std::endl;
    delete file_in;

    return ;
  }
  tr_in = (TTree*)file_in->Get("tree");
  rd = new raw(tr_in); 

  //
  if(!ReadCaliData()){
    throw std::invalid_argument("can not read cali data.");
  }
  PrintCaliData();
  ReadS3CorData();
  PrintS3CorData();

  // read ts offset
  if(!ReadTSOffset()){
    throw std::invalid_argument("can not read ts offset data.");
  }
  PrintTSOffset();

  rndm = new TRandom3((Long64_t)time(0));
  
  //
  file_out = TFile::Open(filename_out.c_str(), "recreate");

  tr_out = new TTree("tree", "GDDAQ cali and sort data");
  tr_out->Branch("cid", &sd.cid);
  tr_out->Branch("sid", &sd.sid);
  tr_out->Branch("ch", &sd.ch);
  tr_out->Branch("evte", &sd.evte);
  tr_out->Branch("ts", &sd.ts);
}

//
sort::~sort()
{
  file_in->Close();
  file_out->Close();
}

//
void sort::Process()
{
  std::cout << "process " << std::endl;

  benchmark->Start("process");

  rd->GetEntry(rd->GetEntries()-1);
  GetTSns();
  std::cout << "total time " << sd.ts/1000000000 << "s" << std::endl; 

  Long64_t ts_save = TIMEBUFFER;
  Long64_t key;
  for(Long64_t i=0;i<rd->GetEntries();++i){
    rd->GetEntry(i);
    sd(rd->cid, rd->sid, rd->ch, 0., 0);

    if(map_cali_data.find(10000*rd->cid+100*rd->sid+rd->ch) == map_cali_data.end()){
      std::cout << "can not find cali data for this channel." << std::endl;
      std::cout << "cid " << rd->cid << " sid " << rd->sid << " ch " << rd->ch << std::endl;
      break;
    }
    
    GetEnergy();
    GetTSns();

    key = ((((((sd.ts)<<4)+sd.cid)<<4)+sd.sid)<<6)+sd.ch;
    map_sort_data.insert(std::make_pair(key, sd));

    if(sd.ts>ts_save){
      Long64_t length = map_sort_data.size() * 0.9;
      for(Long64_t j=0;j<length;++j){
        it_sd = map_sort_data.begin();
        sd = it_sd->second;

        file_out->cd();
        tr_out->Fill();

        map_sort_data.erase(it_sd);
      }
    
      ts_save += TIMEBUFFER;
      std::cout << "\r" << sd.ts/1000000000 << "\r";
      std::cout << std::flush;
    }
  }

  int l = map_sort_data.size();
  for(int i=0;i<l;++i){
    it_sd = map_sort_data.begin();
    sd = it_sd->second;

    file_out->cd();
    tr_out->Fill();

    map_sort_data.erase(it_sd);
  }
  std::cout << "\r" << sd.ts/1000000000 << "\n";

  file_out->cd();
  tr_out->Write();
  file_out->Close();
  
  benchmark->Show("process");
}

//
void sort::GetTSns()
{
  if(rd->cid==0){
    if(rd->cfdft){
      sd.ts = rd->ts*8;
    }else{
      sd.ts = (rd->ts*2-rd->cfds+rd->cfd/16384.)*4.;
    }
  }

  if(rd->cid==1){
    if(rd->sr==250) sd.ts = rd->ts*8;
    if(rd->sr==100) sd.ts = rd->ts*10; 
  }

  int key = 10000*rd->cid+100*rd->sid+rd->ch;
  sd.ts += map_ts_offset[key];
}

//
void sort::GetEnergy()
{
  int key = 10000*rd->cid+100*rd->sid+rd->ch;

  if(rd->cid==0){
    sd.evte = map_cali_data[key][0]+rd->evte*map_cali_data[key][1]+rd->evte*rd->evte*map_cali_data[key][2]+rndm->Uniform(-0.5,0.5);
    sd.evte *= map_k_data[key];
  }

  if(rd->cid==1){
    sd.evte = map_cali_data[key][0]+rd->evte*map_cali_data[key][1]+rd->evte*rd->evte*map_cali_data[key][2]+rndm->Uniform(-5.,5.);

    if(map_s3_sector_id.find(key)!=map_s3_sector_id.end()){
      sd.evte = map_s3_sector_cor_data[map_s3_sector_id[key]][0]+sd.evte*map_s3_sector_cor_data[map_s3_sector_id[key]][1];
    }
    if(map_s3_ring_id.find(key)!=map_s3_ring_id.end()){
      sd.evte = map_s3_ring_cor_data[map_s3_ring_id[key]][0]+sd.evte*map_s3_ring_cor_data[map_s3_ring_id[key]][1];
    }

    if((run>475&&run<616) || (run>=680&&run<=718)){
      if(rd->sid==2){
        sd.evte *= 4.;
      } 
    }
    if((run>=616&&run<=674)){
      if(rd->sid==9){
        sd.evte *= 4.;
      } 
    }
  }
}

//
bool sort::ReadCaliData()
{
  std::cout << "start read cali data" << std::endl;

  //
  std::ifstream fi_cali_ge;
  fi_cali_ge.open("../pars/cali_ge.txt");
  if(!fi_cali_ge){
    std::cout << "can not open cali.txt" << std::endl;
    return 0;
  }

  std::string line;
  std::getline(fi_cali_ge, line);

  int cid, sid, ch;
  double par0, par1, par2, chi2;
  int key = 0;

  while(1){
    fi_cali_ge >> cid >> sid >> ch >> par0 >> par1 >> par2 >> chi2;
    if(!fi_cali_ge.good()) break;

    key = 10000*cid+100*sid+ch; // ch from 0 to 15
    std::vector<double> value;
    value.push_back(par0);
    value.push_back(par1);
    value.push_back(par2);
    value.push_back(chi2);

    map_cali_data.insert(std::pair<int, std::vector<double>>(key, value));
  }

  fi_cali_ge.close();

  //
  std::ifstream fi_k;
  fi_k.open(TString::Format("../pars/run_k/k_%04d.txt", run).Data());
  if(!fi_k){
    std::cout << "can not open k data" << std::endl;
    for(int i=2;i<=5;i++){
      for(int j=0;j<16;j++){
        map_k_data.insert(std::pair<int, double>(100*i+j, 1.));
      }
    }
  }else{
    std::string line;
    std::getline(fi_k, line);

    double k;

    while(1){
      fi_k >> cid >> sid >> ch >> k;
      if(!fi_k.good()) break;

      key = 10000*cid+100*sid+ch; // ch from 0 to 15

      map_k_data.insert(std::pair<int, double>(key, k));
    }

    fi_k.close();
  }

  //
  std::ifstream fi_cali_si;
  if(run==377 || run==460 || (run>=472 && run<=605)){
    fi_cali_si.open("../pars/cali_si_r460.txt");
  }else if((run>=616 && run<=674) || run==722){
    fi_cali_si.open("../pars/cali_si_r457_low_current.txt");
  }else if(run==457 || (run>=680 && run<=718)){
    fi_cali_si.open("../pars/cali_si_r457_high_current.txt");
  }else{
    std::cout << "wrong run number." << std::endl;
    return 0;
  }

  if(!fi_cali_si){
    std::cout << "can not open si cali.txt" << std::endl;
    return 0;
  }

  std::getline(fi_cali_si, line);

  while(1){
    fi_cali_si >> cid >> sid >> ch >> par0 >> par1 >> par2 >> chi2;
    if(!fi_cali_si.good()) break;

    key = 10000*cid+100*sid+ch; // ch from 0 to 15
    std::vector<double> value;
    value.push_back(par0);
    value.push_back(par1);
    value.push_back(par2);
    value.push_back(chi2);

    map_cali_data.insert(std::pair<int, std::vector<double>>(key, value));
  }

  fi_cali_si.close();

  return 1;
}

//
void sort::PrintCaliData()
{
  std::cout << "start print cali data" << std::endl;

  std::map<int, std::vector<double>>::iterator it_cali = map_cali_data.begin();
  for(;it_cali!=map_cali_data.end();it_cali++){
    std::cout << it_cali->first << " => " << it_cali->second[0] << " " << it_cali->second[1] << " " << it_cali->second[2] << '\n';
  }

  std::map<int, double>::iterator it_k = map_k_data.begin();
  for(;it_k!=map_k_data.end();it_k++){
    std::cout << it_k->first << " => " << it_k->second<< '\n';
  }
}

//
void sort::ReadS3CorData() 
{
  int id;
  double par0, par1;

  std::string line;

  std::ifstream fi_fb_cor_sector(TString::Format("../pars/run_fb_cor/correction_sector_%04d.txt",run).Data());
  if(!fi_fb_cor_sector){
    std::cout << "can not open sector correction file." << std::endl;
    for(int i=1;i<=32;i++){
      map_s3_sector_cor_data[i] = {0, 1};
    }
  }else{
    std::getline(fi_fb_cor_sector, line);
    
    while(1){
      fi_fb_cor_sector >> id >> par0 >> par1;
      if(!fi_fb_cor_sector.good()) break;

      map_s3_sector_cor_data[id] = {par0, par1};
    }
    fi_fb_cor_sector.close();
  }

  std::ifstream fi_fb_cor_ring(TString::Format("../pars/run_fb_cor/correction_ring_%04d.txt",run).Data());
  if(!fi_fb_cor_ring){
    std::cout << "can not open ring correction file." << std::endl;
    for(int i=1;i<=24;i++){
      map_s3_ring_cor_data[i] = {0, 1};
    }
  }else{
    std::getline(fi_fb_cor_ring, line);
    
    while(1){
      fi_fb_cor_ring >> id >> par0 >> par1;
      if(!fi_fb_cor_ring.good()) break;

      map_s3_ring_cor_data[id] = {par0, par1};
    }
    fi_fb_cor_ring.close();
  }
}

//
void sort::PrintS3CorData()
{
  std::cout << "\nstart print s3 cor data" << std::endl;

  std::cout << "\nfor sector ..." << std::endl;
  std::map<int, std::vector<double>>::iterator it_sector = map_s3_sector_cor_data.begin();
  for(;it_sector!=map_s3_sector_cor_data.end();it_sector++){
    std::cout << it_sector->first << " => " << it_sector->second[0] << " " << it_sector->second[1] << '\n';
  }

  std::cout << "\nfor ring ..." << std::endl;
  std::map<int, std::vector<double>>::iterator it_ring = map_s3_ring_cor_data.begin();
  for(;it_ring!=map_s3_ring_cor_data.end();it_ring++){
    std::cout << it_ring->first << " => " << it_ring->second[0] << " " << it_ring->second[1] << '\n';
  }
}

//
bool sort::ReadTSOffset()
{
  std::cout << "start read ts offset data" << std::endl;

  std::ifstream fi;
  if(run==377 || (run>=457 && run<=462) || (run>=472 && run<=605)){
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
void sort::PrintTSOffset()
{
  std::cout << "start print ts offset data" << std::endl;

  std::map<int, double>::iterator it = map_ts_offset.begin();
  for(;it!=map_ts_offset.end();it++){
    std::cout << it->first << " => " << it->second << '\n';
  }
}

//
bool sort::InitMapSectorRingID()
{
  std::cout << "start init ring and sector id." << std::endl;

  if(run==377 || (run>=457 && run<=462) || (run>=472 && run<=605) || (run>=680 && run<=718)){
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
void sort::PrintMapSectorRingID()
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

