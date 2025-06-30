#include "doppler.h"
#include <iostream>

#include "set.h"

int main(int argc, char* argv[])
{
  if(argc!= 5){
    std::cout << "./doppler run win jump configure" << std::endl;
    return -1;
  }

  int run = atoi(argv[1]);
  int win = atoi(argv[2]);
  int jump = atoi(argv[3]);

#ifdef TW  
  TString fi_name = TString::Format("../rootfile/data%04d_tw_build_%dns_jump_%dns.root", run, win, jump);
  TString fo_name = TString::Format("../rootfile/data%04d_tw_doppler_%dns_jump_%dns_%s.root", run, win, jump, argv[4]);
#else
  TString fi_name = TString::Format("../rootfile/data%04d_build_%dns_jump_%dns.root", run, win, jump);
  TString fo_name = TString::Format("../rootfile/data%04d_doppler_%dns_jump_%dns_%s.root", run, win, jump, argv[4]);
#endif

  std::cout << "analysis " << fi_name << std::endl;

  doppler *dp = new doppler(fi_name.Data(), fo_name.Data(), argv[4]);

  dp->Process();
  dp->SaveFile();

  return 0;
}
