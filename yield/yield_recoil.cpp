
//
Double_t LinFit(Double_t *dim, Double_t *par);
Double_t QuadFit(Double_t *dim, Double_t *par);  
Double_t PolyBg(Double_t *dim, Double_t *par, Int_t order);

Double_t StepBG(Double_t *dim, Double_t *par);
Double_t StepFunction(Double_t *dim, Double_t *par);

Double_t Gaus(Double_t *dim, Double_t *par);
Double_t DoubleGaus(Double_t *dim, Double_t *par);
Double_t SkewedGaus(Double_t *dim, Double_t *par);
Double_t Efficiency(Double_t *dim, Double_t *par);
Double_t PhotoPeak(Double_t *dim, Double_t *par);
Double_t PhotoPeakBG(Double_t *dim, Double_t *par);
Double_t PhotoPeakBGExcludeRegion(Double_t *dim, Double_t *par);

//
void Fit(TH1D *h1, TCanvas *cc, Double_t x_low, Double_t x_high, Double_t &area, Double_t &area_error, Option_t *opt = "");
bool InitParams(TF1 *tf, TH1 *h);

//
map<int, pair<int,int>> m_e_range = {
  {334, {285,385}},
  {406, {360,420}},
  {439, {420,470}},
  {506, {480,530}},
  {549, {520,570}},
  {585, {560,630}},
  {667, {620,690}},
  {712, {680,725}},
  {737, {725,800}},
  {832, {780,850}},
  {860, {840,900}},
  {1046, {1000,1100}},
  {1166, {1140,1180}},
  {1194, {1140,1220}}
};

//
void yield_recoil(string str)
{
  TFile *fi = TFile::Open(str.c_str());
  if(fi->IsZombie()){
    cout << "can not open " << str << endl;
    return;
  }
  
  ofstream fo;
  fo.open("recoil_area.txt", std::ofstream::out | std::ofstream::app);

  TH1D *hp_event, *hp_bg;

  vector<TCanvas*> v_c;
  for(int i=0;i<8;++i){
    for(auto &[e,range] : m_e_range){
      v_c.push_back(new TCanvas(TString::Format("c_ring%d_%dkeV",i+1,e).Data(),"",500,400));
    }
  }

  int kk = 0;
  double area, area_error;
  for(int i=0;i<8;++i){
    hp_event = (TH1D*)fi->Get(TString::Format("event_e_dc_r_1000fs_spider_p_spider_ring%d", i+1).Data());
    hp_bg = (TH1D*)fi->Get(TString::Format("bg_e_dc_r_1000fs_spider_p_spider_ring%d", i+1).Data());

    hp_event->Add(hp_bg, -1);
    hp_event->Rebin();
    for(auto &[e,range] : m_e_range){
      v_c[kk]->cd();
      v_c[kk]->SetLogy();
      hp_event->GetXaxis()->SetRangeUser(0.8*range.first, 1.2*range.second);
      Fit(hp_event, v_c[kk], range.first, range.second, area, area_error, "");
      kk++;
  
      fo << "ring " << i+1 << " " << e << " keV " << area << " \\" << "\\(" << area_error << ")" << endl;
    }
    fo << endl;
  }


  fo.close();

  return;
}

// real fit function
void Fit(TH1D *h1, TCanvas *cc, Double_t x_low, Double_t x_high, Double_t &area, Double_t &area_error, Option_t *opt = "")
{
  TF1 *tf_peak_bg = new TF1("photo_peak_bg", PhotoPeakBG, x_low, x_high, 8);
  TF1 *tf_bg = new TF1("background", StepBG, x_low, x_high, 6);
  InitParams(tf_peak_bg, h1);

  cc->cd();
  cc->SetLogy();
  h1->Draw("hist");

  TVirtualFitter::SetMaxIterations(100000);
  
  TString options = opt;
  bool verbose = !options.Contains("Q");
  bool no_print = options.Contains("no-print");
  if(no_print) {
    options.ReplaceAll("no-print","");
  }

  if(h1->GetSumw2()->fN != h1->GetNbinsX()+2){
    h1->Sumw2();
  }

  TFitResultPtr fit_res = h1->Fit(tf_peak_bg, Form("%sLRSME", options.Data()));

  fit_res.Get()->Print();
  printf("chi^2/NDF = %.02f\n", tf_peak_bg->GetChisquare()/(double)tf_peak_bg->GetNDF());

  if(!fit_res.Get()->IsValid()){
    printf("fit has failed, trying refit... ");
    h1->GetListOfFunctions()->Last()->Delete();
    fit_res = h1->Fit(tf_peak_bg, Form("%sLRSME",options.Data())); //,Form("%sRSM",options.Data()))
    if(fit_res.Get()->IsValid()){
      printf("refit passed! \n");
    }else{
      printf(" refit also failed :( \n");
    }
  }

  //
  printf("fit peak %.02f keV\n", tf_peak_bg->GetParameter(1));

  //
  double bg_pars[6];
  bg_pars[0] = tf_peak_bg->GetParameters()[0];
  bg_pars[1] = tf_peak_bg->GetParameters()[1];
  bg_pars[2] = tf_peak_bg->GetParameters()[2];
  bg_pars[3] = tf_peak_bg->GetParameters()[5];
  bg_pars[4] = tf_peak_bg->GetParameters()[6];
  bg_pars[5] = tf_peak_bg->GetParameters()[7];

  tf_bg->SetParameters(bg_pars);
  //h1->GetListOfFunctions()->Print();
  tf_peak_bg->SetLineColor(2);
  tf_peak_bg->Draw("same");
  tf_bg->SetLineColor(1);
  tf_bg->Draw("same");

  area = tf_peak_bg->Integral(x_low,x_high) / h1->GetBinWidth(1);
  double bg_area = tf_bg->Integral(x_low, x_high) / h1->GetBinWidth(1);
  area -= bg_area;
  area_error = 0.;

  if(x_low>x_high){
    std::swap(x_low, x_high);
  }
  Double_t sum = h1->Integral(h1->GetXaxis()->FindBin(x_low), h1->GetXaxis()->FindBin(x_high)); // * h1->GetBinWidth(1);
  Double_t sum_error = TMath::Sqrt(sum);
  sum -= bg_area;

  area = sum;
  area_error = TMath::Sqrt(sum + bg_area);
  //printf("area: %.02f error %.02f\n\n", area, area_error);

  /*
  if(!verbose){
    printf("hist: %s\n",h1->GetName());
    tf_peak_bg->Print();
  }
  */

  tf_peak_bg->Copy(*h1->GetListOfFunctions()->FindObject(tf_peak_bg->GetName()));
  h1->GetListOfFunctions()->Add(tf_bg->Clone()); //use to be a clone.
  //h1->GetListOfFunctions()->Print();

  tf_peak_bg->SetParent(0); //h1;

  cc->SaveAs(TString::Format("./fig/%s.png",cc->GetName()).Data());
  delete tf_peak_bg;
  delete tf_bg;
}

//
bool InitParams(TF1 *tf, TH1 *h)
{
  if(!h){
    printf("No histogram is associated yet, no initial guesses made\n");
    return false;
  }

  Double_t x_low, x_high;
  tf->GetRange(x_low, x_high);
  cout << "x_low " << x_low << "x_high " << x_high << endl;

  Int_t bin_low = h->GetXaxis()->FindBin(x_low);
  Int_t bin_high = h->GetXaxis()->FindBin(x_high);

  Double_t y_high = h->GetBinContent(bin_low);
  Double_t y_low = h->GetBinContent(bin_high);
  for(int x=1;x<5;x++) {
    y_high += h->GetBinContent(bin_low-x);
    y_low += h->GetBinContent(bin_high+x);
  }
  y_high /= 5.0;
  y_low /= 5.0;

  if(y_low>y_high){
    std::swap(y_low, y_high);
  }

  double x_largest = 0.;
  double y_largest = 0.;
  int i = bin_low;
  for(;i<=bin_high;i++) {
    if(h->GetBinContent(i) > y_largest) {
      y_largest = h->GetBinContent(i);
      x_largest = h->GetXaxis()->GetBinCenter(i);
    }
  }

  // - par[0]: height of peak
  // - par[1]: cent of peak
  // - par[2]: sigma
  // - par[3]: R:    relative height of skewed gaus to gaus
  // - par[4]: beta: "skewedness" of the skewed gaussin
  // - par[5]: step: size of stepfunction step.
  // - par[6]: base bg height.

  //limits.
  tf->SetParLimits(0, 0, y_largest*2);
  tf->SetParLimits(1, x_low, x_high);
  tf->SetParLimits(2, 0.1, x_high-x_low);
  tf->SetParLimits(3, 0.0, 40);
  tf->SetParLimits(4, 0.01, 5);
  double step = (y_high-y_low)/y_largest*50;
  tf->SetParLimits(5, 0., step+step);
  printf(" y_high = %.02f \t y_low = %.02f \t step = %.02f\n", y_high, y_low, step); fflush(stdout);
  double offset = y_low;
  double slope = -0.001;
  tf->SetParLimits(6, offset-0.5*offset, offset+offset);
  tf->SetParLimits(7, 100.*slope, -100.*slope);

  //Make initial guesses
  tf->SetParameter(0, y_largest);         //h->GetBinContent(bin));
  tf->SetParameter(1, x_largest);         //GetParameter("centroid"));
  tf->SetParameter(2, (x_largest*.01)/2.35);  //2,(x_high-x_low));     //2.0/bin_width); //
  tf->SetParameter(3, 5.);
  tf->SetParameter(4, 1.);
  tf->SetParameter(5, step);
  tf->SetParameter(6, offset);
  tf->SetParameter(7, slope);

  tf->SetParError(0, 0.10 * y_largest);
  tf->SetParError(1, 0.25);
  tf->SetParError(2, 0.10 *((x_largest*.01)/2.35));
  tf->SetParError(3, 5);
  tf->SetParError(4, 0.5);
  tf->SetParError(5, 0.10 * step);
  tf->SetParError(6, 0.10 * offset);
  tf->SetParError(7, 0.10 * slope);

  cout << "y_largest " << y_largest << endl;
  cout << "x_largest " << x_largest << endl;
  cout << "sigma " << (x_largest*.01)/2.35 << endl;
  cout << "R " << 5 << endl;
  cout << "beta " << 1 << endl;
  cout << "step " << step << endl;
  cout << "offset " << offset << endl;
  cout << "slope " << slope << endl;

  return true;
}

//
Double_t PolyBg(Double_t *dim, Double_t *par, Int_t order)
{
  Double_t result = 0.0;
  int j=0;
  for(Int_t i=0;i<=order;i++) {
    result += *(par+j) *TMath::Power(dim[0],i);
    j++;
  }
  //result += par[i]*TMath::Power(dim[0]-par[order+1],i);
  return result;
}

//
Double_t LinFit(Double_t *dim, Double_t *par)
{
  return PolyBg(dim,par,1);
}

//
Double_t QuadFit(Double_t *dim, Double_t *par)
{
  return PolyBg(dim,par,2);
}

//
Double_t StepBG(Double_t *dim, Double_t *par)
{
  return StepFunction(dim,par) + PolyBg(dim,(par+4),1);//0/1/2
}

//
Double_t StepFunction(Double_t *dim, Double_t *par)
{
  //  -dim[0]: channels to fit
  //  -par[0]: height of peak
  //  -par[1]: centroid of peak
  //  -par[2]: sigma of peak
  //  -par[3]: size of step in step function.

  Double_t x       = dim[0];
  
  Double_t height  = par[0];
  Double_t cent    = par[1];
  Double_t sigma   = par[2];
  //Double_t R       = par[4];
  Double_t step    = par[3];

  //return TMath::Abs(step)*height/100.0*TMath::Erfc((x-cent)/(TMath::Sqrt(2.0)*sigma));
  return height*(step/100.0) *TMath::Erfc((x-cent)/(TMath::Sqrt(2.0)*sigma));
}

//
Double_t Gaus(Double_t *dim, Double_t *par)
{
  // - dim[0]: channels to fit
  // - par[0]: height of peak
  // - par[1]: cent of peak
  // - par[2]: sigma
  // - par[3]: relative height of skewed gaus to gaus

  Double_t x      = dim[0];
  Double_t height = par[0];
  Double_t cent   = par[1];
  Double_t sigma  = par[2];
  Double_t R      = par[3];

  return height*(1.0-R/100.0)*TMath::Gaus(x,cent,sigma);
}

//
Double_t DoubleGaus(Double_t *dim, Double_t *par)
{
  // - dim[0]: channels to fit
  // - par[0]: height of peak1
  // - par[1]: cent of peak1
  // - par[2]: height of peak2
  // - par[3]: cent of peak2
  // - par[4]: shared sigma
  // - par[5]: bg offset
  // - par[6]: bg slope

  Double_t x      = dim[0];
  Double_t height1 = par[0];
  Double_t cent1   = par[1];

  Double_t height2 = par[2];
  Double_t cent2   = par[3];
  
  Double_t sigma   = par[4];

  return height1*TMath::Gaus(x,cent1,sigma) + height2*TMath::Gaus(x,cent2,sigma) + PolyBg(dim,(par+5),2);
}


//
Double_t SkewedGaus(Double_t *dim,Double_t *par)
{

  // StepFunction(dim,par) + PolyBg
  // - par[0]: height of peak
  // - par[1]: cent of peak
  // - par[2]: sigma
  // - par[3]: relative height of skewed gaus to gaus
  // - par[4]: "skewedness" of the skewed gaussin

  Double_t x      = dim[0]; //channel number used for fitting
  Double_t height = par[0]; //height of photopeak
  Double_t cent   = par[1]; //Peak Centroid of non skew gaus
  Double_t sigma  = par[2]; //standard deviation of gaussian
  Double_t R      = par[3]; //relative height of skewed gaussian
  Double_t beta   = par[4]; //"skewedness" of the skewed gaussian

  double scaling = R*height/100.0;
  //double x_rel = (x - cent)/sigma;

  double fterm = (x-cent)/(sigma*TMath::Sqrt(2.));
  double sterm = sigma /  (beta *TMath::Sqrt(2.));

  return scaling * TMath::Exp((x-cent)/beta) * TMath::Erfc(fterm + sterm); 
}

//
Double_t PhotoPeakBG(Double_t *dim, Double_t *par)
{
  // - dim[0]: channels to fit
  // - par[0]: height of peak
  // - par[1]: cent of peak
  // - par[2]: sigma
  // - par[3]: relative height of skewed gaus to gaus
  // - par[4]: "skewedness" of the skewed gaussin
  // - par[5]: size of stepfunction step.
  // - par[6]: base bg height.
  // - par[7]: slope of bg.
  
  double spar[4];
  spar[0] = par[0];
  spar[1] = par[1];
  spar[2] = par[2];
  spar[3] = par[5];  //stepsize;

  return Gaus(dim,par) + SkewedGaus(dim,par) + StepFunction(dim,spar) + PolyBg(dim,par+6,1);//0/1/2
}

//
Double_t PhotoPeakBGExcludeRegion(Double_t *dim, Double_t *par)
{
  // - dim[0]: channels to fit
  // - par[0]: height of peak
  // - par[1]: cent of peak
  // - par[2]: sigma
  // - par[3]: relative height of skewed gaus to gaus
  // - par[4]: "skewedness" of the skewed gaussin
  // - par[5]: size of stepfunction step.
  
  // - par[6]: base bg height.
  
  // - par[7]: exclude low;
  // - par[8]: exclude high;

  if(dim[0]>par[7] && dim[0]<par[8]) {
    TF1::RejectPoint();
    return 0;
  }
  double spar[4];
  spar[0] = par[0];
  spar[1] = par[1];
  spar[2] = par[2];
  spar[3] = par[5];  //stepsize;
  return Gaus(dim,par) + SkewedGaus(dim,par) + StepFunction(dim,spar) + PolyBg(dim,par+6,0);
}

