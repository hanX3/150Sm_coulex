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
    "data0472_0718_doppler_100ns_e_hist",
    "data0472_0718_doppler_250ns_jump_250ns_e_hist",
    "data0472_0718_doppler_200ns_jump_200ns_e_hist",
    "data0472_0718_doppler_150ns_jump_150ns_e_hist",
    "data0472_0718_doppler_100ns_jump_100ns_e_hist",
    "data0472_0718_doppler_50ns_jump_50ns_e_hist"
  };

  TCanvas *cc = new TCanvas("cc","",900,500);
  cc->SetLogy();

  TLegend *leg = new TLegend(0.7, 0.4, 0.98, 0.95);

  vector<TFile*> v_fi;
  vector<TH1D*> v_h, v_hh;
  for(int i=0;i<v_filename.size();++i){
    v_fi.push_back(TFile::Open(TString::Format("../rootfile/%s.root", v_filename[i].c_str())));
    if(v_fi[i]->IsZombie()){
      cout << "wrong " << v_filename[i] << endl;
      return;
    }

    v_h.push_back((TH1D*)v_fi[i]->Get("event_e_dc_r_1000fs_spider_p"));
    v_hh.push_back((TH1D*)v_fi[i]->Get("bg_e_dc_r_1000fs_spider_p"));
    v_h[i]->Add(v_hh[i], -1);

    leg->AddEntry(v_h[i], v_filename[i].c_str());
    v_h[i]->SetLineColor(colors[i]);
    if(i==0) v_h[i]->Draw();
    else v_h[i]->Draw("same");
  }

  leg->Draw();
}
