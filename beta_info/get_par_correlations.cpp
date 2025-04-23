//
void get_par_correlations()
{
  ifstream fi;
  fi.open("correlation.txt");
  if(!fi){
    cout << "can not open the file" << endl;
    return;
  }

  string line;
  std::getline(fi, line);
  
  double angle_p[1000], angle_r[1000];
  for(int i=0;i<1000;i++){
    fi >> angle_p[i] >> angle_r[i];
    cout << "angle: " << angle_p[i] << angle_r[i] << endl;
  }

  fi.close();

  TGraph *gr = new TGraph(1000, angle_p, angle_r);
  gr->GetYaxis()->SetRangeUser(0., 120.);
  gr->Draw();
  gr->SetTitle("");
  gr->Fit("pol9");

  TF1 *tf = (TF1*)gROOT->GetListOfFunctions()->FindObject("pol9");

  ofstream fo;
  fo.open("par_correlation.txt");
  if(!fo){
    cout << "can not open the file" << endl;
    return;
  }

  for(int i=0;i<10;i++){
    fo << tf->GetParameter(i) << endl;
  }
  fo.close();
}
