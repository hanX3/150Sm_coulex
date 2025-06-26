#include "set.h"

//
void draw_sample(string str)
{
  TFile *fi = TFile::Open(TString::Format("../rootfile/data0472_0597_ge_tw_th2d_event_%s.root",str.c_str()));
  if(fi->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  int n_max_ge = GENUM; 
  
  TH2D *hh[n_max_ge];
  TH1D *h[n_max_ge];

  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      int k = (i-2)*16+j;

      hh[k] = (TH2D*)fi->Get(TString::Format("hh_spider_ge_sid%d_ch%02d",i,j).Data());
      cout << hh[k]->GetEntries() << endl;
    }
  }

  TCanvas *c3[n_max_ge]; 

  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      int k = (i-2)*16+j;

      if(hh[k]->GetEntries()==0){
        continue;
      }

      c3[k] = new TCanvas(TString::Format("cc3_%d_%02d",i,j), "", 0, 0, 500, 400);

      c3[k]->cd();
      c3[k]->SetLogz();
      hh[k]->Draw("colz");

      c3[k]->cd();
      h[k] = (TH1D*)hh[k]->ProjectionX(TString::Format("sid%d_ch%02d",i,j), 20, 200);
      h[k]->Draw();

      c3[k]->SaveAs(TString::Format("./fig/%s/c3_%s.png",str.c_str(),h[k]->GetName()).Data());
    }
  }

  fi->Close();
}
