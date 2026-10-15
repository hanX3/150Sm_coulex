// event-bg hist
// example str = "100ns_p1", "100ns_p1_no_bg"
void get_hist_add_all(string str="")
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
  TFile *fi = TFile::Open(Form("../../rootfile/doppler_%s_hist.root",str.c_str()));
  if(fi->IsZombie()){
    cout << "cannot open the file." << std::endl;
    return;
  }
  TFile *fo = new TFile(Form("../../rootfile/doppler_%s_hist_add_all.root",str.c_str()), "recreate");
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

  ////
  ////
  ////
  // ge all s3 ring
  // exp1(p1): s3 use rings 1..24 ; exp2(p2): s3 use only rings 1..8
  int n_s3 = 24;
  if(str.find("p2") != string::npos)      n_s3 = 8;
  else if(str.find("p1") != string::npos) n_s3 = 24;
  else{
    cout << "warning: str has neither p1 nor p2, s3 defaults to " << n_s3
         << " rings: " << str << endl;
  }
  cout << "[get_hist_add_all] str=" << str << "  ->  s3 rings = 1.." << n_s3 << endl;

  TH1D *h1_ga_s3r[24];   // keep array size 24 (large enough)
  TH1D *hh1_ga_s3r[24];
  k = 0;
  for(int i=1;i<=n_s3;i++){
    h1_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_s3_ring%d",i));
    hh1_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_s3_ring%d",i));

    k++;
  }

  TH1D *h1_ga_s3a = (TH1D*)h1_ga_s3r[0]->Clone("event_e_dc_r_ge_all_s3_all");
  TH1D *hh1_ga_s3a = (TH1D*)hh1_ga_s3r[0]->Clone("bg_e_dc_r_ge_all_s3_all");
  for(int i=1;i<n_s3;i++){
    h1_ga_s3a->Add(h1_ga_s3r[i], 1);
    hh1_ga_s3a->Add(hh1_ga_s3r[i], 1);
  }
  h1_ga_s3a->Write();
  hh1_ga_s3a->Write();

  //
  fi->Close();
  fo->Close();
}
