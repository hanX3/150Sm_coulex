//
// str_ge example = "ge_all"
// str_p example = "spider"
void get_f3_format(string str_ge="ge_all", string str_p="spider")
{
  int nn_rings = 0;
  if(str_p == "spider"){
    nn_rings = 8;
  }else if(str_p == "s3"){
    nn_rings = 24;
  }else{
    cout << "wrong string particle" << endl;
    return;
  }

  vector<int> v_nn_peaks;

  //
  map<int, pair<int,int>> m_e_indexs = {
    {(int)334.0, {2,1}},
    {(int)406.5, {10,2}},
    {(int)439.4, {3,2}},
    {(int)505.5, {4,3}},
    {(int)584.3, {16,3}},
    {(int)675.9, {12,3}},
    {(int)712.2, {11,2}},
    {(int)737.5, {15,2}},
    {(int)831.8, {14,2}},
    {(int)859.9, {7,2}},
    {(int)1046.2, {11,1}},
    {(int)1165.7, {14,1}},
    {(int)1193.8, {7,1}},
    {(int)1308.7, {8,2}},
    {(int)1350.3, {19,1}}
  };

  //
  ifstream fi(Form("./result/%s_%s_final.txt",str_ge.c_str(),str_p.c_str()));
  if(!fi){
    cout << "can not open yield file" << endl;
    return;
  }

  map<pair<int,int>, pair<double,double>> m_r_e_area;
  int ring;
  double e, a, ae;
  while(fi>>ring>>e>>a>>ae){
    cout << ring << "  " << e << "  " << a << "  " << ae << endl;
    m_r_e_area.insert({{ring,(int)e}, {a,ae}});;
  }
  fi.close();

  for(auto &[key,val]:m_r_e_area){
    cout << key.first << "  " << key.second << endl;
    cout << val.first << "  " << val.second << endl;
  }

  //
  int nn_peaks = 0;
  ofstream fo(Form("result/%s_%s.f3",str_ge.c_str(),str_p.c_str()));

  for(int i=0;i<nn_rings;i++){
    nn_peaks = 0;
    for(auto &[key,val]:m_r_e_area){
      if(key.first == i+1){
        nn_peaks++;
      }
    }

    v_nn_peaks.push_back(nn_peaks);
  }

  for(int i=0;i<nn_rings;i++){
    fo << i+1 << " 1 16 32 100 " << v_nn_peaks[i] << " 1" << endl;
    for(auto &[key,val]:m_r_e_area){
      if(key.first == i+1){
        fo << "  " << m_e_indexs[(int)(key.second)].first << "  " << m_e_indexs[(int)(key.second)].second << "  " << val.first << "  " << val.second << endl;
      }
    }

  }

  fo.close();
}
