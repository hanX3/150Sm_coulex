//
void get_theta_eloss_p()
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

  ifstream fi_par_p;
  fi_par_p.open("par_kinematics_p.txt");

  TF1 *tf_p = new TF1("tf_p", "pol9");
  double par;

  for(int i=0;i<10;i++){
    fi_par_p >> par;
    tf_p->SetParameter(i, par);
    cout << par << endl;
  }

  //
  ofstream fo;
  fo.open("theta_ein_eout_p.txt");

  double theta, e_in, e_out;
  TString cmd;
  FILE *pipe;
  char buffer[128];
  //
  while(1){
    fi_spider >> theta;
    if(!fi_spider.good()) break;
    e_in = tf_p->Eval(theta);
    cmd = TString::Format("elast -Qa %f o \"1(150Sm)\" %f 32S %f", theta, 1.2, e_in);
    pipe = popen(cmd.Data(), "r");
    char buffer[128];
    fgets(buffer, sizeof(buffer), pipe);
    e_out = std::stod(buffer);

    fo << theta << " " << e_in << " " << e_out << endl;
  }

  while(1){
    fi_s3 >> theta;
    if(!fi_s3.good()) break;
    e_in = tf_p->Eval(theta);
    cmd = TString::Format("elast -Qa %f o \"1(150Sm)\" %f 32S %f", theta, 1.2, e_in);
    pipe = popen(cmd.Data(), "r");
    char buffer[128];
    fgets(buffer, sizeof(buffer), pipe);
    e_out = std::stod(buffer);

    fo << theta << " " << e_in << " " << e_out << endl;
  }

  fi_par_p.close();
  fi_spider.close();
  fi_s3.close();
  fo.close();
}
