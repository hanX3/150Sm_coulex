{
  gROOT->ProcessLine(".L get_hist_cali.cpp");

  ifstream fi("../pars/run.txt");
  if(!fi){
    cout << "can not open pars/run.txt" << endl;
    return;
  }

  int run;
  TString cmd;
  while(1){
    fi >> run;
    if(!fi.good())  break;

    cmd = TString::Format("get_hist_cali(\"../rootfile/ge/data_C1_%04d.root\", %d)", run, run);
    cout << cmd << endl;
    gROOT->ProcessLine(cmd.Data());
  }

  fi.close();

  gROOT->ProcessLine(".q");
}
