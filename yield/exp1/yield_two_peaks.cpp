//

std::map<std::pair<int,int>, std::vector<double>> m_r_2e_info;
std::map<std::pair<int,int>, bool> m_r_e_flag;
TFile *fi = TFile::Open("../../rootfile/doppler_100ns_p1_no_bg_hist.root");

bool init_pars(string str="");
void fit_two_peaks(ofstream &f, TH1D *h_event, TH1D *h_bg, int ring, double e1, double e2);

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
void yield_two_peaks()
{
  //
  if(fi->IsZombie()){
    cout << "can not open rootfile" << endl;
    return;
  }

  std::vector<string> v_hist_prefix = {
    "ge_all_spider"
    // "ge_ring3_spider",
    // "ge_ring4_spider"
  };

  vector<TH1D*> v_h_event, v_h_bg;
  int k = 0;

  // ge all, spider
  for(auto &prefix:v_hist_prefix){
    if(!init_pars(prefix)) return;
    cout << "m_r_2e_info " << m_r_2e_info.size() << endl;

    ofstream fo(Form("./result/%s_two_peaks.yld",prefix.c_str()));
    fo << "ring_id   e   area   error" << endl;

    for(auto &[key,val]:m_r_2e_info){
      if(!(key.first==6 && key.second==1174)) continue;

      v_h_event.push_back((TH1D*)fi->Get(Form("event_e_dc_r_%s_ring%d",prefix.c_str(),key.first)));
      v_h_bg.push_back((TH1D*)fi->Get(Form("bg_e_dc_r_%s_ring%d",prefix.c_str(),key.first)));

      fit_two_peaks(fo, v_h_event[k], v_h_bg[k], key.first, val[0], val[1]);

      k++;
    }
    fo.close();
  }

   // v_h_event.clear();
   // v_h_bg.clear();

   // fi->Close();
}

//
void fit_two_peaks(ofstream &f, TH1D *h_event, TH1D *h_bg, int ring, double e1, double e2)
{
  // gROOT->SetBatch(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11);

  int e = (int)(e1+e2);
  TCanvas *cc = new TCanvas(Form("cc_%s_%04dkeV_%04dkeV",std::string(h_event->GetName()).substr(13).c_str(),(int)(e1),(int)(e2)),"",500,400);
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
  double x_min = m_r_2e_info[std::make_pair(ring, (int)e)][2];
  double x_max = m_r_2e_info[std::make_pair(ring, (int)e)][3];

  h_event->GetXaxis()->SetTitle("energy [keV]");
  h_event->GetYaxis()->SetTitle("counts");
  h_event->GetXaxis()->SetRangeUser(x_min-10, x_max+10);
  h_event->SetMinimum(0.1);;
  h_event->SetTitle(Form("%s",std::string(h_event->GetName()).substr(13).c_str()));
  h_event->Draw();
  
  //
  Double_t par[22];
  // peak1
  par[0] = m_r_2e_info[std::make_pair(ring, e)][0];
  par[1] = h_event->GetBinContent(((int)(e1/bin_width)));
  par[2] = m_r_2e_info[std::make_pair(ring, e)][8]; // gaussian, delta
  par[3] = m_r_2e_info[std::make_pair(ring, e)][9]; // Left-Skew, a
  par[4] = m_r_2e_info[std::make_pair(ring, e)][10]; // Left-Skew, beta
  par[5] = m_r_2e_info[std::make_pair(ring, e)][11]; // Right-Skew, r
  par[6] = m_r_2e_info[std::make_pair(ring, e)][12]; // Right-Skew, rho
  par[7] = m_r_2e_info[std::make_pair(ring, e)][13]; // Step, s 
  par[8] = m_r_2e_info[std::make_pair(ring, e)][1];
  par[9] = h_event->GetBinContent(((int)(e1/bin_width)));
  par[10] = m_r_2e_info[std::make_pair(ring, e)][14]; // gaussian, delta
  par[11] = m_r_2e_info[std::make_pair(ring, e)][15]; // Left-Skew, a
  par[12] = m_r_2e_info[std::make_pair(ring, e)][16]; // Left-Skew, beta
  par[13] = m_r_2e_info[std::make_pair(ring, e)][17]; // Right-Skew, r
  par[14] = m_r_2e_info[std::make_pair(ring, e)][18]; // Right-Skew, rho
  par[15] = m_r_2e_info[std::make_pair(ring, e)][19]; // Step, s 
  par[16] = (h_event->GetBinContent(((int)(x_min/bin_width)))+h_event->GetBinContent(((int)(x_max/bin_width))))/2.; // Background
  par[17] = 0.;
  par[18] = m_r_2e_info[std::make_pair(ring, e)][4];
  par[19] = m_r_2e_info[std::make_pair(ring, e)][5];
  par[20] = m_r_2e_info[std::make_pair(ring, e)][6];
  par[21] = m_r_2e_info[std::make_pair(ring, e)][7];

  cout << "== fit info ==" << endl;
  cout << "energy1 " << par[0] << endl;
  cout << "gaussian gamma1 " << par[1] << endl;
  cout << "gaussian delta1 " << par[2] << endl;
  cout << "left-skew a1 " << par[3] << endl;
  cout << "left-skew beta1 " << par[4] << endl;
  cout << "right-skew r1 " << par[5] << endl;
  cout << "right-skew rho1 " << par[6] << endl;
  cout << "step s1 " << par[7] << endl;
  cout << "energy2 " << par[8] << endl;
  cout << "gaussian gamma2 " << par[9] << endl;
  cout << "gaussian delta2 " << par[10] << endl;
  cout << "left-skew a2 " << par[11] << endl;
  cout << "left-skew beta2 " << par[12] << endl;
  cout << "right-skew r2 " << par[13] << endl;
  cout << "right-skew rho2 " << par[14] << endl;
  cout << "step s2 " << par[15] << endl;
  cout << "bg0, bg1" << par[16] << ", " << par[17] << endl;
  cout << "ex region x1, x2 " << par[18] << ", " << par[19] << endl;
  cout << "ex region x3, x4 " << par[20] << ", " << par[21] << endl;

  TF1 *g2peakexregion_tf = new TF1("g2peakexregion_tf", g2peakexregion, x_min, x_max, 22);
  g2peakexregion_tf->SetParameters(par);

  g2peakexregion_tf->SetParName(0, "Energy1");
  g2peakexregion_tf->SetParName(1, "#Gamma1");
  g2peakexregion_tf->SetParName(2, "#delta1");
  g2peakexregion_tf->SetParName(3, "Left_skew A1");
  g2peakexregion_tf->SetParName(4, "Left_skew #beta1");
  g2peakexregion_tf->SetParName(5, "Right_skew R1");
  g2peakexregion_tf->SetParName(6, "Right_skew #rho1");
  g2peakexregion_tf->SetParName(7, "Step S1");
  g2peakexregion_tf->SetParName(8, "Energy2");
  g2peakexregion_tf->SetParName(9, "#Gamma2");
  g2peakexregion_tf->SetParName(10, "#delta2");
  g2peakexregion_tf->SetParName(11, "Left_skew A2");
  g2peakexregion_tf->SetParName(12, "Left_skew #beta2");
  g2peakexregion_tf->SetParName(13, "Right_skew R2");
  g2peakexregion_tf->SetParName(14, "Right_skew #rho2");
  g2peakexregion_tf->SetParName(15, "Step S2");
  g2peakexregion_tf->SetParName(16, "bg p0");
  g2peakexregion_tf->SetParName(17, "bg p1");
  
  g2peakexregion_tf->SetParLimits(0, par[0]-2., par[0]+2.);
  g2peakexregion_tf->SetParLimits(2, 0.8, 10.);
  g2peakexregion_tf->SetParLimits(3, 0.001, 1.);
  g2peakexregion_tf->SetParLimits(4, 0.01, 10.);
  g2peakexregion_tf->SetParLimits(5, 0.001, 1.);
  g2peakexregion_tf->SetParLimits(6, 0.01, 10.);
  g2peakexregion_tf->SetParLimits(7, 0.00001, 0.05);

  g2peakexregion_tf->SetParLimits(8, par[8]-2., par[8]+2.);
  g2peakexregion_tf->SetParLimits(10, 0.8, 10.);
  g2peakexregion_tf->SetParLimits(11, 0.001, 1.);
  g2peakexregion_tf->SetParLimits(12, 0.01, 10.);
  g2peakexregion_tf->SetParLimits(13, 0.001, 1.);
  g2peakexregion_tf->SetParLimits(14, 0.01, 10.);
  g2peakexregion_tf->SetParLimits(15, 0.00001, 0.05);

  g2peakexregion_tf->FixParameter(18, par[18]);
  g2peakexregion_tf->FixParameter(19, par[19]);
  g2peakexregion_tf->FixParameter(20, par[20]);
  g2peakexregion_tf->FixParameter(21, par[21]);

  h_event->Fit("g2peakexregion_tf", "RQ");

  //
  TF1 *gpeak_tf1 = new TF1("gpeak_tf1", gpeak, x_min, x_max, 7);
  gpeak_tf1->SetParameter(0, g2peakexregion_tf->GetParameter(0));
  gpeak_tf1->SetParameter(1, g2peakexregion_tf->GetParameter(1));
  gpeak_tf1->SetParameter(2, g2peakexregion_tf->GetParameter(2));
  gpeak_tf1->SetParameter(3, g2peakexregion_tf->GetParameter(3));
  gpeak_tf1->SetParameter(4, g2peakexregion_tf->GetParameter(4));
  gpeak_tf1->SetParameter(5, g2peakexregion_tf->GetParameter(5));
  gpeak_tf1->SetParameter(6, g2peakexregion_tf->GetParameter(6));

  gpeak_tf1->SetLineColor(3);
  gpeak_tf1->Draw("same");

  //
  TF1 *gpeak_tf2 = new TF1("gpeak_tf2", gpeak, x_min, x_max, 7);
  gpeak_tf2->SetParameter(0, g2peakexregion_tf->GetParameter(8));
  gpeak_tf2->SetParameter(1, g2peakexregion_tf->GetParameter(9));
  gpeak_tf2->SetParameter(2, g2peakexregion_tf->GetParameter(10));
  gpeak_tf2->SetParameter(3, g2peakexregion_tf->GetParameter(11));
  gpeak_tf2->SetParameter(4, g2peakexregion_tf->GetParameter(12));
  gpeak_tf2->SetParameter(5, g2peakexregion_tf->GetParameter(13));
  gpeak_tf2->SetParameter(6, g2peakexregion_tf->GetParameter(14));

  gpeak_tf2->SetLineColor(3);
  gpeak_tf2->Draw("same");

  //
  TF1 *bgstep_tf1 = new TF1("bgstep_tf1", bgstep, x_min, x_max, 6);
  bgstep_tf1->SetParameter(0, g2peakexregion_tf->GetParameter(16));
  bgstep_tf1->SetParameter(1, g2peakexregion_tf->GetParameter(17));
  bgstep_tf1->SetParameter(2, g2peakexregion_tf->GetParameter(0)); // energy
  bgstep_tf1->SetParameter(3, g2peakexregion_tf->GetParameter(1)); // gamma
  bgstep_tf1->SetParameter(4, g2peakexregion_tf->GetParameter(2)); // delta
  bgstep_tf1->SetParameter(5, g2peakexregion_tf->GetParameter(7)); // s
  bgstep_tf1->SetLineColor(5);
  bgstep_tf1->Draw("same");

  //
  TF1 *bgstep_tf2 = new TF1("bgstep_tf2", bgstep, x_min, x_max, 6);
  bgstep_tf2->SetParameter(0, g2peakexregion_tf->GetParameter(16));
  bgstep_tf2->SetParameter(1, g2peakexregion_tf->GetParameter(17));
  bgstep_tf2->SetParameter(2, g2peakexregion_tf->GetParameter(8)); // energy
  bgstep_tf2->SetParameter(3, g2peakexregion_tf->GetParameter(9)); // gamma
  bgstep_tf2->SetParameter(4, g2peakexregion_tf->GetParameter(10)); // delta
  bgstep_tf2->SetParameter(5, g2peakexregion_tf->GetParameter(15)); // s
  bgstep_tf2->SetLineColor(5);
  bgstep_tf2->Draw("same");

  //
  // peak1
  double energy1 = gpeak_tf1->GetParameter(0); 
  double gamma1 = gpeak_tf1->GetParameter(1);
  double delta1 = gpeak_tf1->GetParameter(2);
  double a1 = gpeak_tf1->GetParameter(3);
  double beta1 = gpeak_tf1->GetParameter(4);
  double r1 = gpeak_tf1->GetParameter(5);
  double rho1 = gpeak_tf1->GetParameter(6);

  double xll1 = energy1-delta1/sqrt(2)*2.5;
  double xrr1 = energy1+delta1/sqrt(2)*2.5;
  cout << "xll1 " << xll1 << " xrr1 " << xrr1 << endl;
  cout << "intrgral tf event " << g2peakexregion_tf->Integral(xll1, xrr1) << endl;
  cout << "intrgral tf bg " << bgstep_tf1->Integral(xll1, xrr1) << endl;
  cout << "intrgral event " << h_event->Integral(xll1, xrr1) << endl;
  cout << "intrgral bg " << h_bg->Integral(xll1, xrr1) << endl;

  double sum1 = delta1*sqrt(TMath::Pi());
  sum1 += a1*beta1*exp(-delta1*delta1/4./beta1/beta1);
  sum1 += r1*rho1*exp(-delta1*delta1/4./rho1/rho1);
  sum1 *= gamma1;

  double bg1 = bgstep_tf1->Integral(xll1, xrr1);
  double bg_random1 = h_bg->Integral(xll1, xrr1);
  double sum1_error = sqrt(sum1+2*bg1+2*bg_random1);

  TLine *l_xl_sigma1 = new TLine(xll1, 0.1, xll1, 1000);
  l_xl_sigma1->SetLineColor(9);
  l_xl_sigma1->SetLineWidth(2);
  l_xl_sigma1->SetLineStyle(2);
  l_xl_sigma1->Draw("same");
  
  TLine *l_xr_sigma1 = new TLine(xrr1, 0.1, xrr1, 1000);
  l_xr_sigma1->SetLineColor(9);
  l_xr_sigma1->SetLineWidth(2);
  l_xr_sigma1->SetLineStyle(2);
  l_xr_sigma1->Draw("same");

  //
  // peak2
  double energy2 = gpeak_tf2->GetParameter(0); 
  double gamma2 = gpeak_tf2->GetParameter(1);
  double delta2 = gpeak_tf2->GetParameter(2);
  double a2 = gpeak_tf2->GetParameter(3);
  double beta2 = gpeak_tf2->GetParameter(4);
  double r2 = gpeak_tf2->GetParameter(5);
  double rho2 = gpeak_tf2->GetParameter(6);

  double xll2 = energy2-delta2/sqrt(2)*2.5;
  double xrr2 = energy2+delta2/sqrt(2)*2.5;
  cout << "xll2 " << xll2 << " xrr2 " << xrr2 << endl;
  cout << "intrgral tf event " << g2peakexregion_tf->Integral(xll2, xrr2) << endl;
  cout << "intrgral tf bg " << bgstep_tf2->Integral(xll2, xrr2) << endl;
  cout << "intrgral event " << h_event->Integral(xll2, xrr2) << endl;
  cout << "intrgral bg " << h_bg->Integral(xll2, xrr2) << endl;

  double sum2 = delta2*sqrt(TMath::Pi());
  sum2 += a2*beta2*exp(-delta2*delta2/4./beta2/beta2);
  sum2 += r2*rho2*exp(-delta2*delta2/4./rho2/rho2);
  sum2 *= gamma2;

  double bg2 = bgstep_tf2->Integral(xll2, xrr2);
  double bg_random2 = h_bg->Integral(xll2, xrr2);
  double sum2_error = sqrt(sum2+2*bg2+2*bg_random2);

  TLine *l_xl_sigma2 = new TLine(xll2, 0.1, xll2, 1000);
  l_xl_sigma2->SetLineColor(9);
  l_xl_sigma2->SetLineWidth(2);
  l_xl_sigma2->SetLineStyle(2);
  l_xl_sigma2->Draw("same");
  
  TLine *l_xr_sigma2 = new TLine(xrr2, 0.1, xrr2, 1000);
  l_xr_sigma2->SetLineColor(9);
  l_xr_sigma2->SetLineWidth(2);
  l_xr_sigma2->SetLineStyle(2);
  l_xr_sigma2->Draw("same");

  //
  TLatex *lt_e1 = new TLatex(0.15, 0.85, Form("energy1 %0.1f keV, area %0.1f (%0.1f), integral %0.1f",gpeak_tf1->GetParameter(0),sum1,sum1_error,h_event->Integral(xll1,xrr1)));
  lt_e1->SetNDC();
  lt_e1->SetTextSize(0.04);
  lt_e1->SetTextColor(kBlack);
  lt_e1->Draw();

  TLatex *lt_e2 = new TLatex(0.15, 0.8, Form("energy2 %0.1f keV, area %0.1f (%0.1f), integral %0.1f",gpeak_tf2->GetParameter(0),sum2,sum2_error,h_event->Integral(xll2,xrr2)));
  lt_e2->SetNDC();
  lt_e2->SetTextSize(0.04);
  lt_e2->SetTextColor(kBlack);
  lt_e2->Draw();


  if(m_r_e_flag[std::make_pair(ring, (int)e)]){
    f << ring << "  " << e1 << "  " << sum1 << "  " << sum1_error << endl;  
    f << ring << "  " << e2 << "  " << sum2 << "  " << sum2_error << endl;  

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

  cc->SaveAs(Form("./fig/%s.png",cc->GetName()));

  // delete g2peakexregion_tf;
  // delete gpeak_tf1;
  // delete gpeak_tf2;
  // delete bgstep_tf1;
  // delete bgstep_tf2;
}

//
bool init_pars(string str="")
{
  // read fit par
  ifstream fi_fit_sp_par(Form("./fit_pars/%s_two_peaks.txt",str.c_str())); 
  if(!fi_fit_sp_par){
    std::cout << "can not open " << str << "_two_peaks.txt" << std::endl;
    return 0;
  }
  m_r_2e_info.clear();
  m_r_e_flag.clear();

  std::string line;
  std::getline(fi_fit_sp_par, line);
  
  int ring;
  double e1,e2,x_min,x_max,ex_x1,ex_x2,ex_x3,ex_x4,delta1,a1,beta1,r1,rho1,s1,delta2,a2,beta2,r2,rho2,s2;
  bool flag;
  while(fi_fit_sp_par>>ring>>e1>>e2>>x_min>>x_max>>ex_x1>>ex_x2>>ex_x3>>ex_x4>>delta1>>a1>>beta1>>r1>>rho1>>s1>>delta2>>a2>>beta2>>r2>>rho2>>s2>>flag){
    m_r_2e_info.insert({{ring,(int)(e1+e2)}, {e1,e2,x_min,x_max,ex_x1,ex_x2,ex_x3,ex_x4,delta1,a1,beta1,r1,rho1,s1,delta2,a2,beta2,r2,rho2,s2}});
    m_r_e_flag.insert({{ring,(int)(e1+e2)}, flag});
  }
  fi_fit_sp_par.close();
  
  for(auto &[key,val]:m_r_2e_info){
    cout << "ring " << key.first << " ==> " << key.second << endl;
    cout << " e1 " << val[0] << " e2 " << val[1] << "keV, fit range [" << val[2] << ", " << val[3] << "], ex_region {[";
    cout << val[4] << ", " << val[5] << "]; [" << val[6] << ", " << val[7] << "]} " << endl;
    cout << "delta1=" << val[8] << " a1=" << val[9] << "  beta1=" << val[10] << "  r1=" << val[11] << "  rho1=" << val[12] << "  s1=" << val[13] << endl;
    cout << "delta2=" << val[14] << " a2=" << val[15] << "  beta2=" << val[16] << "  r2=" << val[17] << "  rho2=" << val[18] << "  s2=" << val[19] << endl;
  }

  for(auto &[key,val]:m_r_e_flag){
    cout << "ring " << key.first << " ==> " << key.second << endl;
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

