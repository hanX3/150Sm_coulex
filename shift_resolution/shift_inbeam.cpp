//
void shift_single(int cid, int sid, int ch, double e)
{
  // vector<int> v_run = {472, 496, 519, 546, 576, 603, 634, 660, 691};
  // vector<int> v_run = {478, 480};
  vector<int> v_run = {550, 553};

  vector<TFile*> v_file;

  for(int i=0;i<(int)v_run.size();i++){
    v_file.push_back(TFile::Open(TString::Format("../rootfile/ge/data%04d_hist_cali.root", v_run[i]).Data()));
  }

  vector<TH1D*> v_h;
  for(int i=0;i<(int)v_run.size();i++){
    if(v_file[i]->IsZombie()){
      cout << "can not open " << v_file[i]->GetName() << endl;
      return;
    }
    v_file[i]->cd(TString::Format("cid%d_sid%02d", cid, sid).Data());
    if(gDirectory->Get(TString::Format("h_cid%d_sid%02d_ch%02d", cid, sid, ch).Data())==nullptr){
      cout << "no data" << endl;
      return ;    
    }
    v_h.push_back((TH1D*)gDirectory->Get(TString::Format("h_cid%d_sid%02d_ch%02d", cid, sid, ch).Data()));
  }

  TCanvas *c = new TCanvas(TString::Format("c_cid%d_sid%02d_ch%02d", cid, sid, ch).Data());
  c->cd();
  c->SetLogy();
  
  TLegend *leg = new TLegend(0.1, 0.5, 0.4, 0.9);
  for(int i=0;i<(int)v_run.size();i++){
    cout  << v_h[i]->GetName() << endl;
    
    v_h[i]->GetXaxis()->SetRangeUser(e-50., e+50.);
    v_h[i]->GetYaxis()->SetRangeUser(1, 1000000);
    v_h[i]->SetLineColor(i+1);
    
    TF1 *tf = new TF1("tf", "gaus(0)+pol1(3)", 20, 4096);
    tf->SetParameters(v_h[i]->GetBinContent(e/v_h[i]->GetBinWidth(1)), e, 2*2, 0, 0);
    v_h[i]->Fit(tf, "N", "", e-15., e+15.);

    leg->AddEntry(v_h[i], TString::Format("run%04d --> %6.2f",v_run[i],tf->GetParameter(1)).Data(), "l");

    if(i==0) v_h[i]->Draw();
    else v_h[i]->Draw("same");
  }

  leg->SetTextSize(0.036);
  leg->Draw();

  c->SaveAs(TString::Format("c_cid%d_sid%02d_ch%02d.png", cid, sid, ch).Data());
}

//
void shift_all(double e)
{
  gStyle->SetOptStat(0);
  gROOT->SetBatch(1);

  int cid = 0;
  for(int sid=2;sid<=5;sid++){
    for(int ch=0;ch<16;ch++){
      shift_single(cid, sid, ch, e);
    }
  }
}

