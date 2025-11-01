//
// ge info (ring, sector)
// (3,2) (3,8) (3,9) (3,10)
// (4,1) (4,2) (4,3) (4,4) (4,5) (4,6) (4,7) (4,8)
// (5,2) (5,3) (5,4) (5,5) (5,6)

// pars = 12
// Gaussian + Left-Skew + Step + Background
Double_t gpeakexregion(Double_t *v, Double_t *par);

// pars = 5
// Step + Pol1
Double_t bgstep(Double_t *v, Double_t *par);

// pars = 9
// Gaussian + Left-Skew
Double_t gpeak(Double_t *v, Double_t *par);

//
double eff_fun(double *x, double *p);
double *eff_fit(map<double, double> &m,  map<double, double> &m_err);
void eff_cali_all_ge();
void get_all_ge_h();
double eff_cali_all_ge_single_peak(TH1D *h, double energy, double l_bg, double l_peak, double u_peak, double u_bg, double ll_ex, double lr_ex, double hl_ex, double hr_ex);

//
TFile *fi = TFile::Open("../rootfile/data0722_build_200ns_jump_300ns.root");
TTree *tr = (TTree*)fi->Get("tr_event");
TH1D *h = new TH1D("h", "", 4096, 0, 4096);

//
std::map<double, std::pair<double, double>> m_interested_eff_err = {
  {122, {0, 0}},
  {298, {0, 0}},
  {306, {0, 0}},
  {334, {0, 0}},
  {340, {0, 0}},
  {403, {0, 0}},
  {406, {0, 0}},
  {439, {0, 0}},
  {485, {0, 0}},
  {506, {0, 0}},
  {558, {0, 0}},
  {585, {0, 0}},
  {676, {0, 0}},
  {712, {0, 0}},
  {737, {0, 0}},
  {832, {0, 0}},
  {860, {0, 0}},
  {869, {0, 0}},
  {1046, {0, 0}},
  {1166, {0, 0}},
  {1194, {0, 0}},
  {1350, {0, 0}}
};

//
double eff_fun(double *x, double *p)
{
  double x1 = log(x[0]/100.);
  double x2 = log(x[0]/1000.);

  double z1 = pow(p[0]+p[1]*x1+p[2]*x1*x1, -p[6]);
  double z2 = pow(p[3]+p[4]*x2+p[5]*x2*x2, -p[6]);

  double eff = exp(pow(z1+z2, -1./p[6]));

  return eff;
}

//
double *eff_fit(map<double, double> &m,  map<double, double> &m_err)
{
  gStyle->SetOptFit(1111);

  TF1 *tf = new TF1("tf", eff_fun, 50, 2048, 7);
  
  /*
  tf->SetParameters(5.42, 3.73, 0., 7.03, -0.65, -0.14, 15.);
  delete gROOT->GetListOfCanvases()->FindObject("cc");
  TCanvas *cc = new TCanvas("cc", "", 0, 0, 480, 360);
  cc->cd();
  tf->Draw();
  */

  TGraphErrors *gr = new TGraphErrors();

  int i = 0;
  for(auto it=m.begin();it!=m.end();it++){
    gr->SetPoint(i, it->first, it->second);
    i++;
  }
  i = 0;
  for(auto it=m_err.begin();it!=m_err.end();it++){
    gr->SetPointError(i, 0, it->second);
    cout << it->second << endl;
    i++;
  }

  delete gROOT->GetListOfCanvases()->FindObject("cc");
  TCanvas *cc = new TCanvas("cc", "", 0, 0, 480, 360);
  cc->cd();

  gr->GetXaxis()->SetTitle("Energy [keV]");
  gr->GetYaxis()->SetTitle("Amplitude");
  gr->SetMarkerColor(4);
  gr->SetMarkerStyle(21);
  gr->SetMarkerSize(1);
  gr->Draw("AP");  

  tf->SetParameter(0, 10.);
  tf->SetParameter(1, 2.);
  tf->FixParameter(2, 0.);
  tf->SetParameter(3, 10.);
  tf->SetParameter(4, -0.5);
  tf->SetParameter(5, 0.1);
  tf->FixParameter(6, 15.);

  gr->Fit("tf");

  //
  double err_all = 0.;
  i = 0;
  for(auto it=m.begin();it!=m.end();it++){
    err_all += (tf->Eval(it->first)-it->second)*(tf->Eval(it->first)-it->second)/tf->Eval(it->first)/tf->Eval(it->first);
    cout << err_all << endl;
    i++;
  }
  err_all /= (double)m.size();
  cout << sqrt(err_all) << endl;

  TLatex *lt = new TLatex(0.3, 0.8, Form("eff error %0.4f%%",100.*sqrt(err_all)));
  lt->SetNDC();
  lt->SetTextSize(0.04);
  lt->SetTextColor(kBlack);
  lt->Draw();

  double max = tf->GetMaximum();
  cout << "max " << max << endl;

  int ii = 0;
  for(auto& [key, value] : m_interested_eff_err){
    value.first  = tf->Eval(key)/max;
    value.second = tf->Eval(key)/max*sqrt(err_all);
    ++ii;
  }

  for(auto &[key,value]:m_interested_eff_err){
    cout << "{" << key << ", {" << value.first << ", " << value.second << "}}," << endl;
  }

  cc->Update();
  cc->SaveAs("./fig/all_eff.png");

  return tf->GetParameters();
}

//
void eff_cali_all_ge()
{
  //
  map<int, double> m_133Ba;
  m_133Ba.insert(std::make_pair(81, 34.06));
  m_133Ba.insert(std::make_pair(303, 18.33));
  m_133Ba.insert(std::make_pair(356, 62.05));
  m_133Ba.insert(std::make_pair(384, 8.94));

  //
  map<int, double> m_152Eu;
  m_152Eu.insert(std::make_pair(40, 59.1));
  m_152Eu.insert(std::make_pair(122, 28.58));
  m_152Eu.insert(std::make_pair(245, 7.58));
  m_152Eu.insert(std::make_pair(344, 26.5));
  m_152Eu.insert(std::make_pair(411, 2.234));
  m_152Eu.insert(std::make_pair(444, 3.148));
  m_152Eu.insert(std::make_pair(779, 12.94));
  m_152Eu.insert(std::make_pair(867, 4.245));
  m_152Eu.insert(std::make_pair(964, 14.60));
  m_152Eu.insert(std::make_pair(1112, 13.64));
  m_152Eu.insert(std::make_pair(1408, 21.00));

  // if(r==3 || r==5){
  //   m_152Eu.erase(40);
  // }else{
  //   m_133Ba.erase(81);
  // }

  map<int, vector<double>> m_peak_fit_range;
  ifstream fi("fit_range.txt");
  if(!fi){
    cout << "can not open fit_range.txt";
    return;
  }

  string line;
  getline(fi, line);

  int e;
  double l_bg, l_peak, u_peak, u_bg, ll_ex, lr_ex, hl_ex, hr_ex;
  while(fi >> e >> l_bg >> l_peak >> u_peak >> u_bg >> ll_ex >> lr_ex >> hl_ex >> hr_ex){
    m_peak_fit_range[e] = {l_bg, l_peak, u_peak, u_bg, ll_ex, lr_ex, hl_ex, hr_ex};
  }
  fi.close();

  /*
  for(auto &pfr:m_peak_fit_range){
    cout << pfr.first << " ";
    for(auto &it:pfr.second){
      cout << it << " ";
    }
    cout << endl;
  }
  */

  get_all_ge_h();

  double sum;
  map<int, double> m_s;
  map<int, double> m_s_err;

  //
  for(auto it=m_133Ba.begin();it!=m_133Ba.end();it++){
    e = it->first;
    l_bg = m_peak_fit_range[e][0];
    l_peak = m_peak_fit_range[e][1];
    u_peak = m_peak_fit_range[e][2];
    u_bg = m_peak_fit_range[e][3];
    ll_ex = m_peak_fit_range[e][4];
    lr_ex = m_peak_fit_range[e][5];
    hl_ex = m_peak_fit_range[e][6];
    hr_ex = m_peak_fit_range[e][7];

    sum = eff_cali_all_ge_single_peak(h, e, l_bg, l_peak, u_peak, u_bg, ll_ex, lr_ex, hl_ex, hr_ex);
  
    m_s[e] = sum;
    m_s_err[e] = sqrt(sum);
  }

  //
  for(auto it=m_152Eu.begin();it!=m_152Eu.end();it++){
    e = it->first;
    l_bg = m_peak_fit_range[e][0];
    l_peak = m_peak_fit_range[e][1];
    u_peak = m_peak_fit_range[e][2];
    u_bg = m_peak_fit_range[e][3];
    ll_ex = m_peak_fit_range[e][4];
    lr_ex = m_peak_fit_range[e][5];
    hl_ex = m_peak_fit_range[e][6];
    hr_ex = m_peak_fit_range[e][7];

    sum = eff_cali_all_ge_single_peak(h, e, l_bg, l_peak, u_peak, u_bg, ll_ex, lr_ex, hl_ex, hr_ex);
  
    m_s[e] = sum;
    m_s_err[e] = sqrt(sum);
  }

  for(auto it=m_133Ba.begin();it!=m_133Ba.end();it++){
    cout << m_s[it->first] << " " << m_s_err[it->first] << endl;
  }
  for(auto it=m_152Eu.begin();it!=m_152Eu.end();it++){
    cout << m_s[it->first] << " " << m_s_err[it->first] << endl;
  }

  //using 356(133Ba) and 344(152Eu) normlize
  map<double, double> m_s2;
  map<double, double> m_s2_err;
  double frac = (m_s[356]/m_133Ba[356])/(m_s[344]/m_152Eu[344]);
  cout << "frac " << frac << endl; 
  for(auto it=m_133Ba.begin();it!=m_133Ba.end();it++){
    m_s2.insert(std::make_pair(it->first, m_s[it->first]/it->second));
    m_s2_err.insert(std::make_pair(it->first, m_s_err[it->first]/it->second));
  }

  for(auto it=m_152Eu.begin();it!=m_152Eu.end();it++){
    m_s2.insert(std::make_pair(it->first, frac*m_s[it->first]/it->second));
    m_s2_err.insert(std::make_pair(it->first, frac*m_s_err[it->first]/it->second));
  }

  //
  for(auto it=m_s2.begin();it!=m_s2.end();it++){
    cout << it->first << " " << it->second << endl;
  }
  for(auto it=m_s2_err.begin();it!=m_s2_err.end();it++){
    cout << it->first << " " << it->second << endl;
  }

  //
  double *par = eff_fit(m_s2, m_s2_err);
  for(int i=0;i<7;i++){
    cout << i << " " << par[i] << std::endl;
  }
}

//
void get_all_ge_h()
{
  TCut cut_ge_ring3 = "(ge_ring_id==3&&ge_sector_id==2) || (ge_ring_id==3&&ge_sector_id==8) || (ge_ring_id==3&&ge_sector_id==9) || (ge_ring_id==3&&ge_sector_id==10)";
  TCut cut_ge_ring4 = "(ge_ring_id==4&&ge_sector_id==1) || (ge_ring_id==4&&ge_sector_id==2) || (ge_ring_id==4&&ge_sector_id==3) || (ge_ring_id==4&&ge_sector_id==4) || (ge_ring_id==4&&ge_sector_id==5) || (ge_ring_id==4&&ge_sector_id==6) || (ge_ring_id==4&&ge_sector_id==7) || (ge_ring_id==4&&ge_sector_id==8)";
  TCut cut_ge_ring5 = "(ge_ring_id==5&&ge_sector_id==2) || (ge_ring_id==5&&ge_sector_id==3) || (ge_ring_id==5&&ge_sector_id==4) || (ge_ring_id==5&&ge_sector_id==5) || (ge_ring_id==5&&ge_sector_id==6)";
  
  TCut cut_ge = Form("%s || %s || %s", cut_ge_ring3.GetTitle(), cut_ge_ring4.GetTitle(), cut_ge_ring5.GetTitle());
  cout << cut_ge << endl;

  tr->Draw("ge_energy>>h", cut_ge, "goff");
}

//
double eff_cali_all_ge_single_peak(TH1D *h, double energy, double l_bg, double l_peak, double u_peak, double u_bg, double ll_ex, double lr_ex, double hl_ex, double hr_ex)
{
  gStyle->SetOptFit(1111);

  TCanvas *cc = new TCanvas(Form("cc_%dkeV",(int)energy), "", 0, 0, 480, 360);
  cc->cd();
  cc->SetLogy();
  h->GetXaxis()->SetTitle("Energy [keV]");
  h->GetYaxis()->SetTitle("Counts");
  h->Draw();

  //
  double l_bg_bin = l_bg/h->GetBinWidth(1);
  double l_peak_bin = l_peak/h->GetBinWidth(1);
  double u_peak_bin = u_peak/h->GetBinWidth(1);
  double u_bg_bin = u_bg/h->GetBinWidth(1);
  
  h->GetXaxis()->SetRangeUser(l_bg-10., u_bg+10.);

  // Fit peak
  Double_t par[12];
  par[0] = energy;
  par[1] = h->GetBinContent((l_peak_bin+u_peak_bin)/2.);
  // Gaussian
  par[2] = 1.0;
  // Left-Skew
  par[3] = 0.005;
  par[4] = 1.;
  // Step
  par[5] = 0.02;
  // Background
  par[6] = h->GetBinContent(l_bg_bin);
  par[7] = 0.;
  
  par[8] = ll_ex;
  par[9] = lr_ex;
  par[10] = hl_ex;
  par[11] = hr_ex;

  TF1 *gpeakexregion_tf = new TF1("gpeakexregion_tf", gpeakexregion, l_bg, u_bg, 12);
  gpeakexregion_tf->SetParameters(par);

  gpeakexregion_tf->SetParLimits(0, par[0]-2., par[0]+2.);
  gpeakexregion_tf->SetParLimits(2, 0.1, 10.);
  gpeakexregion_tf->SetParLimits(3, 0.001, 1.);
  gpeakexregion_tf->SetParLimits(4, 0.01, 10.);
  gpeakexregion_tf->SetParLimits(5, 0.0001, 0.05);
  
  gpeakexregion_tf->FixParameter(8, par[8]);
  gpeakexregion_tf->FixParameter(9, par[9]);
  gpeakexregion_tf->FixParameter(10, par[10]);
  gpeakexregion_tf->FixParameter(11, par[11]);

  h->Fit("gpeakexregion_tf", "RQ");

  //
  TF1 *gstep_tf = new TF1("gstep_tf", gpeak, l_bg, u_bg, 5);
  gstep_tf->SetParameter(0, gpeakexregion_tf->GetParameter(0));
  gstep_tf->SetParameter(1, gpeakexregion_tf->GetParameter(1));
  gstep_tf->SetParameter(2, gpeakexregion_tf->GetParameter(2));
  gstep_tf->SetParameter(3, gpeakexregion_tf->GetParameter(3));
  gstep_tf->SetParameter(4, gpeakexregion_tf->GetParameter(4));

  gstep_tf->SetLineColor(9);
  gstep_tf->Draw("same");

  //
  TF1 *bgstep_tf = new TF1("bgstep_tf", bgstep, l_bg, u_bg, 5);
  bgstep_tf->SetParameter(0, gpeakexregion_tf->GetParameter(6));
  bgstep_tf->SetParameter(1, gpeakexregion_tf->GetParameter(7));
  bgstep_tf->SetParameter(2, gpeakexregion_tf->GetParameter(0));
  bgstep_tf->SetParameter(3, gpeakexregion_tf->GetParameter(2));
  bgstep_tf->SetParameter(4, gpeakexregion_tf->GetParameter(1)*gpeakexregion_tf->GetParameter(5));
  bgstep_tf->SetLineColor(3);
  bgstep_tf->Draw("same");

  //
  double gamma = gpeakexregion_tf->GetParameter(1);
  double delta = gpeakexregion_tf->GetParameter(2);
  double a = gpeakexregion_tf->GetParameter(3);
  double beta = gpeakexregion_tf->GetParameter(4);

  double sum = delta*sqrt(TMath::Pi());
  sum += a*beta*exp(-delta*delta/4./beta/beta);
  sum *= gamma;

  TLatex *lt_e = new TLatex(0.15, 0.8, Form("energy %0.4f keV",gpeakexregion_tf->GetParameter(0)));
  lt_e->SetNDC();
  lt_e->SetTextSize(0.04);
  lt_e->SetTextColor(kBlack);
  lt_e->Draw();

  TLatex *lt_result = new TLatex(0.15, 0.7, Form("area %0.2f (%0.2f)",sum,sqrt(sum)));
  lt_result->SetNDC();
  lt_result->SetTextSize(0.04);
  lt_result->SetTextColor(kBlack);
  lt_result->Draw();

  cc->Update();
  cc->SaveAs(Form("./fig/all_%dkeV.png",(int)energy));

  delete gpeakexregion_tf;
  delete gstep_tf;
  delete bgstep_tf;

  return sum;

}

// pars = 12
// Gaussian + Left-Skew + Step + Background
Double_t gpeakexregion(Double_t *v, Double_t *par)
{

  if(v[0]>par[8] && v[0]<par[9]){// low exclude region
    TF1::RejectPoint();
    return 0;
  }
  if(v[0]>par[10] && v[0]<par[11]){// high exclude region
    TF1::RejectPoint();
    return 0;
  }
  
  Double_t energy  = par[0];
  Double_t gamma  = par[1];

  // Gaussian
  Double_t delta = par[2];
  // Left-Skew
  Double_t a = par[3];
  Double_t beta = par[4];
  // Step
  Double_t s = par[5];
  // Background
  Double_t bg0 = par[6];
  Double_t bg1 = par[7];

  // Gaussian
  Double_t gaussian = gamma*exp(-pow((v[0]-energy)/delta, 2.));

  // Left-Skew
  Double_t left_skew = gamma*1/2.*a*exp((v[0]-energy)/beta);
  left_skew *= erfc(delta/2./beta + (v[0]-energy)/delta);

  // Step
  Double_t step = gamma*s*1/2.*erfc((v[0]-energy)/delta);

  // Background
  Double_t background = bg0+bg1*v[0];

  //
  Double_t result = gaussian+left_skew+step+background; 

  return result;

}

// pars 5
// Step + Pol1
Double_t bgstep(Double_t *v, Double_t *par)
{

  Double_t p0 = par[0];
  Double_t p1 = par[1];
  Double_t energy = par[2];
  Double_t delta  = par[3];
  Double_t step   = par[4];

  Double_t result = p0 + p1*v[0];
  result += step*1/2.*erfc((v[0]-energy)/delta);
   
  return result;
   
}

// pars = 5
// Gaussian + Left-Skew
Double_t gpeak(Double_t *v, Double_t *par)
{
  
  Double_t energy  = par[0];
  Double_t gamma  = par[1];

  // Gaussian
  Double_t delta = par[2];
  // Left-Skew
  Double_t a = par[3];
  Double_t beta = par[4];

  // Gaussian
  Double_t gaussian = gamma*exp(-pow((v[0]-energy)/delta, 2.));

  // Left-Skew
  Double_t left_skew = gamma*1/2.*a*exp((v[0]-energy)/beta);
  left_skew *= erfc(delta/2./beta + (v[0]-energy)/delta);

  //
  Double_t result = gaussian+left_skew; 

  return result;

}
