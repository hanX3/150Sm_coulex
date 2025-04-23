//
void get_par_kinematics()
{
  ifstream fi;
  fi.open("kinematics.txt");
  if(!fi){
    cout << "can not open the file" << endl;
    return;
  }

  string line;
  std::getline(fi, line);
  
  double angle_p[1000], e_p[1000], angle_r[1000], e_r[1000];
  for(int i=0;i<1000;i++){
    fi >> angle_p[i] >> e_p[i] >> angle_r[i] >> e_r[i];
    cout << "angle: " << angle_p[i] << " " << e_p[i] << " " << angle_r[i] << " " << e_r[i] << endl;

    e_p[i] *= 32;
    e_r[i] *= 150;
  }

  fi.close();

  //
  TGraph *gr_p = new TGraph(1000, angle_p, e_p);
  gr_p->GetYaxis()->SetRangeUser(0., 120.);
  gr_p->Draw();
  gr_p->SetTitle("");
  gr_p->Fit("pol9");

  TF1 *tf_p = (TF1*)gROOT->GetListOfFunctions()->FindObject("pol9");

  ofstream fo_p;
  fo_p.open("par_kinematics_p.txt");
  if(!fo_p){
    cout << "can not open the file" << endl;
    return;
  }

  for(int i=0;i<10;i++){
    fo_p << tf_p->GetParameter(i) << endl;
  }
  fo_p.close();

  //
  TGraph *gr_r = new TGraph(1000, angle_r, e_r);
  gr_r->GetYaxis()->SetRangeUser(0., 120.);
  gr_r->Draw("same");
  gr_r->SetTitle("");
  gr_r->Fit("pol9");

  TF1 *tf_r = (TF1*)gROOT->GetListOfFunctions()->FindObject("pol9");

  ofstream fo_r;
  fo_r.open("par_kinematics_r.txt");
  if(!fo_r){
    cout << "can not open the file" << endl;
    return;
  }

  for(int i=0;i<10;i++){
    fo_r << tf_r->GetParameter(i) << endl;
  }
  fo_r.close();
}
