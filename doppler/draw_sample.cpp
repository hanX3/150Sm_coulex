#include <sys/stat.h>
#include <sys/types.h>

//
void draw_sample(int run, TString str="")
{
  gROOT->SetBatch(1);

  mkdir(TString::Format("./fig/%04d", run).Data(), 0777);

  int colors[12] = {
    TColor::GetColor("#E41A1C"), TColor::GetColor("#377EB8"),
    TColor::GetColor("#4DAF4A"), TColor::GetColor("#984EA3"),
    TColor::GetColor("#FF7F00"), TColor::GetColor("#A65628"),
    TColor::GetColor("#F781BF"), TColor::GetColor("#999999"),
    TColor::GetColor("#66C2A5"), TColor::GetColor("#FC8D62"),
    TColor::GetColor("#8DA0CB"), TColor::GetColor("#FFD92F")
  };

  //
  TFile *fi[5];
  if(run>471){
    fi[0] = TFile::Open(TString::Format("../rootfile/data%04d_doppler_200ns_a_hist.root",run).Data());
    fi[1] = TFile::Open(TString::Format("../rootfile/data%04d_doppler_200ns_b_hist.root",run).Data());
    fi[2] = TFile::Open(TString::Format("../rootfile/data%04d_doppler_200ns_c_hist.root",run).Data());
    fi[3] = TFile::Open(TString::Format("../rootfile/data%04d_doppler_200ns_d_hist.root",run).Data());
    fi[4] = TFile::Open(TString::Format("../rootfile/data%04d_doppler_200ns_e_hist.root",run).Data());
  }else{
    fi[0] = TFile::Open(TString::Format("../rootfile/data%s_doppler_200ns_a_hist.root",str.Data()).Data());
    fi[1] = TFile::Open(TString::Format("../rootfile/data%s_doppler_200ns_b_hist.root",str.Data()).Data());
    fi[2] = TFile::Open(TString::Format("../rootfile/data%s_doppler_200ns_c_hist.root",str.Data()).Data());
    fi[3] = TFile::Open(TString::Format("../rootfile/data%s_doppler_200ns_d_hist.root",str.Data()).Data());
    fi[4] = TFile::Open(TString::Format("../rootfile/data%s_doppler_200ns_e_hist.root",str.Data()).Data());
  }

  //
  for(int i=0;i<5;i++){
    if(fi[i]->IsZombie()){
      cout << "cannot open the file." << std::endl;
      return;
    }
  }

  TCanvas *c1 = new TCanvas("c1","",0,0,500,360);

  TH1D *h1[5], *h2[5], *h3[5];
  TH1D *hh1[5], *hh2[5], *hh3[5];
  for(int i=0;i<5;i++){
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

  c1->cd();
  c1->SetLogy();

  //
  TLegend *leg1 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<5;i++){
    h1[i]->GetXaxis()->SetRangeUser(0, 1200);
    h1[i]->SetLineColor(colors[i]);
    if(i==0) h1[i]->Draw();
    else h1[i]->Draw("same");

    leg1->AddEntry(h1[i], TString::Format("config_%d",i+1));
  }
  leg1->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/abcde_e_raw.pdf", run).Data());
  
  //
  TLegend *leg2 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<5;i++){
    h2[i]->GetXaxis()->SetRangeUser(0, 1200);
    h2[i]->SetLineColor(colors[i]);
    if(i==0) h2[i]->Draw();
    else h2[i]->Draw("same");

    leg2->AddEntry(h2[i], TString::Format("config_%d",i+1));
  }
  leg2->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/abcde_e_dc_p_1000fs.pdf", run).Data());

  //
  TLegend *leg3 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<5;i++){
    h3[i]->GetXaxis()->SetRangeUser(0, 1200);
    h3[i]->SetLineColor(colors[i]);
    if(i==0) h3[i]->Draw();
    else h3[i]->Draw("same");

    leg3->AddEntry(h3[i], TString::Format("config_%d",i+1));
  }
  leg3->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/abcde_e_dc_r_1000fs.pdf", run).Data());

  ///
  TCanvas *c2 = new TCanvas("c2","",0,0,1000,360);
  c2->SetLogy();
  c2->cd();
  //
  TLegend *leg15 = new TLegend(0.7, 0.4, 0.98, 0.95);
  h3[4]->GetXaxis()->SetRangeUser(0, 1200);
  h3[4]->SetLineColor(colors[0]);
  h3[4]->Draw();
  leg15->AddEntry(h3[4], h3[4]->GetName());
  h1[4]->SetLineColor(colors[1]);
  h1[4]->Draw("same");
  leg15->AddEntry(h1[4], h1[4]->GetName());
  leg15->Draw("same");
  c2->SaveAs(TString::Format("./fig/%04d/e_raw_e_dc_r_spider_p.pdf", run).Data());

  //
  c2->SetLogy(0);
  TLegend *leg16 = new TLegend(0.7, 0.4, 0.98, 0.95);
  h2[4]->GetXaxis()->SetRangeUser(1300, 2800);
  h2[4]->SetLineColor(colors[0]);
  h2[4]->Draw();
  leg16->AddEntry(h2[4], h2[4]->GetName());
  h1[4]->SetLineColor(colors[1]);
  h1[4]->Draw("same");
  leg16->AddEntry(h1[4], h1[4]->GetName());
  leg16->Draw("same");
  c2->SaveAs(TString::Format("./fig/%04d/e_raw_e_dc_p_spider_p.pdf", run).Data());


  c1->cd();
  ///
  vector<string> v_str_e_p = {
    "e_dc_p", 
    "e_dc_p_10fs", 
    "e_dc_p_100fs" 
  };
  vector<string> v_str_e_r = {
    "e_dc_r", 
    "e_dc_r_10fs", 
    "e_dc_r_100fs" 
  };

  TH1D *h4[3], *h5[3];
  TH1D *hh4[3], *hh5[3];
  for(int i=0;i<3;i++){
    h4[i] = (TH1D*)fi[4]->Get(TString::Format("event_%s_spider_p",v_str_e_p[i].c_str()));
    h5[i] = (TH1D*)fi[4]->Get(TString::Format("event_%s_spider_p",v_str_e_r[i].c_str()));
    hh4[i] = (TH1D*)fi[4]->Get(TString::Format("bg_%s_spider_p",v_str_e_p[i].c_str()));
    hh5[i] = (TH1D*)fi[4]->Get(TString::Format("bg_%s_spider_p",v_str_e_r[i].c_str()));

    h4[i]->Add(hh4[i], -1);
    h5[i]->Add(hh5[i], -1);
  }

  //
  TLegend *leg4 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<3;i++){
    h4[i]->GetXaxis()->SetRangeUser(0, 1200);
    h4[i]->SetLineColor(colors[i]);
    if(i==0) h4[i]->Draw();
    else h4[i]->Draw("same");

    leg4->AddEntry(h4[i], h4[i]->GetName());
  }
  h2[4]->SetLineColor(colors[3]);
  leg4->AddEntry(h2[4], h2[4]->GetName());
  leg4->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_dc_p_diff_beta.pdf", run).Data());

  //
  TLegend *leg5 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<3;i++){
    h5[i]->GetXaxis()->SetRangeUser(0, 1200);
    h5[i]->SetLineColor(colors[i]);
    if(i==0) h5[i]->Draw();
    else h5[i]->Draw("same");

    leg5->AddEntry(h5[i], h5[i]->GetName());
  }
  h3[4]->SetLineColor(colors[3]);
  leg5->AddEntry(h3[4], h3[4]->GetName());
  leg5->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_dc_r_diff_beta.pdf", run).Data());

  ///
  TH1D *h6[5], *h7[5], *h8[5];
  TH1D *hh6[5], *hh7[5], *hh8[5];
  for(int i=0;i<5;i++){
    h6[i] = (TH1D*)fi[4]->Get(TString::Format("event_e_raw_spider_p_ge_ring%d",i+1));
    h7[i] = (TH1D*)fi[4]->Get(TString::Format("event_e_dc_p_1000fs_spider_p_ge_ring%d",i+1));
    h8[i] = (TH1D*)fi[4]->Get(TString::Format("event_e_dc_r_1000fs_spider_p_ge_ring%d",i+1));
    hh6[i] = (TH1D*)fi[4]->Get(TString::Format("bg_e_raw_spider_p_ge_ring%d",i+1));
    hh7[i] = (TH1D*)fi[4]->Get(TString::Format("bg_e_dc_p_1000fs_spider_p_ge_ring%d",i+1));
    hh8[i] = (TH1D*)fi[4]->Get(TString::Format("bg_e_dc_r_1000fs_spider_p_ge_ring%d",i+1));

    h6[i]->Add(hh6[i], -1);
    h7[i]->Add(hh7[i], -1);
    h8[i]->Add(hh8[i], -1);
  }

  //
  TLegend *leg6 = new TLegend(0.7, 0.4, 0.98, 0.95);
  h1[4]->GetXaxis()->SetRangeUser(0, 1200);
  h1[4]->SetLineColor(colors[0]);
  h1[4]->Draw();
  leg6->AddEntry(h1[4], h1[4]->GetName());
  for(int i=0;i<5;i++){
    h6[i]->SetLineColor(colors[i+1]);
    h6[i]->Draw("same");

    leg6->AddEntry(h6[i], h6[i]->GetName());
  }
  leg6->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_raw_diff_ge_ring.pdf", run).Data());

  //
  TLegend *leg7 = new TLegend(0.7, 0.4, 0.98, 0.95);
  h2[4]->GetXaxis()->SetRangeUser(0, 1200);
  h2[4]->SetLineColor(colors[0]);
  h2[4]->Draw();
  leg7->AddEntry(h2[4], h2[4]->GetName());
  for(int i=0;i<5;i++){
    h7[i]->SetLineColor(colors[i+1]);
    h7[i]->Draw("same");

    leg7->AddEntry(h7[i], h7[i]->GetName());
  }
  leg7->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_dc_p_1000fs_diff_ge_ring.pdf", run).Data());

  //
  TLegend *leg8 = new TLegend(0.7, 0.4, 0.98, 0.95);
  h3[4]->GetXaxis()->SetRangeUser(0, 1200);
  h3[4]->SetLineColor(colors[0]);
  h3[4]->Draw();
  leg8->AddEntry(h3[4], h3[4]->GetName());
  for(int i=0;i<5;i++){
    h8[i]->SetLineColor(colors[i+1]);
    h8[i]->Draw("same");

    leg8->AddEntry(h8[i], h8[i]->GetName());
  }
  leg8->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_dc_r_1000fs_diff_ge_ring.pdf", run).Data());

  ///
  TH1D *h9[8], *h10[8], *h11[8];
  TH1D *hh9[8], *hh10[8], *hh11[8];
  for(int i=0;i<8;i++){
    h9[i] = (TH1D*)fi[4]->Get(TString::Format("event_e_raw_spider_p_spider_ring%d",i+1));
    h10[i] = (TH1D*)fi[4]->Get(TString::Format("event_e_dc_p_1000fs_spider_p_spider_ring%d",i+1));
    h11[i] = (TH1D*)fi[4]->Get(TString::Format("event_e_dc_r_1000fs_spider_p_spider_ring%d",i+1));
    hh9[i] = (TH1D*)fi[4]->Get(TString::Format("bg_e_raw_spider_p_spider_ring%d",i+1));
    hh10[i] = (TH1D*)fi[4]->Get(TString::Format("bg_e_dc_p_1000fs_spider_p_spider_ring%d",i+1));
    hh11[i] = (TH1D*)fi[4]->Get(TString::Format("bg_e_dc_r_1000fs_spider_p_spider_ring%d",i+1));

    h9[i]->Add(hh9[i], -1);
    h10[i]->Add(hh10[i], -1);
    h11[i]->Add(hh11[i], -1);
  }

  //
  TLegend *leg9 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<8;i++){
    h9[i]->GetXaxis()->SetRangeUser(0, 1200);
    h9[i]->GetYaxis()->SetRangeUser(1, 100000);
    h9[i]->SetLineColor(colors[i]);
    if(i==0) h9[i]->Draw();
    else h9[i]->Draw("same");

    leg9->AddEntry(h9[i], h9[i]->GetName());
  }
  leg9->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_raw_diff_spider_ring.pdf", run).Data());
  
  //
  TLegend *leg10 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<8;i++){
    h10[i]->GetXaxis()->SetRangeUser(0, 1200);
    h10[i]->GetYaxis()->SetRangeUser(1, 100000);
    h10[i]->SetLineColor(colors[i]);
    if(i==0) h10[i]->Draw();
    else h10[i]->Draw("same");

    leg10->AddEntry(h10[i], h10[i]->GetName());
  }
  leg10->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_dc_p_diff_spider_ring.pdf", run).Data());

  //
  TLegend *leg11 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<8;i++){
    h11[i]->GetXaxis()->SetRangeUser(0, 1200);
    h11[i]->GetYaxis()->SetRangeUser(1, 100000);
    h11[i]->SetLineColor(colors[i]);
    if(i==0) h11[i]->Draw();
    else h11[i]->Draw("same");

    leg11->AddEntry(h11[i], h11[i]->GetName());
  }
  leg11->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_dc_r_diff_spider_ring.pdf", run).Data());

  ///
  TH1D *h12[12], *h13[12], *h14[12];
  TH1D *hh12[12], *hh13[12], *hh14[12];
  for(int i=0;i<12;i++){
    h12[i] = (TH1D*)fi[4]->Get(TString::Format("event_e_raw_spider_p_spider_sector%d",i+1));
    h13[i] = (TH1D*)fi[4]->Get(TString::Format("event_e_dc_p_1000fs_spider_p_spider_sector%d",i+1));
    h14[i] = (TH1D*)fi[4]->Get(TString::Format("event_e_dc_r_1000fs_spider_p_spider_sector%d",i+1));
    hh12[i] = (TH1D*)fi[4]->Get(TString::Format("bg_e_raw_spider_p_spider_sector%d",i+1));
    hh13[i] = (TH1D*)fi[4]->Get(TString::Format("bg_e_dc_p_1000fs_spider_p_spider_sector%d",i+1));
    hh14[i] = (TH1D*)fi[4]->Get(TString::Format("bg_e_dc_r_1000fs_spider_p_spider_sector%d",i+1));

    h12[i]->Add(hh12[i], -1);
    h13[i]->Add(hh13[i], -1);
    h14[i]->Add(hh14[i], -1);
  }

  //
  TLegend *leg12 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<12;i++){
    h12[i]->GetXaxis()->SetRangeUser(0, 1200);
    h12[i]->SetLineColor(colors[i]);
    if(i==0) h12[i]->Draw();
    else h12[i]->Draw("same");

    leg12->AddEntry(h12[i], h12[i]->GetName());
  }
  leg12->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_raw_diff_spider_sector.pdf", run).Data());

  //
  TLegend *leg13 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<12;i++){
    h13[i]->GetXaxis()->SetRangeUser(0, 1200);
    h13[i]->SetLineColor(colors[i]);
    if(i==0) h13[i]->Draw();
    else h13[i]->Draw("same");

    leg13->AddEntry(h13[i], h13[i]->GetName());
  }
  leg13->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_dc_p_diff_spider_sector.pdf", run).Data());

  //
  TLegend *leg14 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<12;i++){
    h14[i]->GetXaxis()->SetRangeUser(0, 1200);
    h14[i]->SetLineColor(colors[i]);
    if(i==0) h14[i]->Draw();
    else h14[i]->Draw("same");

    leg14->AddEntry(h14[i], h14[i]->GetName());
  }
  leg14->Draw("same");
  c1->SaveAs(TString::Format("./fig/%04d/e_dc_r_diff_spider_sector.pdf", run).Data());

  /*
  for(int i=0;i<5;i++){
    fi[i]->Close();
  }
  */
}
