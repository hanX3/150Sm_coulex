//
void macro(int run, int win)
{
  gROOT->ProcessLine(".L raw.cpp");
  gROOT->ProcessLine(".L b2hist.cpp");

  TString cmd = TString::Format("b2hist(%d, %d, \"event\")", run, win);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  cmd = TString::Format("b2hist(%d, %d, \"bg\")", run, win);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  gROOT->ProcessLine(".q");
}
