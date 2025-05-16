//
void macro(int run)
{
  gROOT->ProcessLine(".L d2hist.cpp");

  TString cmd;

  cmd = TString::Format("d2hist(\"a\", %d)", run);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  cmd = TString::Format("d2hist(\"b\", %d)", run);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  cmd = TString::Format("d2hist(\"c\", %d)", run);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  cmd = TString::Format("d2hist(\"d\", %d)", run);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  cmd = TString::Format("d2hist(\"e\", %d)", run);
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  gROOT->ProcessLine(".q");
}
