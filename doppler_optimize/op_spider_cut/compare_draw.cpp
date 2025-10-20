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
    "try/04000keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root",
    "try/06000keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root",
    "try/08000keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root",
    "try/10000keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root",
    "try/15000keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root",
    "try/20000keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root",
    "try/25000keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root",
    "try/30000keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root"
  };

  vector<TFile*> v_fi;
  vector<TH1D*> v_h_ga_spr, v_hh_ga_spr, v_h_gr3_spr, v_hh_gr3_spr, v_h_gr4_spr, v_hh_gr4_spr, v_h_gr5_spr, v_hh_gr5_spr, v_h_grs32_spr, v_hh_grs32_spr, v_h_grs41_spr, v_hh_grs41_spr, v_h_grs42_spr, v_hh_grs42_spr, v_h_grs43_spr, v_hh_grs43_spr, v_h_grs52_spr, v_hh_grs52_spr, v_h_grs53_spr, v_hh_grs53_spr;

  int kr = 0;
  for(int i=0;i<v_filename.size();++i){
    v_fi.push_back(TFile::Open(Form("%s", v_filename[i].c_str())));
    if(v_fi[i]->IsZombie()){
      cout << "wrong " << v_filename[i] << endl;
      return;
    }

    // s3 ring
    for(int r=1;r<=8;r++){
      v_h_ga_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_all_spider_ring%d",r)));
      v_hh_ga_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_all_spider_ring%d",r)));
      v_h_ga_spr[kr]->Add(v_hh_ga_spr[kr], -1);

      v_h_gr3_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring3_spider_ring%d",r)));
      v_hh_gr3_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring3_spider_ring%d",r)));
      v_h_gr3_spr[kr]->Add(v_hh_gr3_spr[kr], -1);

      v_h_gr4_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_spider_ring%d",r)));
      v_hh_gr4_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_spider_ring%d",r)));
      v_h_gr4_spr[kr]->Add(v_hh_gr4_spr[kr], -1);

      v_h_gr5_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_spider_ring%d",r)));
      v_hh_gr5_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_spider_ring%d",r)));
      v_h_gr5_spr[kr]->Add(v_hh_gr5_spr[kr], -1);

      v_h_grs32_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring3_sector2_spider_ring%d",r)));
      v_hh_grs32_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring3_sector2_spider_ring%d",r)));
      v_h_grs32_spr[kr]->Add(v_hh_grs32_spr[kr], -1);

      v_h_grs41_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector1_spider_ring%d",r)));
      v_hh_grs41_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector1_spider_ring%d",r)));
      v_h_grs41_spr[kr]->Add(v_hh_grs41_spr[kr], -1);

      v_h_grs52_spr.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_sector2_spider_ring%d",r)));
      v_hh_grs52_spr.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_sector2_spider_ring%d",r)));
      v_h_grs52_spr[kr]->Add(v_hh_grs52_spr[kr], -1);

      kr++;
    }
  }

  //
  int n_files = v_filename.size();

  TLine *l_334 = new TLine(334, 1., 334, 10000000);
  l_334->SetLineColor(kRed);
  l_334->SetLineWidth(2);
  l_334->SetLineStyle(2);

  TLine *l_406 = new TLine(406.5, 1., 406.5, 10000000);
  l_406->SetLineColor(kRed);
  l_406->SetLineWidth(2);
  l_406->SetLineStyle(2);

  TLine *l_439 = new TLine(439.4, 1., 439.4, 10000000);
  l_439->SetLineColor(kRed);
  l_439->SetLineWidth(2);
  l_439->SetLineStyle(2);

  TLine *l_506 = new TLine(506.4, 1., 506.4, 10000000);
  l_506->SetLineColor(kRed);
  l_506->SetLineWidth(2);
  l_506->SetLineStyle(2);

  TLine *l_737 = new TLine(737.4, 1., 737.4, 10000000);
  l_737->SetLineColor(kRed);
  l_737->SetLineWidth(2);
  l_737->SetLineStyle(2);

  //
  // ge all, spider different ring
  TCanvas *cc_ga_spr[n_files];
  TLegend *leg_ga_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_ga_spr[i] = new TCanvas(Form("cc_ga_spr_%s",v_filename[i].substr(4,8).c_str()),"",900,500);
    cc_ga_spr[i]->cd();
    cc_ga_spr[i]->SetLogy();

    leg_ga_spr[i] = new TLegend(0.48, 0.58, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_ga_spr[i]->AddEntry(v_h_ga_spr[i*8+r], v_h_ga_spr[i*8+r]->GetName());
      v_h_ga_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_ga_spr[i*8+r]->SetLineWidth(2);
      v_h_ga_spr[i*8+r]->GetXaxis()->SetRangeUser(300,900);
      if(r==0){
        v_h_ga_spr[i*8+r]->SetTitle(Form("ge all, spider differnet ring, projectile > %s keV",v_filename[i].substr(4,6).c_str()));
        v_h_ga_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_ga_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_ga_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_ga_spr[i*8+r]->Draw("hist");
      }else v_h_ga_spr[i*8+r]->Draw("same hist");
    }

    leg_ga_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    l_506->Draw("same");
    l_737->Draw("same");

    cc_ga_spr[i]->SaveAs(Form("fig/%s.png",cc_ga_spr[i]->GetName()));
  }

  //
  // ge ring3, spider different ring
  TCanvas *cc_gr3_spr[n_files];
  TLegend *leg_gr3_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_gr3_spr[i] = new TCanvas(Form("cc_gr3_spr_%s",v_filename[i].substr(4,8).c_str()),"",900,500);
    cc_gr3_spr[i]->cd();
    cc_gr3_spr[i]->SetLogy();

    leg_gr3_spr[i] = new TLegend(0.48, 0.58, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_gr3_spr[i]->AddEntry(v_h_gr3_spr[i*8+r], v_h_gr3_spr[i*8+r]->GetName());
      v_h_gr3_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_gr3_spr[i*8+r]->SetLineWidth(2);
      v_h_gr3_spr[i*8+r]->GetXaxis()->SetRangeUser(300,900);
      if(r==0){
        v_h_gr3_spr[i*8+r]->SetTitle(Form("ge ring3, spider different ring, projectile > %s keV",v_filename[i].substr(4,6).c_str()));
        v_h_gr3_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_gr3_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_gr3_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_gr3_spr[i*8+r]->Draw("hist");
      }else v_h_gr3_spr[i*8+r]->Draw("same hist");
    }

    leg_gr3_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    l_506->Draw("same");
    l_737->Draw("same");

    cc_gr3_spr[i]->SaveAs(Form("fig/%s.png",cc_gr3_spr[i]->GetName()));
  }

  // ge ring4, spider different ring
  TCanvas *cc_gr4_spr[n_files];
  TLegend *leg_gr4_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_gr4_spr[i] = new TCanvas(Form("cc_gr4_spr_%s",v_filename[i].substr(4,8).c_str()),"",900,500);
    cc_gr4_spr[i]->cd();
    cc_gr4_spr[i]->SetLogy();

    leg_gr4_spr[i] = new TLegend(0.48, 0.58, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_gr4_spr[i]->AddEntry(v_h_gr4_spr[i*8+r], v_h_gr4_spr[i*8+r]->GetName());
      v_h_gr4_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_gr4_spr[i*8+r]->SetLineWidth(2);
      v_h_gr4_spr[i*8+r]->GetXaxis()->SetRangeUser(300,900);
      if(r==0){
        v_h_gr4_spr[i*8+r]->SetTitle(Form("ge ring4, spider different ring, projectile > %s keV",v_filename[i].substr(4,6).c_str()));
        v_h_gr4_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_gr4_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_gr4_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_gr4_spr[i*8+r]->Draw("hist");
      }else v_h_gr4_spr[i*8+r]->Draw("same hist");
    }

    leg_gr4_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    l_506->Draw("same");
    l_737->Draw("same");

    cc_gr4_spr[i]->SaveAs(Form("fig/%s.png",cc_gr4_spr[i]->GetName()));
  }

  // ge ring5, spider different ring
  TCanvas *cc_gr5_spr[n_files];
  TLegend *leg_gr5_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_gr5_spr[i] = new TCanvas(Form("cc_gr5_spr_%s",v_filename[i].substr(4,8).c_str()),"",900,500);
    cc_gr5_spr[i]->cd();
    cc_gr5_spr[i]->SetLogy();

    leg_gr5_spr[i] = new TLegend(0.75, 0.58, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_gr5_spr[i]->AddEntry(v_h_gr5_spr[i*8+r], v_h_gr5_spr[i*8+r]->GetName());
      v_h_gr5_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_gr5_spr[i*8+r]->SetLineWidth(2);
      v_h_gr5_spr[i*8+r]->GetXaxis()->SetRangeUser(300,900);
      if(r==0){
        v_h_gr5_spr[i*8+r]->SetTitle(Form("ge ring5, spider different ring, projectile > %s keV",v_filename[i].substr(4,6).c_str()));
        v_h_gr5_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_gr5_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_gr5_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_gr5_spr[i*8+r]->Draw("hist");
      }else v_h_gr5_spr[i*8+r]->Draw("same hist");
    }

    leg_gr5_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    l_506->Draw("same");
    l_737->Draw("same");

    cc_gr5_spr[i]->SaveAs(Form("fig/%s.png",cc_gr5_spr[i]->GetName()));
  }

  //
  // ge ring3 sector2, spider different ring
  TCanvas *cc_grs32_spr[n_files];
  TLegend *leg_grs32_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs32_spr[i] = new TCanvas(Form("cc_grs32_spr_%s",v_filename[i].substr(4,8).c_str()),"",900,500);
    cc_grs32_spr[i]->cd();
    cc_grs32_spr[i]->SetLogy();

    leg_grs32_spr[i] = new TLegend(0.48, 0.58, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs32_spr[i]->AddEntry(v_h_grs32_spr[i*8+r], v_h_grs32_spr[i*8+r]->GetName());
      v_h_grs32_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_grs32_spr[i*8+r]->SetLineWidth(2);
      v_h_grs32_spr[i*8+r]->GetXaxis()->SetRangeUser(300,900);
      if(r==0){
        v_h_grs32_spr[i*8+r]->SetTitle(Form("ge ring3 sector2, spider different ring, projectile > %s keV",v_filename[i].substr(4,6).c_str()));
        v_h_grs32_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs32_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs32_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs32_spr[i*8+r]->Draw("hist");
      }else v_h_grs32_spr[i*8+r]->Draw("same hist");
    }

    leg_grs32_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    l_506->Draw("same");
    l_737->Draw("same");

    cc_grs32_spr[i]->SaveAs(Form("fig/%s.png",cc_grs32_spr[i]->GetName()));
  }

  // ge ring4 sector1, spider different ring
  TCanvas *cc_grs41_spr[n_files];
  TLegend *leg_grs41_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs41_spr[i] = new TCanvas(Form("cc_grs41_spr_%s",v_filename[i].substr(4,8).c_str()),"",900,500);
    cc_grs41_spr[i]->cd();
    cc_grs41_spr[i]->SetLogy();

    leg_grs41_spr[i] = new TLegend(0.48, 0.58, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs41_spr[i]->AddEntry(v_h_grs41_spr[i*8+r], v_h_grs41_spr[i*8+r]->GetName());
      v_h_grs41_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_grs41_spr[i*8+r]->SetLineWidth(2);
      v_h_grs41_spr[i*8+r]->GetXaxis()->SetRangeUser(300,900);
      if(r==0){
        v_h_grs41_spr[i*8+r]->SetTitle(Form("ge ring4 sector1, spider different ring, projectile > %s keV",v_filename[i].substr(4,6).c_str()));
        v_h_grs41_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs41_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs41_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs41_spr[i*8+r]->Draw("hist");
      }else v_h_grs41_spr[i*8+r]->Draw("same hist");
    }

    leg_grs41_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    l_506->Draw("same");
    l_737->Draw("same");

    cc_grs41_spr[i]->SaveAs(Form("fig/%s.png",cc_grs41_spr[i]->GetName()));
  }

  // ge ring5 sector2, spider different ring
  TCanvas *cc_grs52_spr[n_files];
  TLegend *leg_grs52_spr[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs52_spr[i] = new TCanvas(Form("cc_grs52_spr_%s",v_filename[i].substr(4,8).c_str()),"",900,500);
    cc_grs52_spr[i]->cd();
    cc_grs52_spr[i]->SetLogy();

    leg_grs52_spr[i] = new TLegend(0.48, 0.58, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs52_spr[i]->AddEntry(v_h_grs52_spr[i*8+r], v_h_grs52_spr[i*8+r]->GetName());
      v_h_grs52_spr[i*8+r]->SetLineColor(colors[r]);
      v_h_grs52_spr[i*8+r]->SetLineWidth(2);
      v_h_grs52_spr[i*8+r]->GetXaxis()->SetRangeUser(300,900);
      if(r==0){
        v_h_grs52_spr[i*8+r]->SetTitle(Form("ge ring5 sector2, spider different ring, projectile > %s keV",v_filename[i].substr(4,6).c_str()));
        v_h_grs52_spr[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs52_spr[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs52_spr[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs52_spr[i*8+r]->Draw("hist");
      }else v_h_grs52_spr[i*8+r]->Draw("same hist");
    }

    leg_grs52_spr[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    l_506->Draw("same");
    l_737->Draw("same");

    cc_grs52_spr[i]->SaveAs(Form("fig/%s.png",cc_grs52_spr[i]->GetName()));
  }
}
