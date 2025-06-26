#include <sys/stat.h>
#include <sys/types.h>

//
void draw_sample(string str="")
{
  // gROOT->SetBatch(1);

  int colors[12] = {
    TColor::GetColor("#E41A1C"), TColor::GetColor("#377EB8"),
    TColor::GetColor("#4DAF4A"), TColor::GetColor("#984EA3"),
    TColor::GetColor("#FF7F00"), TColor::GetColor("#A65628"),
    TColor::GetColor("#F781BF"), TColor::GetColor("#999999"),
    TColor::GetColor("#66C2A5"), TColor::GetColor("#FC8D62"),
    TColor::GetColor("#8DA0CB"), TColor::GetColor("#FFD92F")
  };

  map<int, int> m_ge_ring_sector_number = {
    {1, 6},
    {2, 8},
    {3, 12},
    {4, 8},
    {5, 6}
  };
  

  //
  TFile *fi = TFile::Open(str.c_str());
  if(fi->IsZombie()){
    cout << "cannot open the file." << std::endl;
    return;
  }

  //
  TCanvas *c = new TCanvas("c","",0,0,900,360);

  TH1D *h1, *h2, *h3;
  TH1D *hh1, *hh2, *hh3;
  h1 = (TH1D*)fi->Get("event_e_raw_spider_p");
  h2 = (TH1D*)fi->Get("event_e_dc_p_1000fs_spider_p");
  h3 = (TH1D*)fi->Get("event_e_dc_r_1000fs_spider_p");
  hh1 = (TH1D*)fi->Get("bg_e_raw_spider_p");
  hh2 = (TH1D*)fi->Get("bg_e_dc_p_1000fs_spider_p");
  hh3 = (TH1D*)fi->Get("bg_e_dc_r_1000fs_spider_p");

  h1->Add(hh1, -1);
  h2->Add(hh2, -1);
  h3->Add(hh3, -1);

  c->cd();
  c->SetLogy();

  //
  TLegend *leg = new TLegend(0.7, 0.6, 0.98, 0.95);
  h1->GetXaxis()->SetRangeUser(0, 4096);
  h1->SetLineColor(colors[1]);
  h1->Draw();
  leg->AddEntry(h1, h1->GetName());
  h2->SetLineColor(colors[2]);
  h2->Draw("same");
  leg->AddEntry(h2, h2->GetName());
  h3->SetLineColor(colors[3]);
  h3->Draw("same");
  leg->AddEntry(h3, h3->GetName());
  c->SaveAs("./fig/e.pdf");
  
  ///
  for(auto &[rn,sn] : m_ge_ring_sector_number){
    for(int i=1;i<=sn;++i){
      h1 = (TH1D*)fi->Get(TString::Format("event_e_raw_spider_p_ge_ring%d_sector%d",rn,i));
      h2 = (TH1D*)fi->Get(TString::Format("event_e_dc_p_1000fs_spider_p_ge_ring%d_sector%d",rn,i));
      h3 = (TH1D*)fi->Get(TString::Format("event_e_dc_r_1000fs_spider_p_ge_ring%d_sector%d",rn,i));
      hh1 = (TH1D*)fi->Get(TString::Format("bg_e_raw_spider_p_ge_ring%d_sector%d",rn,i));
      hh2 = (TH1D*)fi->Get(TString::Format("bg_e_dc_p_1000fs_spider_p_ge_ring%d_sector%d",rn,i));
      hh3 = (TH1D*)fi->Get(TString::Format("bg_e_dc_r_1000fs_spider_p_ge_ring%d_sector%d",rn,i));
      
      h1->Add(hh1, -1);
      h2->Add(hh2, -1);
      h3->Add(hh3, -1);

      c->cd();
      c->SetLogy();
      h1->GetXaxis()->SetRangeUser(0, 4096);
      h1->SetTitle(TString::Format("./fig/e_raw_spider_p_ge_ring%d_sector%d",rn,i));
      h1->Draw();
      c->SaveAs(TString::Format("./fig/e_raw_spider_p_ge_ring%d_sector%d.pdf",rn,i));

      c->cd();
      c->SetLogy();
      h2->GetXaxis()->SetRangeUser(0, 4096);
      h2->SetTitle(TString::Format("./fig/e_dc_p_1000fs_spider_p_ge_ring%d_sector%d",rn,i));
      h2->Draw();
      c->SaveAs(TString::Format("./fig/e_dc_p_1000fs_spider_p_ge_ring%d_sector%d.pdf",rn,i));

      c->cd();
      c->SetLogy();
      h3->GetXaxis()->SetRangeUser(0, 4096);
      h3->SetTitle(TString::Format("./fig/e_dc_r_1000fs_spider_p_ge_ring%d_sector%d",rn,i));
      h3->Draw();
      c->SaveAs(TString::Format("./fig/e_dc_r_1000fs_spider_p_ge_ring%d_sector%d.pdf",rn,i));
    }
  }

  ///
  for(int i=0;i<8;++i){// spider_ring
    for(int j=0;j<12;++j){// spider sector
      for(auto &[rn,sn] : m_ge_ring_sector_number){// ge ring
        for(int ii=1;ii<=sn;++ii){// ge sector
          h1 = (TH1D*)fi->Get(TString::Format("event_e_raw_spider_p_ring%d_sector%d_ge_ring%d_sector%d",i+1,j+1,rn,ii));
          h2 = (TH1D*)fi->Get(TString::Format("event_e_dc_p_1000fs_spider_p_ring%d_sector%d_ge_ring%d_sector%d",i+1,j+1,rn,ii));
          h3 = (TH1D*)fi->Get(TString::Format("event_e_dc_r_1000fs_spider_p_ring%d_sector%d_ge_ring%d_sector%d",i+1,j+1,rn,ii));
          hh1 = (TH1D*)fi->Get(TString::Format("bg_e_raw_spider_p_ring%d_sector%d_ge_ring%d_sector%d",i+1,j+1,rn,ii));
          hh2 = (TH1D*)fi->Get(TString::Format("bg_e_dc_p_1000fs_spider_p_ring%d_sector%d_ge_ring%d_sector%d",i+1,j+1,rn,ii));
          hh3 = (TH1D*)fi->Get(TString::Format("bg_e_dc_r_1000fs_spider_p_ring%d_sector%d_ge_ring%d_sector%d",i+1,j+1,rn,ii));
          
          h1->Add(hh1, -1);
          h2->Add(hh2, -1);
          h3->Add(hh3, -1);

          c->cd();
          c->SetLogy();
          h1->GetXaxis()->SetRangeUser(0, 4096);
          h1->SetTitle(TString::Format("./fig/e_raw_spider_p_ring%d_sector%d_ge_ring%d_sector%d",i+1,j+1,rn,ii));
          h1->Draw();
          c->SaveAs(TString::Format("./fig/e_raw_spider_p_ring%d_sector%d_ge_ring%d_sector%d.pdf",i+1,j+1,rn,ii));

          c->cd();
          c->SetLogy();
          h2->GetXaxis()->SetRangeUser(0, 4096);
          h2->SetTitle(TString::Format("./fig/e_dc_p_1000fs_spider_p_ring%d_sector%d_ge_ring%d_sector%d",i+1,j+1,rn,ii));
          h2->Draw();
          c->SaveAs(TString::Format("./fig/e_dc_p_1000fs_spider_p_ring%d_sector%d_ge_ring%d_sector%d.pdf",i+1,j+1,rn,ii));

          c->cd();
          c->SetLogy();
          h3->GetXaxis()->SetRangeUser(0, 4096);
          h3->SetTitle(TString::Format("./fig/e_dc_r_1000fs_spider_p_ring%d_sector%d_ge_ring%d_sector%d",i+1,j+1,rn,ii));
          h3->Draw();
          c->SaveAs(TString::Format("./fig/e_dc_r_1000fs_spider_p_ring%d_sector%d_ge_ring%d_sector%d.pdf",i+1,j+1,rn,ii));
    
        }
      }
    }
  }

}
