#include "analysis.h"

#include <vector>
#include "math.h"

//
analysis::analysis(const std::string &filename_in, const std::string &filename_out)
{
  benchmark = new TBenchmark;

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "open file " << filename_in.c_str() << " error!" << std::endl;
    delete file_in;

    return ;
  }
  tr_in = (TTree*)file_in->Get("tree");
  rd = new raw(tr_in); 

  InitMapTS();
  
  rndm = new TRandom3((Long64_t)time(0));
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

    if(rd->cid==0 && rd->evte<100) continue;
    if(rd->cid==1 && rd->evte<100) continue;

    if(rd->sr==250) ts_ns = rd->ts*8;
    if(rd->sr==100) ts_ns = rd->ts*10;

    key = 10000*rd->cid+100*rd->sid+rd->ch;
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

