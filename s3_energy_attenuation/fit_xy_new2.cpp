//
void fit_ring(int run_first, int run_last, int ring_id)
{
  //
  vector<int> v_run;
  for(int i=run_first;i<=run_last;i++){
    if(gSystem->AccessPathName(Form("../rootfile/si/data%04d_ring_hist_no_s3att_no_s3cor.root", i))){
      continue;
    }

    TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_ring_hist_no_s3att_no_s3cor.root", i));
    if(fi->IsZombie()){
      continue;
    }

    v_run.push_back(i);

    fi->Close();
  }

  //
  TCanvas *cc = new TCanvas("cc","",400,300);
  cc->SetLogy();
  cc->cd();

  int count_ref[24];
  for(int i=0;i<24;i++){
    if(i<6) count_ref[i] = 200;
    else if(i<12) count_ref[i] = 500;
    else if(i<18) count_ref[i] = 1000;
    else count_ref[i] = 2000;
  }

  TFile *fi_ref = TFile::Open(Form("../rootfile/si/data%04d_ring_hist_no_s3att_no_s3cor.root", v_run[0]));
  TH1D *h_ring_ref = (TH1D*)fi_ref->Get(Form("h_ring%02d", ring_id));
  TGraph *gr_ref = new TGraph(h_ring_ref->GetNbinsX());
  for(int i=0;i<h_ring_ref->GetNbinsX();i++){
    gr_ref->SetPoint(i, h_ring_ref->GetBinCenter(i), h_ring_ref->GetBinContent(i));
  }
  gr_ref->SetMarkerStyle(20);
  gr_ref->SetMarkerSize(0.4);
  gr_ref->SetMarkerColor(2);

  double bin_ref;
  for(int i=h_ring_ref->GetNbinsX();i>0;i--){
    if(h_ring_ref->GetBinContent(i) < count_ref[ring_id-1]){
      continue;
    }else{
      bin_ref = (double)i;
      break;
    }
  }
  cout << "bin_ref " << bin_ref << endl;

  //
  map<int, double> m_run_kx;
  m_run_kx[v_run[0]] = 1.;

  ofstream fo(Form("./pars/ring%02d.txt",ring_id), std::ios::app);
  
  for(int i=0;i<v_run.size();i++){
    TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_ring_hist_no_s3att_no_s3cor.root", v_run[i]));
    TH1D *h_ring = (TH1D*)fi->Get(Form("h_ring%02d", ring_id));

    double bin;
    for(int j=h_ring->GetNbinsX();j>0;j--){
      if(h_ring->GetBinContent(j) < count_ref[ring_id-1]){
        continue;
      }else{
        bin = (double)j;
        break;
      }
    }
    cout << "bin_ref " << bin_ref << " bin " << bin << endl;
    
    double kx = bin_ref/bin;

    cout << "kx " << kx << endl;
    fo << v_run[i] << "  " << kx << endl;

    TGraph *gr = new TGraph(h_ring->GetNbinsX());
    for(int j=0;j<h_ring->GetNbinsX();j++){
      gr->SetPoint(j, h_ring->GetBinCenter(j)*kx, h_ring->GetBinContent(j));
    }

    gr->SetTitle(Form("run%04d: %f", v_run[i], kx));
    gr->SetMinimum(1.);
    gr->GetXaxis()->SetLimits(0, 120000);
    gr->GetXaxis()->SetTitle("Energy [keV]");
    gr->GetYaxis()->SetTitle("Counts");

    gr->SetMarkerStyle(21);
    gr->SetMarkerSize(0.3);
    gr->SetMarkerColor(4);
    gr->Draw("AP");
    gr_ref->Draw("P SAME");

    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->AddEntry(gr_ref, Form("run%04d",v_run[0]), "p");
    leg->AddEntry(gr, Form("run%04d",v_run[i]), "p");
    leg->SetTextSize(0.04);
    leg->Draw();

    cc->SaveAs(Form("./fig/ring%02d/run%04d_%04d.png", ring_id, v_run[0], v_run[i]));

    fi->Close();
  }

  fi_ref->Close();
  fo.close();
}

//
void fit_sector(int run_first, int run_last, int sector_id)
{
  //
  vector<int> v_run;
  for(int i=run_first;i<=run_last;i++){
    if(gSystem->AccessPathName(Form("../rootfile/si/data%04d_sector_hist_no_s3att_no_s3cor.root", i))){
      continue;
    }

    TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_sector_hist_no_s3att_no_s3cor.root", i));
    if(fi->IsZombie()){
      continue;
    }

    v_run.push_back(i);

    fi->Close();
  }

  //
  TCanvas *cc = new TCanvas("cc","",400,300);
  cc->SetLogy();
  cc->cd();

  int count_ref[32];
  for(int i=0;i<32;i++){
    count_ref[i] = 100;
  }

  TFile *fi_ref = TFile::Open(Form("../rootfile/si/data%04d_sector_hist_no_s3att_no_s3cor.root", v_run[0]));
  TH1D *h_sector_ref = (TH1D*)fi_ref->Get(Form("h_sector%02d", sector_id));
  TGraph *gr_ref = new TGraph(h_sector_ref->GetNbinsX());
  for(int i=0;i<h_sector_ref->GetNbinsX();i++){
    gr_ref->SetPoint(i, h_sector_ref->GetBinCenter(i), h_sector_ref->GetBinContent(i));
  }
  gr_ref->SetMarkerStyle(20);
  gr_ref->SetMarkerSize(0.4);
  gr_ref->SetMarkerColor(2);

  double bin_ref;
  for(int i=h_sector_ref->GetNbinsX();i>0;i--){
    if(h_sector_ref->GetBinContent(i) < count_ref[sector_id-1]){
      continue;
    }else{
      bin_ref = (double)i;
      break;
    }
  }
  cout << "bin_ref " << bin_ref << endl;

  //
  map<int, double> m_run_kx;
  m_run_kx[v_run[0]] = 1.;

  ofstream fo(Form("./pars/sector%02d.txt",sector_id), std::ios::app);
  
  for(int i=0;i<v_run.size();i++){
    TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_sector_hist_no_s3att_no_s3cor.root", v_run[i]));
    TH1D *h_sector = (TH1D*)fi->Get(Form("h_sector%02d", sector_id));

    double bin;
    for(int j=h_sector->GetNbinsX();j>0;j--){
      if(h_sector->GetBinContent(j) < count_ref[sector_id-1]){
        continue;
      }else{
        bin = (double)j;
        break;
      }
    }
    cout << "bin_ref " << bin_ref << " bin " << bin << endl;
    
    double kx = bin_ref/bin;

    cout << "kx " << kx << endl;
    fo << v_run[i] << "  " << kx << endl;

    TGraph *gr = new TGraph(h_sector->GetNbinsX());
    for(int j=0;j<h_sector->GetNbinsX();j++){
      gr->SetPoint(j, h_sector->GetBinCenter(j)*kx, h_sector->GetBinContent(j));
    }

    gr->SetTitle(Form("run%04d: %f", v_run[i], kx));
    gr->SetMinimum(1.);
    gr->GetXaxis()->SetLimits(0, 120000);
    gr->GetXaxis()->SetTitle("Energy [keV]");
    gr->GetYaxis()->SetTitle("Counts");

    gr->SetMarkerStyle(21);
    gr->SetMarkerSize(0.3);
    gr->SetMarkerColor(4);
    gr->Draw("AP");
    gr_ref->Draw("P SAME");

    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->AddEntry(gr_ref, Form("run%04d",v_run[0]), "p");
    leg->AddEntry(gr, Form("run%04d",v_run[i]), "p");
    leg->SetTextSize(0.04);
    leg->Draw();

    cc->SaveAs(Form("./fig/sector%02d/run%04d_%04d.png", sector_id, v_run[0], v_run[i]));

    fi->Close();
  }

  fi_ref->Close();
  fo.close();
}

//
void fit_xy_new2(int ring_id, int sector_id)
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
