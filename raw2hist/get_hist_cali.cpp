//
void get_hist_cali(TString filename, int run)
{
  TFile *fi;
  TTree *tr;
  fi = TFile::Open(filename.Data());
  if(fi->IsZombie()){
    cout << "cannot open the file " << filename << std::endl;
    return;
  }

  Short_t cid;
  Short_t sid;
  Short_t ch;
  UShort_t evte;

  tr = (TTree*)fi->Get("tree");

  tr->SetBranchAddress("cid", &cid); 
  tr->SetBranchAddress("sid", &sid); 
  tr->SetBranchAddress("ch", &ch); 
  tr->SetBranchAddress("evte", &evte);

  // cali par
  std::map<int, std::vector<double>> map_cali_data;
  std::cout << "read cali data" << std::endl;

  std::ifstream fi_cali;
  fi_cali.open("../pars/cali.txt");
  // fi_cali.open(TString::Format("../cali/cali_%04d.txt", run).Data());
  if(!fi_cali){
    std::cout << "can not open cali.txt" << std::endl;
    return ;
  }

  std::string line;
  std::getline(fi_cali, line);

  double par0, par1, par2, chi2;
  int key = 0;

  while(1){
    fi_cali >> cid >> sid >> ch >> par0 >> par1 >> par2 >> chi2;
    if(!fi_cali.good()) break;

    key = 10000*cid+100*sid+ch; // ch from 0 to 15
    std::vector<double> value;
    value.push_back(par0);
    value.push_back(par1);
    value.push_back(par2);
    value.push_back(chi2);

    map_cali_data.insert(std::pair<int, std::vector<double>>(key, value));
  }

  fi_cali.close();

  std::cout << "print cali data" << std::endl;

  std::map<int, std::vector<double>>::iterator it = map_cali_data.begin();
  for(it=map_cali_data.begin();it!=map_cali_data.end();it++){
    std::cout << it->first << " => " << it->second[0] << " " << it->second[1] << " " << it->second[2] << " " << it->second[3] << '\n';
  }

  TRandom3 *rndm = new TRandom3((Long64_t)time(0));

  std::map<int, TH1D*> m_h;
  
  double e = 0;
  TString h_name;
  for(Long64_t i=0;i<tr->GetEntries();i++){
    tr->GetEntry(i);
    key = 10000*cid+100*sid+ch;
    e = map_cali_data[key][0] + evte*map_cali_data[key][1] + evte*evte*map_cali_data[key][2] + rndm->Uniform(-0.5, 0.5);

    if(m_h.count(key)){
      m_h[key]->Fill(e);
    }else{
      h_name = TString::Format("h_cid%d_sid%02d_ch%02d", cid, sid, ch);
      m_h[key] = new TH1D(h_name.Data(), h_name.Data(), 8192, 0, 4096);
      m_h[key]->Fill(e);
    }

    if(i%100000==0){
      std::cout << "\r" << i << "/" << tr->GetEntries();
      std::cout << std::flush;
    }
  }
  std::cout << std::endl;

  //
  TFile *fo = new TFile(TString::Format("../rootfile/ge/data%04d_hist_cali.root",run).Data(), "recreate");

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
