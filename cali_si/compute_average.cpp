//
void compute_average(int run)
{
  vector<pair<int, int>> v_p = {
    {4, 0},
    {4, 1},
    {4, 3},
    {4, 7},
    {4, 13},
    {6, 7},
    {7, 7}
  };

  //
  ifstream fi(TString::Format("./cali_%04d.txt",run).Data());
  if(!fi){
    cout << "can not open the file." << endl;
    return;
  }

  string line;
  getline(fi, line);

  int cid, sid, ch;
  int key;
  double par0, par1, par2, chi2;
  map<int, vector<double>> m_p0, m_p1, m_p2, m_chi2;
  for(int i=2;i<=12;i++){
    m_p0[i] = {};
    m_p1[i] = {};
    m_p2[i] = {};
    m_chi2[i] = {};
  }

  while(1){
    fi >> cid >> sid >> ch >> par0 >> par1 >> par2 >> chi2;
    if(!fi.good()) break;
 
    m_p0[sid].push_back(par0);
    m_p1[sid].push_back(par1);
    m_p2[sid].push_back(par2);
    m_chi2[sid].push_back(chi2);
  }

  for(int i=2;i<=12;i++){
    for(int j=0;j<16;j++){
      cout << "sid " << i << " p0 " << m_p0[i][j] << " p1 " << m_p1[i][j] << " p2 " << m_p2[i][j] << endl;
    }
    cout << endl;
  }

  //
  for(auto &it : v_p){
    m_p0[it.first][it.second] = 0;
    m_p1[it.first][it.second] = 0;
    m_p2[it.first][it.second] = 0;
    m_chi2[it.first][it.second] = 0;
  }

  for(int i=2;i<=12;i++){
    for(int j=0;j<16;j++){
      cout << "sid " << i << " p0 " << m_p0[i][j] << " p1 " << m_p1[i][j] << " p2 " << m_p2[i][j] << endl;
    }
    cout << endl;
  }

  //
  map<int, vector<double>> m_ave;
  for(int i=2;i<=12;i++){
    m_ave[i] = {0., 0., 0., 0.};
  }

  int n = 0;
  for(int i=2;i<=12;i++){
    n = 0;
    for(int j=0;j<16;j++){
      if(m_p0[i][j] != 0) n++;

      m_ave[i][0] += m_p0[i][j];
      m_ave[i][1] += m_p1[i][j];
      m_ave[i][2] += m_p2[i][j];
      m_ave[i][3] += m_chi2[i][j];
    }
    if(n==0) continue;

    m_ave[i][0] /= (double)n;
    m_ave[i][1] /= (double)n;
    m_ave[i][2] /= (double)n;
    m_ave[i][3] /= (double)n;
  }

  for(int i=2;i<=12;i++){
    cout << i << " p0 " << m_ave[i][0]  << " p1 " << m_ave[i][1]  << " p2 " << m_ave[i][2] << endl;
  }

  //
  ofstream fo(TString::Format("./cali_%04d_new.txt",run).Data());

  fi.clear();
  fi.seekg(0, std::ios::beg);
  getline(fi, line);
  fo << line << endl;
  
  TString str_out;
  while(1){
    fi >> cid >> sid >> ch >> par0 >> par1 >> par2 >> chi2;
    if(!fi.good()) break;
 
    if(std::find(v_p.begin(), v_p.end(), std::make_pair(sid,ch))!=v_p.end()){
      str_out = Form("%4d  %4d  %4d  %16.9f  %16.9f  %16.9f  %16.9f", cid, sid, ch, m_ave[sid][0], m_ave[sid][1], m_ave[sid][2], m_ave[sid][3]);
      fo << str_out << endl;
    }else{
      str_out = Form("%4d  %4d  %4d  %16.9f  %16.9f  %16.9f  %16.9f", cid, sid, ch, par0, par1, par2, chi2);
      fo << str_out << endl;
    }
  }

  fi.close();
  fo.close();
}
