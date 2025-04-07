//
void shift_single(int cid, int sid, int ch)
{
  TFile *fi_0377 = TFile::Open("../rootfile/ge/data0377_hist_cali.root");
  TFile *fi_0722 = TFile::Open("../rootfile/ge/data0722_hist_cali.root");

  if(fi_0377->IsZombie()){
    cout << "can not open data0377_hist_cali.root" << endl;
    return;
  }

  if(fi_0722->IsZombie()){
    cout << "can not open data0722_hist_cali.root" << endl;
    return;
  }

  TH1D *h_0377;
  fi_0377->cd(TString::Format("cid%d_sid%02d", cid, sid).Data());
  h_0377 = (TH1D*)gDirectory->Get(TString::Format("h_cid%d_sid%02d_ch%02d", cid, sid, ch).Data());
  if(!h_0377){
    cout << "no data" << endl;
    return;
  }

  TH1D *h_0722;
  fi_0722->cd(TString::Format("cid%d_sid%02d", cid, sid).Data());
  h_0722 = (TH1D*)gDirectory->Get(TString::Format("h_cid%d_sid%02d_ch%02d", cid, sid, ch).Data());
  if(!h_0722){
    cout << "no data" << endl;
    return;
  }

  //
  TCanvas *c1 = new TCanvas(TString::Format("c1_cid%d_sid%02d_ch%02d", cid, sid, ch).Data());
  c1->cd();
  c1->SetLogy();
  h_0377->GetXaxis()->SetRangeUser(310, 410);
  h_0377->SetLineColor(2);

  h_0722->GetXaxis()->SetRangeUser(310, 410);
  h_0722->SetLineColor(4);

  h_0377->Draw();
  h_0722->Draw("same");

  TF1 *tf1 = new TF1("tf1", "gaus(0)+pol1(3)", 20, 4096);
  tf1->SetParameters(h_0377->GetBinContent(344/h_0377->GetBinWidth(1)), 344, 2*2, 0, 0);
  h_0377->Fit(tf1, "N", "", 320, 350);
  double x1_0377 = tf1->GetParameter(1);
  cout << "x1_0377 " << x1_0377 << endl; 

  tf1->SetParameters(h_0722->GetBinContent(344/h_0722->GetBinWidth(1)), 344, 2*2, 0, 0);
  h_0722->Fit(tf1, "N", "", 320, 350);
  double x1_0722 = tf1->GetParameter(1);
  cout << "x1_0722 " << x1_0722 << endl; 

  TLegend *leg1 = new TLegend(0.1, 0.7, 0.4, 0.9);
  leg1->AddEntry(h_0377, TString::Format("%04d -> %6.2fkeV",377,x1_0377).Data(), "l");
  leg1->AddEntry(h_0722, TString::Format("%04d -> %6.2fkeV",722,x1_0722).Data(), "l");
  leg1->SetTextSize(0.036);
  leg1->Draw();

  c1->SaveAs(TString::Format("c1_cid%d_sid%02d_ch%02d.png", cid, sid, ch).Data());

  //
  TCanvas *c2 = new TCanvas(TString::Format("c2_cid%d_sid%02d_ch%02d", cid, sid, ch).Data());
  c2->cd();
  c2->SetLogy();
  h_0377->GetXaxis()->SetRangeUser(1350, 1450);
  h_0377->SetLineColor(2);

  h_0722->GetXaxis()->SetRangeUser(1350, 1450);
  h_0722->SetLineColor(4);

  h_0377->Draw();
  h_0722->Draw("same");

  TF1 *tf2 = new TF1("tf2", "gaus(0)+pol1(3)", 20, 4096);
  tf2->SetParameters(h_0377->GetBinContent(1408/h_0377->GetBinWidth(1)), 1408, 2*2, 0, 0);
  h_0377->Fit(tf2, "N", "", 1380, 1420);
  double x2_0377 = tf2->GetParameter(1);
  cout << "x2_0377 " << x2_0377 << endl; 

  tf2->SetParameters(h_0722->GetBinContent(1408/h_0722->GetBinWidth(1)), 1408, 2*2, 0, 0);
  h_0722->Fit(tf2, "N", "", 1380, 1420);
  double x2_0722 = tf2->GetParameter(1);
  cout << "x2_0722 " << x2_0722 << endl; 

  TLegend *leg2 = new TLegend(0.1, 0.7, 0.4, 0.9);
  leg2->AddEntry(h_0377, TString::Format("%04d -> %6.2fkeV",377,x2_0377).Data(), "l");
  leg2->AddEntry(h_0722, TString::Format("%04d -> %6.2fkeV",722,x2_0722).Data(), "l");
  leg2->SetTextSize(0.036);
  leg2->Draw();

  c2->SaveAs(TString::Format("c2_cid%d_sid%02d_ch%02d.png", cid, sid, ch).Data());

  fi_0377->Close();
  fi_0722->Close();
}

//
void shift_all()
{
  gStyle->SetOptStat(0);
  gROOT->SetBatch(1);

  int cid = 0;
  for(int sid=2;sid<=5;sid++){
    for(int ch=0;ch<16;ch++){
      shift_single(cid, sid, ch);
    }
  }
}

