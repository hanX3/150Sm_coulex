#ifndef _analysis_h_
#define _analysis_h_

#include "s3_tree.h"

#include <vector>
#include <map>
#include <numeric>

#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TRandom3.h"
#include "TBenchmark.h"

class analysis
{
public:
  analysis(int run, const std::string &filename_in, const std::string &filename_out);
  virtual ~analysis();

  void Process();

private:
  std::map<int, double> m_r2theta, m_s2phi;

private:
  double ThetaLab2CMProjectile(double t);
  double ThetaLab2CMRecoil(double t);

private:
  bool ReadS3AngleData();
  void PrintS3AngleData();

private:
  TBenchmark *benchmark;
  TRandom3 *rndm;

  TFile *file_in;
  TTree *tree;

  s3_tree *st;

  //
  Double_t tau, tau_tilde;
  
  //
  Int_t run;

  TFile *file_out;
};

#endif
