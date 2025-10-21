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
    "try/030deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/060deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/090deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/120deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/150deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/180deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/210deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/240deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/270deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/300deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/330deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root"
  };

  vector<TFile*> v_fi;
  vector<TH1D*> v_h_ga_s3a, v_hh_ga_s3a, v_h_grs32_s3a, v_hh_grs32_s3a, v_h_grs41_s3a, v_hh_grs41_s3a, v_h_grs42_s3a, v_hh_grs42_s3a, v_h_grs43_s3a, v_hh_grs43_s3a, v_h_grs52_s3a, v_hh_grs52_s3a, v_h_grs53_s3a, v_hh_grs53_s3a;
  vector<TH1D*> v_h_ga_s3r, v_hh_ga_s3r, v_h_grs32_s3r, v_hh_grs32_s3r, v_h_grs41_s3r, v_hh_grs41_s3r, v_h_grs42_s3r, v_hh_grs42_s3r, v_h_grs43_s3r, v_hh_grs43_s3r, v_h_grs52_s3r, v_hh_grs52_s3r, v_h_grs53_s3r, v_hh_grs53_s3r;
  vector<TH1D*> v_h_ga_s3s, v_hh_ga_s3s, v_h_grs32_s3s, v_hh_grs32_s3s, v_h_grs41_s3s, v_hh_grs41_s3s, v_h_grs42_s3s, v_hh_grs42_s3s, v_h_grs43_s3s, v_hh_grs43_s3s, v_h_grs52_s3s, v_hh_grs52_s3s, v_h_grs53_s3s, v_hh_grs53_s3s;

  int kr = 0, ks = 0;
  for(int i=0;i<v_filename.size();++i){
    v_fi.push_back(TFile::Open(Form("%s", v_filename[i].c_str())));
    if(v_fi[i]->IsZombie()){
      cout << "wrong " << v_filename[i] << endl;
      return;
    }

    // s3 all
    v_h_ga_s3a.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_all_s3_all"));
    v_hh_ga_s3a.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_all_s3_all"));
    v_h_ga_s3a[i]->Add(v_hh_ga_s3a[i], -1);

    v_h_grs32_s3a.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring3_sector2_s3_all"));
    v_hh_grs32_s3a.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring3_sector2_s3_all"));
    v_h_grs32_s3a[i]->Add(v_hh_grs32_s3a[i], -1);

    v_h_grs41_s3a.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring4_sector1_s3_all"));
    v_hh_grs41_s3a.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring4_sector1_s3_all"));
    v_h_grs41_s3a[i]->Add(v_hh_grs41_s3a[i], -1);

    v_h_grs42_s3a.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring4_sector2_s3_all"));
    v_hh_grs42_s3a.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring4_sector2_s3_all"));
    v_h_grs42_s3a[i]->Add(v_hh_grs42_s3a[i], -1);

    v_h_grs43_s3a.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring4_sector3_s3_all"));
    v_hh_grs43_s3a.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring4_sector3_s3_all"));
    v_h_grs43_s3a[i]->Add(v_hh_grs43_s3a[i], -1);

    v_h_grs52_s3a.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring5_sector2_s3_all"));
    v_hh_grs52_s3a.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring5_sector2_s3_all"));
    v_h_grs52_s3a[i]->Add(v_hh_grs52_s3a[i], -1);

    v_h_grs53_s3a.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_ge_ring5_sector3_s3_all"));
    v_hh_grs53_s3a.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_ge_ring5_sector3_s3_all"));
    v_h_grs53_s3a[i]->Add(v_hh_grs53_s3a[i], -1);

    // s3 ring
    for(int r=1;r<=8;r++){
      v_h_ga_s3r.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_all_s3_ring%d",r*3)));
      v_hh_ga_s3r.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_all_s3_ring%d",r*3)));
      v_h_ga_s3r[kr]->Add(v_hh_ga_s3r[kr], -1);

      v_h_grs32_s3r.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring3_sector2_s3_ring%d",r*3)));
      v_hh_grs32_s3r.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring3_sector2_s3_ring%d",r*3)));
      v_h_grs32_s3r[kr]->Add(v_hh_grs32_s3r[kr], -1);

      v_h_grs41_s3r.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector1_s3_ring%d",r*3)));
      v_hh_grs41_s3r.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector1_s3_ring%d",r*3)));
      v_h_grs41_s3r[kr]->Add(v_hh_grs41_s3r[kr], -1);

      v_h_grs42_s3r.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector2_s3_ring%d",r*3)));
      v_hh_grs42_s3r.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector2_s3_ring%d",r*3)));
      v_h_grs42_s3r[kr]->Add(v_hh_grs42_s3r[kr], -1);

      v_h_grs43_s3r.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector3_s3_ring%d",r*3)));
      v_hh_grs43_s3r.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector3_s3_ring%d",r*3)));
      v_h_grs43_s3r[kr]->Add(v_hh_grs43_s3r[kr], -1);

      v_h_grs52_s3r.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_sector2_s3_ring%d",r*3)));
      v_hh_grs52_s3r.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_sector2_s3_ring%d",r*3)));
      v_h_grs52_s3r[kr]->Add(v_hh_grs52_s3r[kr], -1);

      v_h_grs53_s3r.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_sector3_s3_ring%d",r*3)));
      v_hh_grs53_s3r.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_sector3_s3_ring%d",r*3)));
      v_h_grs53_s3r[kr]->Add(v_hh_grs53_s3r[kr], -1);

      kr++;
    }

    // s3 sector
    for(int s=1;s<=8;s++){
      v_h_ga_s3s.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_all_s3_sector%d",s*4)));
      v_hh_ga_s3s.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_all_s3_sector%d",s*4)));
      v_h_ga_s3s[ks]->Add(v_hh_ga_s3s[ks], -1);

      v_h_grs32_s3s.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring3_sector2_s3_sector%d",s*4)));
      v_hh_grs32_s3s.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring3_sector2_s3_sector%d",s*4)));
      v_h_grs32_s3s[ks]->Add(v_hh_grs32_s3s[ks], -1);

      v_h_grs41_s3s.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector1_s3_sector%d",s*4)));
      v_hh_grs41_s3s.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector1_s3_sector%d",s*4)));
      v_h_grs41_s3s[ks]->Add(v_hh_grs41_s3s[ks], -1);

      v_h_grs42_s3s.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector2_s3_sector%d",s*4)));
      v_hh_grs42_s3s.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector2_s3_sector%d",s*4)));
      v_h_grs42_s3s[ks]->Add(v_hh_grs42_s3s[ks], -1);

      v_h_grs43_s3s.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring4_sector3_s3_sector%d",s*4)));
      v_hh_grs43_s3s.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring4_sector3_s3_sector%d",s*4)));
      v_h_grs43_s3s[ks]->Add(v_hh_grs43_s3s[ks], -1);

      v_h_grs52_s3s.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_sector2_s3_sector%d",s*4)));
      v_hh_grs52_s3s.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_sector2_s3_sector%d",s*4)));
      v_h_grs52_s3s[ks]->Add(v_hh_grs52_s3s[ks], -1);

      v_h_grs53_s3s.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring5_sector3_s3_sector%d",s*4)));
      v_hh_grs53_s3s.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring5_sector3_s3_sector%d",s*4)));
      v_h_grs53_s3s[ks]->Add(v_hh_grs53_s3s[ks], -1);

      ks++;
    }
  }
  cout << "ks " << ks << endl;

  //
  int n_files = v_filename.size();

  TLine *l_334 = new TLine(334, 1., 334, v_h_ga_s3a[0]->GetMaximum()*10);
  l_334->SetLineColor(kRed);
  l_334->SetLineWidth(2);
  l_334->SetLineStyle(2);

  TLine *l_406 = new TLine(406.5, 1., 406.5, v_h_ga_s3a[0]->GetMaximum()*10);
  l_406->SetLineColor(kRed);
  l_406->SetLineWidth(2);
  l_406->SetLineStyle(2);

  TLine *l_439 = new TLine(439.4, 1., 439.4, v_h_ga_s3a[0]->GetMaximum()*10);
  l_439->SetLineColor(kRed);
  l_439->SetLineWidth(2);
  l_439->SetLineStyle(2);

  // ge all, s3 all
  TCanvas *cc_ga_s3a = new TCanvas("cc_ga_s3a","",900,500);
  cc_ga_s3a->cd();
  cc_ga_s3a->SetLogy();

  TLegend *leg_ga_s3a = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_ga_s3a->AddEntry(v_h_ga_s3a[i], v_filename[i].substr(4,6).c_str());
    v_h_ga_s3a[i]->SetLineColor(colors[i]);
    v_h_ga_s3a[i]->SetLineWidth(2);
    v_h_ga_s3a[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_ga_s3a[i]->SetTitle("ge all, different s3 phi offset");
      v_h_ga_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_ga_s3a[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_ga_s3a[i]->GetYaxis()->SetRangeUser(1., v_h_ga_s3a[i]->GetMaximum()*10);
      v_h_ga_s3a[i]->Draw("hist");
    }else v_h_ga_s3a[i]->Draw("same hist");
  }
  leg_ga_s3a->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_ga_s3a->SaveAs("fig/cc_ga_s3a.png");

  //
  // ge ring3 sector2, s3 all
  TCanvas *cc_grs32_s3a = new TCanvas("cc_grs32_s3a","",900,500);
  cc_grs32_s3a->cd();
  cc_grs32_s3a->SetLogy();

  TLegend *leg_grs32_s3a = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs32_s3a->AddEntry(v_h_grs32_s3a[i], v_filename[i].substr(4,6).c_str());
    v_h_grs32_s3a[i]->SetTitle("ge ring3 sector2, different s3 phi offset");
    v_h_grs32_s3a[i]->SetLineColor(colors[i]);
    v_h_grs32_s3a[i]->SetLineWidth(2);
    v_h_grs32_s3a[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs32_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs32_s3a[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs32_s3a[i]->GetYaxis()->SetRangeUser(1., v_h_grs32_s3a[i]->GetMaximum()*10);
      v_h_grs32_s3a[i]->Draw("hist");
    }else v_h_grs32_s3a[i]->Draw("same hist");
  }
  leg_grs32_s3a->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs32_s3a->SaveAs("fig/cc_grs32_s3a.png");

  // ge ring3 sector2, s3 different ring
  TCanvas *cc_grs32_s3r[n_files];
  TLegend *leg_grs32_s3r[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs32_s3r[i] = new TCanvas(Form("cc_grs32_s3r_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs32_s3r[i]->cd();
    cc_grs32_s3r[i]->SetLogy();

    leg_grs32_s3r[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs32_s3r[i]->AddEntry(v_h_grs32_s3r[i*8+r], Form("ring%d",(r+1)*3));
      v_h_grs32_s3r[i*8+r]->SetLineColor(colors[r]);
      v_h_grs32_s3r[i*8+r]->SetLineWidth(2);
      v_h_grs32_s3r[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs32_s3r[i*8+r]->SetTitle(Form("ge ring3 sector2, s3 differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs32_s3r[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs32_s3r[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs32_s3r[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs32_s3r[i*8+r]->Draw("hist");
      }else v_h_grs32_s3r[i*8+r]->Draw("same hist");
    }

    leg_grs32_s3r[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs32_s3r[i]->SaveAs(Form("fig/%s.png",cc_grs32_s3r[i]->GetName()));
  }

  // ge ring3 sector2, s3 different sector
  TCanvas *cc_grs32_s3s[n_files];
  TLegend *leg_grs32_s3s[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs32_s3s[i] = new TCanvas(Form("cc_grs32_s3s_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs32_s3s[i]->cd();
    cc_grs32_s3s[i]->SetLogy();

    leg_grs32_s3s[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<8;s++){
      leg_grs32_s3s[i]->AddEntry(v_h_grs32_s3s[i*8+s], Form("sector%d",(s+1)*4));
      v_h_grs32_s3s[i*8+s]->SetLineColor(colors[s]);
      v_h_grs32_s3s[i*8+s]->SetLineWidth(2);
      v_h_grs32_s3s[i*8+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs32_s3s[i*8+s]->SetTitle(Form("ge ring3 sector2, s3 differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs32_s3s[i*8+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs32_s3s[i*8+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs32_s3s[i*8+s]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs32_s3s[i*8+s]->Draw("hist");
      }else v_h_grs32_s3s[i*8+s]->Draw("same hist");
    }

    leg_grs32_s3s[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs32_s3s[i]->SaveAs(Form("fig/%s.png",cc_grs32_s3s[i]->GetName()));
  }

  //
  // ge ring4 sector1, s3 all
  TCanvas *cc_grs41_s3a = new TCanvas("cc_grs41_s3a","",900,500);
  cc_grs41_s3a->cd();
  cc_grs41_s3a->SetLogy();

  TLegend *leg_grs41_s3a = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs41_s3a->AddEntry(v_h_grs41_s3a[i], v_filename[i].substr(4,6).c_str());
    v_h_grs41_s3a[i]->SetTitle("ge ring4 sector1, different s3 phi offset");
    v_h_grs41_s3a[i]->SetLineColor(colors[i]);
    v_h_grs41_s3a[i]->SetLineWidth(2);
    v_h_grs41_s3a[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs41_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs41_s3a[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs41_s3a[i]->GetYaxis()->SetRangeUser(1., 1000000);
      v_h_grs41_s3a[i]->Draw("hist");
    }else v_h_grs41_s3a[i]->Draw("same hist");
  }
  leg_grs41_s3a->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs41_s3a->SaveAs("fig/cc_grs41_s3a.png");

  // ge ring4 sector1, s3 different ring
  TCanvas *cc_grs41_s3r[n_files];
  TLegend *leg_grs41_s3r[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs41_s3r[i] = new TCanvas(Form("cc_grs41_s3r_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs41_s3r[i]->cd();
    cc_grs41_s3r[i]->SetLogy();

    leg_grs41_s3r[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs41_s3r[i]->AddEntry(v_h_grs41_s3r[i*8+r], Form("ring%d",(r+1)*3));
      v_h_grs41_s3r[i*8+r]->SetLineColor(colors[r]);
      v_h_grs41_s3r[i*8+r]->SetLineWidth(2);
      v_h_grs41_s3r[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs41_s3r[i*8+r]->SetTitle(Form("ge ring4 sector1, s3 differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs41_s3r[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs41_s3r[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs41_s3r[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs41_s3r[i*8+r]->Draw("hist");
      }else v_h_grs41_s3r[i*8+r]->Draw("same hist");
    }

    leg_grs41_s3r[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs41_s3r[i]->SaveAs(Form("fig/%s.png",cc_grs41_s3r[i]->GetName()));
  }

  // ge ring4 sector1, s3 different sector
  TCanvas *cc_grs41_s3s[n_files];
  TLegend *leg_grs41_s3s[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs41_s3s[i] = new TCanvas(Form("cc_grs41_s3s_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs41_s3s[i]->cd();
    cc_grs41_s3s[i]->SetLogy();

    leg_grs41_s3s[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<8;s++){
      leg_grs41_s3s[i]->AddEntry(v_h_grs41_s3s[i*8+s], Form("sector%d",(s+1)*4));
      v_h_grs41_s3s[i*8+s]->SetLineColor(colors[s]);
      v_h_grs41_s3s[i*8+s]->SetLineWidth(2);
      v_h_grs41_s3s[i*8+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs41_s3s[i*8+s]->SetTitle(Form("ge ring4 sector1, s3 differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs41_s3s[i*8+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs41_s3s[i*8+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs41_s3s[i*8+s]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs41_s3s[i*8+s]->Draw("hist");
      }else v_h_grs41_s3s[i*8+s]->Draw("same hist");
    }

    leg_grs41_s3s[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs41_s3s[i]->SaveAs(Form("fig/%s.png",cc_grs41_s3s[i]->GetName()));
  }

  //
  // ge ring4 sector2, s3 all
  TCanvas *cc_grs42_s3a = new TCanvas("cc_grs42_s3a","",900,500);
  cc_grs42_s3a->cd();
  cc_grs42_s3a->SetLogy();

  TLegend *leg_grs42_s3a = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs42_s3a->AddEntry(v_h_grs42_s3a[i], v_filename[i].substr(4,6).c_str());
    v_h_grs42_s3a[i]->SetTitle("ge ring4 sector2, s3 differnet ring, different s3 phi offset");
    v_h_grs42_s3a[i]->SetLineColor(colors[i]);
    v_h_grs42_s3a[i]->SetLineWidth(2);
    v_h_grs42_s3a[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs42_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs42_s3a[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs42_s3a[i]->GetYaxis()->SetRangeUser(1., 1000000);
      v_h_grs42_s3a[i]->Draw("hist");
    }else v_h_grs42_s3a[i]->Draw("same hist");
  }
  leg_grs42_s3a->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs42_s3a->SaveAs("fig/cc_grs42_s3a.png");

  // ge ring4 sector2, s3 different ring
  TCanvas *cc_grs42_s3r[n_files];
  TLegend *leg_grs42_s3r[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs42_s3r[i] = new TCanvas(Form("cc_grs42_s3r_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs42_s3r[i]->cd();
    cc_grs42_s3r[i]->SetLogy();

    leg_grs42_s3r[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs42_s3r[i]->AddEntry(v_h_grs42_s3r[i*8+r], Form("ring%d",(r+1)*3));
      v_h_grs42_s3r[i*8+r]->SetLineColor(colors[r]);
      v_h_grs42_s3r[i*8+r]->SetLineWidth(2);
      v_h_grs42_s3r[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs42_s3r[i*8+r]->SetTitle(Form("ge ring4 sector2, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs42_s3r[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs42_s3r[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs42_s3r[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs42_s3r[i*8+r]->Draw("hist");
      }else v_h_grs42_s3r[i*8+r]->Draw("same hist");
    }

    leg_grs42_s3r[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs42_s3r[i]->SaveAs(Form("fig/%s.png",cc_grs42_s3r[i]->GetName()));
  }

  // ge ring4 sector2, s3 different sector
  TCanvas *cc_grs42_s3s[n_files];
  TLegend *leg_grs42_s3s[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs42_s3s[i] = new TCanvas(Form("cc_grs42_s3s_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs42_s3s[i]->cd();
    cc_grs42_s3s[i]->SetLogy();

    leg_grs42_s3s[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<8;s++){
      leg_grs42_s3s[i]->AddEntry(v_h_grs42_s3s[i*8+s], Form("sector%d",(s+1)*4));
      v_h_grs42_s3s[i*8+s]->SetLineColor(colors[s]);
      v_h_grs42_s3s[i*8+s]->SetLineWidth(2);
      v_h_grs42_s3s[i*8+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs42_s3s[i*8+s]->SetTitle(Form("ge ring4 sector2, s3 differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs42_s3s[i*8+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs42_s3s[i*8+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs42_s3s[i*8+s]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs42_s3s[i*8+s]->Draw("hist");
      }else v_h_grs42_s3s[i*8+s]->Draw("same hist");
    }

    leg_grs42_s3s[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs42_s3s[i]->SaveAs(Form("fig/%s.png",cc_grs42_s3s[i]->GetName()));
  }
  
  //
  // ge ring4 sector3, s3 all
  TCanvas *cc_grs43_s3a = new TCanvas("cc_grs43_s3a","",900,500);
  cc_grs43_s3a->cd();
  cc_grs43_s3a->SetLogy();

  TLegend *leg_grs43_s3a = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs43_s3a->AddEntry(v_h_grs43_s3a[i], v_filename[i].substr(4,6).c_str());
    v_h_grs43_s3a[i]->SetTitle("ge ring4 sector3, s3 differnet ring, different s3 phi offset");
    v_h_grs43_s3a[i]->SetLineColor(colors[i]);
    v_h_grs43_s3a[i]->SetLineWidth(2);
    v_h_grs43_s3a[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs43_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs43_s3a[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs43_s3a[i]->GetYaxis()->SetRangeUser(1., 1000000);
      v_h_grs43_s3a[i]->Draw("hist");
    }else v_h_grs43_s3a[i]->Draw("same hist");
  }
  leg_grs43_s3a->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs43_s3a->SaveAs("fig/cc_grs43_s3a.png");

  // ge ring4 sector3, s3 different ring
  TCanvas *cc_grs43_s3r[n_files];
  TLegend *leg_grs43_s3r[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs43_s3r[i] = new TCanvas(Form("cc_grs43_s3r_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs43_s3r[i]->cd();
    cc_grs43_s3r[i]->SetLogy();

    leg_grs43_s3r[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs43_s3r[i]->AddEntry(v_h_grs43_s3r[i*8+r], Form("ring%d",(r+1)*3));
      v_h_grs43_s3r[i*8+r]->SetLineColor(colors[r]);
      v_h_grs43_s3r[i*8+r]->SetLineWidth(2);
      v_h_grs43_s3r[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs43_s3r[i*8+r]->SetTitle(Form("ge ring4 sector3, s3 differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs43_s3r[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs43_s3r[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs43_s3r[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs43_s3r[i*8+r]->Draw("hist");
      }else v_h_grs43_s3r[i*8+r]->Draw("same hist");
    }

    leg_grs43_s3r[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs43_s3r[i]->SaveAs(Form("fig/%s.png",cc_grs43_s3r[i]->GetName()));
  }

  // ge ring4 sector3, s3 different sector
  TCanvas *cc_grs43_s3s[n_files];
  TLegend *leg_grs43_s3s[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs43_s3s[i] = new TCanvas(Form("cc_grs43_s3s_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs43_s3s[i]->cd();
    cc_grs43_s3s[i]->SetLogy();

    leg_grs43_s3s[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<8;s++){
      leg_grs43_s3s[i]->AddEntry(v_h_grs43_s3s[i*8+s], Form("sector%d",(s+1)*4));
      v_h_grs43_s3s[i*8+s]->SetLineColor(colors[s]);
      v_h_grs43_s3s[i*8+s]->SetLineWidth(2);
      v_h_grs43_s3s[i*8+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs43_s3s[i*8+s]->SetTitle(Form("ge ring4 sector3, s3 differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs43_s3s[i*8+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs43_s3s[i*8+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs43_s3s[i*8+s]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs43_s3s[i*8+s]->Draw("hist");
      }else v_h_grs43_s3s[i*8+s]->Draw("same hist");
    }

    leg_grs43_s3s[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs43_s3s[i]->SaveAs(Form("fig/%s.png",cc_grs43_s3s[i]->GetName()));
  }

  //
  // ge ring5 sector2, s3 all
  TCanvas *cc_grs52_s3a = new TCanvas("cc_grs52_s3a","",900,500);
  cc_grs52_s3a->cd();
  cc_grs52_s3a->SetLogy();

  TLegend *leg_grs52_s3a = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs52_s3a->AddEntry(v_h_grs52_s3a[i], v_filename[i].substr(4,6).c_str());
    v_h_grs52_s3a[i]->SetTitle("ge ring5 sector2, different s3 phi offset");
    v_h_grs52_s3a[i]->SetLineColor(colors[i]);
    v_h_grs52_s3a[i]->SetLineWidth(2);
    v_h_grs52_s3a[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs52_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs52_s3a[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs52_s3a[i]->GetYaxis()->SetRangeUser(1., 1000000);
      v_h_grs52_s3a[i]->Draw("hist");
    }else v_h_grs52_s3a[i]->Draw("same hist");
  }
  leg_grs52_s3a->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs52_s3a->SaveAs("fig/cc_grs52_s3a.png");

  // ge ring5 sector2, s3 different ring
  TCanvas *cc_grs52_s3r[n_files];
  TLegend *leg_grs52_s3r[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs52_s3r[i] = new TCanvas(Form("cc_grs52_s3r_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs52_s3r[i]->cd();
    cc_grs52_s3r[i]->SetLogy();

    leg_grs52_s3r[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs52_s3r[i]->AddEntry(v_h_grs52_s3r[i*8+r], Form("ring%d",(r+1)*3));
      v_h_grs52_s3r[i*8+r]->SetLineColor(colors[r]);
      v_h_grs52_s3r[i*8+r]->SetLineWidth(2);
      v_h_grs52_s3r[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs52_s3r[i*8+r]->SetTitle(Form("ge ring5 sector2, s3 differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs52_s3r[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs52_s3r[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs52_s3r[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs52_s3r[i*8+r]->Draw("hist");
      }else v_h_grs52_s3r[i*8+r]->Draw("same hist");
    }

    leg_grs52_s3r[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs52_s3r[i]->SaveAs(Form("fig/%s.png",cc_grs52_s3r[i]->GetName()));
  }
  
  // ge ring5 sector2, s3 different sector
  TCanvas *cc_grs52_s3s[n_files];
  TLegend *leg_grs52_s3s[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs52_s3s[i] = new TCanvas(Form("cc_grs52_s3s_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs52_s3s[i]->cd();
    cc_grs52_s3s[i]->SetLogy();

    leg_grs52_s3s[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<8;s++){
      leg_grs52_s3s[i]->AddEntry(v_h_grs52_s3s[i*8+s], Form("sector%d",(s+1)*4));
      v_h_grs52_s3s[i*8+s]->SetLineColor(colors[s]);
      v_h_grs52_s3s[i*8+s]->SetLineWidth(2);
      v_h_grs52_s3s[i*8+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs52_s3s[i*8+s]->SetTitle(Form("ge ring5 sector2, s3 differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs52_s3s[i*8+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs52_s3s[i*8+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs52_s3s[i*8+s]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs52_s3s[i*8+s]->Draw("hist");
      }else v_h_grs52_s3s[i*8+s]->Draw("same hist");
    }

    leg_grs52_s3s[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs52_s3s[i]->SaveAs(Form("fig/%s.png",cc_grs52_s3s[i]->GetName()));
  }
  //
  // ge ring5 sector3, s3 all
  TCanvas *cc_grs53_s3a = new TCanvas("cc_grs53_s3a","",900,500);
  cc_grs53_s3a->cd();
  cc_grs53_s3a->SetLogy();

  TLegend *leg_grs53_s3a = new TLegend(0.75, 0.38, 0.88, 0.92);
  for(int i=0;i<n_files;++i){
    leg_grs53_s3a->AddEntry(v_h_grs53_s3a[i], v_filename[i].substr(4,6).c_str());
    v_h_grs53_s3a[i]->SetTitle("ge ring5 sector3, different s3 phi offset");
    v_h_grs53_s3a[i]->SetLineColor(colors[i]);
    v_h_grs53_s3a[i]->SetLineWidth(2);
    v_h_grs53_s3a[i]->GetXaxis()->SetRangeUser(300,500);
    if(i==0){
      v_h_grs53_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      v_h_grs53_s3a[i]->GetYaxis()->SetTitle("counts / keV");
      v_h_grs53_s3a[i]->GetYaxis()->SetRangeUser(1., 1000000);
      v_h_grs53_s3a[i]->Draw("hist");
    }else v_h_grs53_s3a[i]->Draw("same hist");
  }
  leg_grs53_s3a->Draw("same");

  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");

  cc_grs53_s3a->SaveAs("fig/cc_grs53_s3a.png");
  
  // ge ring5 sector3, s3 different ring
  TCanvas *cc_grs53_s3r[n_files];
  TLegend *leg_grs53_s3r[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs53_s3r[i] = new TCanvas(Form("cc_grs53_s3r_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs53_s3r[i]->cd();
    cc_grs53_s3r[i]->SetLogy();

    leg_grs53_s3r[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int r=0;r<8;r++){
      leg_grs53_s3r[i]->AddEntry(v_h_grs53_s3r[i*8+r], Form("ring%d",(r+1)*3));
      v_h_grs53_s3r[i*8+r]->SetLineColor(colors[r]);
      v_h_grs53_s3r[i*8+r]->SetLineWidth(2);
      v_h_grs53_s3r[i*8+r]->GetXaxis()->SetRangeUser(300,500);
      if(r==0){
        v_h_grs53_s3r[i*8+r]->SetTitle(Form("ge ring5 sector3, s3 differnet ring, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs53_s3r[i*8+r]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs53_s3r[i*8+r]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs53_s3r[i*8+r]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs53_s3r[i*8+r]->Draw("hist");
      }else v_h_grs53_s3r[i*8+r]->Draw("same hist");
    }

    leg_grs53_s3r[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs53_s3r[i]->SaveAs(Form("fig/%s.png",cc_grs53_s3r[i]->GetName()));
  }

  // ge ring5 sector3, s3 different sector
  TCanvas *cc_grs53_s3s[n_files];
  TLegend *leg_grs53_s3s[n_files];
  for(int i=0;i<n_files;i++){
    cc_grs53_s3s[i] = new TCanvas(Form("cc_grs53_s3s_%s",v_filename[i].substr(4,6).c_str()),"",900,500);
    cc_grs53_s3s[i]->cd();
    cc_grs53_s3s[i]->SetLogy();

    leg_grs53_s3s[i] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int s=0;s<8;s++){
      leg_grs53_s3s[i]->AddEntry(v_h_grs53_s3s[i*8+s], Form("sector%d",(s+1)*4));
      v_h_grs53_s3s[i*8+s]->SetLineColor(colors[s]);
      v_h_grs53_s3s[i*8+s]->SetLineWidth(2);
      v_h_grs53_s3s[i*8+s]->GetXaxis()->SetRangeUser(300,500);
      if(s==0){
        v_h_grs53_s3s[i*8+s]->SetTitle(Form("ge ring5 sector3, s3 differnet sector, phi offset %s",v_filename[i].substr(4,6).c_str()));
        v_h_grs53_s3s[i*8+s]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs53_s3s[i*8+s]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs53_s3s[i*8+s]->GetYaxis()->SetRangeUser(1., 1000000);
        v_h_grs53_s3s[i*8+s]->Draw("hist");
      }else v_h_grs53_s3s[i*8+s]->Draw("same hist");
    }

    leg_grs53_s3s[i]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs53_s3s[i]->SaveAs(Form("fig/%s.png",cc_grs53_s3s[i]->GetName()));
  }
}
