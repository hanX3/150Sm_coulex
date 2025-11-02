//
double eff_fun(double *x, double *p);

// str_ge example = "ge_all"
// str_p example = "spider"
void get_yield_eff(string str_ge="ge_all", string str_p="spider")
{
  // eff pars
  map<string, vector<double>> m_ge_eff_pars;
  m_ge_eff_pars["ge_all"] = {14.8, 4.618, 0., 10.8, -0.7352, -0.1101, 15};
  m_ge_eff_pars["ge_ring3"] = {11.23, 2.49, 0., 9.432, -0.6368, -0.005044, 15};
  m_ge_eff_pars["ge_ring4"] = {11.07, 1.767, 0., 9.663, -0.5738, 0.0251, 15};
  m_ge_eff_pars["ge_ring5"] = {11.06, 0.2759, 0., 9.963, -0.7139, 0.1796, 15};

  // error 
  map<string, double> m_ge_eff_err;
  m_ge_eff_err["ge_all"] = 2.84/100.;
  m_ge_eff_err["ge_ring3"] = 1.78/100.;
  m_ge_eff_err["ge_ring4"] = 1.58/100.;
  m_ge_eff_err["ge_ring4"] = 2.01/100.;

  //
  TF1 *tf = new TF1("tf", eff_fun, 50, 2048, 7);
  for(int i=0;i<7;i++) tf->SetParameter(i, m_ge_eff_pars[str_ge.c_str()][i]);
  double max =  tf->GetMaximum();

  //
  ifstream fi(Form("result/%s_%s.yld",str_ge.c_str(),str_p.c_str()));
  if(!fi){
    cout << "can not open yield file" << endl;
    return;
  }
  std::string line;
  std::getline(fi, line);

  ofstream fo(Form("result/%s_%s_final.txt",str_ge.c_str(),str_p.c_str()));
  int ring;
  double e, area, area_error;
  double area_final, area_error_final;
  while(fi>>ring>>e>>area>>area_error){
    area = area/(tf->Eval(e)/max);
    area_error = area_error/(tf->Eval(e)/max);
    area_error = sqrt(area_error*area_error+(area*m_ge_eff_err[str_ge.c_str()])*(area*m_ge_eff_err[str_ge.c_str()])+(area*0.05)*(area*0.05));

    fo << ring << "  " << e << "  " << area << "  " << area_error << endl;
  }

  fi.close();
  fo.close();
}

//
double eff_fun(double *x, double *p)
{
  double x1 = log(x[0]/100.);
  double x2 = log(x[0]/1000.);

  double z1 = pow(p[0]+p[1]*x1+p[2]*x1*x1, -p[6]);
  double z2 = pow(p[3]+p[4]*x2+p[5]*x2*x2, -p[6]);

  double eff = exp(pow(z1+z2, -1./p[6]));

  return eff;
}

