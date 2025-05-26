//
std::map<int, int> m_i2id;
void init_id()
{
  m_i2id = {
    {0, 10},
    {1, 12},
    {2, 14},
    {3, 16},
    {4, 18},
    {5, 20},
    {6, 22},
    {7, 24},
    {8, 26},
    {9, 28},
    {10, 30},
    {11, 32},
    {12, 2},
    {13, 4},
    {14, 6},
    {15, 8},
    {16, 11},
    {17, 13},
    {18, 15},
    {19, 17},
    {20, 19},
    {21, 21},
    {22, 23},
    {23, 25},
    {24, 27},
    {25, 29},
    {26, 31},
    {27, 1},
    {28, 3},
    {29, 5},
    {30, 7},
    {31, 9}
  };

}

//
void draw_sector(int run)
{
  init_id();

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TFile *fi;
  TH1D *h[32];
  if((run>=457 && run<=462) || (run>=472 && run<=605) || (run>=680 && run<=718)){
    fi = TFile::Open(TString::Format("../rootfile/si/data%04d_hist_cali.root", run).Data());
    if(fi->IsZombie()){
      cout << "wrong rootfile." << endl;
      return;
    }

    gFile->cd();
    gFile->cd("cid1_sid09");
    for(int i=0;i<16;i++){
      h[i] = (TH1D*)gDirectory->Get(TString::Format("h_cid1_sid09_ch%02d", i).Data());
    }

    gFile->cd();
    gFile->cd("cid1_sid10");
    for(int i=0;i<16;i++){
      h[i+16] = (TH1D*)gDirectory->Get(TString::Format("h_cid1_sid10_ch%02d", i).Data());
    }
  }else if((run>=616 && run<=674) || (run>=721 && run<=723)){
    fi = TFile::Open(TString::Format("../rootfile/si/data%04d_hist_cali.root", run).Data());
    if(fi->IsZombie()){
      cout << "wrong rootfile." << endl;
      return;
    }

    gFile->cd();
    gFile->cd("cid1_sid02");
    for(int i=0;i<16;i++){
      h[i] = (TH1D*)gDirectory->Get(TString::Format("h_cid1_sid02_ch%02d", i).Data());
    }

    gFile->cd();
    gFile->cd("cid1_sid03");
    for(int i=0;i<16;i++){
      h[i+16] = (TH1D*)gDirectory->Get(TString::Format("h_cid1_sid03_ch%02d", i).Data());
    }

  }else{
    cout << "wrong run number" << endl;
    return;
  }

  TCanvas *cc = new TCanvas("cc", "", 0, 0, 600, 400);
  cc->cd();

  TLegend *leg = new TLegend(0.1, 0.1, 0.4, 0.9);

  for(int i=0;i<32;i++){
    h[i]->Rebin(20);
    
    if(i==0){
      h[i]->GetYaxis()->SetRangeUser(0, 60000);
      h[i]->Draw("hist");  
    }

    h[i]->Draw("same hist");

    leg->AddEntry(h[i]);
  }

  // leg->Draw();
  // fi->Close();
}

//
void draw_ring(int run)
{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TFile *fi;
  TH1D *h[24];

  fi = TFile::Open(TString::Format("../rootfile/si/data%04d_hist_cali.root", run).Data());
  if(fi->IsZombie()){
    cout << "wrong rootfile." << endl;
    return;
  }

  gFile->cd();
  gFile->cd("cid1_sid11");
  for(int i=2;i<14;i++){
    h[i-2] = (TH1D*)gDirectory->Get(TString::Format("h_cid1_sid11_ch%02d", i).Data());
  }

  gFile->cd();
  gFile->cd("cid1_sid12");
  for(int i=0;i<12;i++){
    h[i+12] = (TH1D*)gDirectory->Get(TString::Format("h_cid1_sid12_ch%02d", i).Data());
  }

  TCanvas *cc = new TCanvas("cc", "", 0, 0, 600, 400);
  cc->cd();

  h[0]->Draw("hist");
  for(int i=1;i<24;i++){
    h[i]->Draw("same hist");
  }

  // fi->Close();
}

//
void draw_th2d(int run)
{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TFile *fi;
  fi = TFile::Open(TString::Format("../rootfile/si/data%04d_hist_cali.root", run).Data());
  if(fi->IsZombie()){
    cout << "wrong rootfile." << endl;
    return;
  }

  TH2D *hh_sector, *hh_ring;
  hh_sector = (TH2D*)fi->Get("hh_sector");
  TCanvas *c2d_1 = new TCanvas("c2d_1", "", 0, 0, 500, 400);
  c2d_1->cd();
  c2d_1->SetLogz();
  hh_sector->Draw("colz");
  c2d_1->SaveAs("c2d_1.png");

  hh_ring = (TH2D*)fi->Get("hh_ring");
  TCanvas *c2d_2 = new TCanvas("c2d_2", "", 0, 0, 500, 400);
  c2d_2->cd();
  c2d_2->SetLogz();
  hh_ring->Draw("colz");
  c2d_2->SaveAs("c2d_2.png");
  
  // fi->Close();
}
