#include <sys/stat.h>
#include <sys/types.h>

//
void draw_sample(int run, int win, int jump)
{
  gROOT->SetBatch(1);

  mkdir(Form("./fig/%04d", run), 0777);

  int colors[12] = {
    TColor::GetColor("#E41A1C"), TColor::GetColor("#377EB8"),
    TColor::GetColor("#4DAF4A"), TColor::GetColor("#984EA3"),
    TColor::GetColor("#FF7F00"), TColor::GetColor("#A65628"),
    TColor::GetColor("#F781BF"), TColor::GetColor("#999999"),
    TColor::GetColor("#66C2A5"), TColor::GetColor("#FC8D62"),
    TColor::GetColor("#8DA0CB"), TColor::GetColor("#FFD92F")
  };

  //
  TFile *fi =  TFile::Open(Form("../../rootfile/data%04d_build_%dns_jump_%dns_hist_event.root",run,win,jump));
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
  hh->SetTitle("spider beam spot");
  hh->GetXaxis()->SetTitle("x");
  hh->GetYaxis()->SetTitle("y");
  hh->Draw("col");
  c1->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_s3_sector_spot");
  hh->SetTitle("s3 sector beam spot");
  hh->GetXaxis()->SetTitle("x");
  hh->GetYaxis()->SetTitle("y");
  hh->Draw("col");
  c1->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_s3_ring_spot");
  hh->SetTitle("s3 ring beam spot");
  hh->GetXaxis()->SetTitle("x");
  hh->GetYaxis()->SetTitle("y");
  hh->Draw("col");
  c1->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  //
  TCanvas *c2 = new TCanvas("c2", "", 0, 0, 400, 300);
  c2->cd();
  c2->SetLogy();

  // ts diff
  TH1D *h;
  
  h = (TH1D*)fi->Get("hpg");
  h->SetTitle("particle-gamma time difference");
  h->GetXaxis()->SetTitle("time difference [ns]");
  h->GetYaxis()->SetTitle("counts");
  h->GetXaxis()->SetRangeUser(-500, 500);
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("hgg");
  h->SetTitle("gamma-gamma time difference");
  h->GetXaxis()->SetTitle("time difference [ns]");
  h->GetYaxis()->SetTitle("counts");
  h->GetXaxis()->SetRangeUser(-500, 500);
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("hpp");
  h->SetTitle("particle-particle time difference");
  h->GetXaxis()->SetTitle("time difference [ns]");
  h->GetYaxis()->SetTitle("counts");
  h->GetXaxis()->SetRangeUser(-500, 500);
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("h_spider_ge");
  h->SetTitle("spider particle and ge gamma time difference");
  h->GetXaxis()->SetTitle("time difference [ns]");
  h->GetYaxis()->SetTitle("counts");
  h->GetXaxis()->SetRangeUser(-500, 500);
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("h_s3_ge");
  h->SetTitle("s3 particle and ge gamma time difference");
  h->GetXaxis()->SetTitle("time difference [ns]");
  h->GetYaxis()->SetTitle("counts");
  h->GetXaxis()->SetRangeUser(-500, 500);
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  h = (TH1D*)fi->Get("h_s3_s3");
  h->SetTitle("s3 particle-particle time difference");
  h->GetXaxis()->SetTitle("time difference [ns]");
  h->GetYaxis()->SetTitle("counts");
  h->GetXaxis()->SetRangeUser(-500, 500);
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  // hits
  c2->SetLogy();
  
  h = (TH1D*)fi->Get("h_n_spider");
  h->SetTitle("n hits in spider");
  h->GetXaxis()->SetTitle("hits number");
  h->GetYaxis()->SetTitle("counts");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_n_s3_sector");
  h->SetTitle("n hits in s3 sector");
  h->GetXaxis()->SetTitle("hits number");
  h->GetYaxis()->SetTitle("counts");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_n_s3_ring");
  h->SetTitle("n hits in s3 ring");
  h->GetXaxis()->SetTitle("hits number");
  h->GetYaxis()->SetTitle("counts");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  // id
  h = (TH1D*)fi->Get("h_spider_sector_id");
  h->SetTitle("spider sector id");
  h->GetXaxis()->SetTitle("sector id");
  h->GetYaxis()->SetTitle("counts");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_spider_ring_id");
  h->SetTitle("spider ring id");
  h->GetXaxis()->SetTitle("ring id");
  h->GetYaxis()->SetTitle("counts");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_s3_sector_id");
  h->GetXaxis()->SetTitle("sector id");
  h->GetYaxis()->SetTitle("counts");
  h->SetTitle("s3 sector id");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_s3_ring_id");
  h->GetXaxis()->SetTitle("ring id");
  h->GetYaxis()->SetTitle("counts");
  h->SetTitle("s3 ring id");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());

  // spider th2
  c2->SetLogy(0); 
  c2->SetLogz(); 

  hh = (TH2D*)fi->Get("hh_spider_sector_energy");
  hh->SetTitle("th2 spider sector energy vs. id");
  hh->GetXaxis()->SetTitle("sector id");
  hh->GetYaxis()->SetTitle("energy [keV]");
  hh->Draw("col");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_spider_ring_energy");
  hh->SetTitle("th2 spider ring energy vs. id");
  hh->GetXaxis()->SetTitle("ring id");
  hh->GetYaxis()->SetTitle("energy [keV]");
  hh->Draw("col");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_spider_energy");
  hh->SetTitle("th2 spider energy vs. id");
  hh->GetXaxis()->SetTitle("spider id");
  hh->GetYaxis()->SetTitle("energy [keV]");
  hh->Draw("col");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  // spider
  TH1D *h1[96];

  c2->SetLogy();
  c2->SetLogz(0);

  TLegend *leg1 = new TLegend(0.5, 0.4, 0.98, 0.95);
  for(int i=0;i<12;i++){
    h1[i] = (TH1D*)fi->Get(TString::Format("h_spider_sector%02d_energy",i+1).Data());
    h1[i]->SetLineColor(colors[i]);
    h1[i]->Rebin(8);

    if(i==0){
      h1[i]->SetTitle("spider different sector energy");
      h1[i]->GetXaxis()->SetTitle("energy [keV]");
      h1[i]->GetYaxis()->SetTitle("counts");
      h1[i]->Draw();
    }else h1[i]->Draw("same");

    leg1->AddEntry(h1[i], h1[i]->GetName());
  }
  leg1->Draw("same");
  c2->SaveAs(TString::Format("./fig/%04d/c_spider_sector_energy_all.png", run).Data());

  TLegend *leg2 = new TLegend(0.6, 0.4, 0.98, 0.95);
  for(int i=0;i<8;i++){
    h1[i] = (TH1D*)fi->Get(TString::Format("h_spider_ring%02d_energy",i+1).Data());
    h1[i]->SetLineColor(colors[i]);
    h1[i]->Rebin(8);

    if(i==0){
      h1[i]->SetTitle("spider different ring energy");
      h1[i]->GetXaxis()->SetTitle("energy [keV]");
      h1[i]->GetYaxis()->SetTitle("counts");
      h1[i]->Draw();
    }else h1[i]->Draw("same");

    leg2->AddEntry(h1[i], h1[i]->GetName());
  }
  leg2->Draw("same");
  c2->SaveAs(TString::Format("./fig/%04d/c_spider_ring_energy_all.png", run).Data());

  // s3 th2
  c2->SetLogy(0);
  c2->SetLogz(); 

  hh = (TH2D*)fi->Get("hh_s3_sector_energy");
  hh->SetTitle("s3 sector energy vs. id");
  hh->GetXaxis()->SetTitle("sector id");
  hh->GetYaxis()->SetTitle("energy [keV]");
  hh->Draw("col");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  hh = (TH2D*)fi->Get("hh_s3_ring_energy");
  hh->SetTitle("s3 ring energy vs. id");
  hh->GetXaxis()->SetTitle("ring id");
  hh->GetYaxis()->SetTitle("energy [keV]");
  hh->Draw("col");
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, hh->GetName()).Data());

  // 
  c2->SetLogy();
  c2->SetLogz(0);

  h = (TH1D*)fi->Get("h_s3_sector_energy");
  h->SetTitle("s3 sector energy");
  h->GetXaxis()->SetTitle("energy [keV]");
  h->GetYaxis()->SetTitle("counts");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  h = (TH1D*)fi->Get("h_s3_ring_energy");
  h->SetTitle("s3 ring energy");
  h->GetXaxis()->SetTitle("energy [keV]");
  h->GetYaxis()->SetTitle("counts");
  h->Draw();
  c2->SaveAs(TString::Format("./fig/%04d/c_%s.png", run, h->GetName()).Data());
  
  TLegend *leg5 = new TLegend(0.7, 0.05, 0.98, 0.95);
  for(int i=0;i<32;i++){
    h1[i] = (TH1D*)fi->Get(TString::Format("h_s3_sector%02d_energy",i+1).Data());
    h1[i]->SetLineColor(colors[i%12]);
    h1[i]->Rebin(8);
      
    if(i==0){
      h1[i]->SetTitle("s3 different sector energy");
      h1[i]->GetXaxis()->SetTitle("energy [keV]");
      h1[i]->GetYaxis()->SetTitle("counts");
      h1[i]->Draw();
    }else h1[i]->Draw("same");
    leg5->AddEntry(h1[i], h1[i]->GetName());
    leg5->Draw("same");
    c2->SaveAs(TString::Format("./fig/%04d/c_s3_diff_sector_energy.png", run).Data());
  }

  TLegend *leg6 = new TLegend(0.7, 0.05, 0.98, 0.95);;
  for(int i=0;i<24;i++){
    h1[i] = (TH1D*)fi->Get(TString::Format("h_s3_ring%02d_energy",i+1).Data());
    h1[i]->SetLineColor(colors[i%12]);
    h1[i]->Rebin(8);

    if(i==0){
      h1[i]->SetTitle("s3 different ring energy");
      h1[i]->GetXaxis()->SetTitle("energy [keV]");
      h1[i]->GetYaxis()->SetTitle("counts");
      h1[i]->Draw();
    }else h1[i]->Draw("same");
    leg6->AddEntry(h1[i], h1[i]->GetName());
    leg6->Draw("same");
    c2->SaveAs(TString::Format("./fig/%04d/c_s3_diff_ring_energy.png", run).Data());
  }

  // ge spectra
  map<int, int> map_ge_ring_sector = {
    {1, 6},
    {2, 8},
    {3, 12},
    {4, 8},
    {5, 6}
  };

  TCanvas *c3 = new TCanvas("c3", "", 0, 0, 1000, 400);
  c3->cd();
  c3->SetLogy();

  TH1D *h_ge_all, *h_ge_spider_all, *h_ge_s3_all, *h_ge_spider_cut_all, *h_ge_s3_cut_all;
  h_ge_all = new TH1D("h_ge_all","",4096,0,4096);
  h_ge_spider_all = new TH1D("h_ge_spider_all","",4096,0,4096);
  h_ge_s3_all = new TH1D("h_ge_s3_all","",4096,0,4096);
  h_ge_spider_cut_all = new TH1D("h_ge_spider_cut_all","",4096,0,4096);
  h_ge_s3_cut_all = new TH1D("h_ge_s3_cut_all","",4096,0,4096);

  TH1D *h_ge[40], *h_ge_spider[40], *h_ge_s3[40], *h_ge_spider_cut[40], *h_ge_s3_cut[40];
  TLegend *leg7[40];

  int k = 0;
  for(int i=1;i<=5;i++){
    for(int j=1;j<=map_ge_ring_sector[i];j++){
      h_ge[k] = (TH1D*)fi->Get(TString::Format("h_ge_ring%d_sector%d",i,j));  
      h_ge_spider[k] = (TH1D*)fi->Get(TString::Format("h_ge_ring%d_sector%d_spider",i,j));  
      h_ge_s3[k] = (TH1D*)fi->Get(TString::Format("h_ge_ring%d_sector%d_s3",i,j));  
      h_ge_spider_cut[k] = (TH1D*)fi->Get(TString::Format("h_ge_ring%d_sector%d_spider_cut",i,j));  
      h_ge_s3_cut[k] = (TH1D*)fi->Get(TString::Format("h_ge_ring%d_sector%d_s3_cut",i,j));  

      if(h_ge[k]->GetEntries()>0){
        //
        h_ge_all->Add(h_ge[k], 1);
        h_ge_spider_all->Add(h_ge_spider[k], 1);
        h_ge_s3_all->Add(h_ge_s3[k], 1);
        h_ge_spider_cut_all->Add(h_ge_spider_cut[k], 1);
        h_ge_s3_cut_all->Add(h_ge_s3_cut[k], 1);

        leg7[k] = new TLegend(0.7, 0.4, 0.98, 0.95);
        h_ge[k]->SetLineColor(colors[0]);
        h_ge[k]->GetYaxis()->SetRangeUser(1., std::pow(10, std::ceil(std::log10(h_ge[k]->GetMaximum()))));
        h_ge[k]->SetTitle(Form("ge ring%d sector%d", i, j));
        h_ge[k]->GetXaxis()->SetTitle("energy [keV]");
        h_ge[k]->GetYaxis()->SetTitle("counts");
        h_ge[k]->Draw();
        leg7[k]->AddEntry(h_ge[k], h_ge[k]->GetName());

        h_ge_spider[k]->SetLineColor(colors[1]);
        h_ge_spider[k]->Draw("same");
        leg7[k]->AddEntry(h_ge_spider[k], h_ge_spider[k]->GetName());

        h_ge_s3[k]->SetLineColor(colors[2]);
        h_ge_s3[k]->Draw("same");
        leg7[k]->AddEntry(h_ge_s3[k], h_ge_s3[k]->GetName());

        h_ge_spider_cut[k]->SetLineColor(colors[3]);
        h_ge_spider_cut[k]->Draw("same");
        leg7[k]->AddEntry(h_ge_spider_cut[k], h_ge_spider_cut[k]->GetName());

        h_ge_s3_cut[k]->SetLineColor(colors[4]);
        h_ge_s3_cut[k]->Draw("same");
        leg7[k]->AddEntry(h_ge_s3_cut[k], h_ge_s3_cut[k]->GetName());

        leg7[k]->Draw("same");
        c3->SaveAs(TString::Format("./fig/%04d/c_ge_ring%d_sector%d.png", run,i,j).Data());
      }
      
      k++;
    }
  }

  TLegend *leg8 = new TLegend(0.7, 0.4, 0.98, 0.95);
  h_ge_all->SetLineColor(colors[0]);
  h_ge_all->GetYaxis()->SetRangeUser(1., std::pow(10, std::ceil(std::log10(h_ge_all->GetMaximum()))));
  h_ge_all->SetTitle("all ge");
  h_ge_all->GetXaxis()->SetTitle("energy [keV]");
  h_ge_all->GetYaxis()->SetTitle("counts");
  h_ge_all->Draw();
  leg8->AddEntry(h_ge_all, h_ge_all->GetName());

  h_ge_spider_all->SetLineColor(colors[1]);
  h_ge_spider_all->Draw("same");
  leg8->AddEntry(h_ge_spider_all, h_ge_spider_all->GetName());

  h_ge_s3_all->SetLineColor(colors[2]);
  h_ge_s3_all->Draw("same");
  leg8->AddEntry(h_ge_s3_all, h_ge_s3_all->GetName());

  h_ge_spider_cut_all->SetLineColor(colors[3]);
  h_ge_spider_cut_all->Draw("same");
  leg8->AddEntry(h_ge_spider_cut_all, h_ge_spider_cut_all->GetName());

  h_ge_s3_cut_all->SetLineColor(colors[4]);
  h_ge_s3_cut_all->Draw("same");
  leg8->AddEntry(h_ge_s3_cut_all, h_ge_s3_cut_all->GetName());

  leg8->Draw("same");
  c3->SaveAs(TString::Format("./fig/%04d/c_ge_all.png", run).Data());
}
