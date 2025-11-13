//

std::map<int, std::vector<double>> m_e_info;
std::map<int, bool> m_e_flag;
TFile *fi = TFile::Open("../../rootfile/doppler_100ns_p1_no_bg_hist_aa.root");

bool init_pars(string str="");
void fit_peak(ofstream &f, TH1D *h_event, TH1D *h_bg, int ring, double e);

//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
//
// pars = 9
// Gaussian + Left-Skew
Double_t gpeak(Double_t *v, Double_t *par);

// pars = 6
// Step + Pol1
Double_t bgstep(Double_t *v, Double_t *par);

// pars = 14
// Gaussian + Left-Skew + Right-Skew + Step + Background
Double_t gpeakexregion(Double_t *v, Double_t *par);

// pars = 22
// two Gaussian + two Left-Skew + two Right-Skew + two Step + Background
Double_t g2peakexregion(Double_t *v, Double_t *par);

// pars = 28
// three Gaussian + three Left-Skew + three Right-Skew + three Step + Background
Double_t g3peakexregion(Double_t *v, Double_t *par);
//
//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

//
void get_delta_aa()
{
  //
  if(fi->IsZombie()){
    cout << "can not open rootfile" << endl;
    return;
  }

  std::vector<string> v_hist_prefix = {
    "ge_all_spider_all"
    // "ge_ring3_spider_all",
  };

  vector<TH1D*> v_h_event, v_h_bg;
  int k = 0;

  // ge all, spider
  for(auto &prefix:v_hist_prefix){
    if(!init_pars(prefix)) return;
    cout << "m_e_info " << m_e_info.size() << endl;

    ofstream fo(Form("./%s_delta_aa.txt",prefix.c_str()));
    fo << "e   delta    s" << endl;

    for(auto &[key,val]:m_e_info){
      if(!(key==505)) continue;

      v_h_event.push_back((TH1D*)fi->Get(Form("event_e_dc_r_%s",prefix.c_str())));
      v_h_bg.push_back((TH1D*)fi->Get(Form("bg_e_dc_r_%s",prefix.c_str())));

      fit_peak(fo, v_h_event[k], v_h_bg[k], key, val[0]);

      k++;
    }

    fo.close();
  }

  v_h_event.clear();
  v_h_bg.clear();

  fi->Close();
}

//
void fit_peak(ofstream &f, TH1D *h_event, TH1D *h_bg, int ring, double e)
{
  gROOT->SetBatch(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11);

  TCanvas *cc = new TCanvas(Form("cc_%s_%04dkeV",std::string(h_event->GetName()).substr(13).c_str(),(int)e),"",500,400);
  cc->cd();
  cc->SetLogy();

  //
  // for(int i=1;i<=h_event->GetNbinsX();++i){
  //   if(h_event->GetBinContent(i)<0){
  //     h_event->SetBinContent(i, 0);
  //   }
  // }

  //
  double bin_width = h_event->GetBinWidth(1);
  double x_min = m_e_info[(int)e][1];
  double x_max = m_e_info[(int)e][2];

  h_event->GetXaxis()->SetTitle("energy [keV]");
  h_event->GetYaxis()->SetTitle("counts");
  h_event->GetXaxis()->SetRangeUser(x_min-10, x_max+10);
  h_event->SetMinimum(0.1);;
  h_event->SetTitle(Form("%s",std::string(h_event->GetName()).substr(13).c_str()));
  h_event->Draw();
  
  //
  Double_t par[14];
  par[0] = m_e_info[(int)e][0];
  par[1] = h_event->GetBinContent(((int)(e/bin_width)));
  par[2] = m_e_info[(int)e][7]; // gaussian, delta
  par[3] = m_e_info[(int)e][8]; // Left-Skew, a
  par[4] = m_e_info[(int)e][9]; // Left-Skew, beta
  par[5] = m_e_info[(int)e][10]; // Right-Skew, r
  par[6] = m_e_info[(int)e][11]; // Right-Skew, rho
  par[7] = m_e_info[(int)e][12]; // Step, s 
  par[8] = (h_event->GetBinContent(((int)(x_min/bin_width)))+h_event->GetBinContent(((int)(x_max/bin_width))))/2.; // Background
  par[9] = 0.;
  par[10] = m_e_info[(int)e][3];
  par[11] = m_e_info[(int)e][4];
  par[12] = m_e_info[(int)e][5];
  par[13] = m_e_info[(int)e][6];

  cout << "== fit info ==" << endl;
  cout << "energy " << par[0] << endl;
  cout << "gaussian gamma " << par[1] << endl;
  cout << "gaussian delta " << par[2] << endl;
  cout << "left-skew a " << par[3] << endl;
  cout << "left-skew beta " << par[4] << endl;
  cout << "right-skew r " << par[5] << endl;
  cout << "right-skew rho " << par[6] << endl;
  cout << "step s " << par[7] << endl;
  cout << "bg0, bg1" << par[8] << ", " << par[9] << endl;
  cout << "ex region x1, x2 " << par[10] << ", " << par[11] << endl;
  cout << "ex region x3, x4 " << par[12] << ", " << par[13] << endl;

  TF1 *gpeakexregion_tf = new TF1("gpeakexregion_tf", gpeakexregion, x_min, x_max, 14);
  gpeakexregion_tf->SetParameters(par);

  gpeakexregion_tf->SetParName(0, "Energy");
  gpeakexregion_tf->SetParName(1, "#Gamma");
  gpeakexregion_tf->SetParName(2, "#delta");
  gpeakexregion_tf->SetParName(3, "Left_skew A");
  gpeakexregion_tf->SetParName(4, "Left_skew #beta");
  gpeakexregion_tf->SetParName(5, "Right_skew R");
  gpeakexregion_tf->SetParName(6, "Right_skew #rho");
  gpeakexregion_tf->SetParName(7, "Step S");
  gpeakexregion_tf->SetParName(8, "bg p0");
  gpeakexregion_tf->SetParName(9, "bg p1");

  gpeakexregion_tf->SetParLimits(0, par[0]-2., par[0]+2.);
  gpeakexregion_tf->SetParLimits(2, 0.1, 10.);
  gpeakexregion_tf->SetParLimits(3, 0.001, 1.);
  gpeakexregion_tf->SetParLimits(4, 0.01, 10.);
  gpeakexregion_tf->SetParLimits(5, 0.001, 1.);
  gpeakexregion_tf->SetParLimits(6, 0.01, 10.);
  gpeakexregion_tf->SetParLimits(7, 0.001, 0.003);

  gpeakexregion_tf->FixParameter(10, par[10]);
  gpeakexregion_tf->FixParameter(11, par[11]);
  gpeakexregion_tf->FixParameter(12, par[12]);
  gpeakexregion_tf->FixParameter(13, par[13]);

  h_event->Fit("gpeakexregion_tf", "R");

  if(m_e_flag[(int)e]){
    f << e << "  " << gpeakexregion_tf->GetParameter(2) << "  " << gpeakexregion_tf->GetParameter(7) << endl; 

    TLatex *lt_good = new TLatex(0.05, 0.92, "Good");
    lt_good->SetNDC();
    lt_good->SetTextSize(0.09);
    lt_good->SetTextColor(kRed);
    lt_good->Draw();
  }else{
    TLatex *lt_bad = new TLatex(0.05, 0.92, "Bad");
    lt_bad->SetNDC();
    lt_bad->SetTextSize(0.09);
    lt_bad->SetTextColor(kBlue);
    lt_bad->Draw();
  }

  //
  double energy = gpeakexregion_tf->GetParameter(0); 
  double gamma = gpeakexregion_tf->GetParameter(1);
  double delta = gpeakexregion_tf->GetParameter(2);

  double xll = energy-delta/sqrt(2)*2.5;
  double xrr = energy+delta/sqrt(2)*2.5;

  TLine *l_xl_sigma = new TLine(xll, 0.1, xll, 1000);
  l_xl_sigma->SetLineColor(9);
  l_xl_sigma->SetLineWidth(2);
  l_xl_sigma->SetLineStyle(2);
  l_xl_sigma->Draw("same");
  
  TLine *l_xr_sigma = new TLine(xrr, 0.1, xrr, 1000);
  l_xr_sigma->SetLineColor(9);
  l_xr_sigma->SetLineWidth(2);
  l_xr_sigma->SetLineStyle(2);
  l_xr_sigma->Draw("same");

  cc->SaveAs(Form("./fig/%s.png",cc->GetName()));

  delete gpeakexregion_tf;
}

//
bool init_pars(string str="")
{
  // read fit par
  ifstream fi_fit_par(Form("./fit_pars/%s.txt",str.c_str())); 
  if(!fi_fit_par){
    std::cout << "can not open fit_spider_pars.txt" << std::endl;
    return 0;
  }
  m_e_info.clear();
  m_e_flag.clear();

  std::string line;
  std::getline(fi_fit_par, line);
  
  double e,x_min,x_max,ex_x1,ex_x2,ex_x3,ex_x4,delta,a,beta,r,rho,s;
  bool flag;
  while(fi_fit_par>>e>>x_min>>x_max>>ex_x1>>ex_x2>>ex_x3>>ex_x4>>delta>>a>>beta>>r>>rho>>s>>flag){
    m_e_info.insert({(int)e, {e,x_min,x_max,ex_x1,ex_x2,ex_x3,ex_x4,delta,a,beta,r,rho,s}});
    m_e_flag.insert({(int)e, flag});
  }
  fi_fit_par.close();
  
  for(auto &[key,val]:m_e_info){
    cout << "e ==> " << key << endl;
    cout << " e " << val[0] << "keV, fit range [" << val[1] << ", " << val[2] << "], ex_region {[";
    cout << val[3] << ", " << val[4] << "]; [" << val[5] << ", " << val[6] << "]} " << endl;
    cout << "delta=" << val[7] << " a=" << val[8] << "  beta=" << val[9] << "  r=" << val[10] << "  rho=" << val[11] << endl;
    cout << "s=" << val[12] << endl;
  }
  for(auto &[key,val]:m_e_flag){
    cout << "e ==> " << key << endl;
    cout << "flag=" << val << endl;
  }

  return 1;
}

//
// pars = 7
// Gaussian + Left-Skew + Right-Skew
Double_t gpeak(Double_t *v, Double_t *par)
{
  Double_t energy  = par[0];
  Double_t gamma  = par[1];

  Double_t delta = par[2]; // Gaussian
  Double_t a = par[3]; // Left-Skew
  Double_t beta = par[4]; // Left-Skew
  Double_t r = par[5]; // Right-Skew
  Double_t rho = par[6]; // Right-Skew
 
  Double_t gaussian = gamma*exp(-pow((v[0]-energy)/delta, 2.));
  Double_t left_skew = gamma*1/2.*a*exp((v[0]-energy)/beta);
  left_skew *= erfc(delta/2./beta + (v[0]-energy)/delta);
  Double_t right_skew = gamma*1/2.*r*exp(-(v[0]-energy)/rho);
  right_skew *= erfc(delta/2./rho - (v[0]-energy)/delta);

  //
  Double_t result = gaussian+left_skew+right_skew;

  return result;
}

// pars 6
// Step + Pol1
Double_t bgstep(Double_t *v, Double_t *par)
{
  Double_t p0 = par[0];
  Double_t p1 = par[1];
  Double_t energy = par[2];
  Double_t gamma = par[3];
  Double_t delta = par[4];
  Double_t s = par[5];

  Double_t result = p0 + p1*v[0];
  result += gamma*s*1/2.*erfc((v[0]-energy)/delta);

  return result;
}

// pars = 14
// Gaussian + Left-Skew + Step + Background
Double_t gpeakexregion(Double_t *v, Double_t *par)
{
  if(v[0]>par[10] && v[0]<par[11]){// exclude region one
    TF1::RejectPoint();
    return 0;
  }
  if(v[0]>par[12] && v[0]<par[13]){// exclude region two
    TF1::RejectPoint();
    return 0;
  }

  Double_t energy  = par[0];
  Double_t gamma  = par[1];

  Double_t delta = par[2]; // Gaussian
  Double_t a = par[3]; // Left-Skew
  Double_t beta = par[4]; // Left-Skew
  Double_t r = par[5]; // Right-Skew
  Double_t rho = par[6]; // Right-Skew
  Double_t s = par[7]; // Step
  Double_t bg0 = par[8]; // Background
  Double_t bg1 = par[9]; // Background

  Double_t gaussian = gamma*exp(-pow((v[0]-energy)/delta, 2.));
  Double_t left_skew = gamma*1/2.*a*exp((v[0]-energy)/beta);
  left_skew *= erfc(delta/2./beta + (v[0]-energy)/delta);
  Double_t right_skew = gamma*1/2.*r*exp(-(v[0]-energy)/rho);
  right_skew *= erfc(delta/2./rho - (v[0]-energy)/delta);
  Double_t step = gamma*s*1/2.*erfc((v[0]-energy)/delta);
  Double_t background = bg0+bg1*v[0];

  //
  Double_t result = gaussian+left_skew+right_skew+step+background;

  return result;
}

//
// pars = 22
// two Gaussian + two Left-Skew + two Right-Skew + two Step + Background
Double_t g2peakexregion(Double_t *v, Double_t *par)
{
  if(v[0]>par[18] && v[0]<par[19]){// exclude region one
    TF1::RejectPoint();
    return 0;
  }
  if(v[0]>par[20] && v[0]<par[21]){// exclude region two
    TF1::RejectPoint();
    return 0;
  }

  // peak1
  Double_t energy1  = par[0];
  Double_t gamma1  = par[1];
  Double_t delta1 = par[2]; // Gaussian
  Double_t a1 = par[3]; // Left-Skew
  Double_t beta1 = par[4]; // Left-Skew
  Double_t r1 = par[5]; // Right-Skew
  Double_t rho1 = par[6]; // Right-Skew
  Double_t s1 = par[7]; // Step

  // peak2
  Double_t energy2  = par[8];
  Double_t gamma2  = par[9];
  Double_t delta2 = par[10]; // Gaussian
  Double_t a2 = par[11]; // Left-Skew
  Double_t beta2 = par[12]; // Left-Skew
  Double_t r2 = par[13]; // Right-Skew
  Double_t rho2 = par[14]; // Right-Skew
  Double_t s2 = par[15]; // Step

  // Background
  Double_t bg0 = par[16];
  Double_t bg1 = par[17];

  // peak1
  Double_t gaussian1 = gamma1*exp(-pow((v[0]-energy1)/delta1, 2.));
  Double_t left_skew1 = gamma1*1/2.*a1*exp((v[0]-energy1)/beta1);
  left_skew1 *= erfc(delta1/2./beta1 + (v[0]-energy1)/delta1);
  Double_t right_skew1 = gamma1*1/2.*r1*exp(-(v[0]-energy1)/rho1);
  right_skew1 *= erfc(delta1/2./rho1 - (v[0]-energy1)/delta1);
  Double_t step1 = gamma1*s1*1/2.*erfc((v[0]-energy1)/delta1);

  // peak2
  Double_t gaussian2 = gamma2*exp(-pow((v[0]-energy2)/delta2, 2.));
  Double_t left_skew2 = gamma2*1/2.*a2*exp((v[0]-energy2)/beta2);
  left_skew2 *= erfc(delta2/2./beta2 + (v[0]-energy2)/delta2);
  Double_t right_skew2 = gamma2*1/2.*r2*exp(-(v[0]-energy2)/rho2);
  right_skew2 *= erfc(delta2/2./rho2 - (v[0]-energy2)/delta2);
  Double_t step2 = gamma2*s2*1/2.*erfc((v[0]-energy2)/delta2);

  // Background
  Double_t background = bg0+bg1*v[0];

  //
  Double_t result = gaussian1+left_skew1+right_skew1+step1
                  + gaussian2+left_skew2+right_skew2+step2
                  + background;

  return result;
}

//
// pars = 30
// three Gaussian + three Left-Skew + three Right-Skew + three Step + Background
Double_t g3peakexregion(Double_t *v, Double_t *par)
{
  if(v[0]>par[26] && v[0]<par[27]){// exclude region one
    TF1::RejectPoint();
    return 0;
  }
  if(v[0]>par[28] && v[0]<par[29]){// exclude region two
    TF1::RejectPoint();
    return 0;
  }

  // peak1
  Double_t energy1  = par[0];
  Double_t gamma1  = par[1];
  Double_t delta1 = par[2]; // Gaussian
  Double_t a1 = par[3]; // Left-Skew
  Double_t beta1 = par[4]; // Left-Skew
  Double_t r1 = par[5]; // Right-Skew
  Double_t rho1 = par[6]; // Right-Skew
  Double_t s1 = par[7]; // Step

  // peak2
  Double_t energy2  = par[8];
  Double_t gamma2  = par[9];
  Double_t delta2 = par[10]; // Gaussian
  Double_t a2 = par[11]; // Left-Skew
  Double_t beta2 = par[12]; // Left-Skew
  Double_t r2 = par[13]; // Right-Skew
  Double_t rho2 = par[14]; // Right-Skew
  Double_t s2 = par[15]; // Step

  // peak3
  Double_t energy3  = par[16];
  Double_t gamma3  = par[17];
  Double_t delta3 = par[18]; // Gaussian
  Double_t a3 = par[19]; // Left-Skew
  Double_t beta3 = par[20]; // Left-Skew
  Double_t r3 = par[21]; // Right-Skew
  Double_t rho3 = par[22]; // Right-Skew
  Double_t s3 = par[23]; // Step

  // Background
  Double_t bg0 = par[24];
  Double_t bg1 = par[25];

  // peak1
  Double_t gaussian1 = gamma1*exp(-pow((v[0]-energy1)/delta1, 2.));
  Double_t left_skew1 = gamma1*1/2.*a1*exp((v[0]-energy1)/beta1);
  left_skew1 *= erfc(delta1/2./beta1 + (v[0]-energy1)/delta1);
  Double_t right_skew1 = gamma1*1/2.*r1*exp(-(v[0]-energy1)/rho1);
  right_skew1 *= erfc(delta1/2./rho1 - (v[0]-energy1)/delta1);
  Double_t step1 = gamma1*s1*1/2.*erfc((v[0]-energy1)/delta1);

  // peak2
  Double_t gaussian2 = gamma2*exp(-pow((v[0]-energy2)/delta2, 2.));
  Double_t left_skew2 = gamma2*1/2.*a2*exp((v[0]-energy2)/beta2);
  left_skew2 *= erfc(delta2/2./beta2 + (v[0]-energy2)/delta2);
  Double_t right_skew2 = gamma2*1/2.*r2*exp(-(v[0]-energy2)/rho2);
  right_skew2 *= erfc(delta2/2./rho2 - (v[0]-energy2)/delta2);
  Double_t step2 = gamma2*s2*1/2.*erfc((v[0]-energy2)/delta2);

  // peak3
  Double_t gaussian3 = gamma3*exp(-pow((v[0]-energy3)/delta3, 2.));
  Double_t left_skew3 = gamma3*1/2.*a3*exp((v[0]-energy3)/beta3);
  left_skew3 *= erfc(delta3/2./beta3 + (v[0]-energy3)/delta3);
  Double_t right_skew3 = gamma3*1/2.*r3*exp(-(v[0]-energy3)/rho3);
  right_skew3 *= erfc(delta3/2./rho3 - (v[0]-energy3)/delta3);
  Double_t step3 = gamma3*s3*1/2.*erfc((v[0]-energy3)/delta3);

  // Background
  Double_t background = bg0+bg1*v[0];

  //
  Double_t result = gaussian1+left_skew1+right_skew1+step1
                  + gaussian2+left_skew2+right_skew2+step2
                  + gaussian3+left_skew3+right_skew3+step3
                  + background;

  return result;
}

