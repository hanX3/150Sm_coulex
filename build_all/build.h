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
#include <map>
#include <vector>
#include <set>

class build
{
public:
  build(const std::string &filename_in, const std::string &filename_out, int r, double w, double j);
  virtual ~build();

  void Process();

  void GetGeSpiderS3EventPrompt();
  void GetGeSpiderS3EventRandom();
  void SaveFile();

private:
  std::map<int, int> map_ge_sector_id, map_ge_ring_id;
  std::map<int, int> map_spider_sector_id, map_spider_ring_id;
  std::map<int, int> map_s3_sector_id, map_s3_ring_id;

  std::map<int, int> map_clover_id;
  std::set<int> set_clover_id;

private:
  bool InitMapSectorRingID();
  void PrintMapSectorRingID();

private:
  void CloverAddback(Short_t *sid, Short_t *ch, Short_t *ring_id, Short_t *sector_id, Double_t *energy, Long64_t *ts, int &n);

private:
  TBenchmark *benchmark;  
  TRandom3 *rndm;  

  double coin_width;
  double jump_width;

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
