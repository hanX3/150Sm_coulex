//
double greedy_match(const vector<double> &energies, const vector<double>& channels, double k, double tol, vector<pair<double, double>> &matches);
double compute_k(const vector<double> &energies, const vector<double> &channels, int iterations, double tol, vector<pair<double, double>> &best_matches);

// for standard source data
double e_3a[3] = {5157., 5486., 5805.};
double e_2a[2] = {5486., 5805.};

//search peak value
double threshold = 0.05;
//search peak sigma
double sigma = 4.0;

//
int auto_cali(TH1D *h, Double_t *par, bool flag_3a_2a = 1)
{
  int i = 0;
  int npeaks = 20;
  if(!h){
    cout << "get hist error, check!" << endl;
    return -1;
  }

  //standard source data
  vector<double> v_source;
  if(flag_3a_2a){
    for(i=0;i<(sizeof(e_3a)/sizeof(e_3a[0]));i++){
      v_source.push_back(e_3a[i]);
    }
  }else{
    for(i=0;i<(sizeof(e_2a)/sizeof(e_2a[0]));i++){
      v_source.push_back(e_2a[i]);
    }
  }

  sort(v_source.begin(), v_source.end());

  cout << "source info " << endl;
  for(auto it=v_source.begin();it!=v_source.end();++it){
    cout << *it << endl;
  }
  cout << endl;

  //Use TSpectrum to find the peak candidates
  TCanvas *c1 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c1");
  if(!c1) c1 = new TCanvas("c1", "c1", 0, 0, 480, 360);
  TSpectrum *s = new TSpectrum(2*npeaks);
  Int_t nfound = s->Search(h, sigma, "", threshold);
  Double_t *xpos = s->GetPositionX();
  Double_t *ypos = s->GetPositionY();
  if(nfound>v_source.size()+20){
    cout << endl;
    cout << "peak found too much, please check the search parameters!" << endl;
    c1->WaitPrimitive();
    return -1;
  }
  if(nfound<2){
    cout << endl;
    cout << "peak found too less, please check the search parameters!" << endl;
    c1->WaitPrimitive();
    return -1;
  }

  vector<double> v_channel;
  TF1 *f1 = new TF1("f1", "gaus(0)+pol1(3)", 100, 65536);
  for(i=0;i<nfound;i++){
    f1->SetParameters(ypos[i], xpos[i], 2*2, 0, 0);
    h->Fit(f1, "Q", "", 0.98*xpos[i], 1.02*xpos[i]);
    xpos[i] = f1->GetParameter(1);
    v_channel.push_back(xpos[i]);
    c1->Update();
  }
  sort(v_channel.begin(), v_channel.end());
  
  cout << "peak info " << endl;
  for(auto it=v_channel.begin();it!=v_channel.end();++it){
    cout << *it << endl;
  }
  cout << endl;

  //
  int iterations = 1000;
  double tol = 50.;
  vector<pair<double, double>> best_matches;
  double candidate_k = compute_k(v_source, v_channel, iterations, tol, best_matches);
  cout << "get k = " << candidate_k << endl;
  cout << "use peaks = " << best_matches.size() << endl;

  cout << "channel-energy info " << endl;
  for(auto it=best_matches.begin();it!=best_matches.end();++it){
    cout << it->first << " " << it->second << endl;
  }
  cout << endl;

  int n = best_matches.size();
  double x[n], y[n];
  i = 0;
  for(auto &p : best_matches){
    x[i] = p.first;
    y[i] = p.second;
    i++;
  }

  //
  TCanvas *c2 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c2");
  if(!c2) c2 = new TCanvas("c2", "c2", 490, 0, 480, 360);
  c2->cd();
  TGraph* gr = new TGraph(n, x, y);
  gr->SetTitle(h->GetTitle());
  gr->Draw("AP*");
  // gr->Fit("pol2", "");
  // TF1 *f2 = (TF1*)gr->GetFunction("pol2");
  gr->Fit("pol1", "Q");
  TF1 *f2 = (TF1*)gr->GetFunction("pol1");
  par[0] = f2->GetParameter(0);
  par[1] = f2->GetParameter(1);
  // par[2] = f2->GetParameter(2);
  par[2] = 0;
  par[3] = f2->GetChisquare();
  c2->Update();
  c2->SaveAs(TString::Format("%s.png",h->GetName()).Data());

  cout << "channel-energy error info " << endl;
  for(auto it=best_matches.begin();it!=best_matches.end();++it){
    cout << it->first << " " << it->second << " " << f2->Eval(it->first) << " |dy| " << abs(it->second-f2->Eval(it->first)) << endl;
  }
  cout << endl;

  cout << par[0] << "  " << par[1] << "  " << par[2] << endl;

  if(!gROOT->IsBatch()){
    printf("\nDouble click in the bottom right corner of the pad to continue\n");
    c1->cd();
    c1->WaitPrimitive();
  }

  return 0;
}

//
double greedy_match(const vector<double> &energies, const vector<double>& channels, double k, double tol, vector<pair<double, double>> &matches)
{
  matches.clear();

  int m = channels.size();
  int last_matched = -1;
  double total_error = 0;
  for(double e : energies){
    double c_pred = e/k;
    double best_diff = numeric_limits<double>::max();
    int best_index = -1;
    for(int j=last_matched+1;j<m;j++){
      double diff = fabs(channels[j] - c_pred);
      if(diff<best_diff){
        best_diff = diff;
        best_index = j;
      }
    }
    if(best_index == -1) break;
    if(best_diff<=tol){
      matches.push_back({channels[best_index], e});
        total_error += best_diff*best_diff;
        last_matched = best_index;
    }
  }

  return total_error;
}

//
double compute_k(const vector<double> &energies, const vector<double> &channels, int iterations, double tol, vector<pair<double, double>> &best_matches)
{
  int n = energies.size();
  int m = channels.size();
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dist_e(0, n-1);
  uniform_int_distribution<int> dist_c(0, m-1);

  int best_inlier_count = -1;
  double best_candidate_k = 0;
  double best_error = numeric_limits<double>::max();
  vector<pair<double, double>> candidate_matches;

  for(int iter=0;iter<iterations;iter++){
    int i = dist_e(gen);
    int j = dist_c(gen);

    if (channels[j] == 0) continue;
    double candidate_k = energies[i]/channels[j];

    vector<pair<double, double>> current_matches;
    double error = greedy_match(energies, channels, candidate_k, tol, current_matches);
    int inlier_count = current_matches.size();
    if(inlier_count>best_inlier_count || (inlier_count==best_inlier_count && error<best_error)){
      best_inlier_count = inlier_count;
      best_candidate_k = candidate_k;
      best_error = error;
      best_matches = current_matches;
    }
  }

  return best_candidate_k;
}

