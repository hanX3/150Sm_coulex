//
void draw_addback()
{
  TFile *fi1 = TFile::Open("../../rootfile/data0472_build_200ns_jump_300ns_no_ab.root");
  if(fi1->IsZombie()){
    cout << "can not open file 1." << endl;
  }

  TFile *fi2 = TFile::Open("../../rootfile/data0588_build_200ns_jump_300ns.root");
  if(fi2->IsZombie()){
    cout << "can not open file 2." << endl;
  }

  TTree *tr1 = (TTree*)fi1->Get("tr_event");
  TTree *tr2 = (TTree*)fi2->Get("tr_event");

  //
  TCanvas *cc2 = new TCanvas("cc2", "", 900, 400);
  cc2->SetLogy();
  cc2->cd();
  TH1D *h2 = new TH1D("h2", "", 4096,0,4096);
  TH1D *h2_ab = new TH1D("h2_ab", "", 4096,0,4096);
  tr1->Draw("ge_energy>>h2","ge_sid==2&&ge_ch>=12&&ge_ch<=15","goff");
  tr2->Draw("ge_energy>>h2_ab","ge_sid==2&&ge_ch>=12&&ge_ch<=15","goff");
  h2->SetLineColor(2);
  h2_ab->SetLineColor(4);
  h2->Draw();
  h2_ab->Draw("same");
  TLegend *leg2 = new TLegend(0.7, 0.4, 0.98, 0.95);
  leg2->AddEntry(h2, "no addback");
  leg2->AddEntry(h2_ab, "addback");
  leg2->Draw("same");
  cc2->SaveAs("clover2.png");

  //
  TCanvas *cc3 = new TCanvas("cc3", "", 900, 400);
  cc3->SetLogy();
  cc3->cd();
  TH1D *h3 = new TH1D("h3", "", 4096,0,4096);
  TH1D *h3_ab = new TH1D("h3_ab", "", 4096,0,4096);
  tr1->Draw("ge_energy>>h3","ge_sid==4&&ge_ch>=0&&ge_ch<=3","goff");
  tr2->Draw("ge_energy>>h3_ab","ge_sid==4&&ge_ch>=0&&ge_ch<=3","goff");
  h3->SetLineColor(2);
  h3_ab->SetLineColor(4);
  h3->Draw();
  h3_ab->Draw("same");
  TLegend *leg3 = new TLegend(0.7, 0.4, 0.98, 0.95);
  leg3->AddEntry(h3, "no addback");
  leg3->AddEntry(h3_ab, "addback");
  leg3->Draw("same");
  cc3->SaveAs("clover3.png");

  //
  TCanvas *cc4 = new TCanvas("cc4", "", 900, 400);
  cc4->SetLogy();
  cc4->cd();
  TH1D *h4 = new TH1D("h4", "", 4096,0,4096);
  TH1D *h4_ab = new TH1D("h4_ab", "", 4096,0,4096);
  tr1->Draw("ge_energy>>h4","ge_sid==4&&ge_ch>=4&&ge_ch<=7","goff");
  tr2->Draw("ge_energy>>h4_ab","ge_sid==4&&ge_ch>=4&&ge_ch<=7","goff");
  h4->SetLineColor(2);
  h4_ab->SetLineColor(4);
  h4->Draw();
  h4_ab->Draw("same");
  TLegend *leg4 = new TLegend(0.7, 0.4, 0.98, 0.95);
  leg4->AddEntry(h4, "no addback");
  leg4->AddEntry(h4_ab, "addback");
  leg4->Draw("same");
  cc4->SaveAs("clover4.png");

  //
  TCanvas *cc5 = new TCanvas("cc5", "", 900, 400);
  cc5->SetLogy();
  cc5->cd();
  TH1D *h5 = new TH1D("h5", "", 4096,0,4096);
  TH1D *h5_ab = new TH1D("h5_ab", "", 4096,0,4096);
  tr1->Draw("ge_energy>>h5","ge_sid==4&&ge_ch>=9&&ge_ch<=11","goff");
  tr2->Draw("ge_energy>>h5_ab","ge_sid==4&&ge_ch>=9&&ge_ch<=11","goff");
  h5->SetLineColor(2);
  h5_ab->SetLineColor(4);
  h5->Draw();
  h5_ab->Draw("same");
  TLegend *leg5 = new TLegend(0.7, 0.4, 0.98, 0.95);
  leg5->AddEntry(h5, "no addback");
  leg5->AddEntry(h5_ab, "addback");
  leg5->Draw("same");
  cc5->SaveAs("clover5.png");
}
