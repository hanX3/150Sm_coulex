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
    "try/330deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/300deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/270deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/240deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/210deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/180deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/150deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/120deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/090deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/060deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/030deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root",
    "try/000deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root"
  };

  std::map<std::pair<int,int>, bool> m_ge_rs_good;
  std::ifstream fi_ge_good("../../pars/config/good_ge.txt");
  if(!fi_ge_good){
    std::cout << "can not open good_ge.txt" << std::endl;
    return;
  }
  std::string line;
  int ring, sector;
  bool gb;
  std::getline(fi_ge_good, line);
  while(fi_ge_good>>ring>>sector>>gb){
    m_ge_rs_good.insert({{ring, sector}, gb});
  }
  fi_ge_good.close();

  int nn = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    nn++;
  }

  vector<TFile*> v_fi;
  vector<TH1D*> v_h_grs_spa, v_hh_grs_spa;

  int krs = 0;
  for(int i=0;i<v_filename.size();++i){
    v_fi.push_back(TFile::Open(Form("%s", v_filename[i].c_str())));
    if(v_fi[i]->IsZombie()){
      cout << "wrong " << v_filename[i] << endl;
      return;
    }

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
  
      cout << "krs ==> " << krs << " ge ring " << key.first << " sector " << key.second << endl;

      //
      v_h_grs_spa.push_back((TH1D*)v_fi[i]->Get(Form("event_e_dc_r_ge_ring%d_sector%d_spider_all",key.first,key.second)));
      v_hh_grs_spa.push_back((TH1D*)v_fi[i]->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_spider_all",key.first,key.second)));
      v_h_grs_spa[krs]->Add(v_hh_grs_spa[krs], -1);
      krs++;
    }
  }

  //
  int n_files = v_filename.size();

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
  // ge single, spider all
  TCanvas *cc_grs_spa = new TCanvas("cc_grs_spa","",900,500);
  cc_grs_spa->cd();
  cc_grs_spa->SetLogy();

  TLegend *leg_grs_spa[nn];
  
  krs = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    cc_grs_spa->SetName(Form("cc_grs%d%d_spa",key.first,key.second));

    leg_grs_spa[krs] = new TLegend(0.75, 0.38, 0.88, 0.92);
    for(int i=0;i<n_files;++i){
      cout << "!!! " << nn*i+krs << endl;
      leg_grs_spa[krs]->AddEntry(v_h_grs_spa[nn*i+krs], v_filename[i].substr(4,6).c_str());
      v_h_grs_spa[nn*i+krs]->SetTitle(Form("ge ring%d sector%d, different ge phi offset",key.first,key.second));
      v_h_grs_spa[nn*i+krs]->SetLineColor(colors[i]);
      v_h_grs_spa[nn*i+krs]->SetLineWidth(2);
      v_h_grs_spa[nn*i+krs]->GetXaxis()->SetRangeUser(300,500);
      if(i==0){
        v_h_grs_spa[nn*i+krs]->GetXaxis()->SetTitle("energy [keV]");
        v_h_grs_spa[nn*i+krs]->GetYaxis()->SetTitle("counts / keV");
        v_h_grs_spa[nn*i+krs]->GetYaxis()->SetRangeUser(1., 100000);
        v_h_grs_spa[nn*i+krs]->Draw("hist");
      }else v_h_grs_spa[nn*i+krs]->Draw("same hist");
    }
    leg_grs_spa[krs]->Draw("same");

    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");

    cc_grs_spa->SaveAs(Form("fig/%s.png",cc_grs_spa->GetName()));
    krs++;
  }
}
