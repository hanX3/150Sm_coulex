//
void gate(int e1, int e2)
{
  int colors[12] = {
    TColor::GetColor("#E41A1C"), TColor::GetColor("#377EB8"),
    TColor::GetColor("#4DAF4A"), TColor::GetColor("#984EA3"),
    TColor::GetColor("#FF7F00"), TColor::GetColor("#A65628"),
    TColor::GetColor("#F781BF"), TColor::GetColor("#999999"),
    TColor::GetColor("#66C2A5"), TColor::GetColor("#FC8D62"),
    TColor::GetColor("#8DA0CB"), TColor::GetColor("#FFD92F")
  };

  TCanvas *c1 = new TCanvas(TString::Format("c1_%d-%d",e1,e2),"",0,0,1200,360);
  TCanvas *c2 = new TCanvas(TString::Format("c2_%d-%d",e1,e2),"",0,0,1200,360);

  TTree * tr_event = (TTree*)gFile->Get("tr_event");
  TTree * tr_bg = (TTree*)gFile->Get("tr_bg");

  raw *rd_event = new raw(tr_event);
  raw *rd_bg = new raw(tr_bg);

  TH1D *h_event = new TH1D("h_event", "", 4096, 0, 4096);
  TH1D *h_bg = new TH1D("h_bg", "", 4096, 0, 4096);

  h_event->SetName(TString::Format("event_%d-%d",e1,e2));
  h_bg->SetName(TString::Format("bg_%d-%d",e1,e2));

  //
  cout <<  rd_event->GetEntries() << endl;
  for(Long64_t i=0;i<rd_event->GetEntries();i++){
    if(i%(rd_event->GetEntries()/10)==0) cout << i << "/" << rd_event->GetEntries() << endl;
    rd_event->GetEntry(i);
    
    if(rd_event->hits<2 || rd_event->who!=1) continue;

    for(int j=0;j<rd_event->hits;j++){
      if(rd_event->e_dc_r_1000fs[j]>=e1 && rd_event->e_dc_r_1000fs[j]<=e2){
        for(int jj=0;jj<rd_event->hits;jj++){
          if(j==jj) continue;
          else h_event->Fill(rd_event->e_dc_r_1000fs[jj]);
        }
      }
    }
  }

  //
  cout <<  rd_bg->GetEntries() << endl;
  for(Long64_t i=0;i<rd_bg->GetEntries();i++){
    if(i%(rd_bg->GetEntries()/10)==0) cout << i << "/" << rd_bg->GetEntries() << endl;
    rd_bg->GetEntry(i);
    
    if(rd_bg->hits<2 || rd_bg->who!=1) continue;

    for(int j=0;j<rd_bg->hits;j++){
      if(rd_bg->e_dc_r_1000fs[j]>=e1 && rd_bg->e_dc_r_1000fs[j]<=e2){
        for(int jj=0;jj<rd_bg->hits;jj++){
          if(j==jj) continue;
          else h_bg->Fill(rd_bg->e_dc_r_1000fs[jj]);
        }
      }
    }
  }

  //
  h_event->SetLineColor(colors[0]);
  h_bg->SetLineColor(colors[1]);

  c1->cd();
  c1->SetLogy();
  h_event->Draw();
  h_bg->Draw("same");

  //
  TH1D *h_event_no_bg = (TH1D*)h_event->Clone(TString::Format("event-bg_%d-%d",e1,e2));
  h_event_no_bg->Add(h_bg, -1);

  c2->cd();
  c2->SetLogy();
  h_event_no_bg->Draw();


}
