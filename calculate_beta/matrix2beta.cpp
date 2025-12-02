//
void matrix2beta()
{
  //
  vector<pair<int,int>> v_e2_index = {
    {1,2},
    {10,11},
    {7,8}
  };

  vector<pair<int,int>> v_e3_index = {
    {1,15},
    {1,19}
  };

  //
  map<int, int> m_id2spin = {
    {1, 0},
    {2, 2},
    {3, 4},
    {4, 6},
    {5, 8},
    {6, 10},
    {7, 2},
    {8, 4},
    {9, 6},
    {10, 0},
    {11, 2},
    {12, 4},
    {13, 6},
    {14, 1},
    {15, 3},
    {16, 5},
    {17, 7},
    {18, 9},
    {19, 3}
  };

  //
  ifstream fi_e2("E2.out");
  if(!fi_e2){
    cout << "can not open E2.out" << endl;
    return;
  }

  string line;
  getline(fi_e2, line);
  cout << line << endl;
  getline(fi_e2, line);
  cout << line << endl;

  int index, l1, l2;
  double me;

  map<pair<int,int>,double> m_be2up;
  double be2up;
  map<pair<int,int>,double> m_beta2;
  double beta2;

  while(getline(fi_e2, line)){
    stringstream ss(line);
    ss >> index >> l1 >> l2 >> me;
    cout << index << " " << l1 << " " << l2 << " " << me << endl;

    if(std::find(v_e2_index.begin(),v_e2_index.end(),std::pair{l1,l2}) != v_e2_index.end()){
      be2up = 1/(2.*m_id2spin[l1]+1)*me*me;
      m_be2up.insert({pair<int,int>{l1,l2}, be2up});
      
      beta2 = (4*TMath::Pi())/(3*62*1.2*1.2*pow(150.,2/3.)/100.)*sqrt(be2up);
      m_beta2.insert({pair<int,int>{l1,l2}, beta2});
    }
  }
  fi_e2.close();

  cout << endl;
  for(auto &[key,val]:m_be2up){
    cout << m_id2spin[key.first] << " --> " << m_id2spin[key.second] << " be2 up " << val << endl;
  }

  for(auto &[key,val]:m_beta2){
    cout << m_id2spin[key.first] << " --> " << m_id2spin[key.second] << " beta2 " << val << endl;
  }

  //
  ifstream fi_e3("E3.out");
  if(!fi_e3){
    cout << "can not open E3.out" << endl;
    return;
  }

  getline(fi_e3, line);
  cout << line << endl;
  getline(fi_e3, line);
  cout << line << endl;

  map<pair<int,int>,double> m_be3up;
  double be3up;
  map<pair<int,int>,double> m_beta3;
  double beta3;

  while(getline(fi_e3, line)){
    stringstream ss(line);
    ss >> index >> l1 >> l2 >> me;
    cout << index << " " << l1 << " " << l2 << " " << me << endl;

    if(std::find(v_e3_index.begin(),v_e3_index.end(),std::pair{l1,l2}) != v_e3_index.end()){
      be3up = 1/(2.*m_id2spin[l1]+1)*me*me;
      m_be3up.insert({pair<int,int>{l1,l2}, be3up});
      
      beta3 = (4*TMath::Pi())/(3*62*1.2*1.2*1.2*150/1000.)*sqrt(be3up);
      m_beta3.insert({pair<int,int>{l1,l2}, beta3});
    }
  }
  fi_e3.close();

  cout << endl;
  for(auto &[key,val]:m_be3up){
    cout << m_id2spin[key.first] << " --> " << m_id2spin[key.second] << " be3 up " << val << endl;
  }

  for(auto &[key,val]:m_beta3){
    cout << m_id2spin[key.first] << " --> " << m_id2spin[key.second] << " beta3 " << val << endl;
  }
}
