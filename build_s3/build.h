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

//
class build
{
public:
  build(const std::string &filename_in, const std::string &filename_out, int r);
  virtual ~build();

  void Process();

  void GetS3FrontBackDataPrompt();
  void GetS3FrontBackDataRandom();
  void SaveFile();

private:
  std::map<int, std::vector<double>> map_cali;
  std::map<int, std::vector<double>> map_s3_sector_cor, map_s3_ring_cor; // key is id 
  std::map<int, double> map_s3_sector_att, map_s3_ring_att; // key is id
  std::map<int, double> map_ts_offset;

  std::map<int, int> map_s3_sector_id, map_s3_ring_id;

private:
  bool ReadCaliData();
  void PrintCaliData();

  bool ReadS3CorData();
  void PrintS3CorData();

  bool ReadS3AttData();
  void PrintS3AttData();

  bool ReadTSOffsetData();
  void PrintTSOffsetData();

  bool InitMapSectorRingID();
  void PrintMapSectorRingID();

private:
  Short_t GetID();
  double GetEnergy();
  Long64_t GetTSns();

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
  Short_t sr;
  Short_t cid;
  Short_t sid;
  Short_t ch;
  UShort_t evte;
  Long64_t ts;

  Long64_t ts_ns;

  TFile *file_out;
};

#endif
