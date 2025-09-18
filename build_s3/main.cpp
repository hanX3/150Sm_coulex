#include "build.h"
#include "set.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"

int main(int argc, char const *argv[])
{
  if(argc != 2){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: analysis 33" << std::endl;
    return -1;
  }

  int time_window = TimeWindow;

  int run = atoi(argv[1]);
  TString file_in = TString::Format("../rootfile/si/data_C1_%04d.root", run);
  std::cout << "analysis " << file_in << std::endl;
  
#ifdef OPENS3COR
  TString file_out = TString::Format("../rootfile/si/data%04d_build_s3_%dns.root", run, time_window);
#else
  TString file_out = TString::Format("../rootfile/si/data%04d_build_s3_%dns_no_s3cor.root", run, time_window);
#endif

  build *bu = new build(file_in.Data(), file_out.Data(), run);
  bu->Process();

  delete bu;

  return 0;
}
