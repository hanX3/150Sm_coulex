{
  ifstream fi("../pars/run.txt");
  if(!fi){
    cout << "can not open pars/run.txt" << endl;
    return;
  }

  int run;
  while(fi >> run){
    i++;
    TString cmd_ring = TString::Format("root -b -q 'correction_ring.cpp(%d)'", run);
    gSystem->Exec(cmd_ring);

    TString cmd_sector = TString::Format("root -b -q 'correction_sector.cpp(%d)'", run);
    gSystem->Exec(cmd_sector);
  }

  fi.close();
  gSystem->Exec("wait");
}

