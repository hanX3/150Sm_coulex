#ifndef SORT_H_
#define SORT_H_

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
struct sort_data {
  Short_t cid;
  Short_t sid;
  Short_t ch;
  Double_t evte;
  Long64_t ts;

  sort_data() : cid(0), sid(0), ch(0), evte(0.), ts(0) {}

  void operator()(Short_t cid_, Short_t sid_, Short_t ch_, Double_t evte_, Long64_t ts_)
  {
    cid = cid_;
    sid = sid_;
    ch = ch_;
    evte = evte_;
    ts = ts_;
  }
};

//
class sort
{
public:
  sort() = default;
  sort(const std::string &filename_in, const std::string &filename_out, const int r);
  ~sort();

public:
  bool ReadCaliData();
  void PrintCaliData();

  void ReadS3CorData();
  void PrintS3CorData();

  bool ReadTSOffset();
  void PrintTSOffset();

  bool InitMapSectorRingID();
  void PrintMapSectorRingID();

public:
  void Process();

private:
  void GetEnergy();
  void GetTSns();

private:
  std::map<int, std::vector<double>> map_cali_data;
  std::map<int, double> map_k_data;
  std::map<int, double> map_ts_offset;

  std::map<int, int> map_s3_sector_id, map_s3_ring_id;
  std::map<int, std::vector<double>> map_s3_sector_cor_data, map_s3_ring_cor_data;

  std::map<Long64_t, sort_data> map_sort_data;
  std::map<Long64_t, sort_data>::iterator it_sd;

private:
  TBenchmark *benchmark;

  int run;
  TFile *file_in;
  TTree *tr_in;

  raw *rd;

  TRandom3 *rndm;

private:
  TFile *file_out;

  TTree *tr_out;
  sort_data sd;
};

#endif
