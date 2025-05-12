
//
void macro_cali_ge(int run)
{
  TString cmd = TString::Format("root -b -q 'get_hist_cali_ge.cpp(%s, %d)'", TString::Format("\"../rootfile/ge/data_C1_%04d.root\"",run).Data(), run);
  gSystem->Exec(cmd);

  gSystem->Exec("wait");
}

