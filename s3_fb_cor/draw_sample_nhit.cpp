//
void draw_sample_nhit()
{
  TFile *fi = TFile::Open("../rootfile/si/data0616_build_s3_200ns.root");
  TTree *tr_event = (TTree*)fi->Get("tr_event");

  //
  TH1D *h_sector = new TH1D("h_sector", "", 32, 1, 33);
  tr_event->Draw("n_s3_sector>>h_sector", "s3_sector_energy>800", "goff", tr_event->GetEntries()/32);
  h_sector->GetXaxis()->SetTitle("n s3 sector");
  h_sector->GetYaxis()->SetTitle("counts");

  TCanvas *cc_sector = new TCanvas("cc_nhit_sector", "", 0, 0, 600, 400);
  cc_sector->cd();
  cc_sector->SetLogy();
  h_sector->Draw("");

  cc_sector->SaveAs(Form("fig/%s.png", cc_sector->GetName()));

  //
  TH1D *h_ring = new TH1D("h_ring", "", 24, 1, 25);
  tr_event->Draw("n_s3_ring>>h_ring", "s3_ring_energy>200.", "goff", tr_event->GetEntries()/32);
  h_ring->GetXaxis()->SetTitle("n s3 sector");
  h_ring->GetYaxis()->SetTitle("counts");

  TCanvas *cc_ring = new TCanvas("cc_nhit_ring", "", 0, 0, 600, 400);
  cc_ring->cd();
  cc_ring->SetLogy();
  h_ring->Draw("");

  cc_ring->SaveAs(Form("fig/%s.png", cc_ring->GetName()));
}
