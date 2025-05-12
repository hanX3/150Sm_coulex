
//
void macro_cali_si(int run)
{
  TString cmd = TString::Format("root -b -q 'get_hist_cali_si.cpp(%s, %d)'", TString::Format("\"../rootfile/si/data_C1_%04d.root\"",run).Data(), run);
  gSystem->Exec(cmd);

  gSystem->Exec("wait");
}

