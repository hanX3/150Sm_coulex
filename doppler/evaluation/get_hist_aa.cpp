// event-bg hist
// example str = "doppler_100ns_p1", "doppler_100ns_p1_no_bg"
void get_hist_aa(string str="")
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
  TFile *fo = new TFile(Form("../../rootfile/%s_hist_aa.root",str.c_str()), "recreate");
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

    k++;
  }

  TH1D *h1_ga_spa = (TH1D*)h1_ga_spr[0]->Clone("event_e_dc_r_ge_all_spider_all");
  TH1D *hh1_ga_spa = (TH1D*)hh1_ga_spr[0]->Clone("bg_e_dc_r_ge_all_spider_all");
  for(int i=1;i<8;i++){
    h1_ga_spa->Add(h1_ga_spr[i], 1);
    hh1_ga_spa->Add(hh1_ga_spr[i], 1);
  }
  h1_ga_spa->Write();
  hh1_ga_spa->Write();

  // ge ring spider ring
  TH1D *h1_gr_spr[3*8];
  TH1D *hh1_gr_spr[3*8];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=8;j++){//k=0-7 ge_ring=3
      h1_gr_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_spider_ring%d",i,j));
      hh1_gr_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_spider_ring%d",i,j));

      k++;
    }
  }

  TH1D *h1_gr3_spa = (TH1D*)h1_gr_spr[0]->Clone("event_e_dc_r_ge_ring3_spider_all");
  TH1D *hh1_gr3_spa = (TH1D*)hh1_gr_spr[0]->Clone("bg_e_dc_r_ge_ring3_spider_all");
  for(int i=1;i<8;i++){
    h1_gr3_spa->Add(h1_gr_spr[i], 1);
    hh1_gr3_spa->Add(hh1_gr_spr[i], 1);
  }
  h1_gr3_spa->Write();
  hh1_gr3_spa->Write();

  TH1D *h1_gr4_spa = (TH1D*)h1_gr_spr[8]->Clone("event_e_dc_r_ge_ring4_spider_all");
  TH1D *hh1_gr4_spa = (TH1D*)hh1_gr_spr[8]->Clone("bg_e_dc_r_ge_ring4_spider_all");
  for(int i=8;i<16;i++){
    h1_gr4_spa->Add(h1_gr_spr[i], 1);
    hh1_gr4_spa->Add(hh1_gr_spr[i], 1);
  }
  h1_gr4_spa->Write();
  hh1_gr4_spa->Write();

  TH1D *h1_gr5_spa = (TH1D*)h1_gr_spr[16]->Clone("event_e_dc_r_ge_ring5_spider_all");
  TH1D *hh1_gr5_spa = (TH1D*)hh1_gr_spr[16]->Clone("bg_e_dc_r_ge_ring5_spider_all");
  for(int i=16;i<24;i++){
    h1_gr5_spa->Add(h1_gr_spr[i], 1);
    hh1_gr5_spa->Add(hh1_gr_spr[i], 1);
  }
  h1_gr5_spa->Write();
  hh1_gr5_spa->Write();

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

  TH1D *h1_ga_s3a = (TH1D*)h1_ga_s3r[0]->Clone("event_e_dc_r_ge_all_s3_all");
  TH1D *hh1_ga_s3a = (TH1D*)hh1_ga_s3r[0]->Clone("bg_e_dc_r_ge_all_s3_all");
  for(int i=1;i<24;i++){
    h1_ga_s3a->Add(h1_ga_s3r[i], 1);
    hh1_ga_s3a->Add(hh1_ga_s3r[i], 1);
  }
  h1_ga_s3a->Write();
  hh1_ga_s3a->Write();

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

  TH1D *h1_gr3_s3a = (TH1D*)h1_gr_s3r[0]->Clone("event_e_dc_r_ge_ring3_s3_all");
  TH1D *hh1_gr3_s3a = (TH1D*)hh1_gr_s3r[0]->Clone("bg_e_dc_r_ge_ring3_s3_all");
  for(int i=1;i<24;i++){
    h1_gr3_s3a->Add(h1_gr_s3r[i], 1);
    hh1_gr3_s3a->Add(hh1_gr_s3r[i], 1);
  }
  h1_gr3_s3a->Write();
  hh1_gr3_s3a->Write();

  TH1D *h1_gr4_s3a = (TH1D*)h1_gr_s3r[24]->Clone("event_e_dc_r_ge_ring4_s3_all");
  TH1D *hh1_gr4_s3a = (TH1D*)hh1_gr_s3r[24]->Clone("bg_e_dc_r_ge_ring4_s3_all");
  for(int i=25;i<48;i++){
    h1_gr4_s3a->Add(h1_gr_s3r[i], 1);
    hh1_gr4_s3a->Add(hh1_gr_s3r[i], 1);
  }
  h1_gr4_s3a->Write();
  hh1_gr4_s3a->Write();

  TH1D *h1_gr5_s3a = (TH1D*)h1_gr_s3r[48]->Clone("event_e_dc_r_ge_ring5_s3_all");
  TH1D *hh1_gr5_s3a = (TH1D*)hh1_gr_s3r[48]->Clone("bg_e_dc_r_ge_ring5_s3_all");
  for(int i=49;i<72;i++){
    h1_gr5_s3a->Add(h1_gr_s3r[i], 1);
    hh1_gr5_s3a->Add(hh1_gr_s3r[i], 1);
  }
  h1_gr5_s3a->Write();
  hh1_gr5_s3a->Write();

  //
  fi->Close();
  fo->Close();
}
