#include "doppler.h"
#include <iostream>

#include "set.h"

int main(int argc, char* argv[])
{
  if(argc!= 2){
    std::cout << "need run number " << std::endl;
    return -1;
  }

  int run = atoi(argv[1]);
  TString fi_name = TString::Format("../rootfile/data%04d_build_%dns.root", run, COINWIDTH);
  std::cout << "analysis " << fi_name << std::endl;

  TString fo_name = TString::Format("../rootfile/data%04d_doppler_%dns.root", run, COINWIDTH);
  doppler *dp = new doppler(fi_name.Data(), fo_name.Data());

  dp->Process();
  dp->SaveFile();

  return 0;
}
