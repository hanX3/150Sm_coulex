#include <sys/stat.h>
#include <sys/types.h>

//
void draw_sector(int run)
{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  //
  mkdir(Form("./fig/%04d",run), 0777);

  //
  TFile *fi;
  TGraph *hh[32];
  fi = TFile::Open(TString::Format("../rootfile/si/data%04d_gr2.root", run).Data());
  if(fi->IsZombie()){
    cout << "wrong rootfile." << endl;
    return;
  }

  for(int i=0;i<32;i++){
    hh[i] = (TGraph*)gFile->Get(Form("hh_sector%02d_ring11", i+1));
  }

  TCanvas *cc[32];
  for(int i=0;i<32;i++){
    cc[i] = new TCanvas(Form("cc_sector%02d_vs_ring11",i+1), Form("cc_ring%02d_vs_sector01",i+1), 0, 0, 600, 400);
    cc[i]->cd();
    cc[i]->SetLeftMargin(0.15);
    
    hh[i]->SetPoint(hh[i]->GetN(), 100000, 100000);
    hh[i]->GetXaxis()->SetRangeUser(0, 100000);
    hh[i]->GetYaxis()->SetRangeUser(0, 100000);
    hh[i]->GetXaxis()->SetTitle("ring11 [keV]");
    hh[i]->GetYaxis()->SetTitle(Form("sector%02d [keV]",i+1));
    hh[i]->Draw("ap");  

    cc[i]->SaveAs(Form("fig/%04d/%s.png",run,cc[i]->GetName()));
  }

  fi->Close();
}

//
void draw_ring(int run)
{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  //
  mkdir(Form("./fig/%04d",run), 0777);

  //
  TFile *fi;
  TGraph *hh[24];
  fi = TFile::Open(TString::Format("../rootfile/si/data%04d_gr2.root", run).Data());
  if(fi->IsZombie()){
    cout << "wrong rootfile." << endl;
    return;
  }

  for(int i=0;i<24;i++){
    hh[i] = (TGraph*)gFile->Get(Form("hh_ring%02d_sector01", i+1));
  }

  TCanvas *cc[24];
  for(int i=0;i<24;i++){
    cc[i] = new TCanvas(Form("cc_ring%02d_vs_sector01",i+1), Form("cc_ring%02d_vs_sector01",i+1), 0, 0, 600, 400);
    cc[i]->cd();
    cc[i]->SetLeftMargin(0.15);
    
    hh[i]->SetPoint(hh[i]->GetN(), 100000, 100000);
    hh[i]->GetXaxis()->SetRangeUser(0, 100000);
    hh[i]->GetYaxis()->SetRangeUser(0, 100000);
    hh[i]->GetXaxis()->SetTitle("sector1 [keV]");
    hh[i]->GetYaxis()->SetTitle(Form("ring%02d [keV]",i+1));
    hh[i]->Draw("ap");  

    cc[i]->SaveAs(Form("fig/%04d/%s.png",run,cc[i]->GetName()));
  }

  fi->Close();
}

//
void draw_sample(int run)
{
  draw_sector(run);
  draw_ring(run);
}
