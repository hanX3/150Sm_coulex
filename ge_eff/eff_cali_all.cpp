//
void init();
void draw_spec();
double eff_fun(double *x, double *p);
double *eff_fit(map<double, double> &m, map<double, double> &m_err);

//
std::string configure = "e";
TFile *fi = TFile::Open("../rootfile/data0722_build_ge_200ns.root");
TTree *tr;
TCut cut_ge;

//
std::map<double, double> m_133Ba;
std::map<double, std::pair<double, double>> m_133Ba_x1x2;
std::map<double, double> m_152Eu;
std::map<double, std::pair<double, double>> m_152Eu_x1x2;

//
void eff_cali_all()
{
  init();
  
  delete gROOT->GetListOfCanvases()->FindObject("cc");
  TCanvas *cc = new TCanvas("cc", "", 0, 0, 480, 360);

  TH1D *h;

  TFile *fi_h = TFile::Open(TString::Format("h%s.root",configure.c_str()));
  if(!fi_h || fi_h->IsZombie()){
    h = new TH1D("h", "", 8192, 0, 4096);
    tr->Draw("ge_energy>>h", cut_ge, "goff");
  }else{
    h = (TH1D*)fi_h->Get("h");
  }

  TSpectrum *sp = new TSpectrum();
  TH1D *h_back = (TH1D*)sp->Background(h);
  h_back->SetLineColor(1);

  TH1D *h_sub = (TH1D*)h->Clone("h_sub");
  h_sub->Add(h_back, -1);
  h_sub->SetLineColor(4);
  cc->cd();
  h_sub->Draw();

  double sum, sum_h, sum_h_back;
  ofstream fo;
  fo.open(TString::Format("./origin/all_area.txt").Data());
  map<double, double> m_s;
  map<double, double> m_s_err;

  int b1 = 0;
  int b2 = 0;
  int b_max = 0;
  //
  for(auto it=m_133Ba.begin();it!=m_133Ba.end();it++){
    double x1 = it->first-20.;
    double x2 = it->first+20.;
    h_sub->GetXaxis()->SetRangeUser(x1, x2);
    h_sub->Fit("gaus", "", "", it->first*m_133Ba_x1x2[it->first].first, it->first*m_133Ba_x1x2[it->first].second);

    b_max = (int)it->first*2;
    while(1){
      if(h_sub->GetBinContent(b_max) < h_sub->GetBinContent(b_max+1)){
        b_max++;
      }else{
        break;
      }
    }
    while(1){
      if(h_sub->GetBinContent(b_max) < h_sub->GetBinContent(b_max-1)){
        b_max--;
      }else{
        break;
      }
    }
    cout << h_sub->GetBinContent(b_max) << endl;
  
    TF1 *tf = ((TF1*)(gROOT->GetListOfFunctions()->FindObject("gaus")));
    cout << tf->GetParameter(2) << endl; 
    int b1 = (int)((tf->GetParameter(1)-2.0*tf->GetParameter(2))*2+0.5);
    int b2 = (int)((tf->GetParameter(1)+2.0*tf->GetParameter(2))*2+0.5);
    cout << "b1 " << b1 << " " << h_sub->GetBinContent(b1) << endl;
    cout << "b2 " << b2 << " " << h_sub->GetBinContent(b2) << endl;

    sum = 0;
    sum_h = 0;
    sum_h_back = 0;
    for(int i=b1;i<=b2;i++){
      sum += h_sub->GetBinContent(i);
      sum_h += h->GetBinContent(i);
      sum_h_back += h_back->GetBinContent(i);
    }
    m_s.insert(std::make_pair(it->first, sum));
    m_s_err.insert(std::make_pair(it->first, std::sqrt(sum_h+sum_h_back)));

    TLine *l1 = new TLine(h_sub->GetBinCenter(b1), 0., h_sub->GetBinCenter(b1), h_sub->GetBinContent(b_max));
    TLine *l2 = new TLine(h_sub->GetBinCenter(b2), 0., h_sub->GetBinCenter(b2), h_sub->GetBinContent(b_max));
    l1->SetLineColor(1);
    l1->SetLineWidth(2);
    l1->SetLineStyle(2);
    l2->SetLineColor(1);
    l2->SetLineWidth(2);
    l2->SetLineStyle(2);
    l1->Draw("same");
    l2->Draw("same");
    
    cc->Update();
    cc->SaveAs(TString::Format("./fig/all_133Ba_%dkeV.png",(int)it->first).Data());
  }
  //
  for(auto it=m_152Eu.begin();it!=m_152Eu.end();it++){
    double x1 = it->first-20.;
    double x2 = it->first+20.;
    h_sub->GetXaxis()->SetRangeUser(x1, x2);
    h_sub->Fit("gaus", "", "", it->first*m_152Eu_x1x2[it->first].first, it->first*m_152Eu_x1x2[it->first].second);

    b_max = (int)it->first*2;
    while(1){
      if(h_sub->GetBinContent(b_max) < h_sub->GetBinContent(b_max+1)){
        b_max++;
      }else{
        break;
      }
    }
    while(1){
      if(h_sub->GetBinContent(b_max) < h_sub->GetBinContent(b_max-1)){
        b_max--;
      }else{
        break;
      }
    }
    cout << h_sub->GetBinContent(b_max) << endl;
  
    TF1 *tf = ((TF1*)(gROOT->GetListOfFunctions()->FindObject("gaus")));
    cout << tf->GetParameter(2) << endl; 
    int b1 = (int)((tf->GetParameter(1)-2.0*tf->GetParameter(2)+0.5)*2.);
    int b2 = (int)((tf->GetParameter(1)+2.0*tf->GetParameter(2)+0.5)*2.);
    cout << "b1 " << b1 << " " << h_sub->GetBinContent(b1) << endl;
    cout << "b2 " << b2 << " " << h_sub->GetBinContent(b2) << endl;

    sum = 0;
    sum_h = 0;
    sum_h_back = 0;
    for(int i=b1;i<=b2;i++){
      sum += h_sub->GetBinContent(i);
      sum_h += h->GetBinContent(i);
      sum_h_back += h_back->GetBinContent(i);
    }
    m_s.insert(std::make_pair(it->first, sum)); 
    m_s_err.insert(std::make_pair(it->first, std::sqrt(sum_h+sum_h_back)));

    TLine *l1 = new TLine(h_sub->GetBinCenter(b1), 0., h_sub->GetBinCenter(b1), h_sub->GetBinContent(b_max));
    TLine *l2 = new TLine(h_sub->GetBinCenter(b2), 0., h_sub->GetBinCenter(b2), h_sub->GetBinContent(b_max));
    l1->SetLineColor(1);
    l1->SetLineWidth(2);
    l1->SetLineStyle(2);
    l2->SetLineColor(1);
    l2->SetLineWidth(2);
    l2->SetLineStyle(2);
    l1->Draw("same");
    l2->Draw("same");
    
    cc->Update();
    cc->SaveAs(TString::Format("./fig/all_152Eu_%dkeV.png",(int)it->first).Data());
  }

  //using 356.014(133Ba) and 344.275(152Eu) normlize
  map<double, double> m_s2;
  map<double, double> m_s2_err;
  double frac = (m_s[356.014]/m_133Ba[356.014])/(m_s[344.275]/m_152Eu[344.275]);
  //double frac = 1.;
  cout << "frac " << frac << endl; 
  for(auto it=m_133Ba.begin();it!=m_133Ba.end();it++){
    m_s2.insert(std::make_pair(it->first, m_s[it->first]/it->second));
    m_s2_err.insert(std::make_pair(it->first, m_s_err[it->first]/it->second));
  }

  for(auto it=m_152Eu.begin();it!=m_152Eu.end();it++){
    m_s2.insert(std::make_pair(it->first, frac*m_s[it->first]/it->second));
    m_s2_err.insert(std::make_pair(it->first, frac*m_s_err[it->first]/it->second));
  }

  for(auto it=m_s2.begin();it!=m_s2.end();it++){
    fo << it->first << " " << it->second << endl;
    cout << it->first << " " << it->second << endl;
  }
  for(auto it=m_s2_err.begin();it!=m_s2_err.end();it++){
    fo << it->first << " " << it->second << endl;
    cout << it->first << " " << it->second << endl;
  }

  //
  double *par = eff_fit(m_s2, m_s2_err);
  for(int i=0;i<7;i++){
    fo << i << " " << par[i] << std::endl; 
  }
  
  fo << endl;
  fo.close();

  delete [] par;
}

//
void init()
{
  std::cout << "read ge configure info" << std::endl;

  std::map<int, bool> map_ge_flag;

  std::string line;
  int sid = 0, ch = 0;
  bool gb = 0;
  int key = 0;

  // ge configure
  std::ifstream fi_conf;
  fi_conf.open(TString::Format("../pars/config/%s.txt", configure.c_str()));
  if(!fi_conf){
    std::cout << "can not open configure.txt." << std::endl;
    return;
  }
  std::getline(fi_conf, line);
  while(1){
    fi_conf >> sid >> ch >> gb;
    if(!fi_conf)  break;

    key = 100*sid + ch;
    map_ge_flag[key] = gb;
  }
  fi_conf.close();

  std::stringstream ss;
  for(auto &[k,f] : map_ge_flag){
    if(f){
      ss << "100*ge_sid+ge_ch==" << k << " || ";
    }
  }

  line = ss.str();
  // cout << "line " << line << endl;
  if(line.size()>=3){
    line.erase(line.size()-3);
  }

  ss.str("");
  ss.clear();
  ss << line;
  cout << ss.str() << endl;
  
  cut_ge = ss.str().c_str();

  //
  tr = (TTree*)fi->Get("tr_event");

  //
  m_133Ba.insert(std::make_pair(80.999, 34.11));
  m_133Ba.insert(std::make_pair(302.858, 18.30));
  m_133Ba.insert(std::make_pair(356.014, 61.94));
  m_133Ba.insert(std::make_pair(383.859, 8.905));

  m_133Ba_x1x2[80.999] = make_pair(0.98, 1.02);
  m_133Ba_x1x2[276.404] = make_pair(0.99, 1.01);
  m_133Ba_x1x2[302.858] = make_pair(0.99, 1.01);
  m_133Ba_x1x2[356.014] = make_pair(0.99, 1.01);
  m_133Ba_x1x2[383.859] = make_pair(0.99, 1.01);

  //
  m_152Eu.insert(std::make_pair(121.783, 28.37));
  m_152Eu.insert(std::make_pair(244.692, 7.53));
  m_152Eu.insert(std::make_pair(344.275, 26.57));
  m_152Eu.insert(std::make_pair(411.115, 2.238));
  m_152Eu.insert(std::make_pair(443.976, 3.125));
  m_152Eu.insert(std::make_pair(778.303, 12.97));
  m_152Eu.insert(std::make_pair(867.388, 4.214));
  m_152Eu.insert(std::make_pair(964.131, 14.63));
  m_152Eu.insert(std::make_pair(1112.116, 13.54));
  m_152Eu.insert(std::make_pair(1408.001, 20.85));
  
  m_152Eu_x1x2[121.783] = make_pair(0.99, 1.01);
  m_152Eu_x1x2[244.692] = make_pair(0.99, 1.01);
  m_152Eu_x1x2[344.275] = make_pair(0.99, 1.01);
  m_152Eu_x1x2[411.115] = make_pair(0.991, 1.009);
  m_152Eu_x1x2[443.976] = make_pair(0.991, 1.009);
  m_152Eu_x1x2[778.303] = make_pair(0.992, 1.008);
  m_152Eu_x1x2[867.388] = make_pair(0.994, 1.004);
  m_152Eu_x1x2[964.131] = make_pair(0.994, 1.004);
  m_152Eu_x1x2[1112.116] = make_pair(0.998, 1.002);
  m_152Eu_x1x2[1408.001] = make_pair(0.998, 1.002);
}

//
void draw_spec()
{
  init();
  TH1D *h = new TH1D("h", "", 8192, 0, 4096);

  tr->Draw("ge_energy>>h", cut_ge, "goff");
  TCanvas *cc = new TCanvas("cc", "", 0, 0, 480, 360);
  cc->cd();
  h->SetLineColor(4);
  h->Draw();

  TFile *fo = new TFile(TString::Format("h%s.root",configure.c_str()), "recreate");
  fo->cd();
  h->Write();
  fo->Close();
}

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
double *eff_fit(map<double, double> &m, map<double, double> &m_err)
{
  TF1 *tf = new TF1("tf", eff_fun, 50, 2000, 7);
  
  // tf->SetParameters(5.42, 3.73, 0., 7.03, -0.65, -0.14, 15.);
  // delete gROOT->GetListOfCanvases()->FindObject("cc");
  // TCanvas *cc = new TCanvas("cc", "", 0, 0, 480, 360);
  // cc->cd();
  // tf->Draw();

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
  gr->SetMarkerColor(4);
  gr->SetMarkerStyle(21);
  gr->SetMarkerSize(1);
  gr->Draw("AP");

  tf->SetParameter(0, 4.);
  tf->SetParameter(1, 1);
  tf->FixParameter(2, 0.);
  tf->SetParameter(3, 3.);
  tf->SetParameter(4, -0.6);
  tf->SetParameter(5, -0.04);
  tf->FixParameter(6, 15.);

  // tf->SetParameters(4.43, 1.3, 0., 3.27, -0.64, -0.04, 15.);
  // TCanvas *cc3 = new TCanvas("cc3", "", 0, 0, 480, 360);
  // cc3->cd();
  // tf->Draw();

  gr->Fit("tf", "R");
  cc->SaveAs("./fig/all_fit.png");

  double *p = new double[7];
  for(int i=0;i<7;i++){
    p[i] = tf->GetParameter(i);
  }

  return p;
}
