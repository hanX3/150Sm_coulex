//
void macro(int run, int win, int jump)
{
  gROOT->ProcessLine(".L raw.cpp");
  gROOT->ProcessLine(".L b2hist.cpp");

  TString cmd = TString::Format("b2hist(%d, %d, %d, \"event\")", run, win, jump);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  cmd = TString::Format("b2hist(%d, %d, %d, \"bg\")", run, win, jump);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  gROOT->ProcessLine(".q");
}
