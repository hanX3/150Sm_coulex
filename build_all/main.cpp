#include "build.h"
#include "set.h"

#include <sys/prctl.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"
#include "TStopwatch.h"

//
int main(int argc, char const *argv[])
{
  prctl(PR_SET_NAME, "build");

  if(argc != 4){
    std::cout << "./build run win jump" << std::endl;
    return -1;
  }

  int run = atoi(argv[1]);
  double win = atof(argv[2]);
  double jump = atof(argv[3]);

#ifdef TW  
  TString file_in = TString::Format("../rootfile/data%04d_tw.root", run);
  TString file_out = TString::Format("../rootfile/data%04d_tw_build_%dns_jump_%dns.root", run, int(win), int(jump));
#else
  TString file_in = TString::Format("../rootfile/data%04d.root", run);
  TString file_out = TString::Format("../rootfile/data%04d_build_%dns_jump_%dns.root", run, int(win), int(jump));
#endif

  std::cout << "analysis " << file_in << std::endl;
  TStopwatch timer;
  timer.Start();

  build *bu = new build(file_in.Data(), file_out.Data(), run, win, jump);
  bu->Process();

  timer.Stop();
  Double_t real_time = timer.RealTime();
  Double_t cpu_time = timer.CpuTime();

  printf("time : RT=%7.3f s, Cpu=%7.3f s\n", real_time, cpu_time);

  delete bu;

  return 0;
}
