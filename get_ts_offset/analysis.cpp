#include "analysis.h"

#include <vector>
#include "math.h"

//
analysis::analysis(const std::string &filename_in, const std::string &filename_out, const int r)
{
  benchmark = new TBenchmark;

  run = r;

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

  rndm = new TRandom3((Long64_t)time(0));
  
  InitMapTS();
  //
  file_out = TFile::Open(filename_out.c_str(), "recreate");

  // time differents Ge
  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      h1[(i-2)*16+j] = new TH1D(TString::Format("ge_sid%d_ch%02d",i,j).Data(), "", 2*ALIGNMENTWINDOW/10, -ALIGNMENTWINDOW, ALIGNMENTWINDOW);
    }
  }

  // time differents Si
  for(int i=2;i<=12;i++){
    for(int j=0;j<16;j++){
      h2[(i-2)*16+j] = new TH1D(TString::Format("si_sid%d_ch%02d",i,j).Data(), "", 2*ALIGNMENTWINDOW/10, -ALIGNMENTWINDOW, ALIGNMENTWINDOW);
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

    if(rd->cid==0 && rd->evte<CUTGE) continue;
    if(rd->cid==1 && rd->evte<CUTSI) continue;

    if(rd->sr==250) ts_ns = rd->ts*8;
    if(rd->sr==100) ts_ns = rd->ts*10;

    map_v_ts[key].push_back(ts_ns);
  }

  for(const auto &it : map_v_ts){
    std::cout << it.first << " " << it.second.size() << std::endl;
  }
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
            h1[(i-2)*16+j]->Fill(ts_ns1-ts_ns2+rndm->Uniform(-10., 10.));
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
            h2[(i-2)*16+j]->Fill(ts_ns1-ts_ns2+rndm->Uniform(-10., 10.));
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
  std::ifstream fi_cali;
  fi_cali.open("../pars/cali_ge.txt");
  if(!fi_cali){
    std::cout << "can not open cali.txt" << std::endl;
    return 0;
  }

  std::string line;
  std::getline(fi_cali, line);

  int cid, sid, ch;
  double par0, par1, par2, chi2;
  int key = 0;

  while(1){
    fi_cali >> cid >> sid >> ch >> par0 >> par1 >> par2 >> chi2;
    if(!fi_cali.good()) break;

    key = 10000*cid+100*sid+ch; // ch from 0 to 15
    std::vector<double> value;
    value.push_back(par0);
    value.push_back(par1);
    value.push_back(par2);
    value.push_back(chi2);

    map_cali_data.insert(std::pair<int, std::vector<double>>(key, value));
  }

  fi_cali.close();

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

  return 1;
}

//
void analysis::PrintCaliData()
{
  std::cout << "start print cali data" << std::endl;

  std::map<int, std::vector<double>>::iterator it_cali = map_cali_data.begin();
  for(it_cali=map_cali_data.begin();it_cali!=map_cali_data.end();it_cali++){
    std::cout << it_cali->first << " => " << it_cali->second[0] << " " << it_cali->second[1] << " " << it_cali->second[2] << '\n';
  }

  std::map<int, double>::iterator it_k = map_k_data.begin();
  for(it_k=map_k_data.begin();it_k!=map_k_data.end();it_k++){
    std::cout << it_k->first << " => " << it_k->second<< '\n';
  }
}

