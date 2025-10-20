#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TRandom3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

//
void d2hist(int run, int cut)
{
  TRandom3 *rndm = new TRandom3((Long64_t)time(0)); 

  TFile *fi = TFile::Open(TString::Format("./try/%05dkeV/rootfile/data%04d_doppler_200ns_jump_300ns.root",cut,run));
  TFile *fo = new TFile(TString::Format("./try/%05dkeV/rootfile/data%04d_doppler_200ns_jump_300ns_hist.root",cut,run), "recreate");

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

  //
  for(auto it=v_str_name.begin();it!=v_str_name.end();++it){
    std::cout << "start " << (*it) << std::endl;
    TTree *tr = (TTree*)fi->Get(TString::Format("tr_%s",(*it).c_str()));

    //
    for(int i=1;i<=8;i++){
      for(auto &str_e : v_str_e){
        // ge all, spider ring
        TH1D h1(Form("%s_%s_ge_all_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h1.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d",i));
        h1.Write();

        // ge ring, spider ring
        TH1D h3(Form("%s_%s_ge_ring3_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h3.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==3",i));
        h3.Write();

        // ge ring, spider ring
        TH1D h4(Form("%s_%s_ge_ring4_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h4.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==4",i));
        h4.Write();

        // ge ring, spider ring
        TH1D h5(Form("%s_%s_ge_ring5_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h5.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==5",i));
        h5.Write();

        // ge single, spider ring
        TH1D h32(Form("%s_%s_ge_ring3_sector2_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h32.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==3 && sg==2",i));
        h32.Write();

        // ge single, spider ring
        TH1D h41(Form("%s_%s_ge_ring4_sector1_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h41.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==4 && sg==1",i));
        h41.Write();

        // ge single, spider ring
        TH1D h52(Form("%s_%s_ge_ring5_sector2_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h52.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d && rg==5 && sg==2",i));
        h52.Write();
      }
    }
  }

  fo->Close();
  fi->Close();
}

