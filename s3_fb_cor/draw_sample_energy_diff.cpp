//
void draw_sample_energy_diff(int run, int ring, int sector)
{
  TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_build_s3_200ns.root",run));
  TTree *tr_event = (TTree*)fi->Get("tr_event");

  //
  TH1D *h_diff = new TH1D("h_diff", "", 5000, -25000, 25000);
  TString cut_str = Form("n_s3_sector==1 && n_s3_ring==1");
  // TString cut_str = Form("n_s3_sector==1 && n_s3_ring==1 && s3_ring_id==%d && s3_sector_id==%d", ring, sector);

  tr_event->Draw("s3_sector_energy-s3_ring_energy>>h_diff", cut_str.Data(), "goff", tr_event->GetEntries()/32);
  h_diff->SetTitle("energy difference between sector and ring");
  h_diff->GetXaxis()->SetTitle("energy [keV]");
  h_diff->GetYaxis()->SetTitle("counts");

  TCanvas *cc_diff = new TCanvas("cc_diff", "", 0, 0, 600, 400);
  cc_diff->cd();
  cc_diff->SetLogy();
  h_diff->Draw("");

  cc_diff->SaveAs(Form("fig/%s.png", cc_diff->GetName()));
}
