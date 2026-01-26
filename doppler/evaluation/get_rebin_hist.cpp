#define E_DC_R
#define E_RAW

// event-bg hist
// example str = "doppler_100ns_p1", "doppler_100ns_p1_no_bg"
void get_rebin_hist(string str="")
{
  gROOT->SetBatch(1);
  gStyle->SetOptStat(0);

  // spider
  std::vector<int> spider_rings;
  if(str.find("p1") != std::string::npos){
    spider_rings = {1, 3, 5, 7};
  }else if(str.find("p2") != std::string::npos){
    spider_rings = {1, 2, 3, 4, 5, 6, 7, 8};
  }else{
    cout << "wrong str" << endl;
    return;
  }

  // s3
  std::vector<int> s3_rings;
  if(str.find("p1") != std::string::npos){
    s3_rings = {1, 5, 9, 13, 17, 21};
  }else if(str.find("p2") != std::string::npos){
    s3_rings = {1, 3, 5, 7};
  }else{
    cout << "wrong str" << endl;
    return;
  }

  //
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
  TFile *fo = new TFile(Form("../../rootfile/doppler_%s_hist_rb.root",str.c_str()), "recreate");
  fo->cd();

#ifdef E_DC_R
  ////
  ////
  //// e_dc_r
  // ge all, spider ring
  TH1D *h1_ga_spr[8];
  TH1D *hh1_ga_spr[8];
  k = 0;
  for(int i=1;i<=8;i++){
    h1_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_spider_ring%d",i));
    hh1_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_spider_ring%d",i));

    k++;
  }

  int nn = (int)(spider_rings[1]-spider_rings[0]);
  cout << str << " spider rebin ring " << nn << endl;
  for(int i=0;i<8;i++){
    if(i%nn==0){
      for(int j=1;j<nn;j++){
        h1_ga_spr[i]->Add(h1_ga_spr[i+j]);
        hh1_ga_spr[i]->Add(hh1_ga_spr[i+j]);
      }
    }
  }

  for(int i=0;i<(int)spider_rings.size();i++){
    h1_ga_spr[spider_rings[i]-1]->Write();
    hh1_ga_spr[spider_rings[i]-1]->Write();
  }
  
  // ge all, s3 ring
  TH1D *h1_ga_s3r[24];
  TH1D *hh1_ga_s3r[24];
  k = 0;
  for(int i=1;i<=24;i++){
    h1_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_s3_ring%d",i));
    hh1_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_s3_ring%d",i));

    k++;
  }
  
  nn = (int)(s3_rings[1]-s3_rings[0]);
  cout << str << " s3 rebin ring " << nn << endl;

  for(int i=0;i<24;i++){
    if(i%nn==0){
      for(int j=1;j<nn;j++){
        h1_ga_s3r[i]->Add(h1_ga_s3r[i+j]);
        hh1_ga_s3r[i]->Add(hh1_ga_s3r[i+j]);
      }
    }  
  }

  for(int i=0;i<(int)s3_rings.size();i++){
    h1_ga_s3r[s3_rings[i]-1]->Write();
    hh1_ga_s3r[s3_rings[i]-1]->Write();
  }
#endif

#ifdef E_RAW
  ////
  //// e_raw
  // ge all, spider ring
  TH1D *h2_ga_spr[8];
  TH1D *hh2_ga_spr[8];
  k = 0;
  for(int i=1;i<=8;i++){
    h2_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_spider_ring%d",i));
    hh2_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_spider_ring%d",i));

    k++;
  }

  nn = (int)(spider_rings[1]-spider_rings[0]);
  cout << str << " spider rebin ring " << nn << endl;
  for(int i=0;i<8;i++){
    if(i%nn==0){
      for(int j=1;j<nn;j++){
        h2_ga_spr[i]->Add(h2_ga_spr[i+j]);
        hh2_ga_spr[i]->Add(hh2_ga_spr[i+j]);
      }
    }
  }

  for(int i=0;i<(int)spider_rings.size();i++){
    h2_ga_spr[spider_rings[i]-1]->Write();
    hh2_ga_spr[spider_rings[i]-1]->Write();
  }
  
  // ge all, s3 ring
  TH1D *h2_ga_s3r[24];
  TH1D *hh2_ga_s3r[24];
  k = 0;
  for(int i=1;i<=24;i++){
    h2_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_s3_ring%d",i));
    hh2_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_s3_ring%d",i));

    k++;
  }
  
  nn = (int)(s3_rings[1]-s3_rings[0]);
  cout << str << " s3 rebin ring " << nn << endl;

  for(int i=0;i<24;i++){
    if(i%nn==0){
      for(int j=1;j<nn;j++){
        h2_ga_s3r[i]->Add(h2_ga_s3r[i+j]);
        hh2_ga_s3r[i]->Add(hh2_ga_s3r[i+j]);
      }
    }  
  }

  for(int i=0;i<(int)s3_rings.size();i++){
    h2_ga_s3r[s3_rings[i]-1]->Write();
    hh2_ga_s3r[s3_rings[i]-1]->Write();
  }
#endif

  fi->Close();
  fo->Close();
}
