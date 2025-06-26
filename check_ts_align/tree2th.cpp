#include "set.h"

//
void tree2th(int run, string tf_str, string cut_str)
{
  TRandom3 *rndm = new TRandom3((Long64_t)time(0)); 

  TFile *fi;

  fi = TFile::Open(TString::Format("../rootfile/data%04d_align_%dns_%s_%s.root",run,TIMEWINDOW,tf_str.c_str(),cut_str.c_str()).Data());
  if(fi->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  TFile *fo = new TFile(TString::Format("../rootfile/data%04d_align_th2d_event_%s_%s.root",run,tf_str.c_str(),cut_str.c_str()).Data(), "recreate");

  int n_max_ge = GENUM;
  int n_max_spider = SPIDERNUM;
  int n_max_s3_sector = S3SECTORNUM;
  int n_max_s3_ring = S3RINGNUM;

  double cut_ge_min = 80.; 
  double cut_ge_max = 4096.;
  // cut spider
  // cut low 5000-10000
  // cut high 20000-90000
  // cut s3
  // cut low 2000-30000
  // cut high 30000-90000
  double cut_spider_min = 0.;
  double cut_spider_max = 0.;
  double cut_s3_min = 0;
  double cut_s3_max = 0;

  if(strcmp(cut_str.c_str(),"cut_low") == 0){
    cut_spider_min = 2000;
    cut_spider_max = 20000;
    cut_s3_min = 2000;
    cut_s3_max = 30000;
  }else if(strcmp(cut_str.c_str(),"cut_high") == 0){
    cut_spider_min = 20000;
    cut_spider_max = 90000;
    cut_s3_min = 30000;
    cut_s3_max = 90000;
  }else{
    cout << "wrong cut str." << endl;
    return;
  }
  
  //
  Int_t n_ge = 0;
  Short_t ge_sid[n_max_ge];
  Short_t ge_ch[n_max_ge];
  Double_t ge_energy[n_max_ge];
  Long64_t ge_ts[n_max_ge];

  Int_t n_spider = 0;
  Short_t spider_sid[n_max_spider];
  Short_t spider_ch[n_max_spider];
  Double_t spider_energy[n_max_spider];
  Long64_t spider_ts[n_max_spider];

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
  
  //
  TTree *tr = (TTree*)fi->Get("tr_event"); 

  tr->SetBranchAddress("n_ge", &n_ge);
  tr->SetBranchAddress("ge_sid", ge_sid);
  tr->SetBranchAddress("ge_ch", ge_ch);
  tr->SetBranchAddress("ge_energy", ge_energy);
  tr->SetBranchAddress("ge_ts", ge_ts);

  tr->SetBranchAddress("n_spider", &n_spider);
  tr->SetBranchAddress("spider_sid", spider_sid);
  tr->SetBranchAddress("spider_ch", spider_ch);
  tr->SetBranchAddress("spider_energy", spider_energy);
  tr->SetBranchAddress("spider_ts", spider_ts);

  tr->SetBranchAddress("n_s3_sector", &n_s3_sector);
  tr->SetBranchAddress("s3_sector_sid", s3_sector_sid);
  tr->SetBranchAddress("s3_sector_ch", s3_sector_ch);
  tr->SetBranchAddress("s3_sector_id", s3_sector_id);
  tr->SetBranchAddress("s3_sector_energy", s3_sector_energy);
  tr->SetBranchAddress("s3_sector_ts", s3_sector_ts);

  tr->SetBranchAddress("n_s3_ring", &n_s3_ring);
  tr->SetBranchAddress("s3_ring_sid", s3_ring_sid);
  tr->SetBranchAddress("s3_ring_ch", s3_ring_ch);
  tr->SetBranchAddress("s3_ring_id", s3_ring_id);
  tr->SetBranchAddress("s3_ring_energy", s3_ring_energy);
  tr->SetBranchAddress("s3_ring_ts", s3_ring_ts);

  TH2D *hhpg = new TH2D("hhpg", "", 150,-1500,1500,1024,0,4096);

  TH2D *hh_spider[n_max_ge], *hh_s3[n_max_ge];
  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      hh_spider[(i-2)*16+j] = new TH2D(TString::Format("hh_spider_ge_sid%d_ch%02d",i,j).Data(), "", 300,-1500,1500,1024,0,4096);
      hh_s3[(i-2)*16+j] = new TH2D(TString::Format("hh_s3_ge_sid%d_ch%02d",i,j).Data(), "", 300,-1500,1500,1024,0,4096);
    }
  }

  TH1D *hpg = new TH1D("hpg", "", 300,-1500,1500);
  TH1D *hgg = new TH1D("hgg", "", 300,-1500,1500);
  TH1D *hpp_spider = new TH1D("hpp_spider", "", 300,-1500,1500);
  TH1D *hpp_s3 = new TH1D("hpp_s3", "", 300,-1500,1500);
  TH1D *hpp_si = new TH1D("hpp_si", "", 300,-1500,1500);

  //
  vector<Long64_t> v_ge_ts;
  vector<Long64_t> v_spider_ts;
  vector<Long64_t> v_s3_sector_ts;
  vector<Long64_t> v_s3_ring_ts;
  vector<Long64_t> v_s3_ts;
  vector<Long64_t> v_si_ts;

  bool flag_ge = 0;
  bool flag_spider = 0;
  bool flag_s3 = 0;
  //
  for(Long64_t i=0;i<tr->GetEntries();i++){
    tr->GetEntry(i);
  
    v_ge_ts.clear();
    v_spider_ts.clear();
    v_s3_sector_ts.clear();
    v_s3_ring_ts.clear();
    v_s3_ts.clear();
    v_si_ts.clear();
    flag_ge = 0;
    flag_spider = 0;
    flag_s3 = 0;

    //
    for(int j=0;j<n_ge;j++){
      v_ge_ts.push_back(ge_ts[j]);
    }
    for(int j=0;j<n_spider;j++){
      if(spider_energy[j]<cut_spider_min || spider_energy[j]>cut_spider_max) continue;
      v_spider_ts.push_back(spider_ts[j]);
    }
    for(int j=0;j<n_s3_sector;j++){
      if(s3_sector_energy[j]<cut_s3_min || s3_sector_energy[j]>cut_s3_max) continue;
      v_s3_sector_ts.push_back(s3_sector_ts[j]);
    }
    for(int j=0;j<n_s3_ring;j++){
      if(s3_ring_energy[j]<cut_s3_min || s3_ring_energy[j]>cut_s3_max) continue;
      v_s3_ring_ts.push_back(s3_ring_ts[j]);
    }
  
    v_s3_ts.insert(v_s3_ts.end(), v_s3_sector_ts.begin(), v_s3_sector_ts.end());
    v_s3_ts.insert(v_s3_ts.end(), v_s3_ring_ts.begin(), v_s3_ring_ts.end());

    v_si_ts.insert(v_si_ts.end(), v_spider_ts.begin(), v_spider_ts.end());
    v_si_ts.insert(v_si_ts.end(), v_s3_ts.begin(), v_s3_ts.end());

    //
    if(v_ge_ts.size()==1) flag_ge = 1;
    if(v_spider_ts.size()>0) flag_spider = 1;
    if(v_s3_ts.size()>0) flag_s3 = 1;

    //
    if(flag_ge && flag_spider){
      int j=0;
      int min = abs(v_spider_ts[0]-v_ge_ts[0]);
      for(int jj=1;jj<v_spider_ts.size();jj++){
        int min_temp = abs(v_spider_ts[jj]-v_ge_ts[0]);
        if(min_temp<min){
          j = jj;
          min = min_temp;
        }else continue;
      }
      
      hpg->Fill(v_spider_ts[j]-v_ge_ts[0]);
      hh_spider[(ge_sid[0]-2)*16+ge_ch[0]]->Fill(v_spider_ts[j]-v_ge_ts[0], ge_energy[0]);
      hhpg->Fill(v_spider_ts[j]-v_ge_ts[0], ge_energy[0]);
    }

    if(flag_ge && flag_s3){
      int j=0;
      int min = abs(v_s3_ts[0]-v_ge_ts[0]);
      for(int jj=1;jj<v_s3_ts.size();jj++){
        int min_temp = abs(v_s3_ts[jj]-v_ge_ts[0]);
        if(min_temp<min){
          j = jj;
          min = min_temp;
        }else continue;
      }

      hh_s3[(ge_sid[0]-2)*16+ge_ch[0]]->Fill(v_s3_ts[j]-v_ge_ts[0], ge_energy[0]);
    }

    //
    if(n_ge>1){
      for(int j=0;j<n_ge;j++){
        for(int jj=j+1;jj<n_ge;jj++){
          if(rndm->Uniform(0,1)>0.5) hgg->Fill(v_ge_ts[j]-v_ge_ts[jj]);
          else hgg->Fill(v_ge_ts[jj]-v_ge_ts[j]);
        }
      }
    }

    //
    if(v_spider_ts.size()>1){
      for(int j=0;j<v_spider_ts.size();j++){
        for(int jj=1;jj<v_spider_ts.size();jj++){
          if(rndm->Uniform(0,1)>0.5) hpp_spider->Fill(v_spider_ts[j]-v_spider_ts[jj]);
          else hpp_spider->Fill(v_spider_ts[jj]-v_spider_ts[j]);
        }
      }
    }

    //
    if(v_s3_ts.size()>1){
      for(int j=0;j<v_s3_ts.size();j++){
        for(int jj=1;jj<v_s3_ts.size();jj++){
          if(rndm->Uniform(0,1)>0.5) hpp_s3->Fill(v_s3_ts[j]-v_s3_ts[jj]);
          else hpp_s3->Fill(v_s3_ts[jj]-v_s3_ts[j]);
        }
      }
    }

    //
    if(v_si_ts.size()>1){
      for(int j=0;j<v_si_ts.size();j++){
        for(int jj=1;jj<v_si_ts.size();jj++){
          if(rndm->Uniform(0,1)>0.5) hpp_si->Fill(v_si_ts[j]-v_si_ts[jj]);
          else hpp_si->Fill(v_si_ts[jj]-v_si_ts[j]);
        }
      }
    }

    if(i%100000==0){
      std::cout << "\r" << i << "/" << tr->GetEntries();
      std::cout << std::flush;
    }
  }

  std::cout << std::endl;

  fo->cd();
  hpg->Write();
  hgg->Write();
  hpp_spider->Write();
  hpp_s3->Write();
  hpp_si->Write();
  
  hhpg->Write();

  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      hh_spider[(i-2)*16+j]->Write();

      hh_s3[(i-2)*16+j]->Write();
    }
  }
  fo->Close();

  fi->Close();
}

