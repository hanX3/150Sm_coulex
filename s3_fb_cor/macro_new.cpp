//
void macro_new(int run)
{
  //
  TString cmd_ring;
  if(run>=472 && run<=605){
    cmd_ring = TString::Format("root -b -q 'correction_ring_new.cpp(%d, %f, %f, %f)'", run, 200., 100000., 0.6);
  }
  if(run>=616 && run<=674){
    cmd_ring = TString::Format("root -b -q 'correction_ring_new.cpp(%d, %f, %f, %f)'", run, 200., 100000., 0.8);
  }
  if(run>=680 && run<=718){
    cmd_ring = TString::Format("root -b -q 'correction_ring_new.cpp(%d, %f, %f, %f)'", run, 200., 100000., 0.6);
  }
  gSystem->Exec(cmd_ring);

  //
  TString cmd_sector;
  if(run>=472 && run<=605){
    cmd_sector = TString::Format("root -b -q 'correction_sector_new.cpp(%d, %f, %f, %f)'", run, 800., 100000., 0.6);
  }
  if(run>=616 && run<=674){
    cmd_sector = TString::Format("root -b -q 'correction_sector_new.cpp(%d, %f, %f, %f)'", run, 800., 100000., 0.8);
  }
  if(run>=680 && run<=718){
    cmd_sector = TString::Format("root -b -q 'correction_sector_new.cpp(%d, %f, %f, %f)'", run, 800., 100000., 0.6);
  }
  gSystem->Exec(cmd_sector);

  gSystem->Exec("wait");
}

