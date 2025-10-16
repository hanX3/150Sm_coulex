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
    "try/005deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/010deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/330deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/335deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/340deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/345deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/350deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/355deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root"
  };

  vector<TFile*> v_fi;
  vector<TH1D*> v_h_ga_spa, v_hh_ga_spa, v_h_grs32_spa, v_hh_grs32_spa, v_h_grs41_spa, v_hh_grs41_spa, v_h_grs42_spa, v_hh_grs42_spa, v_h_grs43_spa, v_hh_grs43_spa, v_h_grs52_spa, v_hh_grs52_spa, v_h_grs53_spa, v_hh_grs53_spa;
  vector<TH1D*> v_h_ga_spr, v_hh_ga_spr, v_h_grs32_spr, v_hh_grs32_spr, v_h_grs41_spr, v_hh_grs41_spr, v_h_grs42_spr, v_hh_grs42_spr, v_h_grs43_spr, v_hh_grs43_spr, v_h_grs52_spr, v_hh_grs52_spr, v_h_grs53_spr, v_hh_grs53_spr;
  vector<TH1D*> v_h_ga_sps, v_hh_ga_sps, v_h_grs32_sps, v_hh_grs32_sps, v_h_grs41_sps, v_hh_grs41_sps, v_h_grs42_sps, v_hh_grs42_sps, v_h_grs43_sps, v_hh_grs43_sps, v_h_grs52_sps, v_hh_grs52_sps, v_h_grs53_sps, v_hh_grs53_sps;

  int kr = 0, ks = 0;
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

    // spider sector
    for(int s=1;s<=12;s++){
      v_h_ga_sps.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_all_spider_sector%d",s)));
      v_hh_ga_sps.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_all_spider_sector%d",s)));
      v_h_ga_sps[ks]->Add(v_hh_ga_sps[ks], -1);

      v_h_grs32_sps.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring3_sector2_spider_sector%d",s)));
      v_hh_grs32_sps.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring3_sector2_spider_sector%d",s)));
      v_h_grs32_sps[ks]->Add(v_hh_grs32_sps[ks], -1);

      v_h_grs41_sps.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector1_spider_sector%d",s)));
      v_hh_grs41_sps.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector1_spider_sector%d",s)));
      v_h_grs41_sps[ks]->Add(v_hh_grs41_sps[ks], -1);

      v_h_grs42_sps.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector2_spider_sector%d",s)));
      v_hh_grs42_sps.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector2_spider_sector%d",s)));
      v_h_grs42_sps[ks]->Add(v_hh_grs42_sps[ks], -1);

      v_h_grs43_sps.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector3_spider_sector%d",s)));
      v_hh_grs43_sps.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector3_spider_sector%d",s)));
      v_h_grs43_sps[ks]->Add(v_hh_grs43_sps[ks], -1);

      v_h_grs52_sps.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_sector2_spider_sector%d",s)));
      v_hh_grs52_sps.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_sector2_spider_sector%d",s)));
      v_h_grs52_sps[ks]->Add(v_hh_grs52_sps[ks], -1);

      v_h_grs53_sps.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_sector3_spider_sector%d",s)));
      v_hh_grs53_sps.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_sector3_spider_sector%d",s)));
      v_h_grs53_sps[ks]->Add(v_hh_grs53_sps[ks], -1);

      ks++;
    }
  }
  cout << "ks " << ks << endl;

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
      v_h_ga_spa[i]->SetTitle("ge all, different spider phi offset");
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
    v_h_grs32_spa[i]->SetTitle("ge ring3 sector2, different spider phi offset");
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

  // ge ring3 sector2, spider different ring
  TCanvas *cc_grs32_spr[n_files];
  TLegend *leg_grs32_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs32_spr[i] = new TCanvas(Form("cc_grs32_spr_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs32_spr[i]->cd();
    cc_grs32_spr[i]->SetLogy();

    leg_grs32_spr[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs32_spr[i]->AddEntry(v_h_grs32_spr[i*8+r], Form("ring%d",r+1));
      v_h_grs32_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_grs32_spr[i*8+r]->SetLineWidth(2);
      v_h_grs32_spr[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs32_spr[i*8+r]->SetTitle(Form("ge ring3 sector2, spider differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs32_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs32_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs32_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs32_spr[i*8+r]->Draw("hist");
      }else v_h_grs32_spr[i*8+r]->Draw("same hist");
    }

    leg_grs32_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs32_spr[i]->SaveAs(Form("fig/%s.png",cc_grs32_spr[i]->GetName()));
  }

  // ge ring3 sector2, spider different sector
  TCanvas *cc_grs32_sps[n_files];
  TLegend *leg_grs32_sps[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs32_sps[i] = new TCanvas(Form("cc_grs32_sps_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs32_sps[i]->cd();
    cc_grs32_sps[i]->SetLogy();

    leg_grs32_sps[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<12;s++){
      leg_grs32_sps[i]->AddEntry(v_h_grs32_sps[i*12+s], Form("sector%d",s+1));
      v_h_grs32_sps[i*12+s]->SetLineColor(colors[s]);
      v_h_grs32_sps[i*12+s]->SetLineWidth(2);
      v_h_grs32_sps[i*12+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs32_sps[i*12+s]->SetTitle(Form("ge ring3 sector2, spider differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs32_sps[i*12+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs32_sps[i*12+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs32_sps[i*12+s]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs32_sps[i*12+s]->Draw("hist");
      }else v_h_grs32_sps[i*12+s]->Draw("same hist");
    }

    leg_grs32_sps[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs32_sps[i]->SaveAs(Form("fig/%s.png",cc_grs32_sps[i]->GetName()));
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

  // ge ring4 sector1, spider different ring
  TCanvas *cc_grs41_spr[n_files];
  TLegend *leg_grs41_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs41_spr[i] = new TCanvas(Form("cc_grs41_spr_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs41_spr[i]->cd();
    cc_grs41_spr[i]->SetLogy();

    leg_grs41_spr[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs41_spr[i]->AddEntry(v_h_grs41_spr[i*8+r], Form("ring%d",r+1));
      v_h_grs41_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_grs41_spr[i*8+r]->SetLineWidth(2);
      v_h_grs41_spr[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs41_spr[i*8+r]->SetTitle(Form("ge ring4 sector1, spider differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs41_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs41_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs41_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs41_spr[i*8+r]->Draw("hist");
      }else v_h_grs41_spr[i*8+r]->Draw("same hist");
    }

    leg_grs41_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs41_spr[i]->SaveAs(Form("fig/%s.png",cc_grs41_spr[i]->GetName()));
  }

  // ge ring4 sector1, spider different sector
  TCanvas *cc_grs41_sps[n_files];
  TLegend *leg_grs41_sps[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs41_sps[i] = new TCanvas(Form("cc_grs41_sps_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs41_sps[i]->cd();
    cc_grs41_sps[i]->SetLogy();

    leg_grs41_sps[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<12;s++){
      leg_grs41_sps[i]->AddEntry(v_h_grs41_sps[i*12+s], Form("sector%d",s+1));
      v_h_grs41_sps[i*12+s]->SetLineColor(colors[s]);
      v_h_grs41_sps[i*12+s]->SetLineWidth(2);
      v_h_grs41_sps[i*12+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs41_sps[i*12+s]->SetTitle(Form("ge ring4 sector1, spider differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs41_sps[i*12+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs41_sps[i*12+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs41_sps[i*12+s]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs41_sps[i*12+s]->Draw("hist");
      }else v_h_grs41_sps[i*12+s]->Draw("same hist");
    }

    leg_grs41_sps[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs41_sps[i]->SaveAs(Form("fig/%s.png",cc_grs41_sps[i]->GetName()));
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

  // ge ring4 sector2, spider different ring
  TCanvas *cc_grs42_spr[n_files];
  TLegend *leg_grs42_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs42_spr[i] = new TCanvas(Form("cc_grs42_spr_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs42_spr[i]->cd();
    cc_grs42_spr[i]->SetLogy();

    leg_grs42_spr[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs42_spr[i]->AddEntry(v_h_grs42_spr[i*8+r], Form("ring%d",r+1));
      v_h_grs42_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_grs42_spr[i*8+r]->SetLineWidth(2);
      v_h_grs42_spr[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs42_spr[i*8+r]->SetTitle(Form("ge ring4 sector2, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs42_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs42_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs42_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs42_spr[i*8+r]->Draw("hist");
      }else v_h_grs42_spr[i*8+r]->Draw("same hist");
    }

    leg_grs42_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs42_spr[i]->SaveAs(Form("fig/%s.png",cc_grs42_spr[i]->GetName()));
  }

  // ge ring4 sector2, spider different sector
  TCanvas *cc_grs42_sps[n_files];
  TLegend *leg_grs42_sps[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs42_sps[i] = new TCanvas(Form("cc_grs42_sps_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs42_sps[i]->cd();
    cc_grs42_sps[i]->SetLogy();

    leg_grs42_sps[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<12;s++){
      leg_grs42_sps[i]->AddEntry(v_h_grs42_sps[i*12+s], Form("sector%d",s+1));
      v_h_grs42_sps[i*12+s]->SetLineColor(colors[s]);
      v_h_grs42_sps[i*12+s]->SetLineWidth(2);
      v_h_grs42_sps[i*12+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs42_sps[i*12+s]->SetTitle(Form("ge ring4 sector2, spider differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs42_sps[i*12+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs42_sps[i*12+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs42_sps[i*12+s]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs42_sps[i*12+s]->Draw("hist");
      }else v_h_grs42_sps[i*12+s]->Draw("same hist");
    }

    leg_grs42_sps[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs42_sps[i]->SaveAs(Form("fig/%s.png",cc_grs42_sps[i]->GetName()));
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

  // ge ring4 sector3, spider different ring
  TCanvas *cc_grs43_spr[n_files];
  TLegend *leg_grs43_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs43_spr[i] = new TCanvas(Form("cc_grs43_spr_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs43_spr[i]->cd();
    cc_grs43_spr[i]->SetLogy();

    leg_grs43_spr[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs43_spr[i]->AddEntry(v_h_grs43_spr[i*8+r], Form("ring%d",r+1));
      v_h_grs43_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_grs43_spr[i*8+r]->SetLineWidth(2);
      v_h_grs43_spr[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs43_spr[i*8+r]->SetTitle(Form("ge ring4 sector3, spider differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs43_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs43_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs43_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs43_spr[i*8+r]->Draw("hist");
      }else v_h_grs43_spr[i*8+r]->Draw("same hist");
    }

    leg_grs43_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs43_spr[i]->SaveAs(Form("fig/%s.png",cc_grs43_spr[i]->GetName()));
  }

  // ge ring4 sector3, spider different sector
  TCanvas *cc_grs43_sps[n_files];
  TLegend *leg_grs43_sps[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs43_sps[i] = new TCanvas(Form("cc_grs43_sps_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs43_sps[i]->cd();
    cc_grs43_sps[i]->SetLogy();

    leg_grs43_sps[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<12;s++){
      leg_grs43_sps[i]->AddEntry(v_h_grs43_sps[i*12+s], Form("sector%d",s+1));
      v_h_grs43_sps[i*12+s]->SetLineColor(colors[s]);
      v_h_grs43_sps[i*12+s]->SetLineWidth(2);
      v_h_grs43_sps[i*12+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs43_sps[i*12+s]->SetTitle(Form("ge ring4 sector3, spider differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs43_sps[i*12+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs43_sps[i*12+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs43_sps[i*12+s]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs43_sps[i*12+s]->Draw("hist");
      }else v_h_grs43_sps[i*12+s]->Draw("same hist");
    }

    leg_grs43_sps[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs43_sps[i]->SaveAs(Form("fig/%s.png",cc_grs43_sps[i]->GetName()));
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

  // ge ring5 sector2, spider different ring
  TCanvas *cc_grs52_spr[n_files];
  TLegend *leg_grs52_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs52_spr[i] = new TCanvas(Form("cc_grs52_spr_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs52_spr[i]->cd();
    cc_grs52_spr[i]->SetLogy();

    leg_grs52_spr[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs52_spr[i]->AddEntry(v_h_grs52_spr[i*8+r], Form("ring%d",r+1));
      v_h_grs52_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_grs52_spr[i*8+r]->SetLineWidth(2);
      v_h_grs52_spr[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs52_spr[i*8+r]->SetTitle(Form("ge ring5 sector2, spider differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs52_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs52_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs52_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs52_spr[i*8+r]->Draw("hist");
      }else v_h_grs52_spr[i*8+r]->Draw("same hist");
    }

    leg_grs52_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs52_spr[i]->SaveAs(Form("fig/%s.png",cc_grs52_spr[i]->GetName()));
  }
  
  // ge ring5 sector2, spider different sector
  TCanvas *cc_grs52_sps[n_files];
  TLegend *leg_grs52_sps[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs52_sps[i] = new TCanvas(Form("cc_grs52_sps_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs52_sps[i]->cd();
    cc_grs52_sps[i]->SetLogy();

    leg_grs52_sps[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<12;s++){
      leg_grs52_sps[i]->AddEntry(v_h_grs52_sps[i*12+s], Form("sector%d",s+1));
      v_h_grs52_sps[i*12+s]->SetLineColor(colors[s]);
      v_h_grs52_sps[i*12+s]->SetLineWidth(2);
      v_h_grs52_sps[i*12+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs52_sps[i*12+s]->SetTitle(Form("ge ring5 sector2, spider differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs52_sps[i*12+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs52_sps[i*12+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs52_sps[i*12+s]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs52_sps[i*12+s]->Draw("hist");
      }else v_h_grs52_sps[i*12+s]->Draw("same hist");
    }

    leg_grs52_sps[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs52_sps[i]->SaveAs(Form("fig/%s.png",cc_grs52_sps[i]->GetName()));
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
  
  // ge ring5 sector3, spider different ring
  TCanvas *cc_grs53_spr[n_files];
  TLegend *leg_grs53_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs53_spr[i] = new TCanvas(Form("cc_grs53_spr_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs53_spr[i]->cd();
    cc_grs53_spr[i]->SetLogy();

    leg_grs53_spr[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs53_spr[i]->AddEntry(v_h_grs53_spr[i*8+r], Form("ring%d",r+1));
      v_h_grs53_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_grs53_spr[i*8+r]->SetLineWidth(2);
      v_h_grs53_spr[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs53_spr[i*8+r]->SetTitle(Form("ge ring5 sector3, spider differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs53_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs53_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs53_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs53_spr[i*8+r]->Draw("hist");
      }else v_h_grs53_spr[i*8+r]->Draw("same hist");
    }

    leg_grs53_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs53_spr[i]->SaveAs(Form("fig/%s.png",cc_grs53_spr[i]->GetName()));
  }

  // ge ring5 sector3, spider different sector
  TCanvas *cc_grs53_sps[n_files];
  TLegend *leg_grs53_sps[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs53_sps[i] = new TCanvas(Form("cc_grs53_sps_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs53_sps[i]->cd();
    cc_grs53_sps[i]->SetLogy();

    leg_grs53_sps[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<12;s++){
      leg_grs53_sps[i]->AddEntry(v_h_grs53_sps[i*12+s], Form("sector%d",s+1));
      v_h_grs53_sps[i*12+s]->SetLineColor(colors[s]);
      v_h_grs53_sps[i*12+s]->SetLineWidth(2);
      v_h_grs53_sps[i*12+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs53_sps[i*12+s]->SetTitle(Form("ge ring5 sector3, spider differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs53_sps[i*12+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs53_sps[i*12+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs53_sps[i*12+s]->GetYaxis()->SetRangeUser(1., 10000);
        v_h_grs53_sps[i*12+s]->Draw("hist");
      }else v_h_grs53_sps[i*12+s]->Draw("same hist");
    }

    leg_grs53_sps[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs53_sps[i]->SaveAs(Form("fig/%s.png",cc_grs53_sps[i]->GetName()));
  }
}
