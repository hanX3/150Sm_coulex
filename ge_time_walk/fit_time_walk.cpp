#include "set.h"

//
void fit_time_walk(TString str)
{
  TFile *fi_event = TFile::Open("../rootfile/data0472_0597_ge_tw_th2d_event.root");
  if(fi_event->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  int n_max_ge = GENUM; 
  
  TH2D *hh_event[n_max_ge];
  TH1D *h_event[n_max_ge];

  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      int k = (i-2)*16+j;

      hh_event[k] = (TH2D*)fi_event->Get(TString::Format("hh_spider_ge_sid%d_ch%02d",i,j).Data());
      cout << hh_event[k]->GetEntries() << endl;
    }
  }

  ofstream fo("./par/ge_time_walk.txt");
  if(str.Contains("tf1")) fo << "sid  ch  p0  p1  p2  p3" << endl;
  else if(str.Contains("tf2")) fo << "sid  ch  p0  p1  p2" << endl;
  else{
    cout << "wrong tf" << endl;
    return;
  }
  TCanvas *c1[n_max_ge], *c2[n_max_ge], *c3[n_max_ge]; 

  for(int i=2;i<=5;i++){
    for(int j=0;j<16;j++){
      int k = (i-2)*16+j;

      if(hh_event[k]->GetEntries()==0){
        if(str.Contains("tf1")) fo << i << " " << j << "  0  0  0  0" << endl;
        else fo << i << " " << j << "  0  0  0" << endl;
        continue;
      }

      c1[k] = new TCanvas(TString::Format("cc1_%d_%02d",i,j), "", 0, 0, 500, 400);
      c2[k] = new TCanvas(TString::Format("cc2_%d_%02d",i,j), "", 0, 0, 500, 400);

      c1[k]->cd();
      c1[k]->SetLogz();
      hh_event[k]->Draw("colz");

      c2[k]->cd();
      h_event[k] = (TH1D*)hh_event[k]->ProfileY(TString::Format("sid%d_ch%02d",i,j).Data());
      h_event[k]->Rebin(4);
      h_event[k]->GetXaxis()->SetRangeUser(60, 2000);
      h_event[k]->Draw();

      TF1 *tf1 = new TF1("tf1","[0]+[1]/pow(x,0.5)+[2]/pow(x,1)+[3]/pow(x,2)", 80, 4096);
      TF1 *tf2 = new TF1("tf2","[0]+[1]*exp([2]/pow(x,1.5))", 80, 4096);
      h_event[k]->Fit(str.Data(), "RW");

      TF1 *tf = h_event[k]->GetFunction(str.Data());
      fo << i << " " << j << " ";
      for(int w=0;w<tf->GetNpar();++w){
        fo << tf->GetParameter(w) << " ";
      }
      fo << endl;
      
      c1[k]->SaveAs(TString::Format("./fig/c1_%s.png",h_event[k]->GetName()).Data());
      c2[k]->SaveAs(TString::Format("./fig/c2_%s.png",h_event[k]->GetName()).Data());
    }
  }

  fo.close();
  fi_event->Close();
}
