//
void macro(int run)
{
  TString cmd;

  cmd = TString::Format("root -b -q 'tree2th_event_new.cpp(%d, \"cut_low\")'", run);
  cout << cmd << endl;
  gSystem->Exec(cmd);

  // cmd = TString::Format("root -b -q 'tree2th_event_new.cpp(%d, \"cut_high\")'", run);
  // cout << cmd << endl;
  // gSystem->Exec(cmd);

  gSystem->Exec("wait");
}

