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

  // for(auto &[key,val] : m_ge_rs_good){
  //   std::cout << "ge ring " << key.first << " sector " << key.second << " => " << val << std::endl;
  // }

  //
  for(auto it=v_str_name.begin();it!=v_str_name.end();++it){
    std::cout << "start " << (*it) << std::endl;
    TTree *tr = (TTree*)fi->Get(TString::Format("tr_%s",(*it).c_str()));

    for(auto &str_e : v_str_e){
      // ge all, spider all
      TH1D h1(Form("%s_%s_ge_all_spider_all",(*it).c_str(),str_e.c_str()), "", 4096, 0, 4096);
      tr->Project(h1.GetName(), str_e.c_str(), "flag_spider_p && !flag_s3_p");
      h1.Write();

      // ge single, spider all
      TH1D h32(Form("%s_%s_ge_ring3_sector2_spider_all",(*it).c_str(),str_e.c_str()), "", 4096, 0, 4096);
      tr->Project(h32.GetName(), str_e.c_str(), "flag_spider_p && !flag_s3_p && rg==3 && sg==2");
      h32.Write();

      // ge single, spider all
      TH1D h41(Form("%s_%s_ge_ring4_sector1_spider_all",(*it).c_str(),str_e.c_str()), "", 4096, 0, 4096);
      tr->Project(h41.GetName(), str_e.c_str(), "flag_spider_p && !flag_s3_p && rg==4 && sg==1");
      h41.Write();
      
      // ge single, spider all
      TH1D h42(Form("%s_%s_ge_ring4_sector2_spider_all",(*it).c_str(),str_e.c_str()), "", 4096, 0, 4096);
      tr->Project(h42.GetName(), str_e.c_str(), "flag_spider_p && !flag_s3_p && rg==4 && sg==2");
      h42.Write();

      // ge single, spider all
      TH1D h43(Form("%s_%s_ge_ring4_sector3_spider_all",(*it).c_str(),str_e.c_str()), "", 4096, 0, 4096);
      tr->Project(h43.GetName(), str_e.c_str(), "flag_spider_p && !flag_s3_p && rg==4 && sg==3");
      h43.Write();

      // ge single, spider all
      TH1D h52(Form("%s_%s_ge_ring5_sector2_spider_all",(*it).c_str(),str_e.c_str()), "", 4096, 0, 4096);
      tr->Project(h52.GetName(), str_e.c_str(), "flag_spider_p && !flag_s3_p && rg==5 && sg==2");
      h52.Write();

      // ge single, spider all
      TH1D h53(Form("%s_%s_ge_ring5_sector3_spider_all",(*it).c_str(),str_e.c_str()), "", 4096, 0, 4096);
      tr->Project(h53.GetName(), str_e.c_str(), "flag_spider_p && !flag_s3_p && rg==5 && sg==3");
      h53.Write();
    }

    //
    for(int i=1;i<=8;i++){
      for(auto &str_e : v_str_e){
        // ge all, spider ring
        TH1D h1(Form("%s_%s_ge_all_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h1.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d",i));
        h1.Write();

        // ge single, spider ring
        TH1D h32(Form("%s_%s_ge_ring3_sector2_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h32.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==3 && sg==2",i));
        h32.Write();

        // ge single, spider ring
        TH1D h41(Form("%s_%s_ge_ring4_sector1_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h41.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==4 && sg==1",i));
        h41.Write();

        // ge single, spider ring
        TH1D h42(Form("%s_%s_ge_ring4_sector2_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h42.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==4 && sg==2",i));
        h42.Write();

        // ge single, spider ring
        TH1D h43(Form("%s_%s_ge_ring4_sector3_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h43.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==4 && sg==2",i));
        h43.Write();

        // ge single, spider ring
        TH1D h52(Form("%s_%s_ge_ring5_sector2_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h52.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==5 && sg==2",i));
        h52.Write();

        // ge single, spider ring
        TH1D h53(Form("%s_%s_ge_ring5_sector3_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h53.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==5 && sg==3",i));
        h53.Write();
      }
    }

    //
    for(int i=1;i<=12;i++){
      for(auto &str_e : v_str_e){
        // ge all, spider sector
        TH1D h1(Form("%s_%s_ge_all_spider_sector%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h1.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && sp==%d",i));
        h1.Write();

        // ge single, spider sector
        TH1D h32(Form("%s_%s_ge_ring3_sector2_spider_sector%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h32.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && sp==%d && rg==3 && sg==2",i));
        h32.Write();

        // ge single, spider sector
        TH1D h41(Form("%s_%s_ge_ring4_sector1_spider_sector%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h41.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && sp==%d && rg==4 && sg==1",i));
        h41.Write();

        // ge single, spider sector
        TH1D h42(Form("%s_%s_ge_ring4_sector2_spider_sector%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h42.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && sp==%d && rg==4 && sg==2",i));
        h42.Write();

        // ge single, spider sector
        TH1D h43(Form("%s_%s_ge_ring4_sector3_spider_sector%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h43.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && sp==%d && rg==4 && sg==2",i));
        h43.Write();

        // ge single, spider sector
        TH1D h52(Form("%s_%s_ge_ring5_sector2_spider_sector%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h52.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && sp==%d && rg==5 && sg==2",i));
        h52.Write();

        // ge single, spider sector
        TH1D h53(Form("%s_%s_ge_ring5_sector3_spider_sector%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h53.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && sp==%d && rg==5 && sg==3",i));
        h53.Write();
      }
    }
  }

  fo->Close();
  fi->Close();
}

