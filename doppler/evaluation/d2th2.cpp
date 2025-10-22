#include "../set.h"

//
// example str = "data0472_0605_doppler_100ns_jump_300ns"
void d2th2(string str="")
{
  TRandom3 *rndm = new TRandom3((Long64_t)time(0)); 

  TFile *fi = TFile::Open(Format("../../rottfile/%s.root",str.c_str()));
  if(fi->IsZombie()){
    std::cout << "open file run error!" << std::endl;

    return ;
  }

  TTree *tr_event = (TTree*)fi->Get("tr_event");
  TTree *tr_bg = (TTree*)fi->Get("tr_bg");

  Int_t hits_event;
  Double_t e_dc_r_event[GENUM];
  tr_event->SetBranchAddress("hits", &hits_event);
  tr_event->SetBranchAddress("e_dc_r", e_dc_r_event);

  Int_t hits_bg;
  Double_t e_dc_r_bg[GENUM];
  tr_bg->SetBranchAddress("hits", &hits_bg);
  tr_bg->SetBranchAddress("e_dc_r", e_dc_r_bg);

  TH2I *hh_event = new TH2I("hh_event","",4096,0,4096,4096,0,4096);
  TH2I *hh_bg = new TH2I("hh_bg","",4096,0,4096,4096,0,4096);

  for(Long64_t i=0;i<tr_event->GetEntries();++i){
    if(i%1000000==0) cout << i << "/" << tr_event->GetEntries() << endl;

    tr_event->GetEntry(i);
    if(hits_event<2) continue;

    for(int j=0;j<hits_event;++j){
      for(int jj=j+1;jj<hits_event;++jj){
        hh_event->Fill(e_dc_r_event[j], e_dc_r_event[jj]);
        hh_event->Fill(e_dc_r_event[jj], e_dc_r_event[j]);
      }
    }
  }

  for(Long64_t i=0;i<tr_bg->GetEntries();++i){
    if(i%1000000==0) cout << i << "/" << tr_bg->GetEntries() << endl;

    tr_bg->GetEntry(i);
    if(hits_bg<2) continue;

    for(int j=0;j<hits_bg;++j){
      for(int jj=j+1;jj<hits_bg;++jj){
        hh_bg->Fill(e_dc_r_bg[j], e_dc_r_bg[jj]);
        hh_bg->Fill(e_dc_r_bg[jj], e_dc_r_bg[j]);
      }
    }
  }

  TCanvas *cc1 = new TCanvas("cc1","",600,500);
  cc1->cd();
  hh_event->Draw("colz");

  TCanvas *cc2 = new TCanvas("cc2","",600,500);
  cc2->cd();
  hh_bg->Draw("colz");

  hh_event->Add(hh_bg, -1);

  TFile *fo = new TFile(TString::Format("%s_th2i.root",file_name.c_str()), "recreate");
  fo->cd();
  hh_event->Write();
  fo->Close();
}
