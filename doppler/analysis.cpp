void analysis()
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

  TCanvas *c1 = new TCanvas("c1","",0,0,500,360);

  TH1D *h1, *hh1;
  h1 = (TH1D*)gFile->Get("event_e_raw_spider_p");
  hh1 = (TH1D*)gFile->Get("bg_e_raw_spider_p");

  h1->Add(hh1, -1);

  c1->cd();
  c1->SetLogy();

  h1->Draw();
}
