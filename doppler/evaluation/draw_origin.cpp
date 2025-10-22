//exaple str="doppler_100ns_p1"
void draw_origin(string str="")
{
  gStyle->SetOptStat(0);

  double x_min = 300;
  double x_max = 800;
  int r, s;
  int k = 0;
 
  int colors[24] = {
    TColor::GetColor("#E41A1C"), TColor::GetColor("#377EB8"),
    TColor::GetColor("#4DAF4A"), TColor::GetColor("#984EA3"),
    TColor::GetColor("#FF7F00"), TColor::GetColor("#A65628"),
    TColor::GetColor("#F781BF"), TColor::GetColor("#999999"),
    TColor::GetColor("#66C2A5"), TColor::GetColor("#FC8D62"),
    TColor::GetColor("#8DA0CB"), TColor::GetColor("#FFD92F"),
    TColor::GetColor("#E7298A"), TColor::GetColor("#1B9E77"),
    TColor::GetColor("#D95F02"), TColor::GetColor("#7570B3"),
    TColor::GetColor("#E6AB02"), TColor::GetColor("#A6761D"),
    TColor::GetColor("#666666"), TColor::GetColor("#A6CEE3"),
    TColor::GetColor("#B2DF8A"), TColor::GetColor("#FB9A99"),
    TColor::GetColor("#FDBF6F"), TColor::GetColor("#CAB2D6")
  };

  //
  TLine *l_334 = new TLine(334, 1., 334, 100000);
  l_334->SetLineColor(kRed);
  l_334->SetLineWidth(2);
  l_334->SetLineStyle(2);

  TLine *l_406 = new TLine(406.5, 1., 406.5, 100000);
  l_406->SetLineColor(kRed);
  l_406->SetLineWidth(2);
  l_406->SetLineStyle(2);

  TLine *l_439 = new TLine(439.4, 1., 439.4, 100000);
  l_439->SetLineColor(kRed);
  l_439->SetLineWidth(2);
  l_439->SetLineStyle(2);

  //
  TFile *fi = TFile::Open(Form("../../rootfile/%s_no_bg_hist.root",str.c_str()));
  if(fi->IsZombie()){
    cout << "cannot open the file." << std::endl;
    return;
  }

  // spider different ring
  TH1D *h_ga_spr[8], *h_gr3_spr[8], *h_gr4_spr[8], *h_gr5_spr[8];
  k = 0;
  for(int i=1;i<=8;i++){
    h_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_spider_ring%d",i));
    h_gr3_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring3_spider_ring%d",i));
    h_gr4_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring4_spider_ring%d",i));
    h_gr5_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring5_spider_ring%d",i));

    k++;
  }

  // s3 different ring
  TH1D *h_ga_s3r[24], *h_gr3_s3r[24], *h_gr4_s3r[24], *h_gr5_s3r[24];
  k = 0;
  for(int i=1;i<=24;i++){
    h_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_s3_ring%d",i));
    h_gr3_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring3_s3_ring%d",i));
    h_gr4_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring4_s3_ring%d",i));
    h_gr5_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring5_s3_ring%d",i));

    k++;
  }

  //
  TLegend *leg1 = new TLegend(0.88, 0.55, 0.98, 0.92); leg1->Clear();
  TLegend *leg2 = new TLegend(0.88, 0.02, 0.98, 0.92); leg2->Clear();

  //
  //
  // h_ga_spr
  TCanvas *c_ga_spr = new TCanvas("c_ga_spr","",0,0,900,360);
  c_ga_spr->cd();
  c_ga_spr->SetLogy();

  k = 0;
  leg1->Clear();
  for(int i=1;i<=8;i++){
    h_ga_spr[k]->SetLineColor(colors[k]);
    h_ga_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h_ga_spr[k]->GetYaxis()->SetRangeUser(1., 100000);
    
    if(k==0){
      h_ga_spr[k]->SetTitle("ge all, spider all");
      h_ga_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h_ga_spr[k]->GetYaxis()->SetTitle("counts");
      h_ga_spr[k]->Draw();
    }

    h_ga_spr[k]->Draw("same");

    leg1->AddEntry(h_ga_spr[k], Form("spider ring%d",i));

    k++;
  }
  leg1->Draw("same");
  c_ga_spr->SaveAs("./fig/c_ga_spr.png");

  // h_gr3_spr
  TCanvas *c_gr3_spr = new TCanvas("c_gr3_spr","",0,0,900,360);
  c_gr3_spr->cd();
  c_gr3_spr->SetLogy();

  k = 0;
  leg1->Clear();
  for(int i=1;i<=8;i++){
    h_gr3_spr[k]->SetLineColor(colors[k]);
    h_gr3_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h_gr3_spr[k]->GetYaxis()->SetRangeUser(1., 100000);
    
    if(k==0){
      h_gr3_spr[k]->SetTitle("ge ring3, spider all");
      h_gr3_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h_gr3_spr[k]->GetYaxis()->SetTitle("counts");
      h_gr3_spr[k]->Draw();
    }

    h_gr3_spr[k]->Draw("same");

    leg1->AddEntry(h_gr3_spr[k], Form("spider ring%d",i));

    k++;
  }
  leg1->Draw("same");
  c_gr3_spr->SaveAs("./fig/c_gr3_spr.png");

  // h_gr4_spr
  TCanvas *c_gr4_spr = new TCanvas("c_gr4_spr","",0,0,900,360);
  c_gr4_spr->cd();
  c_gr4_spr->SetLogy();

  k = 0;
  leg1->Clear();
  for(int i=1;i<=8;i++){
    h_gr4_spr[k]->SetLineColor(colors[k]);
    h_gr4_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h_gr4_spr[k]->GetYaxis()->SetRangeUser(1., 100000);
    
    if(k==0){
      h_gr4_spr[k]->SetTitle("ge ring4, spider all");
      h_gr4_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h_gr4_spr[k]->GetYaxis()->SetTitle("counts");
      h_gr4_spr[k]->Draw();
    }

    h_gr4_spr[k]->Draw("same");

    leg1->AddEntry(h_gr4_spr[k], Form("spider ring%d",i));

    k++;
  }
  leg1->Draw("same");
  c_gr4_spr->SaveAs("./fig/c_gr4_spr.png");

  // h_gr5_spr
  TCanvas *c_gr5_spr = new TCanvas("c_gr5_spr","",0,0,900,360);
  c_gr5_spr->cd();
  c_gr5_spr->SetLogy();

  k = 0;
  leg1->Clear();
  for(int i=1;i<=8;i++){
    h_gr5_spr[k]->SetLineColor(colors[k]);
    h_gr5_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h_gr5_spr[k]->GetYaxis()->SetRangeUser(1., 100000);
    
    if(k==0){
      h_gr5_spr[k]->SetTitle("ge ring5, spider all");
      h_gr5_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h_gr5_spr[k]->GetYaxis()->SetTitle("counts");
      h_gr5_spr[k]->Draw();
    }

    h_gr5_spr[k]->Draw("same");

    leg1->AddEntry(h_gr5_spr[k], Form("spider ring%d",i));

    k++;
  }
  leg1->Draw("same");
  c_gr5_spr->SaveAs("./fig/c_gr5_spr.png");

  //
  //
  // h_ga_s3r
  TCanvas *c_ga_s3r = new TCanvas("c_ga_s3r","",0,0,900,360);
  c_ga_s3r->cd();
  c_ga_s3r->SetLogy();

  k = 0;
  leg2->Clear();
  for(int i=1;i<=24;i++){
    h_ga_s3r[k]->SetLineColor(colors[k]);
    h_ga_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h_ga_s3r[k]->GetYaxis()->SetRangeUser(1., 100000);
    
    if(k==0){
      h_ga_s3r[k]->SetTitle("ge all, s3 all");
      h_ga_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h_ga_s3r[k]->GetYaxis()->SetTitle("counts");
      h_ga_s3r[k]->Draw();
    }

    h_ga_s3r[k]->Draw("same");

    leg2->AddEntry(h_ga_s3r[k], Form("s3 ring%d",i));

    k++;
  }
  leg2->Draw("same");
  c_ga_s3r->SaveAs("./fig/c_ga_s3r.png");

  // h_gr3_s3r
  TCanvas *c_gr3_s3r = new TCanvas("c_gr3_s3r","",0,0,900,360);
  c_gr3_s3r->cd();
  c_gr3_s3r->SetLogy();

  k = 0;
  leg2->Clear();
  for(int i=1;i<=24;i++){
    h_gr3_s3r[k]->SetLineColor(colors[k]);
    h_gr3_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h_gr3_s3r[k]->GetYaxis()->SetRangeUser(1., 100000);
    
    if(k==0){
      h_gr3_s3r[k]->SetTitle("ge ring3, s3 all");
      h_gr3_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h_gr3_s3r[k]->GetYaxis()->SetTitle("counts");
      h_gr3_s3r[k]->Draw();
    }

    h_gr3_s3r[k]->Draw("same");

    leg2->AddEntry(h_gr3_s3r[k], Form("s3 ring%d",i));

    k++;
  }
  leg2->Draw("same");
  c_gr3_s3r->SaveAs("./fig/c_gr3_s3r.png");

  // h_gr4_s3r
  TCanvas *c_gr4_s3r = new TCanvas("c_gr4_s3r","",0,0,900,360);
  c_gr4_s3r->cd();
  c_gr4_s3r->SetLogy();

  k = 0;
  leg2->Clear();
  for(int i=1;i<=24;i++){
    h_gr4_s3r[k]->SetLineColor(colors[k]);
    h_gr4_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h_gr4_s3r[k]->GetYaxis()->SetRangeUser(1., 100000);
    
    if(k==0){
      h_gr4_s3r[k]->SetTitle("ge ring4, s3 all");
      h_gr4_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h_gr4_s3r[k]->GetYaxis()->SetTitle("counts");
      h_gr4_s3r[k]->Draw();
    }

    h_gr4_s3r[k]->Draw("same");

    leg2->AddEntry(h_gr4_s3r[k], Form("s3 ring%d",i));

    k++;
  }
  leg2->Draw("same");
  c_gr4_s3r->SaveAs("./fig/c_gr4_s3r.png");

  // h_gr5_s3r
  TCanvas *c_gr5_s3r = new TCanvas("c_gr5_s3r","",0,0,900,360);
  c_gr5_s3r->cd();
  c_gr5_s3r->SetLogy();

  k = 0;
  leg2->Clear();
  for(int i=1;i<=24;i++){
    h_gr5_s3r[k]->SetLineColor(colors[k]);
    h_gr5_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h_gr5_s3r[k]->GetYaxis()->SetRangeUser(1., 100000);
    
    if(k==0){
      h_gr5_s3r[k]->SetTitle("ge ring5, s3 all");
      h_gr5_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h_gr5_s3r[k]->GetYaxis()->SetTitle("counts");
      h_gr5_s3r[k]->Draw();
    }

    h_gr5_s3r[k]->Draw("same");

    leg2->AddEntry(h_gr5_s3r[k], Form("s3 ring%d",i));

    k++;
  }
  leg2->Draw("same");
  c_gr5_s3r->SaveAs("./fig/c_gr5_s3r.png");

}
