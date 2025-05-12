
//
void macro_raw(int run)
{
  TString cmd = TString::Format("root -b -q 'get_hist_raw.cpp(%s, %d)'", TString::Format("\"../rootfile/data_C2_%04d_%04d.root\"",run,run).Data(), run);
  gSystem->Exec(cmd);

  gSystem->Exec("wait");
}

