#include <sys/stat.h>
#include <sys/types.h>

//
void draw_sample_config(int run, int win, int jump)
{
  // gROOT->SetBatch(1);

  mkdir(TString::Format("./fig/%04d", run).Data(), 0777);

  int colors[12] = {
    TColor::GetColor("#E41A1C"), TColor::GetColor("#377EB8"),
    TColor::GetColor("#4DAF4A"), TColor::GetColor("#984EA3"),
    TColor::GetColor("#FF7F00"), TColor::GetColor("#A65628"),
    TColor::GetColor("#F781BF"), TColor::GetColor("#999999"),
    TColor::GetColor("#66C2A5"), TColor::GetColor("#FC8D62"),
    TColor::GetColor("#8DA0CB"), TColor::GetColor("#FFD92F")
  };

  vector<string> v_config = {
    "a",
    "b",
    "c",
    "d",
    "e"
  };

  int nn = v_config.size();
  cout << "nn " << nn << endl;

  //
  TFile *fi[12];
  for(size_t i=0;i<nn;++i){
    fi[i] = TFile::Open(TString::Format("../rootfile/data%04d_doppler_%dns_jump_%dns_%s_hist.root",run,win,jump,v_config[i].c_str()));

    if(fi[i]->IsZombie()){
      cout << "cannot open the file." << std::endl;
      return;
    }
  }

  //
  TCanvas *c1 = new TCanvas("c1","",0,0,900,360);
  c1->cd();
  c1->SetLogy();
  TCanvas *c2 = new TCanvas("c2","",0,0,900,360);
  c2->cd();
  c2->SetLogy();
  TCanvas *c3 = new TCanvas("c3","",0,0,900,360);
  c3->cd();
  c3->SetLogy();

  TLegend *leg1 = new TLegend(0.7, 0.6, 0.98, 0.95);
  TLegend *leg2 = new TLegend(0.7, 0.6, 0.98, 0.95);
  TLegend *leg3 = new TLegend(0.7, 0.6, 0.98, 0.95);
  TH1D *h1[12], *h2[12], *h3[12];
  TH1D *hh1[12], *hh2[12], *hh3[12];
  for(int i=0;i<nn;i++){
    h1[i] = (TH1D*)fi[i]->Get("event_e_raw_spider_p");
    h2[i] = (TH1D*)fi[i]->Get("event_e_dc_p_1000fs_spider_p");
    h3[i] = (TH1D*)fi[i]->Get("event_e_dc_r_1000fs_spider_p");
    hh1[i] = (TH1D*)fi[i]->Get("bg_e_raw_spider_p");
    hh2[i] = (TH1D*)fi[i]->Get("bg_e_dc_p_1000fs_spider_p");
    hh3[i] = (TH1D*)fi[i]->Get("bg_e_dc_r_1000fs_spider_p");

    h1[i]->Add(hh1[i], -1);
    h2[i]->Add(hh2[i], -1);
    h3[i]->Add(hh3[i], -1);
  }

  //
  c1->cd();
  for(int i=0;i<nn;i++){
    h1[i]->GetXaxis()->SetRangeUser(0, 1024);
    h1[i]->SetMaximum(h1[i]->GetMaximum()*100);
    h1[i]->SetLineColor(colors[i]);
    if(i==0) h1[i]->Draw();
    else h1[i]->Draw("same");

    leg1->AddEntry(h1[i], TString::Format("config_%s",v_config[i].c_str()));
  }
  leg1->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_e_raw.png", run, win, jump).Data());
  
  //
  c2->cd();
  for(int i=0;i<nn;i++){
    h2[i]->GetXaxis()->SetRangeUser(0, 1024);
    h2[i]->SetMaximum(h2[i]->GetMaximum()*100);
    h2[i]->SetLineColor(colors[i]);
    if(i==0) h2[i]->Draw();
    else h2[i]->Draw("same");

    leg2->AddEntry(h2[i], TString::Format("config%s",v_config[i].c_str()));
  }
  leg2->Draw("same");
  c2->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_e_dc_p_1000fs.png", run, win, jump).Data());

  //
  c3->cd();
  for(int i=0;i<nn;i++){
    h3[i]->GetXaxis()->SetRangeUser(0, 1024);
    h3[i]->SetMaximum(h3[i]->GetMaximum()*100);
    h3[i]->SetLineColor(colors[i]);
    if(i==0) h3[i]->Draw();
    else h3[i]->Draw("same");

    leg3->AddEntry(h3[i], TString::Format("config_%s",v_config[i].c_str()));
  }
  leg3->Draw("same");
  c3->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_e_dc_r_1000fs.png", run, win, jump).Data());

  ///
  TCanvas *c4 = new TCanvas("c4","",0,0,900,360);
  c4->cd();
  c4->SetLogy();

  TLegend *leg4 = new TLegend(0.7, 0.6, 0.98, 0.95);
  for(int i=0;i<nn;++i){
    leg4->Clear();

    h2[i]->GetXaxis()->SetRangeUser(0, 1024);
    h2[i]->SetLineColor(colors[0]);
    h2[i]->Draw();
    leg4->AddEntry(h2[i], h2[i]->GetName());

    h1[i]->SetLineColor(colors[1]);
    h1[i]->Draw("same");
    leg4->AddEntry(h1[i], h1[i]->GetName());
    leg4->Draw("same");
    c4->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_%s_e_raw_e_dc_p_1000fs.png", run, win, jump, v_config[i].c_str()).Data());
  }

  ///
  TCanvas *c5 = new TCanvas("c5","",0,0,900,360);
  c5->cd();
  c5->SetLogy();

  TLegend *leg5 = new TLegend(0.7, 0.6, 0.98, 0.95);
  for(int i=0;i<nn;++i){
    leg5->Clear();

    h3[i]->GetXaxis()->SetRangeUser(0, 1024);
    h3[i]->SetLineColor(colors[0]);
    h3[i]->Draw();
    leg5->AddEntry(h3[i], h3[i]->GetName());

    h1[i]->SetLineColor(colors[1]);
    h1[i]->Draw("same");
    leg5->AddEntry(h1[i], h1[i]->GetName());
    leg5->Draw("same");
    c5->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_%s_e_raw_e_dc_r_1000fs.png", run, win, jump, v_config[i].c_str()).Data());
  }

  ///
  TLegend *leg9 = new TLegend(0.7, 0.4, 0.98, 0.95);
  TLegend *leg10 = new TLegend(0.7, 0.4, 0.98, 0.95);
  TLegend *leg11 = new TLegend(0.7, 0.4, 0.98, 0.95);

  TCanvas *c9 = new TCanvas("c9","",0,0,900,360);
  c9->SetLogy();
  TCanvas *c10 = new TCanvas("c10","",0,0,900,360);
  c10->SetLogy();
  TCanvas *c11 = new TCanvas("c11","",0,0,900,360);
  c11->SetLogy();

  TH1D *h9[8], *h10[8], *h11[8];
  TH1D *hh9[8], *hh10[8], *hh11[8];
  
  for(int i=0;i<nn;++i){
    leg9->Clear();
    leg10->Clear();
    leg11->Clear();

    for(int j=0;j<8;++j){
      h9[j] = (TH1D*)fi[i]->Get(TString::Format("event_e_raw_spider_p_spider_ring%d",j+1));
      h10[j] = (TH1D*)fi[i]->Get(TString::Format("event_e_dc_p_1000fs_spider_p_spider_ring%d",j+1));
      h11[j] = (TH1D*)fi[i]->Get(TString::Format("event_e_dc_r_1000fs_spider_p_spider_ring%d",j+1));
      hh9[j] = (TH1D*)fi[i]->Get(TString::Format("bg_e_raw_spider_p_spider_ring%d",j+1));
      hh10[j] = (TH1D*)fi[i]->Get(TString::Format("bg_e_dc_p_1000fs_spider_p_spider_ring%d",j+1));
      hh11[j] = (TH1D*)fi[i]->Get(TString::Format("bg_e_dc_r_1000fs_spider_p_spider_ring%d",j+1));

      h9[j]->Add(hh9[j], -1);
      h10[j]->Add(hh10[j], -1);
      h11[j]->Add(hh11[j], -1);

      c9->cd();
      h9[j]->GetXaxis()->SetRangeUser(0, 1024);
      h9[j]->SetMaximum(h9[j]->GetMaximum()*100);
      h9[j]->SetLineColor(colors[j]);
      if(j==0) h9[j]->Draw();
      else h9[j]->Draw("same");
      leg9->AddEntry(h9[j], h9[j]->GetName());

      c10->cd();
      h10[j]->GetXaxis()->SetRangeUser(0, 1024);
      h10[j]->SetMaximum(h10[j]->GetMaximum()*100);
      h10[j]->SetLineColor(colors[j]);
      if(j==0) h10[j]->Draw();
      else h10[j]->Draw("same");
      leg10->AddEntry(h10[j], h10[j]->GetName());

      c11->cd();
      h11[j]->GetXaxis()->SetRangeUser(0, 1024);
      h11[j]->SetMaximum(h11[j]->GetMaximum()*100);
      h11[j]->SetLineColor(colors[j]);
      if(j==0) h11[j]->Draw();
      else h11[j]->Draw("same");
      leg11->AddEntry(h11[j], h11[j]->GetName());
    }

    c9->cd();
    leg9->Draw("same");
    c9->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_%s_e_raw_diff_spider_ring.png", run, win, jump, v_config[i].c_str()).Data());

    c10->cd();
    leg10->Draw("same");
    c10->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_%s_e_dc_p_diff_spider_ring.png", run, win, jump, v_config[i].c_str()).Data());

    c11->cd();
    leg11->Draw("same");
    c11->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_%s_e_dc_r_diff_spider_ring.png", run, win, jump, v_config[i].c_str()).Data());
  }

  ///
  TLegend *leg12 = new TLegend(0.7, 0.2, 0.98, 0.95);
  TLegend *leg13 = new TLegend(0.7, 0.2, 0.98, 0.95);
  TLegend *leg14 = new TLegend(0.7, 0.2, 0.98, 0.95);

  TCanvas *c12 = new TCanvas("c12","",0,0,900,360);
  c12->SetLogy();
  TCanvas *c13 = new TCanvas("c13","",0,0,900,360);
  c13->SetLogy();
  TCanvas *c14 = new TCanvas("c14","",0,0,900,360);
  c14->SetLogy();

  TH1D *h12[12], *h13[12], *h14[12];
  TH1D *hh12[12], *hh13[12], *hh14[12];

  for(int i=0;i<nn;++i){
    leg12->Clear();
    leg13->Clear();
    leg14->Clear();

    for(int j=0;j<12;++j){
      h12[j] = (TH1D*)fi[i]->Get(TString::Format("event_e_raw_spider_p_spider_sector%d",j+1));
      h13[j] = (TH1D*)fi[i]->Get(TString::Format("event_e_dc_p_1000fs_spider_p_spider_sector%d",j+1));
      h14[j] = (TH1D*)fi[i]->Get(TString::Format("event_e_dc_r_1000fs_spider_p_spider_sector%d",j+1));
      hh12[j] = (TH1D*)fi[i]->Get(TString::Format("bg_e_raw_spider_p_spider_sector%d",j+1));
      hh13[j] = (TH1D*)fi[i]->Get(TString::Format("bg_e_dc_p_1000fs_spider_p_spider_sector%d",j+1));
      hh14[j] = (TH1D*)fi[i]->Get(TString::Format("bg_e_dc_r_1000fs_spider_p_spider_sector%d",j+1));

      h12[j]->Add(hh12[j], -1);
      h13[j]->Add(hh13[j], -1);
      h14[j]->Add(hh14[j], -1);
  
      c12->cd();
      h12[j]->GetXaxis()->SetRangeUser(0, 1024);
      h12[j]->SetMaximum(h12[j]->GetMaximum()*100);
      h12[j]->SetLineColor(colors[j]);
      if(j==0) h12[j]->Draw();
      else h12[j]->Draw("same");
      leg12->AddEntry(h12[j], h12[j]->GetName());

      c13->cd();
      h13[j]->GetXaxis()->SetRangeUser(0, 1024);
      h13[j]->SetMaximum(h13[j]->GetMaximum()*100);
      h13[j]->SetLineColor(colors[j]);
      if(j==0) h13[j]->Draw();
      else h13[j]->Draw("same");
      leg13->AddEntry(h13[j], h13[j]->GetName());

      c14->cd();
      h14[j]->GetXaxis()->SetRangeUser(0, 1024);
      h14[j]->SetMaximum(h14[j]->GetMaximum()*100);
      h14[j]->SetLineColor(colors[j]);
      if(j==0) h14[j]->Draw();
      else h14[j]->Draw("same");
      leg14->AddEntry(h14[j], h14[j]->GetName());
    }

    c12->cd();
    leg12->Draw("same");
    c12->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_%s_e_raw_diff_spider_sector.png", run, win, jump, v_config[i].c_str()).Data());

    c13->cd();
    leg13->Draw("same");
    c13->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_%s_e_dc_p_1000fs_diff_spider_sector.png", run, win, jump, v_config[i].c_str()).Data());

    c14->cd();
    leg14->Draw("same");
    c14->SaveAs(TString::Format("./fig/%04d/%dns_%dns_config_%s_e_raw_1000fs_diff_spider_sector.png", run, win, jump, v_config[i].c_str()).Data());
  }

}
