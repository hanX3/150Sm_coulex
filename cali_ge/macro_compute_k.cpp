{
  gSystem->Exec("rm k_*.txt");
  
  gROOT->ProcessLine(".L compute_k.cpp");

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

    cmd = TString::Format("compute_k_all(%d)", run);
    cout << cmd << endl;
    gROOT->ProcessLine(cmd.Data());
  }

  fi.close();
  
  gROOT->ProcessLine(".q");
}
