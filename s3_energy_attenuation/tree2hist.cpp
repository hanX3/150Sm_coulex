//
void tree2hist_sector(int run)
{
  TFile *fi = TFile::Open(TString::Format("../rootfile/si/data%04d_build_s3_200ns_no_s3cor.root", run).Data());
  if(fi->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  TTree *tr = (TTree*)fi->Get("tr_event");
  TH1D *h_sector[32];

  int n = 0;
  TString draw_str, cut_str;
  for(int i=1;i<=32;i++){
    h_sector[i-1] = new TH1D(TString::Format("h_sector%02d",i).Data(), "", 1000,0,100000);
    TString draw_str = TString::Format("s3_sector_energy>>%s", h_sector[i-1]->GetName());
    TString cut_str = TString::Format("s3_sector_id==%d",i);
    cout << draw_str << endl;
    cout << cut_str << endl;
    cout << endl;

    n = tr->Draw(draw_str.Data(), cut_str.Data(), "goff");
  }

  TFile *fo = new TFile(TString::Format("../rootfile/si/data%04d_sector_hist_no_s3cor.root", run).Data(), "recreate");

  fo->cd();
  for(int i=0;i<32;i++){
    h_sector[i]->Write();
  }

  fo->Close();

  fi->Close();
}

//
void tree2hist_ring(int run)
{
  TFile *fi = TFile::Open(TString::Format("../rootfile/si/data%04d_build_s3_200ns_no_s3cor.root", run).Data());
  if(fi->IsZombie()){
    cout << "can not open rootfile." << endl;
    return;
  }

  TTree *tr = (TTree*)fi->Get("tr_event");
  TH1D *h_ring[24];

  int n = 0;
  TString draw_str, cut_str;
  for(int i=1;i<=24;i++){
    h_ring[i-1] = new TH1D(TString::Format("h_ring%02d",i).Data(), "", 1000,0,100000);
    TString draw_str = TString::Format("s3_ring_energy>>%s", h_ring[i-1]->GetName());
    TString cut_str = TString::Format("s3_ring_id==%d",i);
    cout << draw_str << endl;
    cout << cut_str << endl;
    cout << endl;

    n = tr->Draw(draw_str.Data(), cut_str.Data(), "goff");
  }

  TFile *fo = new TFile(TString::Format("../rootfile/si/data%04d_ring_hist_no_s3cor.root", run).Data(), "recreate");

  fo->cd();
  for(int i=0;i<24;i++){
    h_ring[i]->Write();
  }

  fo->Close();

  fi->Close();
}

//
void tree2hist(int run)
{
  tree2hist_sector(run);
  tree2hist_ring(run);
}
