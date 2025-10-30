// event-bg hist
// example str = "doppler_100ns_p1", "doppler_100ns_p1_no_bg"
void get_rebin_hist(string str="")
{
  gROOT->SetBatch(1);
  gStyle->SetOptStat(0);
  
  double x_min = 0;
  double x_max = 4096;
  int r, s;
  int k = 0, kk = 0;;

  map<pair<int,int>, bool> m_ge_rs_good;
  ifstream fi_ge_good("../../pars/config/good_ge.txt");
  if(!fi_ge_good){
    cout << "can not open good_ge.txt" << endl;
    return;
  }
  string line;
  int ring, sector;
  bool gb;
  std::getline(fi_ge_good, line);
  while(fi_ge_good>>ring>>sector>>gb){
    m_ge_rs_good.insert({{ring, sector}, gb});
  }
  fi_ge_good.close();

  //
  TFile *fi = TFile::Open(Form("../../rootfile/%s_hist.root",str.c_str()));
  if(fi->IsZombie()){
    cout << "cannot open the file." << std::endl;
    return;
  }
  TFile *fo = new TFile(Form("../../rootfile/%s_hist_rb.root",str.c_str()), "recreate");
  fo->cd();

  ////
  ////
  ////
  // ge all spider ring
  TH1D *h1_ga_spr[8];
  TH1D *hh1_ga_spr[8];
  k = 0;
  for(int i=1;i<=8;i++){
    h1_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_spider_ring%d",i));
    hh1_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_spider_ring%d",i));

    h1_ga_spr[k]->Write();
    hh1_ga_spr[k]->Write();

    k++;
  }

  // ge ring spider ring
  TH1D *h1_gr_spr[3*8];
  TH1D *hh1_gr_spr[3*8];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=8;j++){//k=0-7 ge_ring=3
      h1_gr_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_spider_ring%d",i,j));
      hh1_gr_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_spider_ring%d",i,j));

      h1_gr_spr[k]->Write();
      hh1_gr_spr[k]->Write();
      
      k++;
    }
  }

  ////
  ////
  ////
  // ge all s3 ring
  TH1D *h1_ga_s3r[24];
  TH1D *hh1_ga_s3r[24];
  k = 0;
  for(int i=1;i<=24;i++){
    h1_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_s3_ring%d",i));
    hh1_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_s3_ring%d",i));

    k++;
  }
  for(int i=0;i<8;i++){
    h1_ga_s3r[3*i]->Add(h1_ga_s3r[3*i+1], 1);
    h1_ga_s3r[3*i]->Add(h1_ga_s3r[3*i+2], 1);
    h1_ga_s3r[3*i]->Write();

    hh1_ga_s3r[3*i]->Add(hh1_ga_s3r[3*i+1], 1);
    hh1_ga_s3r[3*i]->Add(hh1_ga_s3r[3*i+2], 1);
    hh1_ga_s3r[3*i]->Write();
  }

  // ge ring s3 ring
  TH1D *h1_gr_s3r[3*24];
  TH1D *hh1_gr_s3r[3*24];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=24;j++){//k=0-7 ge_ring=3
      h1_gr_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_s3_ring%d",i,j));
      hh1_gr_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_s3_ring%d",i,j));

      k++;
    }
  }
  for(int i=3;i<=5;i++){
    for(int j=0;j<8;j++){
      h1_gr_s3r[24*(i-3)+3*j]->Add(h1_gr_s3r[24*(i-3)+3*j+1], 1);
      h1_gr_s3r[24*(i-3)+3*j]->Add(h1_gr_s3r[24*(i-3)+3*j+1], 1);
      h1_gr_s3r[24*(i-3)+3*j]->Write();

      hh1_gr_s3r[24*(i-3)+3*j]->Add(hh1_gr_s3r[24*(i-3)+3*j+1], 1);
      hh1_gr_s3r[24*(i-3)+3*j]->Add(hh1_gr_s3r[24*(i-3)+3*j+1], 1);
      hh1_gr_s3r[24*(i-3)+3*j]->Write();
    }
  }

  ////
  ////
  ////
  // ge all spider ring
  TH1D *h2_ga_spr[8];
  TH1D *hh2_ga_spr[8];
  k = 0;
  for(int i=1;i<=8;i++){
    h2_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_spider_ring%d",i));
    hh2_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_spider_ring%d",i));

    h2_ga_spr[k]->Write();
    hh2_ga_spr[k]->Write();

    k++;
  }

  // ge ring spider ring
  TH1D *h2_gr_spr[3*8];
  TH1D *hh2_gr_spr[3*8];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=8;j++){//k=0-7 ge_ring=3
      h2_gr_spr[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_spider_ring%d",i,j));
      hh2_gr_spr[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_spider_ring%d",i,j));

      h2_gr_spr[k]->Write();
      hh2_gr_spr[k]->Write();
      
      k++;
    }
  }

  ////
  ////
  ////
  // ge all s3 ring
  TH1D *h2_ga_s3r[24];
  TH1D *hh2_ga_s3r[24];
  k = 0;
  for(int i=1;i<=24;i++){
    h2_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_s3_ring%d",i));
    hh2_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_s3_ring%d",i));

    k++;
  }
  for(int i=0;i<8;i++){
    h2_ga_s3r[3*i]->Add(h2_ga_s3r[3*i+1], 1);
    h2_ga_s3r[3*i]->Add(h2_ga_s3r[3*i+2], 1);
    h2_ga_s3r[3*i]->Write();

    hh2_ga_s3r[3*i]->Add(hh2_ga_s3r[3*i+1], 1);
    hh2_ga_s3r[3*i]->Add(hh2_ga_s3r[3*i+2], 1);
    hh2_ga_s3r[3*i]->Write();
  }

  // ge ring s3 ring
  TH1D *h2_gr_s3r[3*24];
  TH1D *hh2_gr_s3r[3*24];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=24;j++){//k=0-7 ge_ring=3
      h2_gr_s3r[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_s3_ring%d",i,j));
      hh2_gr_s3r[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_s3_ring%d",i,j));

      k++;
    }
  }
  for(int i=3;i<=5;i++){
    for(int j=0;j<8;j++){
      h2_gr_s3r[24*(i-3)+3*j]->Add(h2_gr_s3r[24*(i-3)+3*j+1], 1);
      h2_gr_s3r[24*(i-3)+3*j]->Add(h2_gr_s3r[24*(i-3)+3*j+1], 1);
      h2_gr_s3r[24*(i-3)+3*j]->Write();

      hh2_gr_s3r[24*(i-3)+3*j]->Add(hh2_gr_s3r[24*(i-3)+3*j+1], 1);
      hh2_gr_s3r[24*(i-3)+3*j]->Add(hh2_gr_s3r[24*(i-3)+3*j+1], 1);
      hh2_gr_s3r[24*(i-3)+3*j]->Write();
    }
  }

  fi->Close();
  fo->Close();
}
