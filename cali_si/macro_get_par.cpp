{
  gSystem->Exec("rm cali_0460.txt");

  gROOT->ProcessLine(".L auto_cali.cpp");
  gROOT->ProcessLine(".L get_par.cpp");

  gROOT->ProcessLine("gROOT->SetBatch(1)");
  gROOT->ProcessLine("get_par_all(460)");


  gROOT->ProcessLine(".q");
}
