{
  gROOT->ProcessLine(".L get_hist_cali.cpp");

  gROOT->ProcessLine("get_hist_cali(\"../rootfile/ge/data_C1_0472.root\", 472)");
  gROOT->ProcessLine("get_hist_cali(\"../rootfile/ge/data_C1_0496.root\", 496)");
  gROOT->ProcessLine("get_hist_cali(\"../rootfile/ge/data_C1_0519.root\", 519)");
  gROOT->ProcessLine("get_hist_cali(\"../rootfile/ge/data_C1_0546.root\", 546)");
  gROOT->ProcessLine("get_hist_cali(\"../rootfile/ge/data_C1_0576.root\", 576)");
  gROOT->ProcessLine("get_hist_cali(\"../rootfile/ge/data_C1_0603.root\", 603)");
  gROOT->ProcessLine("get_hist_cali(\"../rootfile/ge/data_C1_0634.root\", 634)");
  gROOT->ProcessLine("get_hist_cali(\"../rootfile/ge/data_C1_0660.root\", 660)");
  gROOT->ProcessLine("get_hist_cali(\"../rootfile/ge/data_C1_0691.root\", 691)");

  gROOT->ProcessLine(".q");
}
