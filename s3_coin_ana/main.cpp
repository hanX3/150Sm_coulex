#include "analysis.h"
#include <iostream>

#include "set.h"

int main(int argc, char* argv[])
{
  if(argc!= 2){
    std::cout << "need run number" << std::endl;
    return -1;
  }

  int run = atoi(argv[1]);
  TString file_in = Form("../rootfile/si/data%04d_build_s3_200ns_new.root", run);
  std::cout << "analysis " << file_in << std::endl;

  TString file_out = Form("../rootfile/si/data%04d_build_s3_200ns_new_th.root", run);
  analysis *ana = new analysis(run, file_in.Data(), file_out.Data());

  ana->Process();

  return 0;
}
