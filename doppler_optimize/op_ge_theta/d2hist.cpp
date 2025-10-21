#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TRandom3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

//
void d2hist(int run, int phi_offset)
{
  TRandom3 *rndm = new TRandom3((Long64_t)time(0)); 

  TFile *fi = TFile::Open(TString::Format("./try/%03ddeg/rootfile/data%04d_doppler_200ns_jump_300ns.root",phi_offset,run));
  TFile *fo = new TFile(TString::Format("./try/%03ddeg/rootfile/data%04d_doppler_200ns_jump_300ns_hist.root",phi_offset,run), "recreate");

  if(fi->IsZombie()){
    std::cout << "open file run " << run << " error!" << std::endl;
    delete fi;

    return ;
  }
  fo->cd();

  std::vector<std::string> v_str_name = {"event", "bg"};
  std::vector<std::string> v_str_e = {
    "e_dc_r"
  };

  std::map<std::pair<int,int>, bool> m_ge_rs_good;
  std::ifstream fi_ge_good("../../pars/config/good_ge.txt");
  if(!fi_ge_good){
    std::cout << "can not open good_ge.txt" << std::endl;
    return;
  }
  std::string line;
  int ring, sector;
  bool gb;
  std::getline(fi_ge_good, line);
  while(fi_ge_good>>ring>>sector>>gb){
    m_ge_rs_good.insert({{ring, sector}, gb});
  }
  fi_ge_good.close();

  //
  for(auto it=v_str_name.begin();it!=v_str_name.end();++it){
    std::cout << "start " << (*it) << std::endl;
    TTree *tr = (TTree*)fi->Get(TString::Format("tr_%s",(*it).c_str()));

    for(auto &str_e : v_str_e){
      // ge single, spider all
      for(auto &[key,val] : m_ge_rs_good){
        if(!val) continue;
        TH1D h_single(Form("%s_%s_ge_ring%d_sector%d_spider_all",(*it).c_str(),str_e.c_str(),key.first,key.second), "", 4096, 0, 4096);
        tr->Project(h_single.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rg==%d && sg==%d",key.first,key.second));
        h_single.Write();
      }
    }
  }

  fo->Close();
  fi->Close();
}

