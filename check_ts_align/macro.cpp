//
void macro(int run)
{
  TString cmd = TString::Format("root -b -q 'tree2th.cpp(\"event\", %d)'", run);
  // TString cmd = TString::Format("root -b -q 'tree2th.cpp(\"bg\", %d)'", run);
  cout << cmd << endl;

  gSystem->Exec(cmd);

  gSystem->Exec("wait");
}

