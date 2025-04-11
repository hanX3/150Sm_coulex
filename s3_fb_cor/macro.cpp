//
void macro(int run)
{
  TString cmd_ring = TString::Format("root -b -q 'correction_ring.cpp(%d)'", run);
  gSystem->Exec(cmd_ring);

  TString cmd_sector = TString::Format("root -b -q 'correction_sector.cpp(%d)'", run);
  gSystem->Exec(cmd_sector);

  gSystem->Exec("wait");
}

