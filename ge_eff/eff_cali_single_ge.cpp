//
// ge info (ring, sector)
// (1,3) (1,6)
// (2,1) (2,2) (2,3) (2,7)
// (3,2) (3,3) (3,4) (3,5) (3,8) (3,9) (3,10)
// (4,1) (4,2) (4,3) (4,4) (4,5) (4,6) (4,7) (4,8)
// (5,2) (5,3) (5,4) (5,5) (5,6)

//
TFile *fi = TFile::Open("../rootfile/data0722_build_ge_200ns.root");
TTree *tr = (TTree*)fi->Get("tr_event");

//
void draw_spec(int ring, int sector)
{
  TCanvas *cc = new TCanvas("cc", "", 0, 0, 480, 360);
  cc->cd();
  TH1D *h1 = new TH1D("h1", TString::Format("ge_ring%02d_sector%02d",ring,sector).Data(), 8192, 0, 4096);
  tr->Draw("ge_energy>>h1", TString::Format("ge_ring_id==%d&&ge_sector_id==%d",ring,sector).Data());
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
void eff_fit(map<double, double> &m)
{
  TF1 *tf = new TF1("tf", eff_fun, 50, 4000, 7);
  
  /*
  tf->SetParameters(5.42, 3.73, 0., 7.03, -0.65, -0.14, 15.);
  delete gROOT->GetListOfCanvases()->FindObject("cc");
  TCanvas *cc = new TCanvas("cc", "", 0, 0, 480, 360);
  cc->cd();
  tf->Draw();
  */

  TGraph *gr = new TGraph();

  int i = 0;
  for(auto it=m.begin();it!=m.end();it++){
    gr->SetPoint(i, it->first, it->second);
    i++;
  }

  delete gROOT->GetListOfCanvases()->FindObject("cc");
  TCanvas *cc = new TCanvas("cc", "", 0, 0, 480, 360);
  cc->cd();
  gr->Draw("AP*");

  tf->SetParameter(0, 5.);
  tf->SetParameter(1, 4.);
  tf->FixParameter(2, 0.);
  tf->SetParameter(3, 7.);
  tf->SetParameter(4, -0.5);
  tf->SetParameter(5, -0.1);
  tf->FixParameter(6, 15.);

  gr->Fit("tf");

  gr->GetXaxis()->SetRangeUser(0, 3000);
  // gr->GetYaxis()->SetRangeUser(0, 2500);
  cc->Update();
  cc->SaveAs(TString::Format("./fig/single_hpge.png").Data());
}

//
void eff_cali_single_ge(int ring, int sector)
{
  //
  map<double, double> m_133Ba;
  m_133Ba.insert(std::make_pair(80.999, 34.11));
  m_133Ba.insert(std::make_pair(302.858, 18.30));
  m_133Ba.insert(std::make_pair(356.014, 61.94));
  m_133Ba.insert(std::make_pair(383.859, 8.905));

  map<double, pair<double, double>> m_133Ba_x1x2;
  m_133Ba_x1x2[80.999] = make_pair(0.98, 1.02);
  m_133Ba_x1x2[276.404] = make_pair(0.99, 1.01);
  m_133Ba_x1x2[302.858] = make_pair(0.99, 1.01);
  m_133Ba_x1x2[356.014] = make_pair(0.99, 1.01);
  m_133Ba_x1x2[383.859] = make_pair(0.99, 1.01);

  //
  map<double, double> m_152Eu;
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
  
  map<double, pair<double, double>> m_152Eu_x1x2;
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


  TCanvas *cc = new TCanvas("cc", "", 0, 0, 480, 360);

  TH1D *h1 = new TH1D("h1", TString::Format("ge_ring%02d_sector%02d",ring,sector).Data(), 8192, 0, 4096);
  tr->Draw("ge_energy>>h1", TString::Format("ge_ring_id==%d&&ge_sector_id==%d",ring,sector).Data());

  TSpectrum *sp = new TSpectrum();
  TH1D *h_back = (TH1D*)sp->Background(h1);
  h_back->SetLineColor(1);

  TH1D *h_sub = (TH1D*)h1->Clone("h_sub");

  h_sub->Add(h_back, -1);
  h_sub->SetLineColor(4);
  cc->cd();
  h_sub->Draw();

  double sum;
  map<double, double> m_s;

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
    int b1 = (int)((tf->GetParameter(1)-2.0*tf->GetParameter(2)+0.5)*2.);
    int b2 = (int)((tf->GetParameter(1)+2.0*tf->GetParameter(2)+0.5)*2.);
    cout << "b1 " << b1 << " " << h_sub->GetBinContent(b1) << endl;
    cout << "b2 " << b2 << " " << h_sub->GetBinContent(b2) << endl;

    sum = 0;
    for(int i=b1;i<=b2;i++){
      sum += h_sub->GetBinContent(i);
    }
    m_s.insert(std::make_pair(it->first, sum)); 

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
    cc->SaveAs(TString::Format("./fig/133Ba_%dkeV.png",(int)it->first).Data());
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
    for(int i=b1;i<=b2;i++){
      sum += h_sub->GetBinContent(i);
    }
    m_s.insert(std::make_pair(it->first, sum)); 

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
    cc->SaveAs(TString::Format("./fig/152Eu_%dkeV.png",(int)it->first).Data());
  }

  //using 356.014(133Ba) and 344.275(152Eu) normlize
  map<double, double> m_s2;
  double frac = (m_s[356.014]/m_133Ba[356.014])/(m_s[344.275]/m_152Eu[344.275]);
  cout << "frac " << frac << endl; 
  for(auto it=m_133Ba.begin();it!=m_133Ba.end();it++){
    m_s2.insert(std::make_pair(it->first, m_s[it->first]/it->second));
  }

  for(auto it=m_152Eu.begin();it!=m_152Eu.end();it++){
    m_s2.insert(std::make_pair(it->first, frac*m_s[it->first]/it->second));
  }

  //
  eff_fit(m_s2);  
}
