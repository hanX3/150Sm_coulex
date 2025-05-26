#include "set.h"

//
void fit_time_walk()
{
  TFile *fi_event = TFile::Open("../rootfile/data0472_0605_ts_diff_event.root");
  if(fi_event->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  TFile *fi_bg = TFile::Open("../rootfile/data0472_0605_ts_diff_bg.root");
  if(fi_bg->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  int n_max_ge = GENUM; 
  
  TH2D *hh[n_max_ge];
  TH2D *hh_event[n_max_ge], *hh_bg[n_max_ge];

  TH1D *h[n_max_ge];

  int k = -1;
  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      k = (i-2)*16+j;
      if(k!=15) continue;

      hh_event[(i-2)*16+j] = (TH2D*)fi_event->Get(TString::Format("hh_spider_ge_sid%d_ch%02d",i,j).Data());
      hh_bg[(i-2)*16+j] = (TH2D*)fi_bg->Get(TString::Format("hh_spider_ge_sid%d_ch%02d",i,j).Data());
      cout << hh_event[(i-2)*16+j]->GetEntries() << endl;
      cout << hh_bg[(i-2)*16+j]->GetEntries() << endl;
    }
  }

  ofstream fo("./par/ge_time_walk.txt");
  fo << "sid  ch  p0  p1  p2" << endl;
  TCanvas *c1[n_max_ge], *c2[n_max_ge], *c3[n_max_ge]; 

  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      k = (i-2)*16+j;
      if(k!=15) continue;
    
      if(hh_event[k]->GetEntries()==0){
        fo << i << " " << j << "  0  0  0" << endl;
        continue;
      }

      c1[k] = new TCanvas(TString::Format("cc1_%d_%02d",i,j), "", 0, 0, 500, 400);
      c2[k] = new TCanvas(TString::Format("cc2_%d_%02d",i,j), "", 0, 0, 500, 400);

      hh[k] = (TH2D*)hh_event[k]->Clone(TString::Format("ge_sid%d_ch%02d",i,j).Data());
      hh[k]->Add(hh_bg[k], -1);

      c1[k]->cd();
      hh[k]->Draw("colz");

      c2[k]->cd();
      hh[k]->RebinY(4);
      h[k] = (TH1D*)hh[k]->ProfileY(TString::Format("sid%d_ch%02d",i,j).Data(), 80, 120);
      h[k]->GetXaxis()->SetRangeUser(0, 600);
      h[k]->Draw();

      // TF1 *tf1 = new TF1("tf1","[0]+[1]/pow(x,0.5)+[2]/pow(x,1)+[3]/pow(x,2)", 50, 2000);
      TF1 *tf = new TF1("tf","[0]+[1]*exp([2]/pow(x,1.5))", 50, 2000);
      h[k]->Fit("tf", "R");
      fo << i << " " << j << " " << tf->GetParameter(0) << " " << tf->GetParameter(1) << " " << tf->GetParameter(2) << endl;

      c1[k]->SaveAs(TString::Format("./fig/c1_%s.png",h[k]->GetName()).Data());
      c2[k]->SaveAs(TString::Format("./fig/c2_%s.png",h[k]->GetName()).Data());
    }
  }

  // fi_event->Close();
}
