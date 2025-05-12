
//
void macro_cali(int run)
{
  TString cmd = TString::Format("root -b -q 'get_hist_cali.cpp(%s, %d)'", TString::Format("\"../rootfile/data%04d.root\"",run).Data(), run);
  gSystem->Exec(cmd);

  gSystem->Exec("wait");
}

