//
void draw_sample_th1(int run)
{
  TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_build_s3_200ns_new_th.root",run));

  //
  TH1D *h_phi_diff = (TH1D*)fi->Get("h_phi_diff");
  h_phi_diff->SetTitle("phi difference between projectile and recoil (should be 180 deg)");
  h_phi_diff->GetXaxis()->SetTitle("phi difference [deg]");
  h_phi_diff->GetYaxis()->SetTitle("counts");

  TCanvas *cc_phi_diff = new TCanvas("cc_phi_diff", "", 0, 0, 600, 400);
  cc_phi_diff->cd();
  cc_phi_diff->SetLogz();
  h_phi_diff->Draw("");
  cc_phi_diff->SaveAs(Form("fig/%04d/%s.png",run,cc_phi_diff->GetName()));

  //
  TH1D *h_theta_sum = (TH1D*)fi->Get("h_theta_sum");
  h_theta_sum->SetTitle("theta in cm sum between projectile and recoil (should be 180 deg)");
  h_theta_sum->GetXaxis()->SetTitle("theta in cm sum [deg]");
  h_theta_sum->GetYaxis()->SetTitle("counts");

  TCanvas *cc_theta_sum = new TCanvas("cc_theta_sum", "", 0, 0, 600, 400);
  cc_theta_sum->cd();
  cc_theta_sum->SetLogz();
  h_theta_sum->Draw("");
  cc_theta_sum->SaveAs(Form("fig/%04d/%s.png",run,cc_theta_sum->GetName()));

  //
  TH1D *h_p = (TH1D*)fi->Get("h_p");
  h_p->SetTitle("projectile energy of 2 hits in s3");
  h_p->GetXaxis()->SetTitle("energy [keV]");
  h_p->GetYaxis()->SetTitle("counts");

  TCanvas *cc_p = new TCanvas("cc_p", "", 0, 0, 600, 400);
  cc_p->cd();
  cc_p->SetLogy();
  h_p->Draw("");
  cc_p->SaveAs(Form("fig/%04d/%s.png",run,cc_p->GetName()));

  //
  TH1D *h_r = (TH1D*)fi->Get("h_r");
  h_r->SetTitle("recoil energy of 2 hits in s3");
  h_r->GetXaxis()->SetTitle("energy [keV]");
  h_r->GetYaxis()->SetTitle("counts");

  TCanvas *cc_r = new TCanvas("cc_r", "", 0, 0, 600, 400);
  cc_r->cd();
  cc_r->SetLogy();
  h_r->Draw("");
  cc_r->SaveAs(Form("fig/%04d/%s.png",run,cc_r->GetName()));

  fi->Close();
}

//
void draw_sample_th2(int run)
{
  TFile *fi = TFile::Open(Form("../rootfile/si/data%04d_build_s3_200ns_new_th.root",run));

  //
  TH2D *hh_e_ring_id = (TH2D*)fi->Get("hh_e_ring_id");
  hh_e_ring_id->SetTitle("th2 energy vs ring");
  hh_e_ring_id->GetXaxis()->SetTitle("ring id");
  hh_e_ring_id->GetYaxis()->SetTitle("energy [keV]");

  TCanvas *cc_e_ring_id = new TCanvas("cc_e_ring_id", "", 0, 0, 600, 400);
  cc_e_ring_id->cd();
  cc_e_ring_id->SetLogz();
  hh_e_ring_id->Draw("colz");
  cc_e_ring_id->SaveAs(Form("fig/%04d/%s.png",run,cc_e_ring_id->GetName()));

  //
  TH2D *hh_e_theta = (TH2D*)fi->Get("hh_e_theta");
  hh_e_theta->SetTitle("th2 energy vs ring");
  hh_e_theta->GetXaxis()->SetRangeUser(45, 80);
  hh_e_theta->GetXaxis()->SetTitle("theta in lab [deg]");
  hh_e_theta->GetYaxis()->SetTitle("energy [keV]");

  TCanvas *cc_e_theta = new TCanvas("cc_e_theta", "", 0, 0, 600, 400);
  cc_e_theta->cd();
  cc_e_theta->SetLogz();
  hh_e_theta->Draw("colz");
  cc_e_theta->SaveAs(Form("fig/%04d/%s.png",run,cc_e_theta->GetName()));

  //
  TH2D *hh_e_theta_coin = (TH2D*)fi->Get("hh_e_theta_coin");
  hh_e_theta_coin->SetTitle("th2 energy vs ring of 2 hits in s3");
  hh_e_theta_coin->GetXaxis()->SetRangeUser(45, 80);
  hh_e_theta_coin->GetXaxis()->SetTitle("theta in lab [deg]");
  hh_e_theta_coin->GetYaxis()->SetTitle("energy [keV]");

  TCanvas *cc_e_theta_coin = new TCanvas("cc_e_theta_coin", "", 0, 0, 600, 400);
  cc_e_theta_coin->cd();
  cc_e_theta_coin->SetLogz();
  hh_e_theta_coin->Draw("colz");
  cc_e_theta_coin->SaveAs(Form("fig/%04d/%s.png",run,cc_e_theta_coin->GetName()));

  //
  TH2D *hh_theta_pr = (TH2D*)fi->Get("hh_theta_pr");
  hh_theta_pr->SetTitle("th2 energy vs ring");
  hh_theta_pr->GetXaxis()->SetRangeUser(45, 80);
  hh_theta_pr->GetYaxis()->SetRangeUser(45, 80);
  hh_theta_pr->GetXaxis()->SetTitle("theta projectile in lab [deg]");
  hh_theta_pr->GetYaxis()->SetTitle("theta recoil in lab [deg]");

  TCanvas *cc_theta_pr = new TCanvas("cc_theta_pr", "", 0, 0, 600, 400);
  cc_theta_pr->cd();
  cc_theta_pr->SetLogz();
  hh_theta_pr->Draw("colz");
  cc_theta_pr->SaveAs(Form("fig/%04d/%s.png",run,cc_theta_pr->GetName()));

  fi->Close();
}

//
void draw_sample(int run)
{
  gROOT->SetBatch(1);

  draw_sample_th1(run);
  draw_sample_th2(run);
}
