//
void hist2txt(string file_name, string h_name)
{
  TFile *fi = TFile::Open(file_name.c_str());
  if(fi->IsZombie()){
    cout << "cannot open " << file_name << endl;
    return;
  }

  fi->Print();
  TH1D *h_event = (TH1D*)fi->Get(TString::Format("event_%s",h_name.c_str()).Data());
  TH1D *h_bg = (TH1D*)fi->Get(TString::Format("bg_%s",h_name.c_str()).Data());
  h_event->Print();
  h_bg->Print();
  if(h_event->GetEntries()==0 || h_bg->GetEntries()==0){
    cout << "empty h" << endl;
    return;
  }

  h_event->Add(h_bg, -1);

  h_event->Draw();

  ofstream fo(TString::Format("%s.txt",h_name.c_str()).Data());
  for(int i=0;i<h_event->GetNbinsX();++i){
    fo << i << " " << h_event->GetBinContent(i+1) << endl;
  }

  fo.close();
  // fi->Close();
}
