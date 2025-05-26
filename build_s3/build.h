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

  void GetS3FrontBackData(TString tr_name, double abs_time1, double abs_time2);
  void SaveFile();

private:
  std::map<int, std::vector<double>> map_cali_data;
  std::map<int, double> map_ts_offset;

private:
  std::map<int, int> map_ring_id;
  std::map<int, int> map_sector_id;

private:
  bool ReadCaliData();
  void PrintCaliData();

  bool ReadTSOffset();
  void PrintTSOffset();

  bool InitMapSectorRingID();
  void PrintMapSectorRingID();

private:
  Short_t GetID(int crate, int slot, int channel);
  double CaliEnergy(int adc, int crate, int slot, int channel);
  Long64_t GetTSns(Long64_t ts, int sr, int crate, int slot, int channel);

private:
  TBenchmark *benchmark;  
  TRandom3 *rndm;  

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
