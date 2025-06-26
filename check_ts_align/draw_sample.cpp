#include "set.h"

//
void draw_sample(string tf_str, string cut_str)
{
  gStyle->SetOptStat(1);

  TFile *fi_ge_tw = TFile::Open(TString::Format("../rootfile/data0472_0597_ge_tw_th2d_event_%s.root",cut_str.c_str()));
  if(fi_ge_tw->IsZombie()){
    cout << "can not open ge_tw rootfile." << endl;
    return;
  }

  TFile *fi_align = TFile::Open(TString::Format("../rootfile/data0472_0597_align_th2d_event_%s_%s.root",tf_str.c_str(),cut_str.c_str()));
  if(fi_align->IsZombie()){
    cout << "can not open align rootfile." << endl;
    return;
  }

  int n_max_ge = GENUM; 
  
  TH2D *hh_ge_tw[n_max_ge], *hh_align[n_max_ge];
  TH1D *h_align_profile[n_max_ge];
  TH1D *h_ge_tw[n_max_ge], *h_align[n_max_ge];

  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      int k = (i-2)*16+j;

      hh_ge_tw[k] = (TH2D*)fi_ge_tw->Get(TString::Format("hh_spider_ge_sid%d_ch%02d",i,j).Data());
      // cout << hh_ge_tw[k]->GetEntries() << endl;
      hh_align[k] = (TH2D*)fi_align->Get(TString::Format("hh_spider_ge_sid%d_ch%02d",i,j).Data());
      // cout << hh_align[k]->GetEntries() << endl;
    }
  }

  TCanvas *c3[n_max_ge], *c4[n_max_ge];
  TLegend *leg[n_max_ge];

  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      int k = (i-2)*16+j;

      if(hh_ge_tw[k]->GetEntries()==0 || hh_ge_tw[k]->GetEntries()==0){
        continue;
      }

      c3[k] = new TCanvas(TString::Format("cc3_%d_%02d",i,j), "", 0, 0, 500, 400);
      c4[k] = new TCanvas(TString::Format("cc4_%d_%02d",i,j), "", 0, 0, 500, 400);

      c3[k]->cd();
      h_align_profile[k] = (TH1D*)hh_align[k]->ProfileY(TString::Format("sid%d_ch%02d_align_profile",i,j), 10, 1024);
      h_align_profile[k]->Rebin(2);
      h_align_profile[k]->GetXaxis()->SetRangeUser(40, 2000);
      h_align_profile[k]->Draw();

      c4[k]->cd();
      c4[k]->SetGridx();
      c4[k]->SetGridy();
      h_ge_tw[k] = (TH1D*)hh_ge_tw[k]->ProjectionX(TString::Format("sid%d_ch%02d_ge_tw",i,j), 10, 1024);
      h_align[k] = (TH1D*)hh_align[k]->ProjectionX(TString::Format("sid%d_ch%02d_align",i,j), 10, 1024);
      
      h_ge_tw[k]->SetLineColor(2);
      h_align[k]->SetLineColor(4);
      h_align[k]->GetXaxis()->SetRangeUser(-500, 500);
      h_align[k]->Draw();
      h_ge_tw[k]->Draw("same");

      leg[k] = new TLegend(0.75, 0.6, 0.98, 0.75);
      leg[k]->AddEntry(h_ge_tw[k], "before");
      leg[k]->AddEntry(h_align[k], "after");
      leg[k]->Draw("same");

      c3[k]->SaveAs(TString::Format("./fig/%s/%s/c3_%s.png",cut_str.c_str(),tf_str.c_str(),TString::Format("sid%d_ch%02d",i,j).Data()).Data());
      c4[k]->SaveAs(TString::Format("./fig/%s/%s/c4_%s.png",cut_str.c_str(),tf_str.c_str(),TString::Format("sid%d_ch%02d",i,j).Data()).Data());
    }
  }

  //fi_ge_tw->Close();
  //fi_align->Close();
}
