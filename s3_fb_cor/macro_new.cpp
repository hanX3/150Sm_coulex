//
void macro_new(int run)
{
  TString cmd_ring = TString::Format("root -b -q 'correction_ring_new.cpp(%d, %f, %f, %f)'", run, 200., 100000., 0.6);
  gSystem->Exec(cmd_ring);

  TString cmd_sector = TString::Format("root -b -q 'correction_sector_new.cpp(%d, %f, %f, %f)'", run, 800., 100000., 0.6);
  gSystem->Exec(cmd_sector);

  gSystem->Exec("wait");
}

