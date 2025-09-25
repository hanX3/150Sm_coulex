#include <sys/stat.h>
#include <sys/types.h>

//
void draw_sample_single()
{
  TFile *fi = TFile::Open("../rootfile/si/data0616_build_s3_200ns_no_s3cor.root");
  TTree *tr_event = (TTree*)fi->Get("tr_event");

  /*
  TH2D *hh = new TH2D("hh", "", 10000,0,100000,10000,0,100000);
  hh->GetXaxis()->SetTitle("ring energy [keV]");
  hh->GetYaxis()->SetTitle("sector energy [keV]");

  Long64_t n = tr_event->Draw("s3_sector_energy:s3_ring_energy>>hh", "", "goff", tr_event->GetEntries()/32);
  cout << "n " << n << endl;

  TCanvas *cc = new TCanvas("cc", "", 0, 0, 600, 400);
  cc->cd();
  cc->SetLogz();
  hh->Draw("colz");
  cc->SaveAs("fig/cc.png");
  */

  //
  TH1D *h_sector = new TH1D("h_sector", "", 32, 0, 32);
  tr_event->Draw("n_s3_sector>>h_sector", "s3_sector_energy>800", "goff", tr_event->GetEntries()/32);
  h_sector->GetXaxis()->SetTitle("n s3 sector");
  h_sector->GetYaxis()->SetTitle("counts");

  TCanvas *cc_sector = new TCanvas("cc_sector", "", 0, 0, 600, 400);
  cc_sector->cd();
  cc_sector->SetLogy();
  h_sector->Draw("");

  cc_sector->SaveAs("fig/cc_sector.png");

  //
  TH1D *h_ring = new TH1D("h_ring", "", 24, 0, 24);
  tr_event->Draw("n_s3_ring>>h_ring", "s3_ring_energy>200.", "goff", tr_event->GetEntries()/32);
  h_ring->GetXaxis()->SetTitle("n s3 sector");
  h_ring->GetYaxis()->SetTitle("counts");

  TCanvas *cc_ring = new TCanvas("cc_ring", "", 0, 0, 600, 400);
  cc_ring->cd();
  cc_ring->SetLogy();
  h_ring->Draw("");

  cc_sector->SaveAs("fig/cc_ring.png");
}

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
  fi = TFile::Open(TString::Format("../rootfile/si/data%04d_gr_fast.root", run).Data());
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
  fi = TFile::Open(TString::Format("../rootfile/si/data%04d_gr_fast.root", run).Data());
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
