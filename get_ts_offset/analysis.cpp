#include "analysis.h"

#include <vector>
#include "math.h"

//
analysis::analysis(const std::string &filename_in, const std::string &filename_out, const int r)
{
  benchmark = new TBenchmark;

  run = r;
  if(!InitMapRingSector()){
    throw std::invalid_argument("can not init ring and sector id data.");
  }
  

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

  rndm = new TRandom3((Long64_t)time(0));
  
  InitMapTS();
  //
  file_out = TFile::Open(filename_out.c_str(), "recreate");

  // time differents Ge
  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      h1[(i-2)*16+j] = new TH1D(TString::Format("ge_sid%d_ch%02d",i,j).Data(), "", 2*ALIGNMENTWINDOW/BINWIDTH, -ALIGNMENTWINDOW, ALIGNMENTWINDOW);
    }
  }

  // time differents Si
  for(int i=2;i<=12;i++){
    for(int j=0;j<16;j++){
      h2[(i-2)*16+j] = new TH1D(TString::Format("si_sid%d_ch%02d",i,j).Data(), "", 2*ALIGNMENTWINDOW/BINWIDTH, -ALIGNMENTWINDOW, ALIGNMENTWINDOW);
    }
  }
}

//
analysis::~analysis()
{
  file_in->Close();
  file_out->Close();
}

//
void analysis::InitMapTS()
{
  std::cout << "init " << std::endl;
  Long64_t ts_ns;
  int key = 0;

  for(Long64_t i=0;i<rd->GetEntries();++i){
    rd->GetEntry(i);
    key = 10000*rd->cid+100*rd->sid+rd->ch;

    if(rd->cid==0){
      rd->evte = map_cali_data[key][0] + rd->evte*map_cali_data[key][1] + rd->evte*rd->evte*map_cali_data[key][2] + rndm->Uniform(0., 0.5);
      rd->evte *= map_k_data[key];
    }

    if(rd->cid==1){
      rd->evte = map_cali_data[key][0] + rd->evte*map_cali_data[key][1] + rd->evte*rd->evte*map_cali_data[key][2] + rndm->Uniform(-5, 5);
      
      if(map_s3_sector.find(key)!=map_s3_sector.end()){
        rd->evte = map_s3_sector_cor_data[map_s3_sector[key]][0] + map_s3_sector_cor_data[map_s3_sector[key]][1]*rd->evte;
      }
      if(map_s3_ring.find(key)!=map_s3_ring.end()){
        rd->evte = map_s3_ring_cor_data[map_s3_ring[key]][0] + map_s3_ring_cor_data[map_s3_ring[key]][1]*rd->evte;
      }
    }

    if(rd->cid==0 && rd->evte<CUTGE) continue;
    if(rd->cid==1 && rd->evte<CUTSI) continue;

    ts_ns = GetTsns();

    map_v_ts[key].push_back(ts_ns);
  }

  for(const auto &it : map_v_ts){
    std::cout << it.first << " " << it.second.size() << std::endl;
  }
}

//
Long64_t analysis::GetTsns()
{
  Long64_t ts_ns = 0;
  if(rd->cid==0){
    if(rd->cfdft){
      ts_ns = rd->ts*8;
    }else{
      ts_ns = (rd->ts*2-rd->cfds+rd->cfd/16384.)*4.;
    }
  }

  if(rd->cid==1){
    if(rd->sr==250) ts_ns = rd->ts*8;
    if(rd->sr==100) ts_ns = rd->ts*10; 
  }

  return ts_ns;
}

//
void analysis::ProcessGevsGe()
{
  Long64_t ts_ns1, ts_ns2;
  long long m = 0;
  long long n = 0;

  //Ge vs. Ge
  std::cout << "Ge vs. Ge" << std::endl;
  
  int key = 0;
  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      m = 0;
      n = 0;
      key = 100*i+j;
      if(map_v_ts[key].size()==0) continue;
      if(key==205) continue;

      while(true){
        if(static_cast<std::size_t>(m)==map_v_ts[205].size() || static_cast<std::size_t>(n)==map_v_ts[key].size()) break;
        ts_ns1 = map_v_ts[205][m];
        while(true){
          if(static_cast<std::size_t>(n)==map_v_ts[key].size())  break;
          ts_ns2 = map_v_ts[key][n];

          if(abs(ts_ns1-ts_ns2)<ALIGNMENTWINDOW){
            h1[(i-2)*16+j]->Fill(ts_ns1-ts_ns2+rndm->Uniform(-BINWIDTH/2., BINWIDTH/2.));
            n++;
          }else if((ts_ns1-ts_ns2)>ALIGNMENTWINDOW){
            n++;
          }else{
            m++;
            break;
          }
        }//while
      }//while
    }
  }

  file_out->cd();
  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      h1[(i-2)*16+j]->Write();
    }
  }
}

//
void analysis::ProcessSivsGe()
{
  Long64_t ts_ns1, ts_ns2;
  long long m = 0;
  long long n = 0;

  //Si vs. Ge
  std::cout << "Si vs. Ge" << std::endl;
  
  int key = 0;
  for(int i=2;i<=12;i++){
    for(int j=0;j<16;j++){
      m = 0;
      n = 0;
      key = 10000+100*i+j;
      if(map_v_ts[key].size()==0) continue;

      while(true){
        if(static_cast<std::size_t>(m)==map_v_ts[205].size() || static_cast<std::size_t>(n)==map_v_ts[key].size()) break;
        ts_ns1 = map_v_ts[205][m];
        while(true){
          if(static_cast<std::size_t>(n)==map_v_ts[key].size())  break;
          ts_ns2 = map_v_ts[key][n];

          if(abs(ts_ns1-ts_ns2)<ALIGNMENTWINDOW){
            h2[(i-2)*16+j]->Fill(ts_ns1-ts_ns2+rndm->Uniform(-BINWIDTH/2., BINWIDTH/2.));
            n++;
          }else if((ts_ns1-ts_ns2)>ALIGNMENTWINDOW){
            n++;
          }else{
            m++;
            break;
          }
        }//while
      }//while
    }
  }

  file_out->cd();
  for(int i=2;i<=12;i++){
    for(int j=0;j<16;j++){
      h2[(i-2)*16+j]->Write();
    }
  }
}

//
bool analysis::ReadCaliData()
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
  if(run==460 || (run>=472 && run<=605)){
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
void analysis::PrintCaliData()
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
void analysis::ReadS3CorData() 
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
void analysis::PrintS3CorData()
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
bool analysis::InitMapRingSector()
{
  std::cout << "start init ring and sector id." << std::endl;

  if((run>=457 && run<=462) || (run>=472 && run<=605) || (run>=680 && run<=718)){
    map_s3_sector = {
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
    map_s3_sector = {
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

  map_s3_ring = {
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

