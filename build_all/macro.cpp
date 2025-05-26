//
void macro(int run)
{
  gROOT->ProcessLine(".L raw.cpp");
  gROOT->ProcessLine(".L b2hist.cpp");

  TString cmd = TString::Format("b2hist(\"event\", %d)", run);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  gROOT->ProcessLine(".q");
}
