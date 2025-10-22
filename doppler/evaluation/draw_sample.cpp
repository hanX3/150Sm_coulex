#include <sys/stat.h>
#include <sys/types.h>

// example str = "data0472_0605_doppler_100ns_jump_300ns"
void draw_sample(string str="")
{
  gROOT->SetBatch(1);
  gStyle->SetOptStat(0);
  
  double x_min = 300;
  double x_max = 800;
  int r, s;
  int k = 0, kk = 0;;

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

  map<pair<int,int>, bool> m_ge_rs_good;
  ifstream fi_ge_good("../../pars/config/good_ge.txt");
  if(!fi_ge_good){
    cout << "can not open good_ge.txt" << endl;
    return;
  }
  string line;
  int ring, sector;
  bool gb;
  std::getline(fi_ge_good, line);
  while(fi_ge_good>>ring>>sector>>gb){
    m_ge_rs_good.insert({{ring, sector}, gb});
  }
  fi_ge_good.close();

  int nn_ge = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    nn_ge++;
  }
  std::cout << "! nn_ge " << nn_ge << std::endl;

  //
  TLine *l_334 = new TLine(334, 1., 334, 1000000000);
  l_334->SetLineColor(kRed);
  l_334->SetLineWidth(2);
  l_334->SetLineStyle(2);

  TLine *l_406 = new TLine(406.5, 1., 406.5, 1000000000);
  l_406->SetLineColor(kRed);
  l_406->SetLineWidth(2);
  l_406->SetLineStyle(2);

  TLine *l_439 = new TLine(439.4, 1., 439.4, 1000000000);
  l_439->SetLineColor(kRed);
  l_439->SetLineWidth(2);
  l_439->SetLineStyle(2);

  //
  TFile *fi = TFile::Open(Form("../../rootfile/%s_hist.root",str.c_str()));
  if(fi->IsZombie()){
    cout << "cannot open the file." << std::endl;
    return;
  }

  TCanvas *c = new TCanvas("c","",0,0,900,360);
  c->cd();
  c->SetLogy();

  TLegend *leg1 = new TLegend(0.6, 0.75, 0.98, 0.92); leg1->Clear();
  TLegend *leg2 = new TLegend(0.6, 0.45, 0.98, 0.92); leg2->Clear();
  TLegend *leg3 = new TLegend(0.6, 0.25, 0.98, 0.92); leg3->Clear();
  TLegend *leg4 = new TLegend(0.6, 0.05, 0.98, 0.92); leg4->Clear(); 

  ////
  ////
  ////
  // ge all spider all
  TH1D *h1_ga_spa, *h2_ga_spa, *h3_ga_spa;
  TH1D *hh1_ga_spa, *hh2_ga_spa, *hh3_ga_spa;

  h1_ga_spa = (TH1D*)fi->Get("event_e_raw_ge_all_spider_all");
  h2_ga_spa = (TH1D*)fi->Get("event_e_dc_p_ge_all_spider_all");
  h3_ga_spa = (TH1D*)fi->Get("event_e_dc_r_ge_all_spider_all");
  hh1_ga_spa = (TH1D*)fi->Get("bg_e_raw_ge_all_spider_all");
  hh2_ga_spa = (TH1D*)fi->Get("bg_e_dc_p_ge_all_spider_all");
  hh3_ga_spa = (TH1D*)fi->Get("bg_e_dc_r_ge_all_spider_all");

  h1_ga_spa->Add(hh1_ga_spa, -1);
  h2_ga_spa->Add(hh2_ga_spa, -1);
  h3_ga_spa->Add(hh3_ga_spa, -1);

  h1_ga_spa->GetYaxis()->SetRangeUser(1., 1000000000);
  h2_ga_spa->GetYaxis()->SetRangeUser(1., 1000000000);
  h3_ga_spa->GetYaxis()->SetRangeUser(1., 1000000000);

  h1_ga_spa->GetXaxis()->SetRangeUser(x_min, x_max);
  h1_ga_spa->SetTitle("ge all, spider all");
  h1_ga_spa->GetXaxis()->SetTitle("energy [keV]");
  h1_ga_spa->GetYaxis()->SetTitle("counts");
  h1_ga_spa->SetLineColor(colors[1]);
  h1_ga_spa->Draw();

  h2_ga_spa->GetXaxis()->SetRangeUser(x_min, x_max);
  h2_ga_spa->GetXaxis()->SetTitle("energy [keV]");
  h2_ga_spa->GetYaxis()->SetTitle("counts");
  h2_ga_spa->SetLineColor(colors[2]);
  h2_ga_spa->Draw("same");

  h3_ga_spa->GetXaxis()->SetRangeUser(x_min, x_max);
  h3_ga_spa->GetXaxis()->SetTitle("energy [keV]");
  h3_ga_spa->GetYaxis()->SetTitle("counts");
  h3_ga_spa->SetLineColor(colors[3]);
  h3_ga_spa->Draw("same");

  leg1->AddEntry(h1_ga_spa, h1_ga_spa->GetName());
  leg1->AddEntry(h2_ga_spa, h2_ga_spa->GetName());
  leg1->AddEntry(h3_ga_spa, h3_ga_spa->GetName());
  leg1->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/ge_all_spider_all.pdf");
  
  // ge ring spider all
  TH1D *h1_gr_spa[3], *h2_gr_spa[3], *h3_gr_spa[3];
  TH1D *hh1_gr_spa[3], *hh2_gr_spa[3], *hh3_gr_spa[3];
  k = 0;
  for(int i=3;i<=5;i++){
    h1_gr_spa[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_spider_all",i));
    h2_gr_spa[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_spider_all",i));
    h3_gr_spa[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_spider_all",i));
    hh1_gr_spa[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_spider_all",i));
    hh2_gr_spa[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_spider_all",i));
    hh3_gr_spa[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_spider_all",i));

    h1_gr_spa[k]->Add(hh1_gr_spa[k], -1);
    h2_gr_spa[k]->Add(hh2_gr_spa[k], -1);
    h3_gr_spa[k]->Add(hh3_gr_spa[k], -1);

    h1_gr_spa[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h2_gr_spa[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h3_gr_spa[k]->GetYaxis()->SetRangeUser(1., 1000000);

    h1_gr_spa[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h1_gr_spa[k]->SetTitle(Form("ge ring%d spider all",i));
    h1_gr_spa[k]->GetXaxis()->SetTitle("energy [keV]");
    h1_gr_spa[k]->GetYaxis()->SetTitle("counts");
    h1_gr_spa[k]->SetLineColor(colors[1]);
    h1_gr_spa[k]->Draw();

    h2_gr_spa[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h2_gr_spa[k]->GetXaxis()->SetTitle("energy [keV]");
    h2_gr_spa[k]->GetYaxis()->SetTitle("counts");
    h2_gr_spa[k]->SetLineColor(colors[2]);
    h2_gr_spa[k]->Draw("same");

    h3_gr_spa[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h3_gr_spa[k]->GetXaxis()->SetTitle("energy [keV]");
    h3_gr_spa[k]->GetYaxis()->SetTitle("counts");
    h3_gr_spa[k]->SetLineColor(colors[3]);
    h3_gr_spa[k]->Draw("same");

    leg1->Clear();
    leg1->AddEntry(h1_gr_spa[k], h1_gr_spa[k]->GetName());
    leg1->AddEntry(h2_gr_spa[k], h2_gr_spa[k]->GetName());
    leg1->AddEntry(h3_gr_spa[k], h3_gr_spa[k]->GetName());
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/ge_ring%d_spider_all.pdf",i));
    
    k++;
  }

  // e_raw
  leg1->Clear();
  for(int i=0;i<3;i++){
    if(i==0){
      h1_gr_spa[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_gr_spa[i]->SetTitle("e_raw, ge different ring, spider all");
      h1_gr_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      h1_gr_spa[i]->GetYaxis()->SetTitle("counts");
      h1_gr_spa[i]->Draw();
    }
    h1_gr_spa[i]->SetLineColor(colors[i]);
    h1_gr_spa[i]->Draw("same");

    leg1->AddEntry(h1_gr_spa[i], h1_gr_spa[i]->GetName());
  }
  leg1->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_raw_ge_diff_ring_spider_all.pdf");
  
  // e_dc_p
  leg1->Clear();
  for(int i=0;i<3;i++){
    if(i==0){
      h2_gr_spa[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_gr_spa[i]->SetTitle("e_dc_p, ge different ring, spider all");
      h2_gr_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      h2_gr_spa[i]->GetYaxis()->SetTitle("counts");
      h2_gr_spa[i]->Draw();
    }
    h2_gr_spa[i]->SetLineColor(colors[i]);
    h2_gr_spa[i]->Draw("same");

    leg1->AddEntry(h2_gr_spa[i], h2_gr_spa[i]->GetName());
  }
  leg1->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_dc_p_ge_diff_ring_spider_all.pdf");

  // e_dc_r
  leg1->Clear();
  for(int i=0;i<3;i++){
    if(i==0){
      h3_gr_spa[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_gr_spa[i]->SetTitle("e_dc_r, ge different ring, spider all");
      h3_gr_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      h3_gr_spa[i]->GetYaxis()->SetTitle("counts");
      h3_gr_spa[i]->Draw();
    }
    h3_gr_spa[i]->SetLineColor(colors[i]);
    h3_gr_spa[i]->Draw("same");

    leg1->AddEntry(h3_gr_spa[i], h3_gr_spa[i]->GetName());
  }
  leg1->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/e_dc_r_ge_diff_ring_spider_all.pdf");

  // ge single spider all
  TH1D *h1_grs_spa[nn_ge], *h2_grs_spa[nn_ge], *h3_grs_spa[nn_ge];
  TH1D *hh1_grs_spa[nn_ge], *hh2_grs_spa[nn_ge], *hh3_grs_spa[nn_ge];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    h1_grs_spa[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_spider_all",r,s));
    h2_grs_spa[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_spider_all",r,s));
    h3_grs_spa[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_spider_all",r,s));
    hh1_grs_spa[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_spider_all",r,s));
    hh2_grs_spa[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_spider_all",r,s));
    hh3_grs_spa[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_spider_all",r,s));

    h1_grs_spa[k]->Add(hh1_grs_spa[k], -1);
    h2_grs_spa[k]->Add(hh2_grs_spa[k], -1);
    h3_grs_spa[k]->Add(hh3_grs_spa[k], -1);

    h1_grs_spa[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h2_grs_spa[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h3_grs_spa[k]->GetYaxis()->SetRangeUser(1., 1000000);

    h1_grs_spa[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h1_grs_spa[k]->SetTitle(Form("ge ring%d sector%d, spider all",r,s));
    h1_grs_spa[k]->GetXaxis()->SetTitle("energy [keV]");
    h1_grs_spa[k]->GetYaxis()->SetTitle("counts");
    h1_grs_spa[k]->SetLineColor(colors[1]);
    h1_grs_spa[k]->Draw();

    h2_grs_spa[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h2_grs_spa[k]->GetXaxis()->SetTitle("energy [keV]");
    h2_grs_spa[k]->GetYaxis()->SetTitle("counts");
    h2_grs_spa[k]->SetLineColor(colors[2]);
    h2_grs_spa[k]->Draw("same");

    h3_grs_spa[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h3_grs_spa[k]->GetXaxis()->SetTitle("energy [keV]");
    h3_grs_spa[k]->GetYaxis()->SetTitle("counts");
    h3_grs_spa[k]->SetLineColor(colors[3]);
    h3_grs_spa[k]->Draw("same");

    leg1->Clear();
    leg1->AddEntry(h1_grs_spa[k], h1_grs_spa[k]->GetName());
    leg1->AddEntry(h2_grs_spa[k], h2_grs_spa[k]->GetName());
    leg1->AddEntry(h3_grs_spa[k], h3_grs_spa[k]->GetName());
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/ge_ring%d_sector%d_spider_all.pdf",r,s));
    
    k++;
  }

  // e_raw
  leg4->Clear();
  for(int i=0;i<nn_ge;i++){
    if(i==0){
      h1_grs_spa[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_grs_spa[i]->SetTitle("e_raw, ge different ring sector, spider all");
      h1_grs_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      h1_grs_spa[i]->GetYaxis()->SetTitle("counts");
      h1_grs_spa[i]->Draw();
    }
    h1_grs_spa[i]->SetLineColor(colors[i%24]);
    h1_grs_spa[i]->Draw("same");

    leg4->AddEntry(h1_grs_spa[i], h1_grs_spa[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_raw_ge_diff_ring_sector_spider_all.pdf");
  
  // e_dc_p
  leg4->Clear();
  for(int i=0;i<nn_ge;i++){
    if(i==0){
      h2_grs_spa[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_grs_spa[i]->SetTitle("e_dc_p, ge different ring sector, spider all");
      h2_grs_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      h2_grs_spa[i]->GetYaxis()->SetTitle("counts");
      h2_grs_spa[i]->Draw();
    }
    h2_grs_spa[i]->SetLineColor(colors[i%24]);
    h2_grs_spa[i]->Draw("same");

    leg4->AddEntry(h2_grs_spa[i], h2_grs_spa[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_dc_p_ge_diff_ring_sector_spider_all.pdf");

  // e_dc_r
  leg4->Clear();
  for(int i=0;i<nn_ge;i++){
    if(i==0){
      h3_grs_spa[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_grs_spa[i]->SetTitle("e_dc_r, ge different ring sector, spider all");
      h3_grs_spa[i]->GetXaxis()->SetTitle("energy [keV]");
      h3_grs_spa[i]->GetYaxis()->SetTitle("counts");
      h3_grs_spa[i]->Draw();
    }
    h3_grs_spa[i]->SetLineColor(colors[i%24]);
    h3_grs_spa[i]->Draw("same");

    leg4->AddEntry(h3_grs_spa[i], h3_grs_spa[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/e_dc_r_ge_diff_ring_sector_spider_all.pdf");

  ////
  ////
  ////
  // ge all spider ring
  TH1D *h1_ga_spr[8], *h2_ga_spr[8], *h3_ga_spr[8];
  TH1D *hh1_ga_spr[8], *hh2_ga_spr[8], *hh3_ga_spr[8];
  k = 0;
  for(int i=1;i<=8;i++){
    leg1->Clear();
    h1_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_spider_ring%d",i));
    h2_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_all_spider_ring%d",i));
    h3_ga_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_spider_ring%d",i));
    hh1_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_spider_ring%d",i));
    hh2_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_all_spider_ring%d",i));
    hh3_ga_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_spider_ring%d",i));

    h1_ga_spr[k]->Add(hh1_ga_spr[k], -1);
    h2_ga_spr[k]->Add(hh2_ga_spr[k], -1);
    h3_ga_spr[k]->Add(hh3_ga_spr[k], -1);

    h1_ga_spr[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h2_ga_spr[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h3_ga_spr[k]->GetYaxis()->SetRangeUser(1., 1000000);

    h1_ga_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h1_ga_spr[k]->SetTitle(Form("ge all, spider ring%d",i));
    h1_ga_spr[k]->GetXaxis()->SetTitle("energy [keV]");
    h1_ga_spr[k]->GetYaxis()->SetTitle("counts");
    h1_ga_spr[k]->SetLineColor(colors[1]);
    h1_ga_spr[k]->Draw();

    h2_ga_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h2_ga_spr[k]->GetXaxis()->SetTitle("energy [keV]");
    h2_ga_spr[k]->GetYaxis()->SetTitle("counts");
    h2_ga_spr[k]->SetLineColor(colors[2]);
    h2_ga_spr[k]->Draw("same");

    h3_ga_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h3_ga_spr[k]->GetXaxis()->SetTitle("energy [keV]");
    h3_ga_spr[k]->GetYaxis()->SetTitle("counts");
    h3_ga_spr[k]->SetLineColor(colors[3]);
    h3_ga_spr[k]->Draw("same");

    leg1->AddEntry(h1_ga_spr[k], h1_ga_spr[k]->GetName());
    leg1->AddEntry(h2_ga_spr[k], h2_ga_spr[k]->GetName());
    leg1->AddEntry(h3_ga_spr[k], h3_ga_spr[k]->GetName());
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/ge_all_spider_ring%d.pdf",i));

    k++;
  }

  // e_raw
  leg2->Clear();
  for(int i=0;i<8;i++){
    if(i==0){
      h1_ga_spr[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_ga_spr[i]->SetTitle("e_raw, ge all, spider different ring");
      h1_ga_spr[i]->GetXaxis()->SetTitle("energy [keV]");
      h1_ga_spr[i]->GetYaxis()->SetTitle("counts");
      h1_ga_spr[i]->Draw();
    }
    h1_ga_spr[i]->SetLineColor(colors[i%24]);
    h1_ga_spr[i]->Draw("same");

    leg2->AddEntry(h1_ga_spr[i], h1_ga_spr[i]->GetName());
  }
  leg2->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_raw_ge_all_spider_diff_ring.pdf");
  
  // e_dc_p
  leg2->Clear();
  for(int i=0;i<8;i++){
    if(i==0){
      h2_ga_spr[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_ga_spr[i]->SetTitle("e_dc_p, ge all, spider different ring");
      h2_ga_spr[i]->GetXaxis()->SetTitle("energy [keV]");
      h2_ga_spr[i]->GetYaxis()->SetTitle("counts");
      h2_ga_spr[i]->Draw();
    }
    h2_ga_spr[i]->SetLineColor(colors[i%24]);
    h2_ga_spr[i]->Draw("same");

    leg2->AddEntry(h2_ga_spr[i], h2_ga_spr[i]->GetName());
  }
  leg2->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_dc_p_ge_all_spider_diff_ring.pdf");

  // e_dc_r
  leg2->Clear();
  for(int i=0;i<8;i++){
    if(i==0){
      h3_ga_spr[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_ga_spr[i]->SetTitle("e_dc_r, ge all, spider different ring");
      h3_ga_spr[i]->GetXaxis()->SetTitle("energy [keV]");
      h3_ga_spr[i]->GetYaxis()->SetTitle("counts");
      h3_ga_spr[i]->Draw();
    }
    h3_ga_spr[i]->SetLineColor(colors[i%24]);
    h3_ga_spr[i]->Draw("same");

    leg2->AddEntry(h3_ga_spr[i], h3_ga_spr[i]->GetName());
  }
  leg2->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/e_dc_r_ge_all_spider_diff_ring.pdf");

  // ge ring spider ring
  TH1D *h1_gr_spr[3*8], *h2_gr_spr[3*8], *h3_gr_spr[3*8];
  TH1D *hh1_gr_spr[3*8], *hh2_gr_spr[3*8], *hh3_gr_spr[3*8];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=8;j++){//k=0-7 ge_ring=3
      h1_gr_spr[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_spider_ring%d",i,j));
      h2_gr_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_spider_ring%d",i,j));
      h3_gr_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_spider_ring%d",i,j));
      hh1_gr_spr[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_spider_ring%d",i,j));
      hh2_gr_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_spider_ring%d",i,j));
      hh3_gr_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_spider_ring%d",i,j));

      h1_gr_spr[k]->Add(hh1_gr_spr[k], -1);
      h2_gr_spr[k]->Add(hh2_gr_spr[k], -1);
      h3_gr_spr[k]->Add(hh3_gr_spr[k], -1);

      h1_gr_spr[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h2_gr_spr[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h3_gr_spr[k]->GetYaxis()->SetRangeUser(1., 1000000);

      h1_gr_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_gr_spr[k]->SetTitle(Form("ge ring%d, spider ring%d",i,j));
      h1_gr_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h1_gr_spr[k]->GetYaxis()->SetTitle("counts");
      h1_gr_spr[k]->SetLineColor(colors[1]);
      h1_gr_spr[k]->Draw();

      h2_gr_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_gr_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h2_gr_spr[k]->GetYaxis()->SetTitle("counts");
      h2_gr_spr[k]->SetLineColor(colors[2]);
      h2_gr_spr[k]->Draw("same");

      h3_gr_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_gr_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h3_gr_spr[k]->GetYaxis()->SetTitle("counts");
      h3_gr_spr[k]->SetLineColor(colors[3]);
      h3_gr_spr[k]->Draw("same");

      leg1->Clear();
      leg1->AddEntry(h1_gr_spr[k], h1_gr_spr[k]->GetName());
      leg1->AddEntry(h2_gr_spr[k], h2_gr_spr[k]->GetName());
      leg1->AddEntry(h3_gr_spr[k], h3_gr_spr[k]->GetName());
      leg1->Draw("same");
      l_334->Draw("same");
      l_406->Draw("same");
      l_439->Draw("same");
      // c->SaveAs(Form("./fig/ge_ring%d_spider_ring%d.pdf",i,j));
      
      k++;
    }
  }

  // e_raw
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg2->Clear();
    for(int j=1;j<=8;j++){
      k = 8*(i-3)+j-1;
      if(k%8==0){
        h1_gr_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_gr_spr[k]->SetTitle(Form("e_raw, ge ring%d, spider different ring",i));
        h1_gr_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_gr_spr[k]->GetYaxis()->SetTitle("counts");
        h1_gr_spr[k]->Draw();
      }
      h1_gr_spr[k]->SetLineColor(colors[j-1]);
      h1_gr_spr[k]->Draw("same");

      leg2->AddEntry(h1_gr_spr[k], h1_gr_spr[k]->GetName());
    }
    leg2->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_ring%d_spider_diff_ring.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=8;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 8*(j-3)+i-1;
      if(k<8){
        h1_gr_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_gr_spr[k]->SetTitle(Form("e_raw, ge different ring, spider ring%d",i));
        h1_gr_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_gr_spr[k]->GetYaxis()->SetTitle("counts");
        h1_gr_spr[k]->Draw();
      }
      h1_gr_spr[k]->SetLineColor(colors[j-3]);
      h1_gr_spr[k]->Draw("same");

      leg1->AddEntry(h1_gr_spr[k], h1_gr_spr[k]->GetName());
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_diff_ring_spider_ring%d.pdf",i));
  }
  
  // e_dc_p
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg2->Clear();
    for(int j=1;j<=8;j++){
      k = 8*(i-3)+j-1;
      if(k%8==0){
        h2_gr_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_gr_spr[k]->SetTitle(Form("e_dc_p, ge ring%d, spider different ring",i));
        h2_gr_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_gr_spr[k]->GetYaxis()->SetTitle("counts");
        h2_gr_spr[k]->Draw();
      }
      h2_gr_spr[k]->SetLineColor(colors[j-1]);
      h2_gr_spr[k]->Draw("same");

      leg2->AddEntry(h2_gr_spr[k], h2_gr_spr[k]->GetName());
    }
    leg2->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_ring%d_spider_diff_ring.pdf",i));
  }
  //
  k = 0;
  for(int i=1;i<=8;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){  
      k = 8*(j-3)+i-1;
      if(k<8){
        h2_gr_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_gr_spr[k]->SetTitle(Form("e_dc_p, ge different ring, spider ring%d",i));
        h2_gr_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_gr_spr[k]->GetYaxis()->SetTitle("counts");
        h2_gr_spr[k]->Draw();
      }
      h2_gr_spr[k]->SetLineColor(colors[j-3]);
      h2_gr_spr[k]->Draw("same");

      leg1->AddEntry(h2_gr_spr[k], h2_gr_spr[k]->GetName());
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_diff_ring_spider_ring%d.pdf",i));
  }

  // e_dc_r
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg2->Clear();
    for(int j=1;j<=8;j++){
      k = 8*(i-3)+j-1;
      if(k%8==0){
        h3_gr_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_gr_spr[k]->SetTitle(Form("e_dc_r, ge ring%d, spider different ring",i));
        h3_gr_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_gr_spr[k]->GetYaxis()->SetTitle("counts");
        h3_gr_spr[k]->Draw();
      }
      h3_gr_spr[k]->SetLineColor(colors[j-1]);
      h3_gr_spr[k]->Draw("same");

      leg2->AddEntry(h3_gr_spr[k], h3_gr_spr[k]->GetName());

      k++;
    }
    leg2->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_ring%d_spider_diff_ring.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=8;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 8*(j-3)+i-1;
      if(k<8){
        h3_gr_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_gr_spr[k]->SetTitle(Form("e_dc_r, ge different ring, spider ring%d",i));
        h3_gr_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_gr_spr[k]->GetYaxis()->SetTitle("counts");
        h3_gr_spr[k]->Draw();
      }
      h3_gr_spr[k]->SetLineColor(colors[j-3]);
      h3_gr_spr[k]->Draw("same");

      leg1->AddEntry(h3_gr_spr[k], h3_gr_spr[k]->GetName());
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_diff_ring_spider_ring%d.pdf",i));
  }

  // ge single spider ring
  TH1D *h1_grs_spr[nn_ge*8], *h2_grs_spr[nn_ge*8], *h3_grs_spr[nn_ge*8];
  TH1D *hh1_grs_spr[nn_ge*8], *hh2_grs_spr[nn_ge*8], *hh3_grs_spr[nn_ge*8];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    for(int i=1;i<=8;i++){
      h1_grs_spr[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      h2_grs_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      h3_grs_spr[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      hh1_grs_spr[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      hh2_grs_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_spider_ring%d",r,s,i));
      hh3_grs_spr[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_spider_ring%d",r,s,i));

      h1_grs_spr[k]->Add(hh1_grs_spr[k], -1);
      h2_grs_spr[k]->Add(hh2_grs_spr[k], -1);
      h3_grs_spr[k]->Add(hh3_grs_spr[k], -1);

      h1_grs_spr[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h2_grs_spr[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h3_grs_spr[k]->GetYaxis()->SetRangeUser(1., 1000000);

      h1_grs_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_grs_spr[k]->SetTitle(Form("ge ring%d sector%d, spider ring%d",r,s,i));
      h1_grs_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h1_grs_spr[k]->GetYaxis()->SetTitle("counts");
      h1_grs_spr[k]->SetLineColor(colors[1]);
      h1_grs_spr[k]->Draw();

      h2_grs_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_grs_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h2_grs_spr[k]->GetYaxis()->SetTitle("counts");
      h2_grs_spr[k]->SetLineColor(colors[2]);
      h2_grs_spr[k]->Draw("same");

      h3_grs_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_grs_spr[k]->GetXaxis()->SetTitle("energy [keV]");
      h3_grs_spr[k]->GetYaxis()->SetTitle("counts");
      h3_grs_spr[k]->SetLineColor(colors[3]);
      h3_grs_spr[k]->Draw("same");

      leg1->Clear();
      leg1->AddEntry(h1_grs_spr[k], h1_grs_spr[k]->GetName());
      leg1->AddEntry(h2_grs_spr[k], h2_grs_spr[k]->GetName());
      leg1->AddEntry(h3_grs_spr[k], h3_grs_spr[k]->GetName());
      leg1->Draw("same");
      l_334->Draw("same");
      l_406->Draw("same");
      l_439->Draw("same");
      // c->SaveAs(Form("./fig/ge_ring%d_sector%d_spider_ring%d.pdf",r,s,i));
      
      k++;
    }
  }

  // e_raw
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg2->Clear();
    for(int j=1;j<=8;j++){
      if(k%8==0){
        h1_grs_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_grs_spr[k]->SetTitle(Form("e_raw, ge ring%d sector%d, spider different ring",r,s));
        h1_grs_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_grs_spr[k]->GetYaxis()->SetTitle("counts");
        h1_grs_spr[k]->Draw();
      }
      h1_grs_spr[k]->SetLineColor(colors[j-1]);
      h1_grs_spr[k]->Draw("same");

      leg2->AddEntry(h1_grs_spr[k], h1_grs_spr[k]->GetName());

      k++;
    }
    leg2->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_ring%d_sector%d_spider_diff_ring.pdf",r,s));
  }
  
  //
  k = 0;
  for(int i=1;i<=8;i++){
    leg4->Clear();
    kk = 0;
    
    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
    
      k = 8*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<8){
        h1_grs_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_grs_spr[k]->SetTitle(Form("e_raw, ge different ring sector, spider ring%d",i));
        h1_grs_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_grs_spr[k]->GetYaxis()->SetTitle("counts");
        h1_grs_spr[k]->Draw();
      }
      h1_grs_spr[k]->SetLineColor(colors[kk]);
      h1_grs_spr[k]->Draw("same");

      leg4->AddEntry(h1_grs_spr[k], h1_grs_spr[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_diff_ring_sector_spider_ring%d.pdf",i));
  }
  
  // e_dc_p
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg2->Clear();
    for(int j=1;j<=8;j++){
      if(k%8==0){
        h2_grs_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_grs_spr[k]->SetTitle(Form("e_dc_p, ge ring%d sector%d, spider different ring",r,s));
        h2_grs_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_grs_spr[k]->GetYaxis()->SetTitle("counts");
        h2_grs_spr[k]->Draw();
      }
      h2_grs_spr[k]->SetLineColor(colors[j-1]);
      h2_grs_spr[k]->Draw("same");

      leg2->AddEntry(h2_grs_spr[k], h2_grs_spr[k]->GetName());

      k++;
    }
    leg2->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_ring%d_sector%d_spider_diff_ring.pdf",r,s));
  }

  //
  k = 0;
  for(int i=1;i<=8;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
    
      k = 8*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<8){
        h2_grs_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_grs_spr[k]->SetTitle(Form("e_dc_p, ge different ring sector, spider ring%d",i));
        h2_grs_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_grs_spr[k]->GetYaxis()->SetTitle("counts");
        h2_grs_spr[k]->Draw();
      }
      h2_grs_spr[k]->SetLineColor(colors[kk]);
      h2_grs_spr[k]->Draw("same");

      leg4->AddEntry(h2_grs_spr[k], h2_grs_spr[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_diff_ring_sector_spider_ring%d.pdf",i));
  }

  // e_dc_r
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg2->Clear();
    for(int j=1;j<=8;j++){
      if(k%8==0){
        h3_grs_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_grs_spr[k]->SetTitle(Form("e_dc_r, ge ring%d sector%d, spider different ring",r,s));
        h3_grs_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_grs_spr[k]->GetYaxis()->SetTitle("counts");
        h3_grs_spr[k]->Draw();
      }
      h3_grs_spr[k]->SetLineColor(colors[j-1]);
      h3_grs_spr[k]->Draw("same");

      leg2->AddEntry(h3_grs_spr[k], h3_grs_spr[k]->GetName());

      k++;
    }
    leg2->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_ring%d_sector%d_spider_diff_ring.pdf",r,s));
  }
  
  //
  k = 0;
  for(int i=1;i<=8;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
      
      k = 8*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<8){
        h3_grs_spr[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_grs_spr[k]->SetTitle(Form("e_dc_r, ge different ring sector, spider ring%d",i));
        h3_grs_spr[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_grs_spr[k]->GetYaxis()->SetTitle("counts");
        h3_grs_spr[k]->Draw();
      }
      h3_grs_spr[k]->SetLineColor(colors[kk]);
      h3_grs_spr[k]->Draw("same");

      leg4->AddEntry(h3_grs_spr[k], h3_grs_spr[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_diff_ring_sector_spider_ring%d.pdf",i));
  }

  ////
  ////
  ////
  // ge all spider sector
  TH1D *h1_ga_sps[12], *h2_ga_sps[12], *h3_ga_sps[12];
  TH1D *hh1_ga_sps[12], *hh2_ga_sps[12], *hh3_ga_sps[12];
  k = 0;
  for(int i=1;i<=12;i++){
    leg1->Clear();
    h1_ga_sps[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_spider_sector%d",i));
    h2_ga_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_all_spider_sector%d",i));
    h3_ga_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_spider_sector%d",i));
    hh1_ga_sps[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_spider_sector%d",i));
    hh2_ga_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_all_spider_sector%d",i));
    hh3_ga_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_spider_sector%d",i));

    h1_ga_sps[k]->Add(hh1_ga_sps[k], -1);
    h2_ga_sps[k]->Add(hh2_ga_sps[k], -1);
    h3_ga_sps[k]->Add(hh3_ga_sps[k], -1);

    h1_ga_sps[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h2_ga_sps[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h3_ga_sps[k]->GetYaxis()->SetRangeUser(1., 1000000);

    h1_ga_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h1_ga_sps[k]->SetTitle(Form("ge all, spider sector%d",i));
    h1_ga_sps[k]->GetXaxis()->SetTitle("energy [keV]");
    h1_ga_sps[k]->GetYaxis()->SetTitle("counts");
    h1_ga_sps[k]->SetLineColor(colors[1]);
    h1_ga_sps[k]->Draw();

    h2_ga_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h2_ga_sps[k]->GetXaxis()->SetTitle("energy [keV]");
    h2_ga_sps[k]->GetYaxis()->SetTitle("counts");
    h2_ga_sps[k]->SetLineColor(colors[2]);
    h2_ga_sps[k]->Draw("same");

    h3_ga_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h3_ga_sps[k]->GetXaxis()->SetTitle("energy [keV]");
    h3_ga_sps[k]->GetYaxis()->SetTitle("counts");
    h3_ga_sps[k]->SetLineColor(colors[3]);
    h3_ga_sps[k]->Draw("same");

    leg1->AddEntry(h1_ga_sps[k], h1_ga_sps[k]->GetName());
    leg1->AddEntry(h2_ga_sps[k], h2_ga_sps[k]->GetName());
    leg1->AddEntry(h3_ga_sps[k], h3_ga_sps[k]->GetName());
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/ge_all_spider_sector%d.pdf",i));

    k++;
  }

  // e_raw
  leg3->Clear();
  for(int i=0;i<12;i++){
    if(i==0){
      h1_ga_sps[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_ga_sps[i]->SetTitle("e_raw, ge all, spider different sector");
      h1_ga_sps[i]->GetXaxis()->SetTitle("energy [keV]");
      h1_ga_sps[i]->GetYaxis()->SetTitle("counts");
      h1_ga_sps[i]->Draw();
    }
    h1_ga_sps[i]->SetLineColor(colors[i%24]);
    h1_ga_sps[i]->Draw("same");

    leg3->AddEntry(h1_ga_sps[i], h1_ga_sps[i]->GetName());
  }
  leg3->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_raw_ge_all_spider_diff_sector.pdf");
  
  // e_dc_p
  leg3->Clear();
  for(int i=0;i<12;i++){
    if(i==0){
      h2_ga_sps[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_ga_sps[i]->SetTitle("e_dc_p, ge all, spider different sector");
      h2_ga_sps[i]->GetXaxis()->SetTitle("energy [keV]");
      h2_ga_sps[i]->GetYaxis()->SetTitle("counts");
      h2_ga_sps[i]->Draw();
    }
    h2_ga_sps[i]->SetLineColor(colors[i%24]);
    h2_ga_sps[i]->Draw("same");

    leg3->AddEntry(h2_ga_sps[i], h2_ga_sps[i]->GetName());
  }
  leg3->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_dc_p_ge_all_spider_diff_sector.pdf");

  // e_dc_r
  leg3->Clear();
  for(int i=0;i<12;i++){
    if(i==0){
      h3_ga_sps[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_ga_sps[i]->SetTitle("e_dc_r, ge all, spider different sector");
      h3_ga_sps[i]->GetXaxis()->SetTitle("energy [keV]");
      h3_ga_sps[i]->GetYaxis()->SetTitle("counts");
      h3_ga_sps[i]->Draw();
    }
    h3_ga_sps[i]->SetLineColor(colors[i%24]);
    h3_ga_sps[i]->Draw("same");

    leg3->AddEntry(h3_ga_sps[i], h3_ga_sps[i]->GetName());
  }
  leg3->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/e_dc_r_ge_all_spider_diff_sector.pdf");

  // ge ring spider sector
  TH1D *h1_gr_sps[3*12], *h2_gr_sps[3*12], *h3_gr_sps[3*12];
  TH1D *hh1_gr_sps[3*12], *hh2_gr_sps[3*12], *hh3_gr_sps[3*12];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=12;j++){
      h1_gr_sps[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_spider_sector%d",i,j));
      h2_gr_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_spider_sector%d",i,j));
      h3_gr_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_spider_sector%d",i,j));
      hh1_gr_sps[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_spider_sector%d",i,j));
      hh2_gr_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_spider_sector%d",i,j));
      hh3_gr_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_spider_sector%d",i,j));

      h1_gr_sps[k]->Add(hh1_gr_sps[k], -1);
      h2_gr_sps[k]->Add(hh2_gr_sps[k], -1);
      h3_gr_sps[k]->Add(hh3_gr_sps[k], -1);

      h1_gr_sps[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h2_gr_sps[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h3_gr_sps[k]->GetYaxis()->SetRangeUser(1., 1000000);

      h1_gr_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_gr_sps[k]->SetTitle(Form("ge ring%d, spider sector%d",i,j));
      h1_gr_sps[k]->GetXaxis()->SetTitle("energy [keV]");
      h1_gr_sps[k]->GetYaxis()->SetTitle("counts");
      h1_gr_sps[k]->SetLineColor(colors[1]);
      h1_gr_sps[k]->Draw();

      h2_gr_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_gr_sps[k]->GetXaxis()->SetTitle("energy [keV]");
      h2_gr_sps[k]->GetYaxis()->SetTitle("counts");
      h2_gr_sps[k]->SetLineColor(colors[2]);
      h2_gr_sps[k]->Draw("same");

      h3_gr_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_gr_sps[k]->GetXaxis()->SetTitle("energy [keV]");
      h3_gr_sps[k]->GetYaxis()->SetTitle("counts");
      h3_gr_sps[k]->SetLineColor(colors[3]);
      h3_gr_sps[k]->Draw("same");

      leg1->Clear();
      leg1->AddEntry(h1_gr_sps[k], h1_gr_sps[k]->GetName());
      leg1->AddEntry(h2_gr_sps[k], h2_gr_sps[k]->GetName());
      leg1->AddEntry(h3_gr_sps[k], h3_gr_sps[k]->GetName());
      leg1->Draw("same");
      l_334->Draw("same");
      l_406->Draw("same");
      l_439->Draw("same");
      // c->SaveAs(Form("./fig/ge_ring%d_spider_sector%d.pdf",i,j));
      
      k++;
    }
  }

  // e_raw
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg3->Clear();
    for(int j=1;j<=12;j++){
      k = 12*(i-3)+j-1;

      if(k%12==0){
        h1_gr_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_gr_sps[k]->SetTitle(Form("e_raw, ge ring%d, spider different sector",i));
        h1_gr_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_gr_sps[k]->GetYaxis()->SetTitle("counts");
        h1_gr_sps[k]->Draw();
      }
      h1_gr_sps[k]->SetLineColor(colors[j-1]);
      h1_gr_sps[k]->Draw("same");

      leg3->AddEntry(h1_gr_sps[k], h1_gr_sps[k]->GetName());

      k++;
    }
    leg3->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_ring%d_spider_diff_sector.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=12;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 12*(j-3)+i-1;

      if(k<12){
        h1_gr_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_gr_sps[k]->SetTitle(Form("e_raw, ge different ring, spider sector%d",i));
        h1_gr_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_gr_sps[k]->GetYaxis()->SetTitle("counts");
        h1_gr_sps[k]->Draw();
      }
      h1_gr_sps[k]->SetLineColor(colors[j-3]);
      h1_gr_sps[k]->Draw("same");

      leg1->AddEntry(h1_gr_sps[k], h1_gr_sps[k]->GetName());

      k++;
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_diff_ring_spider_sector%d.pdf",i));
  }
  
  // e_dc_p
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg3->Clear();
    for(int j=1;j<=12;j++){
      k = 12*(i-3)+j-1;

      if(k%12==0){
        h2_gr_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_gr_sps[k]->SetTitle(Form("e_dc_p, ge ring%d, spider different sector",i));
        h2_gr_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_gr_sps[k]->GetYaxis()->SetTitle("counts");
        h2_gr_sps[k]->Draw();
      }
      h2_gr_sps[k]->SetLineColor(colors[j-1]);
      h2_gr_sps[k]->Draw("same");

      leg3->AddEntry(h2_gr_sps[k], h2_gr_sps[k]->GetName());

      k++;
    }
    leg3->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_ring%d_spider_diff_sector.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=12;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 12*(j-3)+i-1;

      if(k<12){
        h2_gr_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_gr_sps[k]->SetTitle(Form("e_dc_p, ge different ring, spider sector%d",i));
        h2_gr_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_gr_sps[k]->GetYaxis()->SetTitle("counts");
        h2_gr_sps[k]->Draw();
      }
      h2_gr_sps[k]->SetLineColor(colors[j-3]);
      h2_gr_sps[k]->Draw("same");

      leg1->AddEntry(h2_gr_sps[k], h2_gr_sps[k]->GetName());

      k++;
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_diff_ring_spider_sector%d.pdf",i));
  }

  // e_dc_r
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg3->Clear();
    for(int j=1;j<=12;j++){
      k = 12*(i-3)+j-1;

      if(k%12==0){
        h3_gr_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_gr_sps[k]->SetTitle(Form("e_dc_r, ge ring%d, spider different sector",i));
        h3_gr_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_gr_sps[k]->GetYaxis()->SetTitle("counts");
        h3_gr_sps[k]->Draw();
      }
      h3_gr_sps[k]->SetLineColor(colors[j-1]);
      h3_gr_sps[k]->Draw("same");

      leg3->AddEntry(h3_gr_sps[k], h3_gr_sps[k]->GetName());

      k++;
    }
    leg3->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_ring%d_spider_diff_sector.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=12;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 12*(j-3)+i-1;

      if(k<12){
        h3_gr_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_gr_sps[k]->SetTitle(Form("e_dc_r, ge different ring, spider sector%d",i));
        h3_gr_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_gr_sps[k]->GetYaxis()->SetTitle("counts");
        h3_gr_sps[k]->Draw();
      }
      h3_gr_sps[k]->SetLineColor(colors[j-3]);
      h3_gr_sps[k]->Draw("same");

      leg1->AddEntry(h3_gr_sps[k], h3_gr_sps[k]->GetName());

      k++;
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_diff_ring_spider_sector%d.pdf",i));
  }

  // ge single spider sector
  TH1D *h1_grs_sps[nn_ge*12], *h2_grs_sps[nn_ge*12], *h3_grs_sps[nn_ge*12];
  TH1D *hh1_grs_sps[nn_ge*12], *hh2_grs_sps[nn_ge*12], *hh3_grs_sps[nn_ge*12];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    for(int i=1;i<=12;i++){
      h1_grs_sps[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      h2_grs_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      h3_grs_sps[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      hh1_grs_sps[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      hh2_grs_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_spider_sector%d",r,s,i));
      hh3_grs_sps[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_spider_sector%d",r,s,i));

      h1_grs_sps[k]->Add(hh1_grs_sps[k], -1);
      h2_grs_sps[k]->Add(hh2_grs_sps[k], -1);
      h3_grs_sps[k]->Add(hh3_grs_sps[k], -1);

      h1_grs_sps[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h2_grs_sps[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h3_grs_sps[k]->GetYaxis()->SetRangeUser(1., 1000000);

      h1_grs_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_grs_sps[k]->SetTitle(Form("ge ring%d sector%d, spider sector%d",r,s,i));
      h1_grs_sps[k]->GetXaxis()->SetTitle("energy [keV]");
      h1_grs_sps[k]->GetYaxis()->SetTitle("counts");
      h1_grs_sps[k]->SetLineColor(colors[1]);
      h1_grs_sps[k]->Draw();

      h2_grs_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_grs_sps[k]->GetXaxis()->SetTitle("energy [keV]");
      h2_grs_sps[k]->GetYaxis()->SetTitle("counts");
      h2_grs_sps[k]->SetLineColor(colors[2]);
      h2_grs_sps[k]->Draw("same");

      h3_grs_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_grs_sps[k]->GetXaxis()->SetTitle("energy [keV]");
      h3_grs_sps[k]->GetYaxis()->SetTitle("counts");
      h3_grs_sps[k]->SetLineColor(colors[3]);
      h3_grs_sps[k]->Draw("same");

      leg1->Clear();
      leg1->AddEntry(h1_grs_sps[k], h1_grs_sps[k]->GetName());
      leg1->AddEntry(h2_grs_sps[k], h2_grs_sps[k]->GetName());
      leg1->AddEntry(h3_grs_sps[k], h3_grs_sps[k]->GetName());
      leg1->Draw("same");
      l_334->Draw("same");
      l_406->Draw("same");
      l_439->Draw("same");
      // c->SaveAs(Form("./fig/ge_ring%d_sector%d_spider_sector%d.pdf",r,s,i));
      
      k++;
    }
  }

  // e_raw
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg3->Clear();
    for(int j=1;j<=12;j++){
      if(k%12==0){
        h1_grs_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_grs_sps[k]->SetTitle(Form("e_raw, ge ring%d sector%d, spider different sector",r,s));
        h1_grs_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_grs_sps[k]->GetYaxis()->SetTitle("counts");
        h1_grs_sps[k]->Draw();
      }
      h1_grs_sps[k]->SetLineColor(colors[j-1]);
      h1_grs_sps[k]->Draw("same");

      leg3->AddEntry(h1_grs_sps[k], h1_grs_sps[k]->GetName());

      k++;
    }
    leg3->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_ring%d_sector%d_spider_diff_sector.pdf",r,s));
  }
  
  //
  k = 0;
  for(int i=1;i<=12;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
      
      k = 12*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<12){
        h1_grs_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_grs_sps[k]->SetTitle(Form("e_raw, ge different ring sector, spider sector%d",i));
        h1_grs_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_grs_sps[k]->GetYaxis()->SetTitle("counts");
        h1_grs_sps[k]->Draw();
      }
      h1_grs_sps[k]->SetLineColor(colors[kk]);
      h1_grs_sps[k]->Draw("same");

      leg4->AddEntry(h1_grs_sps[k], h1_grs_sps[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_diff_ring_sector_spider_sector%d.pdf",i));
  }
  
  // e_dc_p
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg3->Clear();
    for(int j=1;j<=12;j++){
      if(k%12==0){
        h2_grs_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_grs_sps[k]->SetTitle(Form("e_dc_p, ge ring%d sector%d, spider different sector",r,s));
        h2_grs_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_grs_sps[k]->GetYaxis()->SetTitle("counts");
        h2_grs_sps[k]->Draw();
      }
      h2_grs_sps[k]->SetLineColor(colors[j-1]);
      h2_grs_sps[k]->Draw("same");

      leg3->AddEntry(h2_grs_sps[k], h2_grs_sps[k]->GetName());

      k++;
    }
    leg3->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_ring%d_sector%d_spider_diff_sector.pdf",r,s));
  }
  
  //
  k = 0;
  for(int i=1;i<=12;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
    
      r = key.first;
      s = key.second;

      k = 12*kk+i-1;
      if(k<12){
        h2_grs_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_grs_sps[k]->SetTitle(Form("e_dc_p, ge different ring sector, spider sector%d",i));
        h2_grs_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_grs_sps[k]->GetYaxis()->SetTitle("counts");
        h2_grs_sps[k]->Draw();
      }
      h2_grs_sps[k]->SetLineColor(colors[kk]);
      h2_grs_sps[k]->Draw("same");

      leg4->AddEntry(h2_grs_sps[k], h2_grs_sps[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_diff_ring_sector_spider_sector%d.pdf",i));
  }

  // e_dc_r
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg3->Clear();
    for(int j=1;j<=12;j++){
      if(k%12==0){
        h3_grs_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_grs_sps[k]->SetTitle(Form("e_dc_r, ge ring%d sector%d, spider different sector",r,s));
        h3_grs_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_grs_sps[k]->GetYaxis()->SetTitle("counts");
        h3_grs_sps[k]->Draw();
      }
      h3_grs_sps[k]->SetLineColor(colors[j-1]);
      h3_grs_sps[k]->Draw("same");

      leg3->AddEntry(h3_grs_sps[k], h3_grs_sps[k]->GetName());

      k++;
    }
    leg3->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_ring%d_sector%d_spider_diff_sector.pdf",r,s));
  }
  
  //
  k = 0;
  for(int i=1;i<=12;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
    
      k = 12*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<12){
        h3_grs_sps[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_grs_sps[k]->SetTitle(Form("e_dc_r, ge different ring sector, spider sector%d",i));
        h3_grs_sps[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_grs_sps[k]->GetYaxis()->SetTitle("counts");
        h3_grs_sps[k]->Draw();
      }
      h3_grs_sps[k]->SetLineColor(colors[kk]);
      h3_grs_sps[k]->Draw("same");

      leg4->AddEntry(h3_grs_sps[k], h3_grs_sps[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_diff_ring_sector_spider_sector%d.pdf",i));
  }

  leg1->Clear();
  leg2->Clear();
  leg3->Clear();
  leg4->Clear();

  ////
  ////
  ////
  // ge all s3 all
  TH1D *h1_ga_s3a, *h2_ga_s3a, *h3_ga_s3a;
  TH1D *hh1_ga_s3a, *hh2_ga_s3a, *hh3_ga_s3a;

  h1_ga_s3a = (TH1D*)fi->Get("event_e_raw_ge_all_s3_all");
  h2_ga_s3a = (TH1D*)fi->Get("event_e_dc_p_ge_all_s3_all");
  h3_ga_s3a = (TH1D*)fi->Get("event_e_dc_r_ge_all_s3_all");
  hh1_ga_s3a = (TH1D*)fi->Get("bg_e_raw_ge_all_s3_all");
  hh2_ga_s3a = (TH1D*)fi->Get("bg_e_dc_p_ge_all_s3_all");
  hh3_ga_s3a = (TH1D*)fi->Get("bg_e_dc_r_ge_all_s3_all");

  h1_ga_s3a->Add(hh1_ga_s3a, -1);
  h2_ga_s3a->Add(hh2_ga_s3a, -1);
  h3_ga_s3a->Add(hh3_ga_s3a, -1);

  h1_ga_s3a->GetYaxis()->SetRangeUser(1., 1000000000);
  h2_ga_s3a->GetYaxis()->SetRangeUser(1., 1000000000);
  h3_ga_s3a->GetYaxis()->SetRangeUser(1., 1000000000);

  h1_ga_s3a->GetXaxis()->SetRangeUser(x_min, x_max);
  h1_ga_s3a->SetTitle("ge all, s3 all");
  h1_ga_s3a->GetXaxis()->SetTitle("energy [keV]");
  h1_ga_s3a->GetYaxis()->SetTitle("counts");
  h1_ga_s3a->SetLineColor(colors[1]);
  h1_ga_s3a->Draw();

  h2_ga_s3a->GetXaxis()->SetRangeUser(x_min, x_max);
  h2_ga_s3a->GetXaxis()->SetTitle("energy [keV]");
  h2_ga_s3a->GetYaxis()->SetTitle("counts");
  h2_ga_s3a->SetLineColor(colors[2]);
  h2_ga_s3a->Draw("same");

  h3_ga_s3a->GetXaxis()->SetRangeUser(x_min, x_max);
  h3_ga_s3a->GetXaxis()->SetTitle("energy [keV]");
  h3_ga_s3a->GetYaxis()->SetTitle("counts");
  h3_ga_s3a->SetLineColor(colors[3]);
  h3_ga_s3a->Draw("same");

  leg1->AddEntry(h1_ga_s3a, h1_ga_s3a->GetName());
  leg1->AddEntry(h2_ga_s3a, h2_ga_s3a->GetName());
  leg1->AddEntry(h3_ga_s3a, h3_ga_s3a->GetName());
  leg1->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/ge_all_s3_all.pdf");
  
  // ge ring s3 all
  TH1D *h1_gr_s3a[3], *h2_gr_s3a[3], *h3_gr_s3a[3];
  TH1D *hh1_gr_s3a[3], *hh2_gr_s3a[3], *hh3_gr_s3a[3];
  k = 0;
  for(int i=3;i<=5;i++){
    h1_gr_s3a[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_s3_all",i));
    h2_gr_s3a[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_s3_all",i));
    h3_gr_s3a[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_s3_all",i));
    hh1_gr_s3a[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_s3_all",i));
    hh2_gr_s3a[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_s3_all",i));
    hh3_gr_s3a[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_s3_all",i));

    h1_gr_s3a[k]->Add(hh1_gr_s3a[k], -1);
    h2_gr_s3a[k]->Add(hh2_gr_s3a[k], -1);
    h3_gr_s3a[k]->Add(hh3_gr_s3a[k], -1);

    h1_gr_s3a[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h2_gr_s3a[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h3_gr_s3a[k]->GetYaxis()->SetRangeUser(1., 1000000);

    h1_gr_s3a[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h1_gr_s3a[k]->SetTitle(Form("ge ring%d s3 all",i));
    h1_gr_s3a[k]->GetXaxis()->SetTitle("energy [keV]");
    h1_gr_s3a[k]->GetYaxis()->SetTitle("counts");
    h1_gr_s3a[k]->SetLineColor(colors[1]);
    h1_gr_s3a[k]->Draw();

    h2_gr_s3a[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h2_gr_s3a[k]->GetXaxis()->SetTitle("energy [keV]");
    h2_gr_s3a[k]->GetYaxis()->SetTitle("counts");
    h2_gr_s3a[k]->SetLineColor(colors[2]);
    h2_gr_s3a[k]->Draw("same");

    h3_gr_s3a[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h3_gr_s3a[k]->GetXaxis()->SetTitle("energy [keV]");
    h3_gr_s3a[k]->GetYaxis()->SetTitle("counts");
    h3_gr_s3a[k]->SetLineColor(colors[3]);
    h3_gr_s3a[k]->Draw("same");

    leg1->Clear();
    leg1->AddEntry(h1_gr_s3a[k], h1_gr_s3a[k]->GetName());
    leg1->AddEntry(h2_gr_s3a[k], h2_gr_s3a[k]->GetName());
    leg1->AddEntry(h3_gr_s3a[k], h3_gr_s3a[k]->GetName());
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/ge_ring%d_s3_all.pdf",i));
    
    k++;
  }

  // e_raw
  leg1->Clear();
  for(int i=0;i<3;i++){
    if(i==0){
      h1_gr_s3a[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_gr_s3a[i]->SetTitle("e_raw, ge different ring, s3 all");
      h1_gr_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      h1_gr_s3a[i]->GetYaxis()->SetTitle("counts");
      h1_gr_s3a[i]->Draw();
    }
    h1_gr_s3a[i]->SetLineColor(colors[i]);
    h1_gr_s3a[i]->Draw("same");

    leg1->AddEntry(h1_gr_s3a[i], h1_gr_s3a[i]->GetName());
  }
  leg1->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_raw_ge_diff_ring_s3_all.pdf");
  
  // e_dc_p
  leg1->Clear();
  for(int i=0;i<3;i++){
    if(i==0){
      h2_gr_s3a[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_gr_s3a[i]->SetTitle("e_dc_p, ge different ring, s3 all");
      h2_gr_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      h2_gr_s3a[i]->GetYaxis()->SetTitle("counts");
      h2_gr_s3a[i]->Draw();
    }
    h2_gr_s3a[i]->SetLineColor(colors[i]);
    h2_gr_s3a[i]->Draw("same");

    leg1->AddEntry(h2_gr_s3a[i], h2_gr_s3a[i]->GetName());
  }
  leg1->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_dc_p_ge_diff_ring_s3_all.pdf");

  // e_dc_r
  leg1->Clear();
  for(int i=0;i<3;i++){
    if(i==0){
      h3_gr_s3a[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_gr_s3a[i]->SetTitle("e_dc_r, ge different ring, s3 all");
      h3_gr_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      h3_gr_s3a[i]->GetYaxis()->SetTitle("counts");
      h3_gr_s3a[i]->Draw();
    }
    h3_gr_s3a[i]->SetLineColor(colors[i]);
    h3_gr_s3a[i]->Draw("same");

    leg1->AddEntry(h3_gr_s3a[i], h3_gr_s3a[i]->GetName());
  }
  leg1->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/e_dc_r_ge_diff_ring_s3_all.pdf");

  // ge single s3 all
  TH1D *h1_grs_s3a[nn_ge], *h2_grs_s3a[nn_ge], *h3_grs_s3a[nn_ge];
  TH1D *hh1_grs_s3a[nn_ge], *hh2_grs_s3a[nn_ge], *hh3_grs_s3a[nn_ge];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    h1_grs_s3a[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_s3_all",r,s));
    h2_grs_s3a[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_s3_all",r,s));
    h3_grs_s3a[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_s3_all",r,s));
    hh1_grs_s3a[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_s3_all",r,s));
    hh2_grs_s3a[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_s3_all",r,s));
    hh3_grs_s3a[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_s3_all",r,s));

    h1_grs_s3a[k]->Add(hh1_grs_s3a[k], -1);
    h2_grs_s3a[k]->Add(hh2_grs_s3a[k], -1);
    h3_grs_s3a[k]->Add(hh3_grs_s3a[k], -1);

    h1_grs_s3a[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h2_grs_s3a[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h3_grs_s3a[k]->GetYaxis()->SetRangeUser(1., 1000000);

    h1_grs_s3a[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h1_grs_s3a[k]->SetTitle(Form("ge ring%d sector%d, s3 all",r,s));
    h1_grs_s3a[k]->GetXaxis()->SetTitle("energy [keV]");
    h1_grs_s3a[k]->GetYaxis()->SetTitle("counts");
    h1_grs_s3a[k]->SetLineColor(colors[1]);
    h1_grs_s3a[k]->Draw();

    h2_grs_s3a[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h2_grs_s3a[k]->GetXaxis()->SetTitle("energy [keV]");
    h2_grs_s3a[k]->GetYaxis()->SetTitle("counts");
    h2_grs_s3a[k]->SetLineColor(colors[2]);
    h2_grs_s3a[k]->Draw("same");

    h3_grs_s3a[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h3_grs_s3a[k]->GetXaxis()->SetTitle("energy [keV]");
    h3_grs_s3a[k]->GetYaxis()->SetTitle("counts");
    h3_grs_s3a[k]->SetLineColor(colors[3]);
    h3_grs_s3a[k]->Draw("same");

    leg1->Clear();
    leg1->AddEntry(h1_grs_s3a[k], h1_grs_s3a[k]->GetName());
    leg1->AddEntry(h2_grs_s3a[k], h2_grs_s3a[k]->GetName());
    leg1->AddEntry(h3_grs_s3a[k], h3_grs_s3a[k]->GetName());
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/ge_ring%d_sector%d_s3_all.pdf",r,s));
    
    k++;
  }

  // e_raw
  leg4->Clear();
  for(int i=0;i<nn_ge;i++){
    if(i==0){
      h1_grs_s3a[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_grs_s3a[i]->SetTitle("e_raw, ge different ring sector, s3 all");
      h1_grs_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      h1_grs_s3a[i]->GetYaxis()->SetTitle("counts");
      h1_grs_s3a[i]->Draw();
    }
    h1_grs_s3a[i]->SetLineColor(colors[i%24]);
    h1_grs_s3a[i]->Draw("same");

    leg4->AddEntry(h1_grs_s3a[i], h1_grs_s3a[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_raw_ge_diff_ring_sector_s3_all.pdf");
  
  // e_dc_p
  leg4->Clear();
  for(int i=0;i<nn_ge;i++){
    if(i==0){
      h2_grs_s3a[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_grs_s3a[i]->SetTitle("e_dc_p, ge different ring sector, s3 all");
      h2_grs_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      h2_grs_s3a[i]->GetYaxis()->SetTitle("counts");
      h2_grs_s3a[i]->Draw();
    }
    h2_grs_s3a[i]->SetLineColor(colors[i%24]);
    h2_grs_s3a[i]->Draw("same");

    leg4->AddEntry(h2_grs_s3a[i], h2_grs_s3a[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_dc_p_ge_diff_ring_sector_s3_all.pdf");

  // e_dc_r
  leg4->Clear();
  for(int i=0;i<nn_ge;i++){
    if(i==0){
      h3_grs_s3a[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_grs_s3a[i]->SetTitle("e_dc_r, ge different ring sector, s3 all");
      h3_grs_s3a[i]->GetXaxis()->SetTitle("energy [keV]");
      h3_grs_s3a[i]->GetYaxis()->SetTitle("counts");
      h3_grs_s3a[i]->Draw();
    }
    h3_grs_s3a[i]->SetLineColor(colors[i%24]);
    h3_grs_s3a[i]->Draw("same");

    leg4->AddEntry(h3_grs_s3a[i], h3_grs_s3a[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/e_dc_r_ge_diff_ring_sector_s3_all.pdf");

  ////
  ////
  ////
  // ge all s3 ring
  TH1D *h1_ga_s3r[24], *h2_ga_s3r[24], *h3_ga_s3r[24];
  TH1D *hh1_ga_s3r[24], *hh2_ga_s3r[24], *hh3_ga_s3r[24];
  k = 0;
  for(int i=1;i<=24;i++){
    leg1->Clear();
    h1_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_s3_ring%d",i));
    h2_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_all_s3_ring%d",i));
    h3_ga_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_s3_ring%d",i));
    hh1_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_s3_ring%d",i));
    hh2_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_all_s3_ring%d",i));
    hh3_ga_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_s3_ring%d",i));

    h1_ga_s3r[k]->Add(hh1_ga_s3r[k], -1);
    h2_ga_s3r[k]->Add(hh2_ga_s3r[k], -1);
    h3_ga_s3r[k]->Add(hh3_ga_s3r[k], -1);

    h1_ga_s3r[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h2_ga_s3r[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h3_ga_s3r[k]->GetYaxis()->SetRangeUser(1., 1000000);

    h1_ga_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h1_ga_s3r[k]->SetTitle(Form("ge all, s3 ring%d",i));
    h1_ga_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
    h1_ga_s3r[k]->GetYaxis()->SetTitle("counts");
    h1_ga_s3r[k]->SetLineColor(colors[1]);
    h1_ga_s3r[k]->Draw();

    h2_ga_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h2_ga_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
    h2_ga_s3r[k]->GetYaxis()->SetTitle("counts");
    h2_ga_s3r[k]->SetLineColor(colors[2]);
    h2_ga_s3r[k]->Draw("same");

    h3_ga_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h3_ga_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
    h3_ga_s3r[k]->GetYaxis()->SetTitle("counts");
    h3_ga_s3r[k]->SetLineColor(colors[3]);
    h3_ga_s3r[k]->Draw("same");

    leg1->AddEntry(h1_ga_s3r[k], h1_ga_s3r[k]->GetName());
    leg1->AddEntry(h2_ga_s3r[k], h2_ga_s3r[k]->GetName());
    leg1->AddEntry(h3_ga_s3r[k], h3_ga_s3r[k]->GetName());
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/ge_all_s3_ring%d.pdf",i));

    k++;
  }

  // e_raw
  leg4->Clear();
  for(int i=0;i<24;i++){
    if(i==0){
      h1_ga_s3r[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_ga_s3r[i]->SetTitle("e_raw, ge all, s3 different ring");
      h1_ga_s3r[i]->GetXaxis()->SetTitle("energy [keV]");
      h1_ga_s3r[i]->GetYaxis()->SetTitle("counts");
      h1_ga_s3r[i]->Draw();
    }
    h1_ga_s3r[i]->SetLineColor(colors[i%24]);
    h1_ga_s3r[i]->Draw("same");

    leg4->AddEntry(h1_ga_s3r[i], h1_ga_s3r[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_raw_ge_all_s3_diff_ring.pdf");
  
  // e_dc_p
  leg4->Clear();
  for(int i=0;i<24;i++){
    if(i==0){
      h2_ga_s3r[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_ga_s3r[i]->SetTitle("e_dc_p, ge all, s3 different ring");
      h2_ga_s3r[i]->GetXaxis()->SetTitle("energy [keV]");
      h2_ga_s3r[i]->GetYaxis()->SetTitle("counts");
      h2_ga_s3r[i]->Draw();
    }
    h2_ga_s3r[i]->SetLineColor(colors[i%24]);
    h2_ga_s3r[i]->Draw("same");

    leg4->AddEntry(h2_ga_s3r[i], h2_ga_s3r[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_dc_p_ge_all_s3_diff_ring.pdf");

  // e_dc_r
  leg4->Clear();
  for(int i=0;i<24;i++){
    if(i==0){
      h3_ga_s3r[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_ga_s3r[i]->SetTitle("e_dc_r, ge all, s3 different ring");
      h3_ga_s3r[i]->GetXaxis()->SetTitle("energy [keV]");
      h3_ga_s3r[i]->GetYaxis()->SetTitle("counts");
      h3_ga_s3r[i]->Draw();
    }
    h3_ga_s3r[i]->SetLineColor(colors[i%24]);
    h3_ga_s3r[i]->Draw("same");

    leg4->AddEntry(h3_ga_s3r[i], h3_ga_s3r[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/e_dc_r_ge_all_s3_diff_ring.pdf");

  // ge ring s3 ring
  TH1D *h1_gr_s3r[3*24], *h2_gr_s3r[3*24], *h3_gr_s3r[3*24];
  TH1D *hh1_gr_s3r[3*24], *hh2_gr_s3r[3*24], *hh3_gr_s3r[3*24];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=24;j++){//k=0-23 ge_ring=3
      h1_gr_s3r[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_s3_ring%d",i,j));
      h2_gr_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_s3_ring%d",i,j));
      h3_gr_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_s3_ring%d",i,j));
      hh1_gr_s3r[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_s3_ring%d",i,j));
      hh2_gr_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_s3_ring%d",i,j));
      hh3_gr_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_s3_ring%d",i,j));

      h1_gr_s3r[k]->Add(hh1_gr_s3r[k], -1);
      h2_gr_s3r[k]->Add(hh2_gr_s3r[k], -1);
      h3_gr_s3r[k]->Add(hh3_gr_s3r[k], -1);

      h1_gr_s3r[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h2_gr_s3r[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h3_gr_s3r[k]->GetYaxis()->SetRangeUser(1., 1000000);

      h1_gr_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_gr_s3r[k]->SetTitle(Form("ge ring%d, s3 ring%d",i,j));
      h1_gr_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h1_gr_s3r[k]->GetYaxis()->SetTitle("counts");
      h1_gr_s3r[k]->SetLineColor(colors[1]);
      h1_gr_s3r[k]->Draw();

      h2_gr_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_gr_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h2_gr_s3r[k]->GetYaxis()->SetTitle("counts");
      h2_gr_s3r[k]->SetLineColor(colors[2]);
      h2_gr_s3r[k]->Draw("same");

      h3_gr_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_gr_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h3_gr_s3r[k]->GetYaxis()->SetTitle("counts");
      h3_gr_s3r[k]->SetLineColor(colors[3]);
      h3_gr_s3r[k]->Draw("same");

      leg1->Clear();
      leg1->AddEntry(h1_gr_s3r[k], h1_gr_s3r[k]->GetName());
      leg1->AddEntry(h2_gr_s3r[k], h2_gr_s3r[k]->GetName());
      leg1->AddEntry(h3_gr_s3r[k], h3_gr_s3r[k]->GetName());
      leg1->Draw("same");
      l_334->Draw("same");
      l_406->Draw("same");
      l_439->Draw("same");
      // c->SaveAs(Form("./fig/ge_ring%d_s3_ring%d.pdf",i,j));
      
      k++;
    }
  }

  // e_raw
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg4->Clear();
    for(int j=1;j<=24;j++){
      k = 24*(i-3)+j-1;
      if(k%24==0){
        h1_gr_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_gr_s3r[k]->SetTitle(Form("e_raw, ge ring%d, s3 different ring",i));
        h1_gr_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_gr_s3r[k]->GetYaxis()->SetTitle("counts");
        h1_gr_s3r[k]->Draw();
      }
      h1_gr_s3r[k]->SetLineColor(colors[k%24]);
      h1_gr_s3r[k]->Draw("same");

      leg4->AddEntry(h1_gr_s3r[k], h1_gr_s3r[k]->GetName());
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_ring%d_s3_diff_ring.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=24;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 24*(j-3)+i-1;
      if(k<24){
        h1_gr_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_gr_s3r[k]->SetTitle(Form("e_raw, ge different ring, s3 ring%d",i));
        h1_gr_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_gr_s3r[k]->GetYaxis()->SetTitle("counts");
        h1_gr_s3r[k]->Draw();
      }
      h1_gr_s3r[k]->SetLineColor(colors[j-3]);
      h1_gr_s3r[k]->Draw("same");

      leg1->AddEntry(h1_gr_s3r[k], h1_gr_s3r[k]->GetName());
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_diff_ring_s3_ring%d.pdf",i));
  }
  
  // e_dc_p
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg4->Clear();
    for(int j=1;j<=24;j++){
      k = 24*(i-3)+j-1;
      if(k%24==0){
        h2_gr_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_gr_s3r[k]->SetTitle(Form("e_dc_p, ge ring%d, s3 different ring",i));
        h2_gr_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_gr_s3r[k]->GetYaxis()->SetTitle("counts");
        h2_gr_s3r[k]->Draw();
      }
      h2_gr_s3r[k]->SetLineColor(colors[k%24]);
      h2_gr_s3r[k]->Draw("same");

      leg4->AddEntry(h2_gr_s3r[k], h2_gr_s3r[k]->GetName());
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_ring%d_s3_diff_ring.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=24;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){  
      k = 24*(j-3)+i-1;
      if(k<24){
        h2_gr_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_gr_s3r[k]->SetTitle(Form("e_dc_p, ge different ring, s3 ring%d",i));
        h2_gr_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_gr_s3r[k]->GetYaxis()->SetTitle("counts");
        h2_gr_s3r[k]->Draw();
      }
      h2_gr_s3r[k]->SetLineColor(colors[j-3]);
      h2_gr_s3r[k]->Draw("same");

      leg1->AddEntry(h2_gr_s3r[k], h2_gr_s3r[k]->GetName());
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_diff_ring_s3_ring%d.pdf",i));
  }

  // e_dc_r
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg4->Clear();
    for(int j=1;j<=24;j++){
      k = 24*(i-3)+j-1;
      if(k%24==0){
        h3_gr_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_gr_s3r[k]->SetTitle(Form("e_dc_r, ge ring%d, s3 different ring",i));
        h3_gr_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_gr_s3r[k]->GetYaxis()->SetTitle("counts");
        h3_gr_s3r[k]->Draw();
      }
      h3_gr_s3r[k]->SetLineColor(colors[k%24]);
      h3_gr_s3r[k]->Draw("same");

      leg4->AddEntry(h3_gr_s3r[k], h3_gr_s3r[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_ring%d_s3_diff_ring.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=24;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 24*(j-3)+i-1;
      if(k<24){
        h3_gr_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_gr_s3r[k]->SetTitle(Form("e_dc_r, ge different ring, s3 ring%d",i));
        h3_gr_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_gr_s3r[k]->GetYaxis()->SetTitle("counts");
        h3_gr_s3r[k]->Draw();
      }
      h3_gr_s3r[k]->SetLineColor(colors[j-3]);
      h3_gr_s3r[k]->Draw("same");

      leg1->AddEntry(h3_gr_s3r[k], h3_gr_s3r[k]->GetName());
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_diff_ring_s3_ring%d.pdf",i));
  }

  // ge single s3 ring
  TH1D *h1_grs_s3r[nn_ge*24], *h2_grs_s3r[nn_ge*24], *h3_grs_s3r[nn_ge*24];
  TH1D *hh1_grs_s3r[nn_ge*24], *hh2_grs_s3r[nn_ge*24], *hh3_grs_s3r[nn_ge*24];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    for(int i=1;i<=24;i++){
      h1_grs_s3r[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      h2_grs_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      h3_grs_s3r[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      hh1_grs_s3r[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      hh2_grs_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_s3_ring%d",r,s,i));
      hh3_grs_s3r[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_s3_ring%d",r,s,i));

      h1_grs_s3r[k]->Add(hh1_grs_s3r[k], -1);
      h2_grs_s3r[k]->Add(hh2_grs_s3r[k], -1);
      h3_grs_s3r[k]->Add(hh3_grs_s3r[k], -1);

      h1_grs_s3r[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h2_grs_s3r[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h3_grs_s3r[k]->GetYaxis()->SetRangeUser(1., 1000000);

      h1_grs_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_grs_s3r[k]->SetTitle(Form("ge ring%d sector%d, s3 ring%d",r,s,i));
      h1_grs_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h1_grs_s3r[k]->GetYaxis()->SetTitle("counts");
      h1_grs_s3r[k]->SetLineColor(colors[1]);
      h1_grs_s3r[k]->Draw();

      h2_grs_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_grs_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h2_grs_s3r[k]->GetYaxis()->SetTitle("counts");
      h2_grs_s3r[k]->SetLineColor(colors[2]);
      h2_grs_s3r[k]->Draw("same");

      h3_grs_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_grs_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
      h3_grs_s3r[k]->GetYaxis()->SetTitle("counts");
      h3_grs_s3r[k]->SetLineColor(colors[3]);
      h3_grs_s3r[k]->Draw("same");

      leg1->Clear();
      leg1->AddEntry(h1_grs_s3r[k], h1_grs_s3r[k]->GetName());
      leg1->AddEntry(h2_grs_s3r[k], h2_grs_s3r[k]->GetName());
      leg1->AddEntry(h3_grs_s3r[k], h3_grs_s3r[k]->GetName());
      leg1->Draw("same");
      l_334->Draw("same");
      l_406->Draw("same");
      l_439->Draw("same");
      // c->SaveAs(Form("./fig/ge_ring%d_sector%d_s3_ring%d.pdf",r,s,i));
      
      k++;
    }
  }

  // e_raw
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg4->Clear();
    for(int j=1;j<=24;j++){
      if(k%24==0){
        h1_grs_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_grs_s3r[k]->SetTitle(Form("e_raw, ge ring%d sector%d, s3 different ring",r,s));
        h1_grs_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_grs_s3r[k]->GetYaxis()->SetTitle("counts");
        h1_grs_s3r[k]->Draw();
      }
      h1_grs_s3r[k]->SetLineColor(colors[k%24]);
      h1_grs_s3r[k]->Draw("same");

      leg4->AddEntry(h1_grs_s3r[k], h1_grs_s3r[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_ring%d_sector%d_s3_diff_ring.pdf",r,s));
  }
  
  //
  k = 0;
  for(int i=1;i<=24;i++){
    leg4->Clear();
    kk = 0;
    
    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
    
      k = 24*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<24){
        h1_grs_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_grs_s3r[k]->SetTitle(Form("e_raw, ge different ring sector, s3 ring%d",i));
        h1_grs_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_grs_s3r[k]->GetYaxis()->SetTitle("counts");
        h1_grs_s3r[k]->Draw();
      }
      h1_grs_s3r[k]->SetLineColor(colors[kk]);
      h1_grs_s3r[k]->Draw("same");

      leg4->AddEntry(h1_grs_s3r[k], h1_grs_s3r[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_diff_ring_sector_s3_ring%d.pdf",i));
  }
  
  // e_dc_p
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg4->Clear();
    for(int j=1;j<=24;j++){
      if(k%24==0){
        h2_grs_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_grs_s3r[k]->SetTitle(Form("e_dc_p, ge ring%d sector%d, s3 different ring",r,s));
        h2_grs_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_grs_s3r[k]->GetYaxis()->SetTitle("counts");
        h2_grs_s3r[k]->Draw();
      }
      h2_grs_s3r[k]->SetLineColor(colors[k%24]);
      h2_grs_s3r[k]->Draw("same");

      leg4->AddEntry(h2_grs_s3r[k], h2_grs_s3r[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_ring%d_sector%d_s3_diff_ring.pdf",r,s));
  }

  //
  k = 0;
  for(int i=1;i<=24;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
    
      k = 24*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<24){
        h2_grs_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_grs_s3r[k]->SetTitle(Form("e_dc_p, ge different ring sector, s3 ring%d",i));
        h2_grs_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_grs_s3r[k]->GetYaxis()->SetTitle("counts");
        h2_grs_s3r[k]->Draw();
      }
      h2_grs_s3r[k]->SetLineColor(colors[k%24]);
      h2_grs_s3r[k]->Draw("same");

      leg4->AddEntry(h2_grs_s3r[k], h2_grs_s3r[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_diff_ring_sector_s3_ring%d.pdf",i));
  }

  // e_dc_r
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg4->Clear();
    for(int j=1;j<=24;j++){
      if(k%24==0){
        h3_grs_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_grs_s3r[k]->SetTitle(Form("e_dc_r, ge ring%d sector%d, s3 different ring",r,s));
        h3_grs_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_grs_s3r[k]->GetYaxis()->SetTitle("counts");
        h3_grs_s3r[k]->Draw();
      }
      h3_grs_s3r[k]->SetLineColor(colors[k%24]);
      h3_grs_s3r[k]->Draw("same");

      leg4->AddEntry(h3_grs_s3r[k], h3_grs_s3r[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_ring%d_sector%d_s3_diff_ring.pdf",r,s));
  }
  
  //
  k = 0;
  for(int i=1;i<=24;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
      
      k = 24*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<24){
        h3_grs_s3r[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_grs_s3r[k]->SetTitle(Form("e_dc_r, ge different ring sector, s3 ring%d",i));
        h3_grs_s3r[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_grs_s3r[k]->GetYaxis()->SetTitle("counts");
        h3_grs_s3r[k]->Draw();
      }
      h3_grs_s3r[k]->SetLineColor(colors[kk]);
      h3_grs_s3r[k]->Draw("same");

      leg4->AddEntry(h3_grs_s3r[k], h3_grs_s3r[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_diff_ring_sector_s3_ring%d.pdf",i));
  }

  ////
  ////
  ////
  // ge all s3 sector
  TH1D *h1_ga_s3s[32], *h2_ga_s3s[32], *h3_ga_s3s[32];
  TH1D *hh1_ga_s3s[32], *hh2_ga_s3s[32], *hh3_ga_s3s[32];
  k = 0;
  for(int i=1;i<=32;i++){
    leg1->Clear();
    h1_ga_s3s[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_all_s3_sector%d",i));
    h2_ga_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_all_s3_sector%d",i));
    h3_ga_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_all_s3_sector%d",i));
    hh1_ga_s3s[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_all_s3_sector%d",i));
    hh2_ga_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_all_s3_sector%d",i));
    hh3_ga_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_all_s3_sector%d",i));

    h1_ga_s3s[k]->Add(hh1_ga_s3s[k], -1);
    h2_ga_s3s[k]->Add(hh2_ga_s3s[k], -1);
    h3_ga_s3s[k]->Add(hh3_ga_s3s[k], -1);

    h1_ga_s3s[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h2_ga_s3s[k]->GetYaxis()->SetRangeUser(1., 1000000);
    h3_ga_s3s[k]->GetYaxis()->SetRangeUser(1., 1000000);

    h1_ga_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h1_ga_s3s[k]->SetTitle(Form("ge all, s3 sector%d",i));
    h1_ga_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
    h1_ga_s3s[k]->GetYaxis()->SetTitle("counts");
    h1_ga_s3s[k]->SetLineColor(colors[1]);
    h1_ga_s3s[k]->Draw();

    h2_ga_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h2_ga_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
    h2_ga_s3s[k]->GetYaxis()->SetTitle("counts");
    h2_ga_s3s[k]->SetLineColor(colors[2]);
    h2_ga_s3s[k]->Draw("same");

    h3_ga_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
    h3_ga_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
    h3_ga_s3s[k]->GetYaxis()->SetTitle("counts");
    h3_ga_s3s[k]->SetLineColor(colors[3]);
    h3_ga_s3s[k]->Draw("same");

    leg1->AddEntry(h1_ga_s3s[k], h1_ga_s3s[k]->GetName());
    leg1->AddEntry(h2_ga_s3s[k], h2_ga_s3s[k]->GetName());
    leg1->AddEntry(h3_ga_s3s[k], h3_ga_s3s[k]->GetName());
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/ge_all_s3_sector%d.pdf",i));

    k++;
  }

  // e_raw
  leg4->Clear();
  for(int i=0;i<32;i++){
    if(i==0){
      h1_ga_s3s[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_ga_s3s[i]->SetTitle("e_raw, ge all, s3 different sector");
      h1_ga_s3s[i]->GetXaxis()->SetTitle("energy [keV]");
      h1_ga_s3s[i]->GetYaxis()->SetTitle("counts");
      h1_ga_s3s[i]->Draw();
    }
    h1_ga_s3s[i]->SetLineColor(colors[i%24]);
    h1_ga_s3s[i]->Draw("same");

    leg4->AddEntry(h1_ga_s3s[i], h1_ga_s3s[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_raw_ge_all_s3_diff_sector.pdf");
  
  // e_dc_p
  leg4->Clear();
  for(int i=0;i<32;i++){
    if(i==0){
      h2_ga_s3s[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_ga_s3s[i]->SetTitle("e_dc_p, ge all, s3 different sector");
      h2_ga_s3s[i]->GetXaxis()->SetTitle("energy [keV]");
      h2_ga_s3s[i]->GetYaxis()->SetTitle("counts");
      h2_ga_s3s[i]->Draw();
    }
    h2_ga_s3s[i]->SetLineColor(colors[i%24]);
    h2_ga_s3s[i]->Draw("same");

    leg4->AddEntry(h2_ga_s3s[i], h2_ga_s3s[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  // c->SaveAs("./fig/e_dc_p_ge_all_s3_diff_sector.pdf");

  // e_dc_r
  leg4->Clear();
  for(int i=0;i<32;i++){
    if(i==0){
      h3_ga_s3s[i]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_ga_s3s[i]->SetTitle("e_dc_r, ge all, s3 different sector");
      h3_ga_s3s[i]->GetXaxis()->SetTitle("energy [keV]");
      h3_ga_s3s[i]->GetYaxis()->SetTitle("counts");
      h3_ga_s3s[i]->Draw();
    }
    h3_ga_s3s[i]->SetLineColor(colors[i%24]);
    h3_ga_s3s[i]->Draw("same");

    leg4->AddEntry(h3_ga_s3s[i], h3_ga_s3s[i]->GetName());
  }
  leg4->Draw("same");
  l_334->Draw("same");
  l_406->Draw("same");
  l_439->Draw("same");
  c->SaveAs("./fig/e_dc_r_ge_all_s3_diff_sector.pdf");

  // ge ring s3 sector
  TH1D *h1_gr_s3s[3*32], *h2_gr_s3s[3*32], *h3_gr_s3s[3*32];
  TH1D *hh1_gr_s3s[3*32], *hh2_gr_s3s[3*32], *hh3_gr_s3s[3*32];
  k = 0;
  for(int i=3;i<=5;i++){
    for(int j=1;j<=32;j++){
      h1_gr_s3s[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_s3_sector%d",i,j));
      h2_gr_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_s3_sector%d",i,j));
      h3_gr_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_s3_sector%d",i,j));
      hh1_gr_s3s[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_s3_sector%d",i,j));
      hh2_gr_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_s3_sector%d",i,j));
      hh3_gr_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_s3_sector%d",i,j));

      h1_gr_s3s[k]->Add(hh1_gr_s3s[k], -1);
      h2_gr_s3s[k]->Add(hh2_gr_s3s[k], -1);
      h3_gr_s3s[k]->Add(hh3_gr_s3s[k], -1);

      h1_gr_s3s[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h2_gr_s3s[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h3_gr_s3s[k]->GetYaxis()->SetRangeUser(1., 1000000);

      h1_gr_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_gr_s3s[k]->SetTitle(Form("ge ring%d, s3 sector%d",i,j));
      h1_gr_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
      h1_gr_s3s[k]->GetYaxis()->SetTitle("counts");
      h1_gr_s3s[k]->SetLineColor(colors[1]);
      h1_gr_s3s[k]->Draw();

      h2_gr_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_gr_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
      h2_gr_s3s[k]->GetYaxis()->SetTitle("counts");
      h2_gr_s3s[k]->SetLineColor(colors[2]);
      h2_gr_s3s[k]->Draw("same");

      h3_gr_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_gr_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
      h3_gr_s3s[k]->GetYaxis()->SetTitle("counts");
      h3_gr_s3s[k]->SetLineColor(colors[3]);
      h3_gr_s3s[k]->Draw("same");

      leg1->Clear();
      leg1->AddEntry(h1_gr_s3s[k], h1_gr_s3s[k]->GetName());
      leg1->AddEntry(h2_gr_s3s[k], h2_gr_s3s[k]->GetName());
      leg1->AddEntry(h3_gr_s3s[k], h3_gr_s3s[k]->GetName());
      leg1->Draw("same");
      l_334->Draw("same");
      l_406->Draw("same");
      l_439->Draw("same");
      // c->SaveAs(Form("./fig/ge_ring%d_s3_sector%d.pdf",i,j));
      
      k++;
    }
  }

  // e_raw
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg4->Clear();
    for(int j=1;j<=32;j++){
      k = 32*(i-3)+j-1;

      if(k%32==0){
        h1_gr_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_gr_s3s[k]->SetTitle(Form("e_raw, ge ring%d, s3 different sector",i));
        h1_gr_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_gr_s3s[k]->GetYaxis()->SetTitle("counts");
        h1_gr_s3s[k]->Draw();
      }
      h1_gr_s3s[k]->SetLineColor(colors[j%24]);
      h1_gr_s3s[k]->Draw("same");

      leg4->AddEntry(h1_gr_s3s[k], h1_gr_s3s[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_ring%d_s3_diff_sector.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=32;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 32*(j-3)+i-1;

      if(k<32){
        h1_gr_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_gr_s3s[k]->SetTitle(Form("e_raw, ge different ring, s3 sector%d",i));
        h1_gr_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_gr_s3s[k]->GetYaxis()->SetTitle("counts");
        h1_gr_s3s[k]->Draw();
      }
      h1_gr_s3s[k]->SetLineColor(colors[j-3]);
      h1_gr_s3s[k]->Draw("same");

      leg1->AddEntry(h1_gr_s3s[k], h1_gr_s3s[k]->GetName());

      k++;
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_diff_ring_s3_sector%d.pdf",i));
  }
  
  // e_dc_p
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg4->Clear();
    for(int j=1;j<=32;j++){
      k = 32*(i-3)+j-1;

      if(k%32==0){
        h2_gr_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_gr_s3s[k]->SetTitle(Form("e_dc_p, ge ring%d, s3 different sector",i));
        h2_gr_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_gr_s3s[k]->GetYaxis()->SetTitle("counts");
        h2_gr_s3s[k]->Draw();
      }
      h2_gr_s3s[k]->SetLineColor(colors[j%24]);
      h2_gr_s3s[k]->Draw("same");

      leg4->AddEntry(h2_gr_s3s[k], h2_gr_s3s[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_ring%d_s3_diff_sector.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=32;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 32*(j-3)+i-1;

      if(k<32){
        h2_gr_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_gr_s3s[k]->SetTitle(Form("e_dc_p, ge different ring, s3 sector%d",i));
        h2_gr_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_gr_s3s[k]->GetYaxis()->SetTitle("counts");
        h2_gr_s3s[k]->Draw();
      }
      h2_gr_s3s[k]->SetLineColor(colors[j-3]);
      h2_gr_s3s[k]->Draw("same");

      leg1->AddEntry(h2_gr_s3s[k], h2_gr_s3s[k]->GetName());

      k++;
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_diff_ring_s3_sector%d.pdf",i));
  }

  // e_dc_r
  //
  k = 0;
  for(int i=3;i<=5;i++){
    leg4->Clear();
    for(int j=1;j<=32;j++){
      k = 32*(i-3)+j-1;

      if(k%32==0){
        h3_gr_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_gr_s3s[k]->SetTitle(Form("e_dc_r, ge ring%d, s3 different sector",i));
        h3_gr_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_gr_s3s[k]->GetYaxis()->SetTitle("counts");
        h3_gr_s3s[k]->Draw();
      }
      h3_gr_s3s[k]->SetLineColor(colors[j%24]);
      h3_gr_s3s[k]->Draw("same");

      leg4->AddEntry(h3_gr_s3s[k], h3_gr_s3s[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_ring%d_s3_diff_sector.pdf",i));
  }
  
  //
  k = 0;
  for(int i=1;i<=32;i++){
    leg1->Clear();
    for(int j=3;j<=5;j++){
      k = 32*(j-3)+i-1;

      if(k<32){
        h3_gr_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_gr_s3s[k]->SetTitle(Form("e_dc_r, ge different ring, s3 sector%d",i));
        h3_gr_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_gr_s3s[k]->GetYaxis()->SetTitle("counts");
        h3_gr_s3s[k]->Draw();
      }
      h3_gr_s3s[k]->SetLineColor(colors[j-3]);
      h3_gr_s3s[k]->Draw("same");

      leg1->AddEntry(h3_gr_s3s[k], h3_gr_s3s[k]->GetName());

      k++;
    }
    leg1->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_diff_ring_s3_sector%d.pdf",i));
  }

  // ge single s3 sector
  TH1D *h1_grs_s3s[nn_ge*32], *h2_grs_s3s[nn_ge*32], *h3_grs_s3s[nn_ge*32];
  TH1D *hh1_grs_s3s[nn_ge*32], *hh2_grs_s3s[nn_ge*32], *hh3_grs_s3s[nn_ge*32];
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;

    r = key.first;
    s = key.second;
    for(int i=1;i<=32;i++){
      h1_grs_s3s[k] = (TH1D*)fi->Get(Form("event_e_raw_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      h2_grs_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_p_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      h3_grs_s3s[k] = (TH1D*)fi->Get(Form("event_e_dc_r_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      hh1_grs_s3s[k] = (TH1D*)fi->Get(Form("bg_e_raw_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      hh2_grs_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_p_ge_ring%d_sector%d_s3_sector%d",r,s,i));
      hh3_grs_s3s[k] = (TH1D*)fi->Get(Form("bg_e_dc_r_ge_ring%d_sector%d_s3_sector%d",r,s,i));

      h1_grs_s3s[k]->Add(hh1_grs_s3s[k], -1);
      h2_grs_s3s[k]->Add(hh2_grs_s3s[k], -1);
      h3_grs_s3s[k]->Add(hh3_grs_s3s[k], -1);

      h1_grs_s3s[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h2_grs_s3s[k]->GetYaxis()->SetRangeUser(1., 1000000);
      h3_grs_s3s[k]->GetYaxis()->SetRangeUser(1., 1000000);

      h1_grs_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h1_grs_s3s[k]->SetTitle(Form("ge ring%d sector%d, s3 sector%d",r,s,i));
      h1_grs_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
      h1_grs_s3s[k]->GetYaxis()->SetTitle("counts");
      h1_grs_s3s[k]->SetLineColor(colors[1]);
      h1_grs_s3s[k]->Draw();

      h2_grs_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h2_grs_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
      h2_grs_s3s[k]->GetYaxis()->SetTitle("counts");
      h2_grs_s3s[k]->SetLineColor(colors[2]);
      h2_grs_s3s[k]->Draw("same");

      h3_grs_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
      h3_grs_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
      h3_grs_s3s[k]->GetYaxis()->SetTitle("counts");
      h3_grs_s3s[k]->SetLineColor(colors[3]);
      h3_grs_s3s[k]->Draw("same");

      leg1->Clear();
      leg1->AddEntry(h1_grs_s3s[k], h1_grs_s3s[k]->GetName());
      leg1->AddEntry(h2_grs_s3s[k], h2_grs_s3s[k]->GetName());
      leg1->AddEntry(h3_grs_s3s[k], h3_grs_s3s[k]->GetName());
      leg1->Draw("same");
      l_334->Draw("same");
      l_406->Draw("same");
      l_439->Draw("same");
      // c->SaveAs(Form("./fig/ge_ring%d_sector%d_s3_sector%d.pdf",r,s,i));
      
      k++;
    }
  }

  // e_raw
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg4->Clear();
    for(int j=1;j<=32;j++){
      if(k%32==0){
        h1_grs_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_grs_s3s[k]->SetTitle(Form("e_raw, ge ring%d sector%d, s3 different sector",r,s));
        h1_grs_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_grs_s3s[k]->GetYaxis()->SetTitle("counts");
        h1_grs_s3s[k]->Draw();
      }
      h1_grs_s3s[k]->SetLineColor(colors[j%24]);
      h1_grs_s3s[k]->Draw("same");

      leg4->AddEntry(h1_grs_s3s[k], h1_grs_s3s[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_ring%d_sector%d_s3_diff_sector.pdf",r,s));
  }
  
  //
  k = 0;
  for(int i=1;i<=32;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
      
      k = 32*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<32){
        h1_grs_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h1_grs_s3s[k]->SetTitle(Form("e_raw, ge different ring sector, s3 sector%d",i));
        h1_grs_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h1_grs_s3s[k]->GetYaxis()->SetTitle("counts");
        h1_grs_s3s[k]->Draw();
      }
      h1_grs_s3s[k]->SetLineColor(colors[kk]);
      h1_grs_s3s[k]->Draw("same");

      leg4->AddEntry(h1_grs_s3s[k], h1_grs_s3s[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_raw_ge_diff_ring_sector_s3_sector%d.pdf",i));
  }
  
  // e_dc_p
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg4->Clear();
    for(int j=1;j<=32;j++){
      if(k%32==0){
        h2_grs_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_grs_s3s[k]->SetTitle(Form("e_dc_p, ge ring%d sector%d, s3 different sector",r,s));
        h2_grs_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_grs_s3s[k]->GetYaxis()->SetTitle("counts");
        h2_grs_s3s[k]->Draw();
      }
      h2_grs_s3s[k]->SetLineColor(colors[j%24]);
      h2_grs_s3s[k]->Draw("same");

      leg4->AddEntry(h2_grs_s3s[k], h2_grs_s3s[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    //c->SaveAs(Form("./fig/e_dc_p_ge_ring%d_sector%d_s3_diff_sector.pdf",r,s));
  }
  
  //
  k = 0;
  for(int i=1;i<=32;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
    
      r = key.first;
      s = key.second;

      k = 32*kk+i-1;
      if(k<32){
        h2_grs_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h2_grs_s3s[k]->SetTitle(Form("e_dc_p, ge different ring sector, s3 sector%d",i));
        h2_grs_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h2_grs_s3s[k]->GetYaxis()->SetTitle("counts");
        h2_grs_s3s[k]->Draw();
      }
      h2_grs_s3s[k]->SetLineColor(colors[kk]);
      h2_grs_s3s[k]->Draw("same");

      leg4->AddEntry(h2_grs_s3s[k], h2_grs_s3s[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    // c->SaveAs(Form("./fig/e_dc_p_ge_diff_ring_sector_s3_sector%d.pdf",i));
  }

  // e_dc_r
  //
  k = 0;
  for(auto &[key,val] : m_ge_rs_good){
    if(!val) continue;
    
    r = key.first;
    s = key.second;

    leg4->Clear();
    for(int j=1;j<=32;j++){
      if(k%32==0){
        h3_grs_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_grs_s3s[k]->SetTitle(Form("e_dc_r, ge ring%d sector%d, s3 different sector",r,s));
        h3_grs_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_grs_s3s[k]->GetYaxis()->SetTitle("counts");
        h3_grs_s3s[k]->Draw();
      }
      h3_grs_s3s[k]->SetLineColor(colors[j%24]);
      h3_grs_s3s[k]->Draw("same");

      leg4->AddEntry(h3_grs_s3s[k], h3_grs_s3s[k]->GetName());

      k++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_ring%d_sector%d_s3_diff_sector.pdf",r,s));
  }

  //
  k = 0;
  for(int i=1;i<=32;i++){
    leg4->Clear();
    kk = 0;

    for(auto &[key,val] : m_ge_rs_good){
      if(!val) continue;
    
      k = 32*kk+i-1;
      r = key.first;
      s = key.second;

      if(k<32){
        h3_grs_s3s[k]->GetXaxis()->SetRangeUser(x_min, x_max);
        h3_grs_s3s[k]->SetTitle(Form("e_dc_r, ge different ring sector, s3 sector%d",i));
        h3_grs_s3s[k]->GetXaxis()->SetTitle("energy [keV]");
        h3_grs_s3s[k]->GetYaxis()->SetTitle("counts");
        h3_grs_s3s[k]->Draw();
      }
      h3_grs_s3s[k]->SetLineColor(colors[kk]);
      h3_grs_s3s[k]->Draw("same");

      leg4->AddEntry(h3_grs_s3s[k], h3_grs_s3s[k]->GetName());

      kk++;
    }
    leg4->Draw("same");
    l_334->Draw("same");
    l_406->Draw("same");
    l_439->Draw("same");
    c->SaveAs(Form("./fig/e_dc_r_ge_diff_ring_sector_s3_sector%d.pdf",i));
  }
}
