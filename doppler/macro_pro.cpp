//
void macro_pro(int run, int win, int jump, string config)
{
  gROOT->ProcessLine(".L d2hist_pro.cpp");

  TString cmd;

  //
  cmd = TString::Format("d2hist_pro(%d,%d,%d,\"%s\")",run,win,jump,config.c_str());
  cout << cmd << endl;
  gROOT->ProcessLine(cmd.Data());

  //
  gROOT->ProcessLine(".q");
}
