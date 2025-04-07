
void compute_resolution_single(int run, int cid, int sid, int ch)
{
  TFile *fi = TFile::Open(TString::Format("../rootfile/ge/data%04d_hist_cali.root", run).Data());
  if(fi->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }
  TH1D *h1;
  
  gFile->cd(TString::Format("cid%d_sid%02d", cid, sid).Data());
  h1 = (TH1D*)gDirectory->Get(TString::Format("h_cid%d_sid%02d_ch%02d", cid, sid, ch).Data());
  if(!h1){
    cout << "no data" << endl;
    return;
  }

  TCanvas *c1 = new TCanvas(TString::Format("c_cid%d_sid%02d_ch%02d", cid, sid, ch).Data());
  c1->cd();
  c1->SetLogy();
  h1->GetXaxis()->SetRangeUser(1350, 1450);
  h1->Draw();
  
  TF1 *f1 = new TF1("f1", "gaus(0)+pol1(3)", 20, 4096);
  f1->SetParameters(h1->GetBinContent(1408/h1->GetBinWidth(1)), 1408, 2*2, 0, 0);
  h1->Fit(f1, "", "", 1380, 1430);

  double mean = f1->GetParameter(1);
  double sigma = f1->GetParameter(2);
  double r = 2.355*sigma/mean;

  cout << TString::Format("h_cid%d_sid%02d_ch%02d", cid, sid, ch) << " resolution " << 100*r << "%" << endl;


  TPaveLabel *pp = new TPaveLabel(0.12,0.8,0.3,0.88, TString::Format("%10.3f%%",100*r).Data(), "cnNDC");
  pp->SetTextColor(2);
  pp->Draw();

  c1->Update();
  c1->SaveAs(TString::Format("c_cid%d_sid%02d_ch%02d.png", cid, sid, ch).Data());

  fi->Close();
}

//
void compute_resolution_all(int run)
{
  gROOT->SetBatch(1);

  int cid = 0;
  for(int sid=2;sid<=5;sid++){
    for(int ch=0;ch<16;ch++){
      compute_resolution_single(run, cid, sid, ch);
    }
  }
}
