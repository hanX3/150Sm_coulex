//
void get_hist(int run)
{
  TFile *fi = TFile::Open(Form("../rootfile/data%04d.root", run));
  if(fi->IsZombie()){
    cout << "can not open rootfile " << std::endl;
    return;
  }

  Short_t cid;
  Short_t sid;
  Short_t ch;
  Double_t evte;

  TTree *tr = (TTree*)fi->Get("tree");

  tr->SetBranchAddress("cid", &cid); 
  tr->SetBranchAddress("sid", &sid); 
  tr->SetBranchAddress("ch", &ch); 
  tr->SetBranchAddress("evte", &evte); 

  std::map<int, TH1D*> m_h;
  
  int key = 0;
  TString h_name;
  for(Long64_t i=0;i<tr->GetEntries();i++){
    tr->GetEntry(i);
    key = 10000*cid+100*sid+ch;

    if(m_h.count(key)){
      m_h[key]->Fill(evte);
    }else{
      h_name = TString::Format("h_cid%d_sid%02d_ch%02d", cid, sid, ch);
      if(cid==0)
        m_h[key] = new TH1D(h_name.Data(), h_name.Data(), 4096, 0, 4096);
      if(cid==1)
        m_h[key] = new TH1D(h_name.Data(), h_name.Data(), 1000, 0, 100000);

      m_h[key]->Fill(evte);
    }

    if(i%100000==0){
      std::cout << "\r" << i << "/" << tr->GetEntries();
      std::cout << std::flush;
    }
  }
  std::cout << std::endl;

  //
  TFile *fo = new TFile(TString::Format("../rootfile/data%04d_hist.root",run).Data(), "recreate");

  map<int, TDirectory*> m_dir;
  for(const auto &it : m_h){
    fo->cd();
    m_dir[it.first/100] = (TDirectory*)fo->mkdir(TString::Format("cid%d_sid%02d", it.first/10000, it.first/100%100).Data(), "", true);
  }

  for(const auto &it : m_h){
    fo->cd();
    m_dir[it.first/100]->cd();
    it.second->Write();
  }

  fo->Close();
}
