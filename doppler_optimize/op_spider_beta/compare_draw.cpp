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
    "try/020/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/018/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/016/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/014/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/012/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/010/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/008/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/006/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/004/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/002/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root"
  };

  vector<TFile*> v_fi;
  vector<TH1D*> v_h_ga_spa, v_hh_ga_spa, v_h_grs32_spa, v_hh_grs32_spa, v_h_grs41_spa, v_hh_grs41_spa, v_h_grs42_spa, v_hh_grs42_spa, v_h_grs43_spa, v_hh_grs43_spa, v_h_grs52_spa, v_hh_grs52_spa, v_h_grs53_spa, v_hh_grs53_spa;

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
    leg_ga_spa->AddEntry(v_h_ga_spa[i], v_filename[i].substr(4,3).c_str());
    v_h_ga_spa[i]->SetLineColor(colors[i]);
    v_h_ga_spa[i]->SetLineWidth(2);
    v_h_ga_spa[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_ga_spa[i]->SetTitle("ge all, different beta pars");
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
    leg_grs32_spa->AddEntry(v_h_grs32_spa[i], v_filename[i].substr(4,3).c_str());
    v_h_grs32_spa[i]->SetTitle("ge ring3 sector2, different beta pars");
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

  //
  // ge ring4 sector1, spider all
  TCanvas *cc_grs41_spa = new TCanvas("cc_grs41_spa","",900,500);
  cc_grs41_spa->cd();
  cc_grs41_spa->SetLogy();

  TLegend *leg_grs41_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs41_spa->AddEntry(v_h_grs41_spa[i], v_filename[i].substr(4,3).c_str());
    v_h_grs41_spa[i]->SetTitle("ge ring4 sector1, different spider beta pars");
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

  //
  // ge ring4 sector2, spider all
  TCanvas *cc_grs42_spa = new TCanvas("cc_grs42_spa","",900,500);
  cc_grs42_spa->cd();
  cc_grs42_spa->SetLogy();

  TLegend *leg_grs42_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs42_spa->AddEntry(v_h_grs42_spa[i], v_filename[i].substr(4,3).c_str());
    v_h_grs42_spa[i]->SetTitle("ge ring4 sector2, spider differnet ring, different spider beta pars");
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

  //
  // ge ring4 sector3, spider all
  TCanvas *cc_grs43_spa = new TCanvas("cc_grs43_spa","",900,500);
  cc_grs43_spa->cd();
  cc_grs43_spa->SetLogy();

  TLegend *leg_grs43_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs43_spa->AddEntry(v_h_grs43_spa[i], v_filename[i].substr(4,3).c_str());
    v_h_grs43_spa[i]->SetTitle("ge ring4 sector3, spider differnet ring, different spider beta pars");
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

  //
  // ge ring5 sector2, spider all
  TCanvas *cc_grs52_spa = new TCanvas("cc_grs52_spa","",900,500);
  cc_grs52_spa->cd();
  cc_grs52_spa->SetLogy();

  TLegend *leg_grs52_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs52_spa->AddEntry(v_h_grs52_spa[i], v_filename[i].substr(4,3).c_str());
    v_h_grs52_spa[i]->SetTitle("ge ring5 sector2, different spider beta pars");
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

  //
  // ge ring5 sector3, spider all
  TCanvas *cc_grs53_spa = new TCanvas("cc_grs53_spa","",900,500);
  cc_grs53_spa->cd();
  cc_grs53_spa->SetLogy();

  TLegend *leg_grs53_spa = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs53_spa->AddEntry(v_h_grs53_spa[i], v_filename[i].substr(4,3).c_str());
    v_h_grs53_spa[i]->SetTitle("ge ring5 sector3, different spider beta pars");
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
  
}
