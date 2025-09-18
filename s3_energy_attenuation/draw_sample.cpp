//
void draw_ring(int run)
{
  TFile *fi = TFile::Open(TString::Format("../rootfile/si/data%04d_ring_hist.root", run).Data());
  if(fi->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  TCanvas *cc = new TCanvas(Form("cc_ring_run%04d", run), "", 600., 400.);
  cc->SetLogy();
  cc->cd();
  
  for(int i=0;i<24;i++){
    TH1D *h_ring = (TH1D*)fi->Get(Form("h_ring%02d", i+1));
    h_ring->SetTitle(Form("run%04d", run));
    h_ring->GetXaxis()->SetTitle("Energy[keV]");
    h_ring->GetYaxis()->SetTitle("Counts");
    h_ring->Draw();

    cc->SaveAs(Form("./fig/ring%02d/cc_run%04d.png", i+1, run));
    delete h_ring;
  }

  delete cc;
  fi->Close();
}

//
void draw_sector(int run)
{
  TFile *fi = TFile::Open(TString::Format("../rootfile/si/data%04d_sector_hist.root", run).Data());
  if(fi->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  TCanvas *cc = new TCanvas(Form("cc_sector_run%04d", run), "", 600., 400.);
  cc->SetLogy();
  cc->cd();
  
  for(int i=0;i<32;i++){
    TH1D *h_sector = (TH1D*)fi->Get(Form("h_sector%02d", i+1));
    h_sector->SetTitle(Form("run%04d", run));
    h_sector->GetXaxis()->SetTitle("Energy[keV]");
    h_sector->GetYaxis()->SetTitle("Counts");
    h_sector->Draw();

    cc->SaveAs(Form("./fig/sector%02d/cc_run%04d.png", i+1, run));
    delete h_sector;
  }

  delete cc;
  fi->Close();
}

//
void draw_sample(int run)
{
  draw_ring(run);
  draw_sector(run);
}
