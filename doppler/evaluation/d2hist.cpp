#include "set.h"

//
void d2hist(int run, int win, int jump, string config_str)
{
  TRandom3 *rndm = new TRandom3((Long64_t)time(0)); 

#ifdef TW
  TFile *fi = TFile::Open(TString::Format("../rootfile/data%04d_tw_doppler_%dns_jump_%dns_%s.root",run,win,jump,config_str.c_str()));
  TFile *fo = new TFile(TString::Format("../rootfile/data%04d_tw_doppler_%dns_jump_%dns_%s_hist.root",run,win,jump,config_str.c_str()), "recreate");
#else
  TFile *fi = TFile::Open(TString::Format("../rootfile/data%04d_doppler_%dns_jump_%dns_%s.root",run,win,jump,config_str.c_str()));
  TFile *fo = new TFile(TString::Format("../rootfile/data%04d_doppler_%dns_jump_%dns_%s_hist.root",run,win,jump,config_str.c_str()), "recreate");
#endif  

  if(fi->IsZombie()){
    std::cout << "open file run " << run << " error!" << std::endl;
    delete fi;

    return ;
  }
  fo->cd();

  vector<string> v_str_name = {"event", "bg"};
  vector<string> v_str_e = {
    "e_raw",
    "e_dc_p_1000fs",
    "e_dc_r_1000fs"
  };

  map<int, int> m_ge_ring_sector_number = {
    {1, 6},
    {2, 8},
    {3, 12},
    {4, 8},
    {5, 6}
  };

  //
  for(auto it=v_str_name.begin();it!=v_str_name.end();++it){
    cout << "start " << (*it) << endl;
    TTree *tr = (TTree*)fi->Get(TString::Format("tr_%s",(*it).c_str()));

    TString cmd_str;

    TH1D *h[9];

    // spider p
    cout << "spider_p\n";
    for(int i=0;i<v_str_e.size();i++){
      h[i] = new TH1D(TString::Format("%s_%s_spider_p",(*it).c_str(),v_str_e[i].c_str()), "", 4096, 0, 4096);
      cmd_str = TString::Format("%s>>%s",v_str_e[i].c_str(),h[i]->GetName());
      tr->Draw(cmd_str.Data(), "flag_spider_p && !flag_s3_p && rp>1", "goff");
      h[i]->Write();

      delete h[i];
    }

    // spider p, different spider ring
    cout << "spider p, different spider ring\n";
    for(int i=0;i<8;i++){
      for(int j=0;j<v_str_e.size();j++){
        h[j] = new TH1D(TString::Format("%s_%s_spider_p_ring%d",(*it).c_str(),v_str_e[j].c_str(),i+1), "", 4096, 0, 4096);
        cmd_str = TString::Format("%s>>%s",v_str_e[j].c_str(),h[j]->GetName());
        tr->Draw(cmd_str.Data(), TString::Format("flag_spider_p && !flag_s3_p && rp==%d",i+1), "goff");
        h[j]->Write();

        delete h[j];
      }
    }

    // spider p, different ge
    cout << "spider p, different ge\n";
    for(auto &[rn,sn] : m_ge_ring_sector_number){
      for(int i=1;i<=sn;++i){
        for(int j=0;j<v_str_e.size();j++){
          h[j] = new TH1D(TString::Format("%s_%s_spider_p_ge_ring%d_sector%d",(*it).c_str(),v_str_e[j].c_str    (),rn,i), "", 4096, 0, 4096);
          cmd_str = TString::Format("%s>>%s",v_str_e[j].c_str(),h[j]->GetName());
          tr->Draw(cmd_str.Data(), TString::Format("flag_spider_p && !flag_s3_p && rg==%d && sg==%d && rp>1",rn,i), "goff");
          if(h[j]->GetEntries()>0) h[j]->Write();

          delete h[j];
        }
      }
    }
  
     // spider p, different spider ring, different ge
     cout << "spider p, different spider ring, different ge\n";
     for(int i=0;i<8;i++){// spider ring
       for(auto &[rn,sn] : m_ge_ring_sector_number){// ge ring
         for(int ii=1;ii<=sn;++ii){// ge sector
           for(int jj=0;jj<v_str_e.size();jj++){
             h[jj] = new TH1D(TString::Format("%s_%s_spider_p_ring%d_ge_ring%d_sector%d",(*it).c_str(),v_str_e[jj].c_str(),i+1,rn,ii), "", 4096, 0, 4096);
             cout << h[jj]->GetName() << endl;
             cmd_str = TString::Format("%s>>%s",v_str_e[jj].c_str(),h[jj]->GetName());
             tr->Draw(cmd_str.Data(), TString::Format("flag_spider_p && !flag_s3_p && rp==%d && rg==%d && sg==%d",i+1,rn,ii), "goff");
             if(h[jj]->GetEntries()>0) h[jj]->Write();

             delete h[jj];
           }
         }
       }
     }

  }

  fo->Close();
  fi->Close();
}
