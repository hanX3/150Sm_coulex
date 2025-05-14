#ifndef _build_h_
#define _build_h_

#include "set.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"
#include "TBenchmark.h"

#include "TSystem.h"

#include <iostream>

class build
{
public:
  build(const std::string &filename_in, const std::string &filename_out, int r);
  virtual ~build();

  void Process();

  void GetGeSpiderS3Event(TString tr_name, double abs_time1, double abs_time2);
  void SaveFile();

private:
  std::map<int, int> map_ge_sector_id, map_ge_ring_id;
  std::map<int, int> map_spider_sector_id, map_spider_ring_id;
  std::map<int, int> map_s3_sector_id, map_s3_ring_id;

private:
  bool InitMapSectorRingID();
  void PrintMapSectorRingID();

private:
  TBenchmark *benchmark;  
  TRandom3 *rndm;  

private:
  Int_t run;
  std::vector<int> v_s3_sid;

private:
  TFile *file_in;
  TTree *tr_in;
  Short_t cid;
  Short_t sid;
  Short_t ch;
  Double_t evte;
  Long64_t ts;

  TFile *file_out;
};


#endif
