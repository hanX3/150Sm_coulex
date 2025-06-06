#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include "set.h"

#include "TBenchmark.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom3.h"

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>

#include "raw.h"

//
class analysis
{
public:
  analysis() = default;
  analysis(const std::string &filename_in, const std::string &filename_out, const int r);
  ~analysis();

public:
  bool ReadCaliData();
  void PrintCaliData();

  void ReadS3CorData();
  void PrintS3CorData();

public:
  void ProcessGevsGe();
  void ProcessSivsGe();

private:
  bool InitMapRingSector();
  void InitMapTS();

private:
  Long64_t GetTsns();

private:
  std::map<int, std::vector<Long64_t>> map_v_ts;
  std::map<int, std::vector<double>> map_cali_data;
  std::map<int, double> map_k_data;

  std::map<int, int> map_s3_sector, map_s3_ring;
  std::map<int, std::vector<double>> map_s3_sector_cor_data, map_s3_ring_cor_data;

private:
  TBenchmark *benchmark;

  int run;
  TFile *file_in;
  TTree *tr_in;

  raw *rd;

  TRandom3 *rndm;
  TFile *file_out;

  // all compare with sid=2 ch=5
  TH1D *h1[4*16];//Ge vs. Ge
  TH1D *h2[11*16];//Si vs. Ge
};

#endif
