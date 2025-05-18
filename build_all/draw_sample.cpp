#include <sys/stat.h>
#include <sys/types.h>

//
void draw_sample(int run)
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
  TFile *fi =  TFile::Open(TString::Format("../rootfile/data%04d_build_200ns_hist_event.root",run).Data());
  if(fi->IsZombie()){
    cout << "cannot open the file." << std::endl;
    return;
  }

  TCanvas *c1 = new TCanvas("c1", "", 0, 0, 500, 520);
  c1->cd();
  c1->SetLogz();

  // spot
  TH2D *hh;

  hh = (TH2D*)fi->Get("hh_spider_spot");
  hh->Draw("colz");
  c1->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_s3_sector_spot");
  hh->Draw("colz");
  c1->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_s3_ring_spot");
  hh->Draw("colz");
  c1->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  //
  TCanvas *c2 = new TCanvas("c2", "", 0, 0, 400, 300);
  c2->cd();

  // ts diff
  TH1D *h;
  
  h = (TH1D*)fi->Get("hpg");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("hgg");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("hpp");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("h_spider_ge");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("h_s3_ge");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("h_spider_spider");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("h_s3_s3");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  // hits
  c2->SetLogy();
  
  h = (TH1D*)fi->Get("h_n_spider");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_n_s3_sector");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_n_s3_ring");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  // id
  h = (TH1D*)fi->Get("h_spider_sector_id");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_spider_ring_id");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_s3_sector_id");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_s3_ring_id");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  // spider th2
  c2->SetLogy(0); 
  c2->SetLogz(); 

  hh = (TH2D*)fi->Get("hh_spider_sector_energy");
  hh->Draw("colz");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_spider_ring_energy");
  hh->Draw("colz");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_spider_energy");
  hh->Draw("colz");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  // spider sector
  TH1D *h1[96];

  c2->SetLogy();
  c2->SetLogz(0);

  TLegend *leg1 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<12;i++){
    h1[i] = (TH1D*)fi->Get(TString::Format("h_spider_sector%02d_energy",i+1).Data());
    h1[i]->SetLineColor(colors[i]);

    if(i==0) h1[i]->Draw();
    else h1[i]->Draw("same");

    leg1->AddEntry(h1[i], h1[i]->GetName());
  }
  leg1->Draw("same");
  c2->SaveAs(TString::Format("./fig/%04d/c_spider_sector_energy_all.png", run).Data());

  TLegend *leg2 = new TLegend(0.7, 0.4, 0.98, 0.95);
  for(int i=0;i<8;i++){
    h1[i] = (TH1D*)fi->Get(TString::Format("h_spider_ring%02d_energy",i+1).Data());
    h1[i]->SetLineColor(colors[i]);

    if(i==0) h1[i]->Draw();
    else h1[i]->Draw("same");

    leg2->AddEntry(h1[i], h1[i]->GetName());
  }
  leg2->Draw("same");
  c2->SaveAs(TString::Format("./fig/%04d/c_spider_ring_energy_all.png", run).Data());

  TLegend *leg3[8];
  for(int i=0;i<8;i++){
    leg3[i] = new TLegend(0.7, 0.4, 0.98, 0.95);
    for(int j=0;j<12;j++){
      h1[i*8+j] = (TH1D*)fi->Get(TString::Format("h_spider_sector%02d_ring%02d_energy",j+1,i+1).Data());
      h1[i*8+j]->SetLineColor(colors[j]);
      if(j==0) h1[i*8+j]->Draw();
      else h1[i*8+j]->Draw("same");

      leg3[i]->AddEntry(h1[i*8+j], h1[i*8+j]->GetName());
    }
    leg3[i]->Draw("same");
    c2->SaveAs(TString::Format("./fig/%04d/c_spider_ring%02d_energy.png", run, i+1).Data());
  }

  TLegend *leg4[12];
  for(int i=0;i<12;i++){
    leg4[i] = new TLegend(0.7, 0.4, 0.98, 0.95);
    for(int j=0;j<8;j++){
      h1[i*12+j] = (TH1D*)fi->Get(TString::Format("h_spider_sector%02d_ring%02d_energy",i+1,j+1).Data());
      h1[i*12+j]->SetLineColor(colors[j]);
      if(j==0) h1[i*12+j]->Draw();
      else h1[i*12+j]->Draw("same");

      leg4[i]->AddEntry(h1[i*12+j], h1[i*12+j]->GetName());
    }
    leg4[i]->Draw("same");
    c2->SaveAs(TString::Format("./fig/%04d/c_spider_sector%02d_energy.png", run, i+1).Data());
  }

  // s3 th2
  c2->SetLogy(0);
  c2->SetLogz(); 

  hh = (TH2D*)fi->Get("hh_s3_sector_energy");
  hh->Draw("colz");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_s3_ring_energy");
  hh->Draw("colz");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  // 
  c2->SetLogy();
  c2->SetLogz(0);

  h = (TH1D*)fi->Get("h_s3_sector_energy");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_s3_ring_energy");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  TLegend *leg5[4];
  for(int i=0;i<4;i++){
    leg5[i] = new TLegend(0.7, 0.4, 0.98, 0.95);
    for(int j=0;j<8;j++){
      h1[i*4+j] = (TH1D*)fi->Get(TString::Format("h_s3_sector%02d_energy",i*4+j+1).Data());
      h1[i*4+j]->SetLineColor(colors[j]);
      
      if(j==0) h1[i*4+j]->Draw();
      else h1[i*4+j]->Draw("same");
      leg5[i]->AddEntry(h1[i*4+j], h1[i*4+j]->GetName());
    }
    leg5[i]->Draw("same");
    c2->SaveAs(TString::Format("./fig/%04d/c_s3_sector%02d_energy.png", run, i+1).Data());
  }

  TLegend *leg6[4];
  for(int i=0;i<4;i++){
    leg6[i] = new TLegend(0.7, 0.4, 0.98, 0.95);
    for(int j=0;j<6;j++){
      h1[i*4+j] = (TH1D*)fi->Get(TString::Format("h_s3_ring%02d_energy",i*4+j+1).Data());
      h1[i*4+j]->SetLineColor(colors[j]);

      if(j==0) h1[i*4+j]->Draw();
      else h1[i*4+j]->Draw("same");
      leg6[i]->AddEntry(h1[i*4+j], h1[i*4+j]->GetName());
    }
    leg6[i]->Draw("same");
    c2->SaveAs(TString::Format("./fig/%04d/c_s3_ring%02d_energy.png", run, i+1).Data());
  }
}
