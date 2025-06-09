//
void macro(int run)
{
  TString cmd = TString::Format("root -b -q 'tree2th.cpp(%d)'", run);
  cout << cmd << endl;

  gSystem->Exec(cmd);

  gSystem->Exec("wait");
}

