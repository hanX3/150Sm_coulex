// g++ d2hist.cpp $(root-config --cflags --libs) -O3 -o d2hist

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TRandom3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

//
void d2hist(int run, int win, int jump)
{
  TRandom3 *rndm = new TRandom3((Long64_t)time(0)); 

  TFile *fi = TFile::Open(TString::Format("../../rootfile/data%04d_doppler_%dns_jump_%dns.root",run,win,jump));
  TFile *fo = new TFile(TString::Format("../../rootfile/data%04d_doppler_%dns_jump_%dns_hist.root",run,win,jump), "recreate");

  if(fi->IsZombie()){
    std::cout << "open file run " << run << " error!" << std::endl;
    delete fi;

    return ;
  }
  fo->cd();

  std::vector<std::string> v_str_name = {"event", "bg"};
  std::vector<std::string> v_str_e = {
    "e_raw",
    "e_dc_p",
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

    // ge all, spider all
    std::cout << "ge all, spider all\n";
    for(auto &str_e : v_str_e){
      TH1D h(Form("%s_%s_ge_all_spider_all",(*it).c_str(),str_e.c_str()), "", 4096, 0, 4096);
      tr->Project(h.GetName(), str_e.c_str(), "flag_spider_p && !flag_s3_p");
      h.Write();
    }

    // ge ring, spider all
    std::cout << "ge ring, spider all\n";
    for(int i=3;i<=5;i++){
      for(auto &str_e : v_str_e){
        TH1D h(Form("%s_%s_ge_ring%d_spider_all",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rg==%d",i));
        h.Write();
      }
    }

    // ge single, spider all
    std::cout << "ge single, spider all\n";
    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;

      int r = key.first;
      int s = key.second;
      for(auto &str_e : v_str_e){
        TH1D h(Form("%s_%s_ge_ring%d_sector%d_spider_all",(*it).c_str(),str_e.c_str(),r,s), "", 4096, 0, 4096);
        tr->Project(h.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rg==%d && sg==%d",r,s));
        h.Write();
      }
    }

    // ge all, spider ring
    std::cout << "ge all, spider ring\n";
    for(int i=1;i<=8;i++){
      for(auto &str_e : v_str_e){
        TH1D h(Form("%s_%s_ge_all_spider_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rp==%d",i));
        h.Write();
      }
    }

    // ge ring, spider ring
    std::cout << "ge ring, spider ring\n";
    for(int i=3;i<=5;i++){
      for(int j=1;j<=8;j++){
        for(auto &str_e : v_str_e){
          TH1D h(Form("%s_%s_ge_ring%d_spider_ring%d",(*it).c_str(),str_e.c_str(),i,j), "", 4096, 0, 4096);
          tr->Project(h.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rg==%d && rp==%d",i,j));
          h.Write();
        }
      }
    }

    // ge single, spider ring
    std::cout << "ge single, spider ring\n";
    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;

      int r = key.first;
      int s = key.second;
      for(int i=1;i<=8;i++){
        for(auto &str_e : v_str_e){
          TH1D h(Form("%s_%s_ge_ring%d_sector%d_spider_ring%d",(*it).c_str(),str_e.c_str(),r,s,i), "", 4096, 0, 4096);
          tr->Project(h.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rg==%d && sg==%d && rp==%d",r,s,i));
          h.Write();
        }
      }
    }

    // ge all, spider sector
    std::cout << "ge all, spider sector\n";
    for(int i=1;i<=12;i++){
      for(auto &str_e : v_str_e){
        TH1D h(Form("%s_%s_ge_all_spider_sector%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && sp==%d",i));
        h.Write();
      }
    }

    // ge ring, spider sector
    std::cout << "ge ring, spider sector\n";
    for(int i=3;i<=5;i++){
      for(int j=1;j<=12;j++){
        for(auto &str_e : v_str_e){
          TH1D h(Form("%s_%s_ge_ring%d_spider_sector%d",(*it).c_str(),str_e.c_str(),i,j), "", 4096, 0, 4096);
          tr->Project(h.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rg==%d && sp==%d",i,j));
          h.Write();
        }
      }
    }

    // ge single, spider sector
    std::cout << "ge single, spider sector\n";
    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;

      int r = key.first;
      int s = key.second;
      for(int i=1;i<=12;i++){
        for(auto &str_e : v_str_e){
          TH1D h(Form("%s_%s_ge_ring%d_sector%d_spider_sector%d",(*it).c_str(),str_e.c_str(),r,s,i), "", 4096, 0, 4096);
          tr->Project(h.GetName(), str_e.c_str(), Form("flag_spider_p && !flag_s3_p && rg==%d && sg==%d && sp==%d",r,s,i));
          h.Write();
        }
      }
    }

    ////
    // ge all, s3 all
    std::cout << "ge all, s3 all\n";
    for(auto &str_e : v_str_e){
      TH1D h(Form("%s_%s_ge_all_s3_all",(*it).c_str(),str_e.c_str()), "", 4096, 0, 4096);
      tr->Project(h.GetName(), str_e.c_str(), "!flag_spider_p && flag_s3_p");
      h.Write();
    }

    // ge ring, s3 all
    std::cout << "ge ring, s3 all\n";
    for(int i=3;i<=5;i++){
      for(auto &str_e : v_str_e){
        TH1D h(Form("%s_%s_ge_ring%d_s3_all",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h.GetName(), str_e.c_str(), Form("!flag_spider_p && flag_s3_p && rg==%d",i));
        h.Write();
      }
    }

    // ge single, s3 all
    std::cout << "ge single, s3 all\n";
    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;

      int r = key.first;
      int s = key.second;
      for(auto &str_e : v_str_e){
        TH1D h(Form("%s_%s_ge_ring%d_sector%d_s3_all",(*it).c_str(),str_e.c_str(),r,s), "", 4096, 0, 4096);
        tr->Project(h.GetName(), str_e.c_str(), Form("!flag_spider_p && flag_s3_p && rg==%d && sg==%d",r,s));
        h.Write();
      }
    }

    // ge all, s3 ring
    std::cout << "ge all, s3 ring\n";
    for(int i=1;i<=24;i++){
      for(auto &str_e : v_str_e){
        TH1D h(Form("%s_%s_ge_all_s3_ring%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h.GetName(), str_e.c_str(), Form("!flag_spider_p && flag_s3_p && rp==%d",i));
        h.Write();
      }
    }

    // ge ring, s3 ring
    std::cout << "ge ring, s3 ring\n";
    for(int i=3;i<=5;i++){
      for(int j=1;j<=24;j++){
        for(auto &str_e : v_str_e){
          TH1D h(Form("%s_%s_ge_ring%d_s3_ring%d",(*it).c_str(),str_e.c_str(),i,j), "", 4096, 0, 4096);
          tr->Project(h.GetName(), str_e.c_str(), Form("!flag_spider_p && flag_s3_p && rg==%d && rp==%d",i,j));
          h.Write();
        }
      }
    }

    // ge single, s3 ring
    std::cout << "ge single, s3 ring\n";
    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;

      int r = key.first;
      int s = key.second;
      for(int i=1;i<=24;i++){
        for(auto &str_e : v_str_e){
          TH1D h(Form("%s_%s_ge_ring%d_sector%d_s3_ring%d",(*it).c_str(),str_e.c_str(),r,s,i), "", 4096, 0, 4096);
          tr->Project(h.GetName(), str_e.c_str(), Form("!flag_spider_p && flag_s3_p && rg==%d && sg==%d && rp==%d",r,s,i));
          h.Write();
        }
      }
    }

    // ge all, s3 sector
    std::cout << "ge all, s3 sector\n";
    for(int i=1;i<=32;i++){
      for(auto &str_e : v_str_e){
        TH1D h(Form("%s_%s_ge_all_s3_sector%d",(*it).c_str(),str_e.c_str(),i), "", 4096, 0, 4096);
        tr->Project(h.GetName(), str_e.c_str(), Form("!flag_spider_p && flag_s3_p && sp==%d",i));
        h.Write();
      }
    }

    // ge ring, s3 sector
    std::cout << "ge ring, s3 sector\n";
    for(int i=3;i<=5;i++){
      for(int j=1;j<=32;j++){
        for(auto &str_e : v_str_e){
          TH1D h(Form("%s_%s_ge_ring%d_s3_sector%d",(*it).c_str(),str_e.c_str(),i,j), "", 4096, 0, 4096);
          tr->Project(h.GetName(), str_e.c_str(), Form("!flag_spider_p && flag_s3_p && rg==%d && sp==%d",i,j));
          h.Write();
        }
      }
    }

    // ge single, s3 sector
    std::cout << "ge single, s3 sector\n";
    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;

      int r = key.first;
      int s = key.second;
      for(int i=1;i<=32;i++){
        for(auto &str_e : v_str_e){
          TH1D h(Form("%s_%s_ge_ring%d_sector%d_s3_sector%d",(*it).c_str(),str_e.c_str(),r,s,i), "", 4096, 0, 4096);
          tr->Project(h.GetName(), str_e.c_str(), Form("!flag_spider_p && flag_s3_p && rg==%d && sg==%d && sp==%d",r,s,i));
          h.Write();
        }
      }
    }

  }

  fo->Close();
  fi->Close();
}

//
int main(int argc, char* argv[])
{
  if(argc!=4){
    std::cerr << "Usage: " << argv[0] << " <run> <win> <jump>" << std::endl;
    return 1;
  }

  int run  = std::atoi(argv[1]);
  int win  = std::atoi(argv[2]);
  int jump = std::atoi(argv[3]);

  d2hist(run, win, jump);

  return 0;
}
