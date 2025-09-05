//
void fit_ring(int run_first, int run_last, int ring_id)
{
  //
  map<int, double> m_run_injection;

  for(int i=run_first;i<=run_last;i++){
    if(gSystem->AccessPathName(Form("../rootfile/si/data%04d_ring_hist.root", i))){
      continue;
    }

    TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_ring_hist.root", i));
    if(fi->IsZombie()){
      continue;
    }

    TH1D *h_ring = (TH1D*)fi->Get(Form("h_ring%02d", ring_id));
    m_run_injection[i] = h_ring->GetEntries();

    fi->Close();
  }

  //
  /*
  for(auto &ri:m_run_injection){
    cout << ri.first << " " << ri.second << endl;
  }
  */

  vector<int> v_run;
  for(auto &ri:m_run_injection){
    v_run.push_back(ri.first);
  }

  map<int, double> m_run_ky;
  for(int i=0;i<v_run.size();i++){
    if(i==0){
      m_run_ky[v_run[i]] = 1.;
    }else{
      m_run_ky[v_run[i]] = m_run_injection[v_run[i-1]]/m_run_injection[v_run[i]];
    }
  }

  //
  TCanvas *cc = new TCanvas("cc","",400,300);
  cc->SetLogy();
  cc->cd();

  map<int, double> m_run_kx;
  m_run_kx[v_run[0]] = 1.;

  ofstream fo(Form("./pars/run%04d_%04d_ring%02d.txt", run_first, run_last, ring_id));
  fo << "run    kx    ky" << endl;
  fo << v_run[0] << "  " << 1. << endl;

  for(int ii=0;ii<v_run.size()-1;ii++){
    TFile *fi_ref = TFile::Open(Form("../rootfile/si/data%04d_ring_hist.root", v_run[ii]));
    TH1D *h_ring_ref = (TH1D*)fi_ref->Get(Form("h_ring%02d", ring_id));
    TGraph *gr_ref = new TGraph(h_ring_ref->GetNbinsX());
    for(int i=0;i<h_ring_ref->GetNbinsX();i++){
      gr_ref->SetPoint(i, h_ring_ref->GetBinCenter(i), h_ring_ref->GetBinContent(i));
    }

    //
    TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_ring_hist.root", v_run[ii+1]));
    TH1D *h_ring = (TH1D*)fi->Get(Form("h_ring%02d", ring_id));

    double kx_initial = 0.8;
    double step = 0.0001;
    map<double, double> m_chi2_kx;
    for(int i=0;i<5000;i++){
      TGraph *gr = new TGraph(h_ring->GetNbinsX());
      for(int j=0;j<h_ring->GetNbinsX();j++){
        gr->SetPoint(j, h_ring->GetBinCenter(j)*(kx_initial+step*i), h_ring->GetBinContent(j)*m_run_ky[v_run[ii+1]]);
      }
    
      double chi2 = 0;
      for(int j=20000;j<100000;j+=100){
        // cout << j << " " << gr_ref->Eval(j) << " " << gr->Eval(j) << endl;
        chi2 += (gr_ref->Eval(j)-gr->Eval(j))*(gr_ref->Eval(j)-gr->Eval(j));
      }
      m_chi2_kx[chi2] = kx_initial+step*i;

      // cout << "chi2 " << chi2 << endl;
      delete gr;
    }

    /*
    for(auto &ck:m_chi2_kx){
      cout << ck.first << " " << ck.second << endl;
    }
    */

    cout << "kx " << m_chi2_kx.begin()->second << endl;
    m_run_kx[v_run[ii+1]] = m_chi2_kx.begin()->second*m_run_kx[v_run[ii]];
    fo << v_run[ii+1] << "  " << m_run_kx[v_run[ii+1]] << endl;

    TGraph *gr = new TGraph(h_ring->GetNbinsX());
    for(int j=0;j<h_ring->GetNbinsX();j++){
      gr->SetPoint(j, h_ring->GetBinCenter(j)*m_chi2_kx.begin()->second, h_ring->GetBinContent(j)*m_run_ky[v_run[ii+1]]);
    }

    gr_ref->SetTitle(Form("run%04d: %f", v_run[ii+1], m_chi2_kx.begin()->second));
    gr_ref->GetXaxis()->SetTitle("Energy [keV]");
    gr_ref->GetYaxis()->SetTitle("Counts");

    gr_ref->SetMarkerStyle(20);
    gr_ref->SetMarkerSize(0.4);
    gr_ref->SetMarkerColor(2);
    gr->SetMarkerStyle(21);
    gr->SetMarkerSize(0.3);
    gr->SetMarkerColor(4);
    gr_ref->Draw("AP");
    gr->Draw("P SAME");

    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->AddEntry(gr_ref, Form("run%04d",v_run[ii]), "p");
    leg->AddEntry(gr, Form("run%04d",v_run[ii+1]), "p");
    leg->SetTextSize(0.04);
    leg->Draw();

    cc->SaveAs(Form("./fig/ring%02d/run%04d_%04d.png", ring_id, v_run[ii], v_run[ii+1]));

    fi_ref->Close();
    fi->Close();
  }
}

//
void fit_sector(int run_first, int run_last, int sector_id)
{
  //
  map<int, double> m_run_injection;

  for(int i=run_first;i<=run_last;i++){
    if(gSystem->AccessPathName(Form("../rootfile/si/data%04d_sector_hist.root", i))){
      continue;
    }

    TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_sector_hist.root", i));
    if(fi->IsZombie()){
      continue;
    }

    TH1D *h_sector = (TH1D*)fi->Get(Form("h_sector%02d", sector_id));
    m_run_injection[i] = h_sector->GetEntries();

    fi->Close();
  }

  //
  /*
  for(auto &ri:m_run_injection){
    cout << ri.first << " " << ri.second << endl;
  }
  */

  vector<int> v_run;
  for(auto &ri:m_run_injection){
    v_run.push_back(ri.first);
  }

  map<int, double> m_run_ky;
  for(int i=0;i<v_run.size();i++){
    if(i==0){
      m_run_ky[v_run[i]] = 1.;
    }else{
      m_run_ky[v_run[i]] = m_run_injection[v_run[i-1]]/m_run_injection[v_run[i]];
    }
  }

  //
  TCanvas *cc = new TCanvas("cc","",400,300);
  cc->SetLogy();
  cc->cd();

  map<int, double> m_run_kx;
  m_run_kx[v_run[0]] = 1.;

  ofstream fo(Form("./pars/run%04d_%04d_sector%02d.txt", run_first, run_last, sector_id));
  fo << "run    kx    ky" << endl;
  fo << v_run[0] << "  " << 1. << endl;

  for(int ii=0;ii<v_run.size()-1;ii++){
    TFile *fi_ref = TFile::Open(Form("../rootfile/si/data%04d_sector_hist.root", v_run[ii]));
    TH1D *h_sector_ref = (TH1D*)fi_ref->Get(Form("h_sector%02d", sector_id));
    TGraph *gr_ref = new TGraph(h_sector_ref->GetNbinsX());
    for(int i=0;i<h_sector_ref->GetNbinsX();i++){
      gr_ref->SetPoint(i, h_sector_ref->GetBinCenter(i), h_sector_ref->GetBinContent(i));
    }

    //
    TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_sector_hist.root", v_run[ii+1]));
    TH1D *h_sector = (TH1D*)fi->Get(Form("h_sector%02d", sector_id));

    double kx_initial = 0.8;
    double step = 0.0001;
    map<double, double> m_chi2_kx;
    for(int i=0;i<5000;i++){
      TGraph *gr = new TGraph(h_sector->GetNbinsX());
      for(int j=0;j<h_sector->GetNbinsX();j++){
        gr->SetPoint(j, h_sector->GetBinCenter(j)*(kx_initial+step*i), h_sector->GetBinContent(j)*m_run_ky[v_run[ii+1]]);
      }
    
      double chi2 = 0;
      for(int j=20000;j<100000;j+=100){
        // cout << j << " " << gr_ref->Eval(j) << " " << gr->Eval(j) << endl;
        chi2 += (gr_ref->Eval(j)-gr->Eval(j))*(gr_ref->Eval(j)-gr->Eval(j));
      }
      m_chi2_kx[chi2] = kx_initial+step*i;

      // cout << "chi2 " << chi2 << endl;
      delete gr;
    }

    /*
    for(auto &ck:m_chi2_kx){
      cout << ck.first << " " << ck.second << endl;
    }
    */

    cout << "kx " << m_chi2_kx.begin()->second << endl;
    m_run_kx[v_run[ii+1]] = m_chi2_kx.begin()->second*m_run_kx[v_run[ii]];
    fo << v_run[ii+1] << "  " << m_run_kx[v_run[ii+1]] << endl;

    TGraph *gr = new TGraph(h_sector->GetNbinsX());
    for(int j=0;j<h_sector->GetNbinsX();j++){
      gr->SetPoint(j, h_sector->GetBinCenter(j)*m_chi2_kx.begin()->second, h_sector->GetBinContent(j)*m_run_ky[v_run[ii+1]]);
    }

    gr_ref->SetTitle(Form("run%04d: %f", v_run[ii+1], m_chi2_kx.begin()->second));
    gr_ref->GetXaxis()->SetTitle("Energy [keV]");
    gr_ref->GetYaxis()->SetTitle("Counts");

    gr_ref->SetMarkerStyle(20);
    gr_ref->SetMarkerSize(0.4);
    gr_ref->SetMarkerColor(2);
    gr->SetMarkerStyle(21);
    gr->SetMarkerSize(0.3);
    gr->SetMarkerColor(4);
    gr_ref->Draw("AP");
    gr->Draw("P SAME");

    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->AddEntry(gr_ref, Form("run%04d",v_run[ii]), "p");
    leg->AddEntry(gr, Form("run%04d",v_run[ii+1]), "p");
    leg->SetTextSize(0.04);
    leg->Draw();

    cc->SaveAs(Form("./fig/sector%02d/run%04d_%04d.png", sector_id, v_run[ii], v_run[ii+1]));

    fi_ref->Close();
    fi->Close();
  }
}

//
void fit_xy(int ring_id, int sector_id)
{
  gROOT->SetBatch(1);

  if(ring_id*sector_id != 0){
    cout << "wrong ring_id or sector_id " << endl;
    return;
  }

  if(ring_id>0 && sector_id==0){
    fit_ring(472, 605, ring_id);  
    fit_ring(616, 674, ring_id);  
    fit_ring(680, 718, ring_id);  
  }

  if(ring_id==0 && sector_id>0){
    fit_sector(472, 605, sector_id);
    fit_sector(616, 674, sector_id);
    fit_sector(680, 718, sector_id);
  }

  return;
}
