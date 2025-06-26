//
void macro(int run)
{
  TString cmd;
  
  // cmd = TString::Format("root -b -q 'tree2th.cpp(%d, \"tf1\", \"cut_low\")'", run);
  // cout << cmd << endl;
  // gSystem->Exec(cmd);

  // cmd = TString::Format("root -b -q 'tree2th.cpp(%d, \"tf1\", \"cut_high\")'", run);
  // cout << cmd << endl;
  // gSystem->Exec(cmd);

  cmd = TString::Format("root -b -q 'tree2th.cpp(%d, \"tf2\", \"cut_low\")'", run);
  cout << cmd << endl;
  gSystem->Exec(cmd);

  // cmd = TString::Format("root -b -q 'tree2th.cpp(%d, \"tf2\", \"cut_high\")'", run);
  // cout << cmd << endl;
  // gSystem->Exec(cmd);

  gSystem->Exec("wait");
}

