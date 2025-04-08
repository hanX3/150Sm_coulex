//
double greedy_match(const vector<double> &energies, const vector<double>& channels, double k, double tol, vector<pair<double, double>> &matches);
double compute_k(const vector<double> &energies, const vector<double> &channels, int iterations, double tol, vector<pair<double, double>> &best_matches);

// for standard source data
const int num_152Eu = 11;
double e_152Eu[num_152Eu] = {121.8, 244.7, 344.3, 411.1, 444.0, 778.9, 867.4, 964.1, 1085.8, 1112.0, 1408.0};

const int num_133Ba = 5;
Float_t e_133Ba[num_133Ba] = {80.9, 276.4, 302.8, 356.0, 383.8};

bool flag_152Eu = 1;
bool flag_133Ba = 1;

//search peak value
double threshold = 0.005;
//search peak sigma
double sigma = 4.0;

//
int auto_cali(TH1D *h, Double_t *par)
{
  int i = 0;
  int npeaks = 20;
  if(!h){
    cout << "get hist error, check!" << endl;
    return -1;
  }

  //standard source data
  vector<double> v_source;
  if(flag_152Eu){
    for(i=0;i<num_152Eu;i++){
      v_source.push_back(e_152Eu[i]);
    }
  }
  if(flag_133Ba){
    for(i=0;i<num_133Ba;i++){
      v_source.push_back(e_133Ba[i]);
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
  gr->Fit("pol2", "");
  TF1 *f2 = (TF1*)gr->GetFunction("pol2");
  //gr->Fit("pol1", "Q");
  //TF1 *f2 = (TF1*)gr->GetFunction("pol1");
  par[0] = f2->GetParameter(0);
  par[1] = f2->GetParameter(1);
  par[2] = f2->GetParameter(2);
  //par[2]=0;
  par[3]=f2->GetChisquare();
  c2->Update();

  cout << "channel-energy error info " << endl;
  for(auto it=best_matches.begin();it!=best_matches.end();++it){
    cout << it->first << " " << it->second << " " << f2->Eval(it->first) << " |dy| " << abs(it->second-f2->Eval(it->first)) << endl;
  }
  cout << endl;

  //show peaks used
  TCanvas *c3 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c3");
  if(!c3) c3 = new TCanvas("c3", "c3", 980, 0, 520, 600);
  c3->cd();
  c3->SetTitle(h->GetTitle());
  c3->Range(-1,0,19,30);
  stringstream ss;

  //152Eu
  TPaveText *pt0 = new TPaveText(0, 26, 8, 28);
  TText *t0  = pt0->AddText("152Eu:");
  pt0->Draw();
  TPaveText *pt1 = new TPaveText(0, 5, 8, 25);
  TText *t1[num_152Eu];
  for(int k=0;k<num_152Eu;k++){
    ss.str("");
    ss << e_152Eu[k] << " keV";
    t1[k] = pt1->AddText(ss.str().c_str());
    for(int l=0;l<i;l++){
      if(y[l]==e_152Eu[k]){
        t1[k]->SetTextColor(2);
      }
    }
  }
  pt1->Draw();

  //133Ba
  TPaveText *pt2 = new TPaveText(9, 26, 17, 28);
  TText *t2  = pt2->AddText("133Ba:");
  pt2->Draw();
  TPaveText *pt3 = new TPaveText(9, 16, 17, 25);
  TText *t3[num_133Ba];
  for(int k=0;k<num_133Ba;k++){
    ss.str("");
    ss << e_133Ba[k] << " keV";
    t3[k] = pt3->AddText(ss.str().c_str());
    for(int l=0;l<i;l++){
      if(y[l]==e_133Ba[k]){
        t3[k]->SetTextColor(2);
      }
    }
  }
  pt3->Draw();
  c3->Update();

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
