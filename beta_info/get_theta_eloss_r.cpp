//
void get_theta_eloss_r()
{
  ifstream fi_spider;
  fi_spider.open("theta_spider.txt");
  if(!fi_spider){
    cout << "can not open the file" << endl;
    return;
  }

  ifstream fi_s3;
  fi_s3.open("theta_s3.txt");
  if(!fi_s3){
    cout << "can not open the file" << endl;
    return;
  }

  //
  ifstream fi_par_cor;
  fi_par_cor.open("par_correlation.txt");
  TF1 *tf_cor = new TF1("tf_cor", "pol9", 0, 180);
  double par;

  for(int i=0;i<10;i++){
    fi_par_cor >> par;
    tf_cor->SetParameter(i, par);
    cout << par << endl;
  }

  ifstream fi_par_r;
  fi_par_r.open("par_kinematics_r.txt");

  TF1 *tf_r = new TF1("tf_r", "pol9");

  for(int i=0;i<10;i++){
    fi_par_r >> par;
    tf_r->SetParameter(i, par);
    cout << par << endl;
  }

  tf_r->Draw();

  //
  ofstream fo;
  fo.open("theta_ein_eout_r.txt");

  double theta_p, theta_r, e_in, e_out;
  TString cmd;
  FILE *pipe;
  char buffer[128];
  //
  while(1){
    fi_spider >> theta_p;
    if(!fi_spider.good()) break;
    theta_r = tf_cor->Eval(theta_p);

    e_in = tf_r->Eval(theta_r);
    cmd = TString::Format("elast -Qa %f o \"1(150Sm)\" %f 150Sm %f", theta_r, 0.6, e_in);
    pipe = popen(cmd.Data(), "r");
    char buffer[128];
    fgets(buffer, sizeof(buffer), pipe);
    e_out = std::stod(buffer);

    fo << theta_p << " " << theta_r << " " << e_in << " " << e_out << endl;
  }

  while(1){
    fi_s3 >> theta_p;
    if(!fi_s3.good()) break;
    theta_r = tf_cor->Eval(theta_p);

    e_in = tf_r->Eval(theta_r);
    cmd = TString::Format("elast -Qa %f o \"1(150Sm)\" %f 150Sm %f", theta_r, 0.6, e_in);
    pipe = popen(cmd.Data(), "r");
    char buffer[128];
    fgets(buffer, sizeof(buffer), pipe);
    e_out = std::stod(buffer);

    fo << theta_p << " " << theta_r << " " << e_in << " " << e_out << endl;
  }

  fi_par_r.close();
  fi_spider.close();
  fi_s3.close();
  fo.close();
}
