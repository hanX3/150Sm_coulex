//
bool InitMapRingSector(int &run, map<int, int> &m_s, map<int, int> &m_r);

//
void get_hist_cali(TString filename, int run)
{
  TFile *fi;
  TTree *tr;
  fi = TFile::Open(filename.Data());
  if(fi->IsZombie()){
    cout << "can not open the file " << filename << std::endl;
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
  if(run==460 || (run>=472 && run<=605)){
    fi_cali.open("../pars/cali_si_r460.txt");
  }else if((run>=616 && run<=674) || run==722){
    fi_cali.open("../pars/cali_si_r457_low_current.txt");
  }else if(run==457 || (run>=680 && run<=718)){
    fi_cali.open("../pars/cali_si_r457_high_current.txt");
  }else{
    std::cout << "wrong run number." << std::endl;
    return ;
  }

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

  std::map<int, std::vector<double>>::iterator it_cali = map_cali_data.begin();
  for(;it_cali!=map_cali_data.end();it_cali++){
    std::cout << it_cali->first << " => " << it_cali->second[0] << " " << it_cali->second[1] << " " << it_cali->second[2] << " " << it_cali->second[3] << '\n';
  }

  // fb cor par
  map<int, int> map_sector, map_ring;
  InitMapRingSector(run, map_sector, map_ring);

  map<int, vector<double>> map_sector_cor_par, map_ring_cor_par;

  int id;
  ifstream fi_fb_cor_sector(TString::Format("../pars/run_fb_cor/correction_sector_%04d.txt",run).Data());
  if(!fi_fb_cor_sector){
    cout << "can not open sector correction file." << endl;
    for(int i=1;i<=32;i++){
      map_sector_cor_par[i] = {0, 1};
    }
  }else{
    std::getline(fi_fb_cor_sector, line);
    
    while(1){
      fi_fb_cor_sector >> id >> par0 >> par1;
      if(!fi_fb_cor_sector.good()) break;

      map_sector_cor_par[id] = {par0, par1};
    }
    fi_fb_cor_sector.close();
  }

  ifstream fi_fb_cor_ring(TString::Format("../pars/run_fb_cor/correction_ring_%04d.txt",run).Data());
  if(!fi_fb_cor_ring){
    cout << "can not open ring correction file." << endl;
    for(int i=1;i<=24;i++){
      map_ring_cor_par[i] = {0, 1};
    }
  }else{
    std::getline(fi_fb_cor_ring, line);
    
    while(1){
      fi_fb_cor_ring >> id >> par0 >> par1;
      if(!fi_fb_cor_ring.good()) break;

      map_ring_cor_par[id] = {par0, par1};
    }
    fi_fb_cor_ring.close();
  }

  //
  TRandom3 *rndm = new TRandom3((Long64_t)time(0));

  std::map<int, TH1D*> m_h;
  TH2D *hh_ring = new TH2D("hh_ring", "", 50, 0, 50, 10000, 0, 100000);
  TH2D *hh_sector = new TH2D("hh_sector", "", 50, 0, 50, 10000, 0, 100000);
  
  double e = 0;
  TString h_name;
  for(Long64_t i=0;i<tr->GetEntries();i++){
    tr->GetEntry(i);
    key = 10000*cid+100*sid+ch;
    e = map_cali_data[key][0] + evte*map_cali_data[key][1] + evte*evte*map_cali_data[key][2] + rndm->Uniform(0., 0.5);
    if(map_sector.find(key)!=map_sector.end()){
      e = map_sector_cor_par[map_sector[key]][0] + map_sector_cor_par[map_sector[key]][1]*e;
      hh_sector->Fill(map_sector[key], e);
    }

    if(map_ring.find(key)!=map_ring.end()){
      e = map_ring_cor_par[map_ring[key]][0] + map_ring_cor_par[map_ring[key]][1]*e;
      hh_ring->Fill(map_ring[key], e);
    }

    if(m_h.count(key)){
      m_h[key]->Fill(e);
    }else{
      h_name = TString::Format("h_cid%d_sid%02d_ch%02d", cid, sid, ch);
      m_h[key] = new TH1D(h_name.Data(), h_name.Data(), 5000, 0, 100000);
      m_h[key]->Fill(e);
    }

    if(i%100000==0){
      std::cout << "\r" << i << "/" << tr->GetEntries();
      std::cout << std::flush;
    }
  }
  std::cout << std::endl;

  //
  TFile *fo = new TFile(TString::Format("../rootfile/si/data%04d_hist_cali.root",run).Data(), "recreate");

  map<int, TDirectory*> m_dir;
  for(const auto &it : m_h){
    fo->cd();
    m_dir[it.first/100] = (TDirectory*)fo->mkdir(TString::Format("cid%d_sid%02d", it.first/10000, it.first/100%100).Data(), "", true);
  }

  fo->cd();
  hh_ring->Write();
  hh_sector->Write();

  for(const auto &it : m_h){
    fo->cd();
    m_dir[it.first/100]->cd();
    it.second->Write();
  }

  fo->Close();
}

//
bool InitMapRingSector(int &run, map<int, int> &m_s, map<int, int> &m_r)
{
  std::cout << "start init ring and sector id." << std::endl;

  if((run>=457 && run<=462) || (run>=472 && run<=605) || (run>=680 && run<=718)){
    m_s = {
      {10900, 10},
      {10901, 12},
      {10902, 14},
      {10903, 16},
      {10904, 18},
      {10905, 20},
      {10906, 22},
      {10907, 24},
      {10908, 26},
      {10909, 28},
      {10910, 30},
      {10911, 32},
      {10912, 2},
      {10913, 4},
      {10914, 6},
      {10915, 8},
      {11000, 11},
      {11001, 13},
      {11002, 15},
      {11003, 17},
      {11004, 19},
      {11005, 21},
      {11006, 23},
      {11007, 25},
      {11008, 27},
      {11009, 29},
      {11010, 31},
      {11011, 1},
      {11012, 3},
      {11013, 5},
      {11014, 7},
      {11015, 9}
    };
  }else if((run>=616 && run<=674) || (run>=721 && run<=723)){
    m_s = {
      {10200, 10},
      {10201, 12},
      {10202, 14},
      {10203, 16},
      {10204, 18},
      {10205, 20},
      {10206, 22},
      {10207, 24},
      {10208, 26},
      {10209, 28},
      {10210, 30},
      {10211, 32},
      {10212, 2},
      {10213, 4},
      {10214, 6},
      {10215, 8},
      {10300, 11},
      {10301, 13},
      {10302, 15},
      {10303, 17},
      {10304, 19},
      {10305, 21},
      {10306, 23},
      {10307, 25},
      {10308, 27},
      {10309, 29},
      {10310, 31},
      {10311, 1},
      {10312, 3},
      {10313, 5},
      {10314, 7},
      {10315, 9}
    };
  }else{
  std::cout << "wrong run number." << std::endl;
    return 0;
  }

  m_r = {
    {11102, 2},
    {11103, 4},
    {11104, 6},
    {11105, 8},
    {11106, 10},
    {11107, 12},
    {11108, 14},
    {11109, 16},
    {11110, 18},
    {11111, 20},
    {11112, 22},
    {11113, 24},
    {11200, 1},
    {11201, 3},
    {11202, 5},
    {11203, 7},
    {11204, 9},
    {11205, 11},
    {11206, 13},
    {11207, 15},
    {11208, 17},
    {11209, 19},
    {11210, 21},
    {11211, 23}
  };

  return 1;
}

