//
void draw_sample_rs_th2(int run, int ring, int sector)
{
  TString cut_str = Form("s3_ring_id==%d && s3_sector_id==%d", ring, sector);

  //
  TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_build_s3_200ns.root",run));
  TTree *tr_event = (TTree*)fi->Get("tr_event");

  TH2D *hh = new TH2D("hh", "", 1000, 0, 100000, 1000, 0, 100000);

  tr_event->Draw("s3_sector_energy:s3_ring_energy>>hh", cut_str.Data(), "goff", tr_event->GetEntries());
  hh->SetTitle("th2 between sector and ring");
  hh->GetXaxis()->SetTitle("energy [keV]");
  hh->GetYaxis()->SetTitle("energy [keV]");

  TCanvas *cc = new TCanvas("cc", "", 0, 0, 600, 400);
  cc->cd();
  cc->SetLogz();
  hh->Draw("");

  cc->SaveAs(Form("fig/%s.png", cc->GetName()));

  //
  TFile *fi_new = TFile::Open(Form("../rootfile/si/data%04d_build_s3_200ns_new.root",run));
  TTree *tr_event_new = (TTree*)fi_new->Get("tr_event");

  TH2D *hh_new = new TH2D("hh_new", "", 1000, 0, 100000, 1000, 0, 100000);

  tr_event_new->Draw("s3_sector_energy:s3_ring_energy>>hh_new", cut_str.Data(), "goff", tr_event_new->GetEntries());
  hh_new->SetTitle("th2 between sector and ring new");
  hh_new->GetXaxis()->SetTitle("energy [keV]");
  hh_new->GetYaxis()->SetTitle("energy [keV]");

  TCanvas *cc_new = new TCanvas("cc_new", "", 0, 0, 600, 400);
  cc_new->cd();
  cc_new->SetLogz();
  hh_new->Draw("");

  cc_new->SaveAs(Form("fig/%s.png", cc_new->GetName()));
}
