void compare_draw()
{
  gStyle->SetOptStat(0);

  int colors[12] = {
    TColor::GetColor("#E41A1C"), TColor::GetColor("#377EB8"),
    TColor::GetColor("#4DAF4A"), TColor::GetColor("#984EA3"),
    TColor::GetColor("#FF7F00"), TColor::GetColor("#A65628"),
    TColor::GetColor("#F781BF"), TColor::GetColor("#999999"),
    TColor::GetColor("#66C2A5"), TColor::GetColor("#FC8D62"),
    TColor::GetColor("#8DA0CB"), TColor::GetColor("#FFD92F")
  };

  vector<string> v_filename = {
    "try/000deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/002deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/004deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/006deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/008deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/010deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/350deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/352deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/354deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/356deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/358deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root"
  };

  vector<TFile*> v_fi;
  vector<TH1D*> v_h_ga_spa, v_hh_ga_spa, v_h_grs32_spa, v_hh_grs32_spa, v_h_grs41_spa, v_hh_grs41_spa, v_h_grs42_spa, v_hh_grs42_spa, v_h_grs43_spa, v_hh_grs43_spa, v_h_grs52_spa, v_hh_grs52_spa, v_h_grs53_spa, v_hh_grs53_spa;
  vector<TH1D*> v_h_ga_spr, v_hh_ga_spr, v_h_grs32_spr, v_hh_grs32_spr, v_h_grs41_spr, v_hh_grs41_spr, v_h_grs42_spr, v_hh_grs42_spr, v_h_grs43_spr, v_hh_grs43_spr, v_h_grs52_spr, v_hh_grs52_spr, v_h_grs53_spr, v_hh_grs53_spr;

  int kr = 0;
  for(int i=0;i<v_filename.size();++i){
    v_fi.push_back(TFile::Open(Form("%s", v_filename[i].c_str())));
    if(v_fi[i]->IsZombie()){
      cout << "wrong " << v_filename[i] << endl;
      return;
    }

    // spider all
    v_h_ga_spa.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_all_spider_all"));
    v_hh_ga_spa.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_all_spider_all"));
    v_h_ga_spa[i]->Add(v_hh_ga_spa[i], -1);

    v_h_grs32_spa.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring3_sector2_spider_all"));
    v_hh_grs32_spa.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring3_sector2_spider_all"));
    v_h_grs32_spa[i]->Add(v_hh_grs32_spa[i], -1);

    v_h_grs41_spa.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring4_sector1_spider_all"));
    v_hh_grs41_spa.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring4_sector1_spider_all"));
    v_h_grs41_spa[i]->Add(v_hh_grs41_spa[i], -1);

    v_h_grs42_spa.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring4_sector2_spider_all"));
    v_hh_grs42_spa.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring4_sector2_spider_all"));
    v_h_grs42_spa[i]->Add(v_hh_grs42_spa[i], -1);

    v_h_grs43_spa.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring4_sector3_spider_all"));
    v_hh_grs43_spa.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring4_sector3_spider_all"));
    v_h_grs43_spa[i]->Add(v_hh_grs43_spa[i], -1);

    v_h_grs52_spa.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring5_sector2_spider_all"));
    v_hh_grs52_spa.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring5_sector2_spider_all"));
    v_h_grs52_spa[i]->Add(v_hh_grs52_spa[i], -1);

    v_h_grs53_spa.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring5_sector3_spider_all"));
    v_hh_grs53_spa.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring5_sector3_spider_all"));
    v_h_grs53_spa[i]->Add(v_hh_grs53_spa[i], -1);

    // spider ring
    for(int r=1;r<=8;r++){
      v_h_ga_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_all_spider_ring%d",r)));
      v_hh_ga_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_all_spider_ring%d",r)));
      v_h_ga_spr[kr]->Add(v_hh_ga_spr[kr], -1);

      v_h_grs32_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring3_sector2_spider_ring%d",r)));
      v_hh_grs32_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring3_sector2_spider_ring%d",r)));
      v_h_grs32_spr[kr]->Add(v_hh_grs32_spr[kr], -1);

      v_h_grs41_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector1_spider_ring%d",r)));
      v_hh_grs41_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector1_spider_ring%d",r)));
      v_h_grs41_spr[kr]->Add(v_hh_grs41_spr[kr], -1);

      v_h_grs42_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector2_spider_ring%d",r)));
      v_hh_grs42_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector2_spider_ring%d",r)));
      v_h_grs42_spr[kr]->Add(v_hh_grs42_spr[kr], -1);

      v_h_grs43_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector3_spider_ring%d",r)));
      v_hh_grs43_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector3_spider_ring%d",r)));
      v_h_grs43_spr[kr]->Add(v_hh_grs43_spr[kr], -1);

      v_h_grs52_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_sector2_spider_ring%d",r)));
      v_hh_grs52_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_sector2_spider_ring%d",r)));
      v_h_grs52_spr[kr]->Add(v_hh_grs52_spr[kr], -1);

      v_h_grs53_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_sector3_spider_ring%d",r)));
      v_hh_grs53_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_sector3_spider_ring%d",r)));
      v_h_grs53_spr[kr]->Add(v_hh_grs53_spr[kr], -1);

      kr++;
    }
  }

  //
  int n_files = v_filename.size();

  TLine *l_334 = new TLine(334, 1., 334, v_h_ga_spa[0]->GetMaximum()*10);
  l_334->SetLineColor(kRed);
  l_334->SetLineWidth(2);
  l_334->SetLineStyle(2);

  TLine *l_406 = new TLine(406.5, 1., 406.5, v_h_ga_spa[0]->GetMaximum()*10);
  l_406->SetLineColor(kRed);
  l_406->SetLineWidth(2);
  l_406->SetLineStyle(2);

  TLine *l_439 = new TLine(439.4, 1., 439.4, v_h_ga_spa[0]->GetMaximum()*10);
  l_439->SetLineColor(kRed);
  l_439->SetLineWidth(2);
  l_439->SetLineStyle(2);

  // ge all, spider all
  TCanvas *cc_ga_spa = new TCanvas("cc_ga_spa","",900,500);
  cc_ga_spa->cd();
  cc_ga_spa->SetLogy();

  TLegend *leg_ga_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_ga_spa->AddEntry(v_h_ga_spa[i], v_filename[i].substr(4,6).c_str());
    v_h_ga_spa[i]->SetLineColor(colors[i]);
    v_h_ga_spa[i]->SetLineWidth(2);
    v_h_ga_spa[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_ga_spa[i]->SetTitle("ge all, different spider theta offset");
      v_h_ga_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_ga_spa[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_ga_spa[i]->GetYaxis()->SetRangeUser(1., v_h_ga_spa[i]->GetMaximum()*10);
      v_h_ga_spa[i]->Draw("hist");
    }else v_h_ga_spa[i]->Draw("same hist");
  }
  leg_ga_spa->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_ga_spa->SaveAs("fig/cc_ga_spa.png");

  //
  // ge ring3 sector2, spider all
  TCanvas *cc_grs32_spa = new TCanvas("cc_grs32_spa","",900,500);
  cc_grs32_spa->cd();
  cc_grs32_spa->SetLogy();

  TLegend *leg_grs32_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs32_spa->AddEntry(v_h_grs32_spa[i], v_filename[i].substr(4,6).c_str());
    v_h_grs32_spa[i]->SetTitle("ge ring3 sector2, different spider theta offset");
    v_h_grs32_spa[i]->SetLineColor(colors[i]);
    v_h_grs32_spa[i]->SetLineWidth(2);
    v_h_grs32_spa[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs32_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs32_spa[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs32_spa[i]->GetYaxis()->SetRangeUser(1., v_h_grs32_spa[i]->GetMaximum()*10);
      v_h_grs32_spa[i]->Draw("hist");
    }else v_h_grs32_spa[i]->Draw("same hist");
  }
  leg_grs32_spa->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs32_spa->SaveAs("fig/cc_grs32_spa.png");

  // ge ring3 sector2, spider different ring, spider theta different offset
  TCanvas *cc_grs32_spr[n_files];
  TLegend *leg_grs32_spr[n_files];
  for(int r=0;r<8;r++){
    cc_grs32_spr[r] = new TCanvas(Form("cc_grs32_spr%d",r+1),"",900,500);
    cc_grs32_spr[r]->cd();
    cc_grs32_spr[r]->SetLogy();

    leg_grs32_spr[r] = new TLegend(0.75, 0.38, 0.88, 0.92);

    for(int i=0;i<n_files;i++){
      leg_grs32_spr[r]->AddEntry(v_h_grs32_spr[8*i+r], Form("%s",v_filename[i].substr(4,6).c_str()));
      v_h_grs32_spr[8*i+r]->SetLineColor(colors[i]);
      v_h_grs32_spr[8*i+r]->SetLineWidth(2);
      v_h_grs32_spr[8*i+r]->GetXaxis()->SetRangeUser(300,500);
      if(i==0){
        v_h_grs32_spr[8*i+r]->SetTitle(Form("ge ring3 sector2, spider ring%d, spider theta different offset",r+1));
        v_h_grs32_spr[8*i+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs32_spr[8*i+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs32_spr[8*i+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs32_spr[8*i+r]->Draw("hist");
      }else v_h_grs32_spr[8*i+r]->Draw("same hist");
    }

    leg_grs32_spr[r]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs32_spr[r]->SaveAs(Form("fig/%s.png",cc_grs32_spr[r]->GetName()));
  }

  //
  // ge ring4 sector1, spider all
  TCanvas *cc_grs41_spa = new TCanvas("cc_grs41_spa","",900,500);
  cc_grs41_spa->cd();
  cc_grs41_spa->SetLogy();

  TLegend *leg_grs41_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs41_spa->AddEntry(v_h_grs41_spa[i], v_filename[i].substr(4,6).c_str());
    v_h_grs41_spa[i]->SetTitle("ge ring4 sector1, different spider phi offset");
    v_h_grs41_spa[i]->SetLineColor(colors[i]);
    v_h_grs41_spa[i]->SetLineWidth(2);
    v_h_grs41_spa[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs41_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs41_spa[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs41_spa[i]->GetYaxis()->SetRangeUser(1., 10000);
      v_h_grs41_spa[i]->Draw("hist");
    }else v_h_grs41_spa[i]->Draw("same hist");
  }
  leg_grs41_spa->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs41_spa->SaveAs("fig/cc_grs41_spa.png");

  // ge ring4 sector1, spider different ring, spider theta different offset
  TCanvas *cc_grs41_spr[n_files];
  TLegend *leg_grs41_spr[n_files];
  for(int r=0;r<8;r++){
    cc_grs41_spr[r] = new TCanvas(Form("cc_grs41_spr%d",r+1),"",900,500);
    cc_grs41_spr[r]->cd();
    cc_grs41_spr[r]->SetLogy();

    leg_grs41_spr[r] = new TLegend(0.75, 0.38, 0.88, 0.92);

    for(int i=0;i<n_files;i++){
      leg_grs41_spr[r]->AddEntry(v_h_grs41_spr[8*i+r], Form("%s",v_filename[i].substr(4,6).c_str()));
      v_h_grs41_spr[8*i+r]->SetLineColor(colors[i]);
      v_h_grs41_spr[8*i+r]->SetLineWidth(2);
      v_h_grs41_spr[8*i+r]->GetXaxis()->SetRangeUser(300,500);
      if(i==0){
        v_h_grs41_spr[8*i+r]->SetTitle(Form("ge ring4 sector1, spider ring%d, spider theta different offset",r+1));
        v_h_grs41_spr[8*i+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs41_spr[8*i+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs41_spr[8*i+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs41_spr[8*i+r]->Draw("hist");
      }else v_h_grs41_spr[8*i+r]->Draw("same hist");
    }

    leg_grs41_spr[r]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs41_spr[r]->SaveAs(Form("fig/%s.png",cc_grs41_spr[r]->GetName()));
  }

  //
  // ge ring4 sector2, spider all
  TCanvas *cc_grs42_spa = new TCanvas("cc_grs42_spa","",900,500);
  cc_grs42_spa->cd();
  cc_grs42_spa->SetLogy();

  TLegend *leg_grs42_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs42_spa->AddEntry(v_h_grs42_spa[i], v_filename[i].substr(4,6).c_str());
    v_h_grs42_spa[i]->SetTitle("ge ring4 sector2, spider differnet ring, different spider phi offset");
    v_h_grs42_spa[i]->SetLineColor(colors[i]);
    v_h_grs42_spa[i]->SetLineWidth(2);
    v_h_grs42_spa[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs42_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs42_spa[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs42_spa[i]->GetYaxis()->SetRangeUser(1., 10000);
      v_h_grs42_spa[i]->Draw("hist");
    }else v_h_grs42_spa[i]->Draw("same hist");
  }
  leg_grs42_spa->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs42_spa->SaveAs("fig/cc_grs42_spa.png");

  // ge ring4 sector2, spider different ring, spider theta different offset
  TCanvas *cc_grs42_spr[n_files];
  TLegend *leg_grs42_spr[n_files];
  for(int r=0;r<8;r++){
    cc_grs42_spr[r] = new TCanvas(Form("cc_grs42_spr%d",r+1),"",900,500);
    cc_grs42_spr[r]->cd();
    cc_grs42_spr[r]->SetLogy();

    leg_grs42_spr[r] = new TLegend(0.75, 0.38, 0.88, 0.92);

    for(int i=0;i<n_files;i++){
      leg_grs42_spr[r]->AddEntry(v_h_grs42_spr[8*i+r], Form("%s",v_filename[i].substr(4,6).c_str()));
      v_h_grs42_spr[8*i+r]->SetLineColor(colors[i]);
      v_h_grs42_spr[8*i+r]->SetLineWidth(2);
      v_h_grs42_spr[8*i+r]->GetXaxis()->SetRangeUser(300,500);
      if(i==0){
        v_h_grs42_spr[8*i+r]->SetTitle(Form("ge ring4 sector2, spider ring%d, spider theta different offset",r+1));
        v_h_grs42_spr[8*i+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs42_spr[8*i+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs42_spr[8*i+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs42_spr[8*i+r]->Draw("hist");
      }else v_h_grs42_spr[8*i+r]->Draw("same hist");
    }

    leg_grs42_spr[r]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs42_spr[r]->SaveAs(Form("fig/%s.png",cc_grs42_spr[r]->GetName()));
  }

  //
  // ge ring4 sector3, spider all
  TCanvas *cc_grs43_spa = new TCanvas("cc_grs43_spa","",900,500);
  cc_grs43_spa->cd();
  cc_grs43_spa->SetLogy();

  TLegend *leg_grs43_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs43_spa->AddEntry(v_h_grs43_spa[i], v_filename[i].substr(4,6).c_str());
    v_h_grs43_spa[i]->SetTitle("ge ring4 sector3, spider differnet ring, different spider phi offset");
    v_h_grs43_spa[i]->SetLineColor(colors[i]);
    v_h_grs43_spa[i]->SetLineWidth(2);
    v_h_grs43_spa[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs43_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs43_spa[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs43_spa[i]->GetYaxis()->SetRangeUser(1., 10000);
      v_h_grs43_spa[i]->Draw("hist");
    }else v_h_grs43_spa[i]->Draw("same hist");
  }
  leg_grs43_spa->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs43_spa->SaveAs("fig/cc_grs43_spa.png");

  // ge ring4 sector3, spider different ring, spider theta different offset
  TCanvas *cc_grs43_spr[n_files];
  TLegend *leg_grs43_spr[n_files];
  for(int r=0;r<8;r++){
    cc_grs43_spr[r] = new TCanvas(Form("cc_grs43_spr%d",r+1),"",900,500);
    cc_grs43_spr[r]->cd();
    cc_grs43_spr[r]->SetLogy();

    leg_grs43_spr[r] = new TLegend(0.75, 0.38, 0.88, 0.92);

    for(int i=0;i<n_files;i++){
      leg_grs43_spr[r]->AddEntry(v_h_grs43_spr[8*i+r], Form("%s",v_filename[i].substr(4,6).c_str()));
      v_h_grs43_spr[8*i+r]->SetLineColor(colors[i]);
      v_h_grs43_spr[8*i+r]->SetLineWidth(2);
      v_h_grs43_spr[8*i+r]->GetXaxis()->SetRangeUser(300,500);
      if(i==0){
        v_h_grs43_spr[8*i+r]->SetTitle(Form("ge ring4 sector3, spider ring%d, spider theta different offset",r+1));
        v_h_grs43_spr[8*i+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs43_spr[8*i+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs43_spr[8*i+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs43_spr[8*i+r]->Draw("hist");
      }else v_h_grs43_spr[8*i+r]->Draw("same hist");
    }

    leg_grs43_spr[r]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs43_spr[r]->SaveAs(Form("fig/%s.png",cc_grs43_spr[r]->GetName()));
  }

  //
  // ge ring5 sector2, spider all
  TCanvas *cc_grs52_spa = new TCanvas("cc_grs52_spa","",900,500);
  cc_grs52_spa->cd();
  cc_grs52_spa->SetLogy();

  TLegend *leg_grs52_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs52_spa->AddEntry(v_h_grs52_spa[i], v_filename[i].substr(4,6).c_str());
    v_h_grs52_spa[i]->SetTitle("ge ring5 sector2, different spider phi offset");
    v_h_grs52_spa[i]->SetLineColor(colors[i]);
    v_h_grs52_spa[i]->SetLineWidth(2);
    v_h_grs52_spa[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs52_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs52_spa[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs52_spa[i]->GetYaxis()->SetRangeUser(1., 10000);
      v_h_grs52_spa[i]->Draw("hist");
    }else v_h_grs52_spa[i]->Draw("same hist");
  }
  leg_grs52_spa->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs52_spa->SaveAs("fig/cc_grs52_spa.png");

  // ge ring5 sector2, spider different ring, spider theta different offset
  TCanvas *cc_grs52_spr[n_files];
  TLegend *leg_grs52_spr[n_files];
  for(int r=0;r<8;r++){
    cc_grs52_spr[r] = new TCanvas(Form("cc_grs52_spr%d",r+1),"",900,500);
    cc_grs52_spr[r]->cd();
    cc_grs52_spr[r]->SetLogy();

    leg_grs52_spr[r] = new TLegend(0.75, 0.38, 0.88, 0.92);

    for(int i=0;i<n_files;i++){
      leg_grs52_spr[r]->AddEntry(v_h_grs52_spr[8*i+r], Form("%s",v_filename[i].substr(4,6).c_str()));
      v_h_grs52_spr[8*i+r]->SetLineColor(colors[i]);
      v_h_grs52_spr[8*i+r]->SetLineWidth(2);
      v_h_grs52_spr[8*i+r]->GetXaxis()->SetRangeUser(300,500);
      if(i==0){
        v_h_grs52_spr[8*i+r]->SetTitle(Form("ge ring5 sector2, spider ring%d, spider theta different offset",r+1));
        v_h_grs52_spr[8*i+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs52_spr[8*i+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs52_spr[8*i+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs52_spr[8*i+r]->Draw("hist");
      }else v_h_grs52_spr[8*i+r]->Draw("same hist");
    }

    leg_grs52_spr[r]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs52_spr[r]->SaveAs(Form("fig/%s.png",cc_grs52_spr[r]->GetName()));
  }
  
  //
  // ge ring5 sector3, spider all
  TCanvas *cc_grs53_spa = new TCanvas("cc_grs53_spa","",900,500);
  cc_grs53_spa->cd();
  cc_grs53_spa->SetLogy();

  TLegend *leg_grs53_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs53_spa->AddEntry(v_h_grs53_spa[i], v_filename[i].substr(4,6).c_str());
    v_h_grs53_spa[i]->SetTitle("ge ring5 sector3, different spider phi offset");
    v_h_grs53_spa[i]->SetLineColor(colors[i]);
    v_h_grs53_spa[i]->SetLineWidth(2);
    v_h_grs53_spa[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs53_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs53_spa[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs53_spa[i]->GetYaxis()->SetRangeUser(1., 10000);
      v_h_grs53_spa[i]->Draw("hist");
    }else v_h_grs53_spa[i]->Draw("same hist");
  }
  leg_grs53_spa->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs53_spa->SaveAs("fig/cc_grs53_spa.png");
  
  // ge ring5 sector3, spider different ring, spider theta different offset
  TCanvas *cc_grs53_spr[n_files];
  TLegend *leg_grs53_spr[n_files];
  for(int r=0;r<8;r++){
    cc_grs53_spr[r] = new TCanvas(Form("cc_grs53_spr%d",r+1),"",900,500);
    cc_grs53_spr[r]->cd();
    cc_grs53_spr[r]->SetLogy();

    leg_grs53_spr[r] = new TLegend(0.75, 0.38, 0.88, 0.92);

    for(int i=0;i<n_files;i++){
      leg_grs53_spr[r]->AddEntry(v_h_grs53_spr[8*i+r], Form("%s",v_filename[i].substr(4,6).c_str()));
      v_h_grs53_spr[8*i+r]->SetLineColor(colors[i]);
      v_h_grs53_spr[8*i+r]->SetLineWidth(2);
      v_h_grs53_spr[8*i+r]->GetXaxis()->SetRangeUser(300,500);
      if(i==0){
        v_h_grs53_spr[8*i+r]->SetTitle(Form("ge ring5 sector3, spider ring%d, spider theta different offset",r+1));
        v_h_grs53_spr[8*i+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs53_spr[8*i+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs53_spr[8*i+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs53_spr[8*i+r]->Draw("hist");
      }else v_h_grs53_spr[8*i+r]->Draw("same hist");
    }

    leg_grs53_spr[r]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs53_spr[r]->SaveAs(Form("fig/%s.png",cc_grs53_spr[r]->GetName()));
  }
}
