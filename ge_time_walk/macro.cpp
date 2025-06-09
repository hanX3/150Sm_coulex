//
void macro(int run)
{
  // TString cmd = TString::Format("root -b -q 'tree2th.cpp(\"event\", %d)'", run);
  // TString cmd = TString::Format("root -b -q 'tree2th.cpp(\"bg\", %d)'", run);

  TString cmd = TString::Format("root -b -q 'tree2th_event_new.cpp(%d)'", run);
  // TString cmd = TString::Format("root -b -q 'tree2th_bg_new.cpp(%d)'", run);
  cout << cmd << endl;

  gSystem->Exec(cmd);

  gSystem->Exec("wait");
}

