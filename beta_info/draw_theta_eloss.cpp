//
void draw_theta_eloss_elast()
{
  //
  ifstream fi_par_p;
  fi_par_p.open("par_kinematics_p.txt");

  TF1 *tf_p = new TF1("tf_p", "pol9");
  double par;

  for(int i=0;i<10;i++){
    fi_par_p >> par;
    tf_p->SetParameter(i, par);
    cout << par << endl;
  }
  fi_par_p.close();

  //
  ifstream fi_par_r;
  fi_par_r.open("par_kinematics_r.txt");

  TF1 *tf_r = new TF1("tf_r", "pol9");

  for(int i=0;i<10;i++){
    fi_par_r >> par;
    tf_r->SetParameter(i, par);
    cout << par << endl;
  }
  fi_par_r.close();

  //
  ifstream fi_par_cor;
  fi_par_cor.open("par_correlation.txt");

  TF1 *tf_cor = new TF1("tf_cor", "pol9");

  for(int i=0;i<10;i++){
    fi_par_cor >> par;
    tf_cor->SetParameter(i, par);
    cout << par << endl;
  }
  fi_par_cor.close();

  TGraph *gr_p = new TGraph(180);
  TGraph *gr_r = new TGraph(180);
  //
  double theta_p, theta_r, e_in, e_out;
  TString cmd;
  FILE *pipe;
  //
  for(int i=0;i<180;i++){
    theta_p = (double)i;

    e_in = tf_p->Eval(theta_p);
    cmd = TString::Format("elast -Qa %f o \"1(150Sm)\" %f 32S %f", theta_p, 0.6, e_in);
    pipe = popen(cmd.Data(), "r");
    char buffer_p[128];
    fgets(buffer_p, sizeof(buffer_p), pipe);
    e_out = std::stod(buffer_p);

    gr_p->SetPoint(i+1, theta_p, e_out);

    //
    theta_r = tf_cor->Eval(theta_p);
    e_in = tf_r->Eval(theta_r);
    cmd = TString::Format("elast -Qa %f o \"1(150Sm)\" %f 150Sm %f", theta_r, 0.6, e_in);
    pipe = popen(cmd.Data(), "r");
    char buffer_r[128];
    fgets(buffer_r, sizeof(buffer_r), pipe);
    e_out = std::stod(buffer_r);

    gr_r->SetPoint(i+1, theta_r, e_out);
  }

  TCanvas *cc = new TCanvas("cc", "", 0, 0, 600, 400);
  cc->cd();
  gr_p->SetTitle("");
  gr_p->SetLineColor(2);
  gr_p->Draw();

  gr_r->SetLineColor(2);
  gr_r->Draw("same");

}

//
void draw_theta_eloss_lise()
{
  ifstream fi;
  fi.open("kinematics_after_target.txt");
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
  gr_p->SetTitle("");
  gr_p->SetLineColor(2);
  gr_p->Draw();

  TGraph *gr_r = new TGraph(1000, angle_r, e_r);
  gr_r->SetTitle("");
  gr_r->SetLineColor(2);
  gr_r->Draw("same");
}
