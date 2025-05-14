#include "build.h"
#include "set.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"
#include "TStopwatch.h"

//
int main(int argc, char const *argv[])
{
  if(argc != 2){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: build 33" << std::endl;
    return -1;
  }

  int run = atoi(argv[1]);
  TString file_in = TString::Format("../rootfile/data%04d.root", run);
  std::cout << "analysis " << file_in << std::endl;
  
  TString file_out = TString::Format("../rootfile/data%04d_build_%dns.root", run, TIMEWINDOW);

  TStopwatch timer;
  timer.Start();

  build *bu = new build(file_in.Data(), file_out.Data(), run);
  bu->Process();

  timer.Stop();
  Double_t real_time = timer.RealTime();
  Double_t cpu_time = timer.CpuTime();

  printf("time : RT=%7.3f s, Cpu=%7.3f s\n", real_time, cpu_time);

  delete bu;

  return 0;
}
