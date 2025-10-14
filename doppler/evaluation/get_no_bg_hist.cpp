// event-bg hist
// example str = "data0472_0605_doppler_200ns_jump_300ns"
void get_no_bg_hist(string str="")
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

  // for(auto &[key,val] : m_ge_rs_good){
  //   cout << "ge ring " << key.first << " sector " << key.second << " => " << val << endl;
  // }

  //
  TFile *fi = TFile::Open(Form("../../rootfile/%s.root",str.c_str()));
  if(fi->IsZombie()){
    cout << "cannot open the file." << std::endl;
    return;
  }
  TFile *fo = new TFile(Form("../../rootfile/%s_no_bg.root",str.c_str()), "recreate");
  fo->cd();

  ////
  ////
  ////
  // ge all spider all
  TH1D *h1_ga_spa, *h2_ga_spa, *h3_ga_spa;
  TH1D *hh1_ga_spa, *hh2_ga_spa, *hh3_ga_spa;

  h1_ga_spa = (TH1D*)fi->Get("event_e_raw_ge_all_spider_all");
  h2_ga_spa = (TH1D*)fi->Get("event_e_dc_p_ge_all_spider_all");
  h3_ga_spa = (TH1D*)fi->Get("event_e_dc_r_ge_all_spider_all");
  hh1_ga_spa = (TH1D*)fi->Get("bg_e_raw_ge_all_spider_all");
  hh2_ga_spa = (TH1D*)fi->Get("bg_e_dc_p_ge_all_spider_all");
  hh3_ga_spa = (TH1D*)fi->Get("bg_e_dc_r_ge_all_spider_all");

  h1_ga_spa->Add(hh1_ga_spa, -1);
  h2_ga_spa->Add(hh2_ga_spa, -1);
  h3_ga_spa->Add(hh3_ga_spa, -1);

  h1_ga_spa->Write();
  h2_ga_spa->Write();
  h3_ga_spa->Write();

  // ge ring spider all
  TDirectory *dir_gr_spa = (TDirectory*)fo->mkdir("ge_ring_spider_all", "", true);
  dir_gr_spa->cd();

  TH1D *h1_gr_spa[3], *h2_gr_spa[3], *h3_gr_spa[3];
  TH1D *hh1_gr_spa[3], *hh2_gr_spa[3], *hh3_gr_spa[3];
  k = 0;
  for(int i=3;i<=5;i++){
    h1_gr_spa[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_spider_all",i));
    h2_gr_spa[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_spider_all",i));
    h3_gr_spa[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_spider_all",i));
    hh1_gr_spa[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_spider_all",i));
    hh2_gr_spa[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_spider_all",i));
    hh3_gr_spa[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_spider_all",i));

    h1_gr_spa[k]->Add(hh1_gr_spa[k], -1);
    h2_gr_spa[k]->Add(hh2_gr_spa[k], -1);
    h3_gr_spa[k]->Add(hh3_gr_spa[k], -1);

    h1_gr_spa[k]->Write();
    h2_gr_spa[k]->Write();
    h3_gr_spa[k]->Write();

    k++;
  }

  // ge single spider all
  TDirectory *dir_grs_spa = (TDirectory*)fo->mkdir("ge_ring_sector_spider_all", "", true);
  dir_grs_spa->cd();

  TH1D *h1_grs_spa[19], *h2_grs_spa[19], *h3_grs_spa[19];
  TH1D *hh1_grs_spa[19], *hh2_grs_spa[19], *hh3_grs_spa[19];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    h1_grs_spa[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_spider_all",r,s));
    h2_grs_spa[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_spider_all",r,s));
    h3_grs_spa[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_spider_all",r,s));
    hh1_grs_spa[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_spider_all",r,s));
    hh2_grs_spa[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_spider_all",r,s));
    hh3_grs_spa[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_spider_all",r,s));

    h1_grs_spa[k]->Add(hh1_grs_spa[k], -1);
    h2_grs_spa[k]->Add(hh2_grs_spa[k], -1);
    h3_grs_spa[k]->Add(hh3_grs_spa[k], -1);

    h1_grs_spa[k]->Write();
    h2_grs_spa[k]->Write();
    h3_grs_spa[k]->Write();

    k++;
  }

  ////
  ////
  ////
  // ge all spider ring
  TDirectory *dir_ga_spr = (TDirectory*)fo->mkdir("ge_all_spider_ring", "", true);
  dir_ga_spr->cd();

  TH1D *h1_ga_spr[8], *h2_ga_spr[8], *h3_ga_spr[8];
  TH1D *hh1_ga_spr[8], *hh2_ga_spr[8], *hh3_ga_spr[8];
  k = 0;
  for(int i=1;i<=8;i++){
    h1_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_spider_ring%d",i));
    h2_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_all_spider_ring%d",i));
    h3_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_spider_ring%d",i));
    hh1_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_spider_ring%d",i));
    hh2_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_all_spider_ring%d",i));
    hh3_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_spider_ring%d",i));

    h1_ga_spr[k]->Add(hh1_ga_spr[k], -1);
    h2_ga_spr[k]->Add(hh2_ga_spr[k], -1);
    h3_ga_spr[k]->Add(hh3_ga_spr[k], -1);

    h1_ga_spr[k]->Write();
    h2_ga_spr[k]->Write();
    h3_ga_spr[k]->Write();

    k++;
  }

  // ge ring spider ring
  TDirectory *dir_gr_spr = (TDirectory*)fo->mkdir("ge_ring_spider_ring", "", true);
  dir_gr_spr->cd();

  TH1D *h1_gr_spr[3*8], *h2_gr_spr[3*8], *h3_gr_spr[3*8];
  TH1D *hh1_gr_spr[3*8], *hh2_gr_spr[3*8], *hh3_gr_spr[3*8];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=8;j++){//k=0-7 ge_ring=3
      h1_gr_spr[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_spider_ring%d",i,j));
      h2_gr_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_spider_ring%d",i,j));
      h3_gr_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_spider_ring%d",i,j));
      hh1_gr_spr[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_spider_ring%d",i,j));
      hh2_gr_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_spider_ring%d",i,j));
      hh3_gr_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_spider_ring%d",i,j));

      h1_gr_spr[k]->Add(hh1_gr_spr[k], -1);
      h2_gr_spr[k]->Add(hh2_gr_spr[k], -1);
      h3_gr_spr[k]->Add(hh3_gr_spr[k], -1);

      h1_gr_spr[k]->Write();
      h2_gr_spr[k]->Write();
      h3_gr_spr[k]->Write();
      
      k++;
    }
  }

  // ge single spider ring
  TDirectory *dir_grs_spr = (TDirectory*)fo->mkdir("ge_ring_sector_spider_ring", "", true);
  dir_grs_spr->cd();

  TH1D *h1_grs_spr[19*8], *h2_grs_spr[19*8], *h3_grs_spr[19*8];
  TH1D *hh1_grs_spr[19*8], *hh2_grs_spr[19*8], *hh3_grs_spr[19*8];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    for(int i=1;i<=8;i++){
      h1_grs_spr[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      h2_grs_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      h3_grs_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      hh1_grs_spr[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      hh2_grs_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      hh3_grs_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_spider_ring%d",r,s,i));

      h1_grs_spr[k]->Add(hh1_grs_spr[k], -1);
      h2_grs_spr[k]->Add(hh2_grs_spr[k], -1);
      h3_grs_spr[k]->Add(hh3_grs_spr[k], -1);

      h1_grs_spr[k]->Write();
      h2_grs_spr[k]->Write();
      h3_grs_spr[k]->Write();
      
      k++;
    }
  }

  ////
  ////
  ////
  // ge all spider sector
  TDirectory *dir_ga_sps = (TDirectory*)fo->mkdir("ge_all_spider_sector", "", true);
  dir_ga_sps->cd();

  TH1D *h1_ga_sps[12], *h2_ga_sps[12], *h3_ga_sps[12];
  TH1D *hh1_ga_sps[12], *hh2_ga_sps[12], *hh3_ga_sps[12];
  k = 0;
  for(int i=1;i<=12;i++){
    h1_ga_sps[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_spider_sector%d",i));
    h2_ga_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_all_spider_sector%d",i));
    h3_ga_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_spider_sector%d",i));
    hh1_ga_sps[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_spider_sector%d",i));
    hh2_ga_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_all_spider_sector%d",i));
    hh3_ga_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_spider_sector%d",i));

    h1_ga_sps[k]->Add(hh1_ga_sps[k], -1);
    h2_ga_sps[k]->Add(hh2_ga_sps[k], -1);
    h3_ga_sps[k]->Add(hh3_ga_sps[k], -1);

    h1_ga_sps[k]->Write();
    h2_ga_sps[k]->Write();
    h3_ga_sps[k]->Write();

    k++;
  }

  // ge ring spider sector
  TDirectory *dir_gr_sps = (TDirectory*)fo->mkdir("ge_ring_spider_sector", "", true);
  dir_gr_sps->cd();

  TH1D *h1_gr_sps[3*12], *h2_gr_sps[3*12], *h3_gr_sps[3*12];
  TH1D *hh1_gr_sps[3*12], *hh2_gr_sps[3*12], *hh3_gr_sps[3*12];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=12;j++){
      h1_gr_sps[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_spider_sector%d",i,j));
      h2_gr_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_spider_sector%d",i,j));
      h3_gr_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_spider_sector%d",i,j));
      hh1_gr_sps[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_spider_sector%d",i,j));
      hh2_gr_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_spider_sector%d",i,j));
      hh3_gr_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_spider_sector%d",i,j));

      h1_gr_sps[k]->Add(hh1_gr_sps[k], -1);
      h2_gr_sps[k]->Add(hh2_gr_sps[k], -1);
      h3_gr_sps[k]->Add(hh3_gr_sps[k], -1);

      h1_gr_sps[k]->Write();
      h2_gr_sps[k]->Write();
      h3_gr_sps[k]->Write();
      
      k++;
    }
  }

  // ge single spider sector
  TDirectory *dir_grs_sps = (TDirectory*)fo->mkdir("ge_ring_sector_spider_sector", "", true);
  dir_grs_sps->cd();

  TH1D *h1_grs_sps[19*12], *h2_grs_sps[19*12], *h3_grs_sps[19*12];
  TH1D *hh1_grs_sps[19*12], *hh2_grs_sps[19*12], *hh3_grs_sps[19*12];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    for(int i=1;i<=12;i++){
      h1_grs_sps[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      h2_grs_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      h3_grs_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      hh1_grs_sps[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      hh2_grs_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      hh3_grs_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_spider_sector%d",r,s,i));

      h1_grs_sps[k]->Add(hh1_grs_sps[k], -1);
      h2_grs_sps[k]->Add(hh2_grs_sps[k], -1);
      h3_grs_sps[k]->Add(hh3_grs_sps[k], -1);

      h1_grs_sps[k]->Write();
      h2_grs_sps[k]->Write();
      h3_grs_sps[k]->Write();
      
      k++;
    }
  }

  ////
  ////
  ////
  // ge all s3 all
  fo->cd();
  TH1D *h1_ga_s3a, *h2_ga_s3a, *h3_ga_s3a;
  TH1D *hh1_ga_s3a, *hh2_ga_s3a, *hh3_ga_s3a;

  h1_ga_s3a = (TH1D*)fi->Get("event_e_raw_ge_all_s3_all");
  h2_ga_s3a = (TH1D*)fi->Get("event_e_dc_p_ge_all_s3_all");
  h3_ga_s3a = (TH1D*)fi->Get("event_e_dc_r_ge_all_s3_all");
  hh1_ga_s3a = (TH1D*)fi->Get("bg_e_raw_ge_all_s3_all");
  hh2_ga_s3a = (TH1D*)fi->Get("bg_e_dc_p_ge_all_s3_all");
  hh3_ga_s3a = (TH1D*)fi->Get("bg_e_dc_r_ge_all_s3_all");

  h1_ga_s3a->Add(hh1_ga_s3a, -1);
  h2_ga_s3a->Add(hh2_ga_s3a, -1);
  h3_ga_s3a->Add(hh3_ga_s3a, -1);

  h1_ga_s3a->Write();
  h2_ga_s3a->Write();
  h3_ga_s3a->Write();

  // ge ring s3 all
  TDirectory *dir_gr_s3a = (TDirectory*)fo->mkdir("ge_ring_s3_all", "", true);
  dir_gr_s3a->cd();

  TH1D *h1_gr_s3a[3], *h2_gr_s3a[3], *h3_gr_s3a[3];
  TH1D *hh1_gr_s3a[3], *hh2_gr_s3a[3], *hh3_gr_s3a[3];
  k = 0;
  for(int i=3;i<=5;i++){
    h1_gr_s3a[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_s3_all",i));
    h2_gr_s3a[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_s3_all",i));
    h3_gr_s3a[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_s3_all",i));
    hh1_gr_s3a[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_s3_all",i));
    hh2_gr_s3a[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_s3_all",i));
    hh3_gr_s3a[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_s3_all",i));

    h1_gr_s3a[k]->Add(hh1_gr_s3a[k], -1);
    h2_gr_s3a[k]->Add(hh2_gr_s3a[k], -1);
    h3_gr_s3a[k]->Add(hh3_gr_s3a[k], -1);

    h1_gr_s3a[k]->Write();
    h2_gr_s3a[k]->Write();
    h3_gr_s3a[k]->Write();

    k++;
  }

  // ge single s3 all
  TDirectory *dir_grs_s3a = (TDirectory*)fo->mkdir("ge_ring_sector_s3_all", "", true);
  dir_grs_s3a->cd();

  TH1D *h1_grs_s3a[19], *h2_grs_s3a[19], *h3_grs_s3a[19];
  TH1D *hh1_grs_s3a[19], *hh2_grs_s3a[19], *hh3_grs_s3a[19];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    h1_grs_s3a[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_s3_all",r,s));
    h2_grs_s3a[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_s3_all",r,s));
    h3_grs_s3a[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_s3_all",r,s));
    hh1_grs_s3a[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_s3_all",r,s));
    hh2_grs_s3a[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_s3_all",r,s));
    hh3_grs_s3a[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_s3_all",r,s));

    h1_grs_s3a[k]->Add(hh1_grs_s3a[k], -1);
    h2_grs_s3a[k]->Add(hh2_grs_s3a[k], -1);
    h3_grs_s3a[k]->Add(hh3_grs_s3a[k], -1);

    h1_grs_s3a[k]->Write();
    h2_grs_s3a[k]->Write();
    h3_grs_s3a[k]->Write();

    k++;
  }

  ////
  ////
  ////
  // ge all s3 ring
  TDirectory *dir_ga_s3r = (TDirectory*)fo->mkdir("ge_all_s3_ring", "", true);
  dir_ga_s3r->cd();

  TH1D *h1_ga_s3r[24], *h2_ga_s3r[24], *h3_ga_s3r[24];
  TH1D *hh1_ga_s3r[24], *hh2_ga_s3r[24], *hh3_ga_s3r[24];
  k = 0;
  for(int i=1;i<=24;i++){
    h1_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_s3_ring%d",i));
    h2_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_all_s3_ring%d",i));
    h3_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_s3_ring%d",i));
    hh1_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_s3_ring%d",i));
    hh2_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_all_s3_ring%d",i));
    hh3_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_s3_ring%d",i));

    h1_ga_s3r[k]->Add(hh1_ga_s3r[k], -1);
    h2_ga_s3r[k]->Add(hh2_ga_s3r[k], -1);
    h3_ga_s3r[k]->Add(hh3_ga_s3r[k], -1);

    h1_ga_s3r[k]->Write();
    h2_ga_s3r[k]->Write();
    h3_ga_s3r[k]->Write();

    k++;
  }

  // ge ring s3 ring
  TDirectory *dir_gr_s3r = (TDirectory*)fo->mkdir("ge_ring_s3_ring", "", true);
  dir_gr_s3r->cd();

  TH1D *h1_gr_s3r[3*24], *h2_gr_s3r[3*24], *h3_gr_s3r[3*24];
  TH1D *hh1_gr_s3r[3*24], *hh2_gr_s3r[3*24], *hh3_gr_s3r[3*24];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=24;j++){//k=0-7 ge_ring=3
      h1_gr_s3r[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_s3_ring%d",i,j));
      h2_gr_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_s3_ring%d",i,j));
      h3_gr_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_s3_ring%d",i,j));
      hh1_gr_s3r[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_s3_ring%d",i,j));
      hh2_gr_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_s3_ring%d",i,j));
      hh3_gr_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_s3_ring%d",i,j));

      h1_gr_s3r[k]->Add(hh1_gr_s3r[k], -1);
      h2_gr_s3r[k]->Add(hh2_gr_s3r[k], -1);
      h3_gr_s3r[k]->Add(hh3_gr_s3r[k], -1);

      h1_gr_s3r[k]->Write();
      h2_gr_s3r[k]->Write();
      h3_gr_s3r[k]->Write();
      
      k++;
    }
  }

  // ge single s3 ring
  TDirectory *dir_grs_s3r = (TDirectory*)fo->mkdir("ge_ring_sector_s3_ring", "", true);
  dir_grs_s3r->cd();

  TH1D *h1_grs_s3r[19*24], *h2_grs_s3r[19*24], *h3_grs_s3r[19*24];
  TH1D *hh1_grs_s3r[19*24], *hh2_grs_s3r[19*24], *hh3_grs_s3r[19*24];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    for(int i=1;i<=24;i++){
      h1_grs_s3r[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      h2_grs_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      h3_grs_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      hh1_grs_s3r[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      hh2_grs_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      hh3_grs_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_s3_ring%d",r,s,i));

      h1_grs_s3r[k]->Add(hh1_grs_s3r[k], -1);
      h2_grs_s3r[k]->Add(hh2_grs_s3r[k], -1);
      h3_grs_s3r[k]->Add(hh3_grs_s3r[k], -1);

      h1_grs_s3r[k]->Write();
      h2_grs_s3r[k]->Write();
      h3_grs_s3r[k]->Write();
      
      k++;
    }
  }

  ////
  ////
  ////
  // ge all s3 sector
  TDirectory *dir_ga_s3s = (TDirectory*)fo->mkdir("ge_all_s3_sector", "", true);
  dir_ga_s3s->cd();

  TH1D *h1_ga_s3s[32], *h2_ga_s3s[32], *h3_ga_s3s[32];
  TH1D *hh1_ga_s3s[32], *hh2_ga_s3s[32], *hh3_ga_s3s[32];
  k = 0;
  for(int i=1;i<=32;i++){
    h1_ga_s3s[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_s3_sector%d",i));
    h2_ga_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_all_s3_sector%d",i));
    h3_ga_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_s3_sector%d",i));
    hh1_ga_s3s[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_s3_sector%d",i));
    hh2_ga_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_all_s3_sector%d",i));
    hh3_ga_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_s3_sector%d",i));

    h1_ga_s3s[k]->Add(hh1_ga_s3s[k], -1);
    h2_ga_s3s[k]->Add(hh2_ga_s3s[k], -1);
    h3_ga_s3s[k]->Add(hh3_ga_s3s[k], -1);

    h1_ga_s3s[k]->Write();
    h2_ga_s3s[k]->Write();
    h3_ga_s3s[k]->Write();

    k++;
  }

  // ge ring s3 sector
  TDirectory *dir_gr_s3s = (TDirectory*)fo->mkdir("ge_ring_s3_sector", "", true);
  dir_gr_s3s->cd();

  TH1D *h1_gr_s3s[3*32], *h2_gr_s3s[3*32], *h3_gr_s3s[3*32];
  TH1D *hh1_gr_s3s[3*32], *hh2_gr_s3s[3*32], *hh3_gr_s3s[3*32];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=32;j++){
      h1_gr_s3s[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_s3_sector%d",i,j));
      h2_gr_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_s3_sector%d",i,j));
      h3_gr_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_s3_sector%d",i,j));
      hh1_gr_s3s[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_s3_sector%d",i,j));
      hh2_gr_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_s3_sector%d",i,j));
      hh3_gr_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_s3_sector%d",i,j));

      h1_gr_s3s[k]->Add(hh1_gr_s3s[k], -1);
      h2_gr_s3s[k]->Add(hh2_gr_s3s[k], -1);
      h3_gr_s3s[k]->Add(hh3_gr_s3s[k], -1);

      h1_gr_s3s[k]->Write();
      h2_gr_s3s[k]->Write();
      h3_gr_s3s[k]->Write();
      
      k++;
    }
  }

  // ge single s3 sector
  TDirectory *dir_grs_s3s = (TDirectory*)fo->mkdir("ge_ring_sector_s3_sector", "", true);
  dir_grs_s3s->cd();

  TH1D *h1_grs_s3s[19*32], *h2_grs_s3s[19*32], *h3_grs_s3s[19*32];
  TH1D *hh1_grs_s3s[19*32], *hh2_grs_s3s[19*32], *hh3_grs_s3s[19*32];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    for(int i=1;i<=32;i++){
      h1_grs_s3s[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      h2_grs_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      h3_grs_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      hh1_grs_s3s[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      hh2_grs_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      hh3_grs_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_s3_sector%d",r,s,i));

      h1_grs_s3s[k]->Add(hh1_grs_s3s[k], -1);
      h2_grs_s3s[k]->Add(hh2_grs_s3s[k], -1);
      h3_grs_s3s[k]->Add(hh3_grs_s3s[k], -1);

      h1_grs_s3s[k]->Write();
      h2_grs_s3s[k]->Write();
      h3_grs_s3s[k]->Write();
      
      k++;
    }
  }


  fi->Close();
  fo->Close();
}
