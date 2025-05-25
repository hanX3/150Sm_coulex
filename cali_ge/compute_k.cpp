//
void compute_k_single(ofstream &fo, int run, int cid, int sid, int ch)
{
  TFile *fi =TFile::Open(TString::Format("../rootfile/ge/data%04d_hist_cali.root", run).Data());
  if(fi->IsZombie()){
    cout << "can not open input file" << endl;
    return;
  }

  TH1D *h = (TH1D*)fi->Get<TNamed>(TString::Format("cid%d_sid%02d/h_cid%d_sid%02d_ch%02d", cid, sid, cid, sid, ch).Data());
  if(!h){
    fo << TString::Format("%4d  %4d  %4d  %16.9f", cid, sid, ch, 0.) << endl;
    fi->Close();
    
    return;
  }

  TF1 *tf = new TF1("tf", "gaus(0)+pol1(3)", 20, 4096);
  tf->SetParameters(h->GetBinContent(511./h->GetBinWidth(1)), 511, 2*2, 0, 0);
  h->Fit(tf, "N", "", 500, 530);
  double k = 511./tf->GetParameter(1);

  fo << TString::Format("%4d  %4d  %4d  %16.9f", cid, sid, ch, k) << endl;

  fi->Close();
}

//
void compute_k_all(int run)
{
  ofstream fo(TString::Format("k_%04d.txt",run).Data(), std::ios::app);

  fo << " cid   sid    ch                k" << endl;
  
  int cid = 0;
  for(int sid=2;sid<=5;sid++){
    for(int ch=0;ch<16;ch++){
      compute_k_single(fo, run, cid, sid, ch);
    }
  }
  
  fo.close();
}
