#include "build.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>

//
build::build(const std::string &filename_in, const std::string &filename_out, int r)
{
  benchmark = new TBenchmark;

  run = r;
  if((run>=457 && run<=462) || (run>=472 && run<=605) || (run>=680 && run<=718)){
    v_s3_sid = {9, 10, 11, 12};
  }else if((run>=616 && run<=674) || (run>=721 && run<=723)){
    v_s3_sid = {2, 3, 11, 12};
  }else{
    throw std::invalid_argument("wrong run number.");
  }

  if(!InitMapSectorRingID()){
    throw std::invalid_argument("can not init ring and sector id data.");
  }
  PrintMapSectorRingID();

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "open file " << filename_in << " error!" << std::endl;
    delete file_in;

    return ;
  }

  tr_in = (TTree*)file_in->Get("tree");
  tr_in->SetBranchAddress("cid", &cid);
  tr_in->SetBranchAddress("sid", &sid);
  tr_in->SetBranchAddress("ch", &ch);
  tr_in->SetBranchAddress("evte", &evte);
  tr_in->SetBranchAddress("ts", &ts);

  rndm = new TRandom3((Long64_t)time(0));
  file_out = TFile::Open(filename_out.c_str(), "recreate");
}

//
build::~build()
{
  delete file_in;
  delete file_out;
}

//
void build::Process()
{
  benchmark->Start("build");

  //
  double time_window = TIMEWINDOW;
  double time_jump = TIMEJUMP;
  //
  GetGeSpiderS3Event("tr_event", 0, time_window);
  GetGeSpiderS3Event("tr_bg", time_jump, time_window+time_jump);
  
  benchmark->Show("build");
}

//
void build::GetGeSpiderS3Event(TString tr_name, double abs_time1, double abs_time2)
{
  std::cout << "start get ge si data" << std::endl;

  int n_max_ge = GENUM;
  int n_max_spider = SPIDERNUM;
  int n_max_s3_sector = S3SECTORNUM;
  int n_max_s3_ring = S3RINGNUM;

  double cut_ge = CUTGE; 
  double cut_si = CUTSI;

  //
  Int_t n_ge = 0;
  Short_t ge_sid[n_max_ge];
  Short_t ge_ch[n_max_ge];
  Short_t ge_ring_id[n_max_ge];
  Short_t ge_sector_id[n_max_ge];
  Double_t ge_energy[n_max_ge];
  Long64_t ge_ts[n_max_ge];

  Int_t n_spider = 0;
  Short_t spider_sid[n_max_spider];
  Short_t spider_ch[n_max_spider];
  Short_t spider_ring_id[n_max_spider];
  Short_t spider_sector_id[n_max_spider];
  Double_t spider_energy[n_max_spider];
  Long64_t spider_ts[n_max_spider];

  Int_t n_s3_sector = 0;
  Short_t s3_sector_sid[n_max_s3_sector];
  Short_t s3_sector_ch[n_max_s3_sector];
  Short_t s3_sector_id[n_max_s3_sector];
  Double_t s3_sector_energy[n_max_s3_sector];
  Long64_t s3_sector_ts[n_max_s3_sector];
  Int_t n_s3_ring = 0;
  Short_t s3_ring_sid[n_max_s3_ring];
  Short_t s3_ring_ch[n_max_s3_ring];
  Short_t s3_ring_id[n_max_s3_ring];
  Double_t s3_ring_energy[n_max_s3_ring];
  Long64_t s3_ring_ts[n_max_s3_ring];

  std::stringstream ss;
  ss.str("");
  ss << "timewindow_" << TIMEWINDOW;
  TTree *tr = new TTree(tr_name.Data(), ss.str().c_str());

  //
  memset(ge_sid, 0, sizeof(ge_sid));
  memset(ge_ch, 0, sizeof(ge_ch));
  memset(ge_ring_id, 0, sizeof(ge_ring_id));
  memset(ge_sector_id, 0, sizeof(ge_sector_id));
  memset(ge_energy, 0, sizeof(ge_energy));
  memset(ge_ts, 0, sizeof(ge_ts));

  memset(spider_sid, 0, sizeof(spider_sid));
  memset(spider_ch, 0, sizeof(spider_ch));
  memset(spider_ring_id, 0, sizeof(spider_ring_id));
  memset(spider_sector_id, 0, sizeof(spider_sector_id));
  memset(spider_energy, 0, sizeof(spider_energy));
  memset(spider_ts, 0, sizeof(spider_ts));

  memset(s3_sector_sid, 0, sizeof(s3_sector_sid));
  memset(s3_sector_ch, 0, sizeof(s3_sector_ch));
  memset(s3_sector_id, 0, sizeof(s3_sector_id));
  memset(s3_sector_energy, 0, sizeof(s3_sector_energy));
  memset(s3_sector_ts, 0, sizeof(s3_sector_ts));

  memset(s3_ring_sid, 0, sizeof(s3_ring_sid));
  memset(s3_ring_ch, 0, sizeof(s3_ring_ch));
  memset(s3_ring_id, 0, sizeof(s3_ring_id));
  memset(s3_ring_energy, 0, sizeof(s3_ring_energy));
  memset(s3_ring_ts, 0, sizeof(s3_ring_ts));

  //
  tr->Branch("n_ge", &n_ge, "n_ge/I");
  tr->Branch("ge_sid", ge_sid, "ge_sid[n_ge]/S");
  tr->Branch("ge_ch", ge_ch, "ge_ch[n_ge]/S");
  tr->Branch("ge_ring_id", ge_ring_id, "ge_ch[n_ge]/S");
  tr->Branch("ge_sector_id", ge_ch, "ge_sector_id[n_ge]/S");
  tr->Branch("ge_energy", ge_energy, "ge_energy[n_ge]/D");
  tr->Branch("ge_ts", ge_ts, "ge_ts[n_ge]/L");

  tr->Branch("n_spider", &n_spider, "n_spider/I");
  tr->Branch("spider_sid", spider_sid, "spider_sid[n_spider]/S");
  tr->Branch("spider_ch", spider_ch, "spider_ch[n_spider]/S");
  tr->Branch("spider_ring_id", spider_ring_id, "spider_ring_id[n_spider]/S");
  tr->Branch("spider_sector_id", spider_sector_id, "spider_sector_id[n_spider]/S");
  tr->Branch("spider_energy", spider_energy, "spider_energy[n_spider]/D");
  tr->Branch("spider_ts", spider_ts, "spider_ts[n_spider]/L");
  
  tr->Branch("n_s3_sector", &n_s3_sector, "n_s3_sector/I");
  tr->Branch("s3_sector_sid", s3_sector_sid, "s3_sector_sid[n_s3_sector]/S");
  tr->Branch("s3_sector_ch", s3_sector_ch, "s3_sector_ch[n_s3_sector]/S");
  tr->Branch("s3_sector_id", s3_sector_id, "s3_sector_id[n_s3_sector]/S");
  tr->Branch("s3_sector_energy", s3_sector_energy, "s3_sector_energy[n_s3_sector]/D");
  tr->Branch("s3_sector_ts", s3_sector_ts, "s3_sector_ts[n_s3_sector]/L");

  tr->Branch("n_s3_ring", &n_s3_ring, "n_s3_ring/I");
  tr->Branch("s3_ring_sid", s3_ring_sid, "s3_ring_sid[n_s3_ring]/S");
  tr->Branch("s3_ring_ch", s3_ring_ch, "s3_ring_ch[n_s3_ring]/S");
  tr->Branch("s3_ring_id", s3_ring_id, "s3_ring_id[n_s3_ring]/S");
  tr->Branch("s3_ring_energy", s3_ring_energy, "s3_ring_energy[n_s3_ring]/D");
  tr->Branch("s3_ring_ts", s3_ring_ts, "s3_ring_ts[n_s3_ring]/L");
  
  //
  Long64_t i_start = 0;

  Short_t sid1 = 0;
  Short_t ch1 = 0;
  Double_t energy1 = 0;
  Long64_t ts1 = 0;
  
  Short_t sid2 = 0;
  Short_t ch2 = 0;
  Double_t energy2 = 0;
  Long64_t ts2 = 0;

  while(true){//get first Ge data
    tr_in->GetEntry(i_start);
    i_start++;
    if(cid==0 && evte>cut_ge){
      break;
    }
  }
  
  Long64_t n_evt = 0;
  Long64_t i = i_start;
  Long64_t i_current = i-1;
  while(true){
    if(i==tr_in->GetEntries()) break;

    tr_in->GetEntry(i_current);

    sid1 = sid;
    ch1 = ch;
    energy1 = evte;
    ts1 = ts;

    ge_sid[n_ge] = sid1;
    ge_ch[n_ge] = ch1; 
    ge_ring_id[n_ge] = map_ge_ring_id[100*sid1+ch1]; 
    ge_sector_id[n_ge] = map_ge_sector_id[100*sid1+ch1]; 
    ge_energy[n_ge] = energy1; 
    ge_ts[n_ge] = ts1; 
    n_ge++;

    // std::cout << std::endl;
    // std::cout << "info ge " << cid << " " << sid << " " << ch << " " << energy << " " << ts << std::endl;

    while(true){//search backward
      if(i_current--==0) break;
      tr_in->GetEntry(i_current);
      // std::cout << "info backward " << cid << " " << sid << " " << ch << " " << energy << " " << ts << std::endl;
      // std::cout << "time diff " << abs(ts-ts1) << std::endl;

      if(cid==0 && evte<=cut_ge){//if Ge data and small energy
        i_current--;
        continue;
      }
      if(cid==1 && evte<=cut_si){//if Si/S3 data and small energy
        i_current--;
        continue;
      }

      sid2 = sid;
      ch2 = ch;
      energy2 = evte;
      ts2 = ts;
    
      if((abs(ts2-ts1))>=abs_time1 && abs(ts2-ts1)<=abs_time2){//this coincidence
        if(cid==0){
          ge_sid[n_ge] = sid2; 
          ge_ch[n_ge] = ch2; 
          ge_ring_id[n_ge] = map_ge_ring_id[100*sid2+ch2]; 
          ge_sector_id[n_ge] = map_ge_sector_id[100*sid2+ch2]; 
          ge_energy[n_ge] = energy2; 
          ge_ts[n_ge] = ts2; 
          n_ge++;
        }
        if(cid==1){
          if(!(std::find(v_s3_sid.begin(),v_s3_sid.end(),sid) != v_s3_sid.end())){// spider data
            spider_sid[n_spider] = sid2;
            spider_ch[n_spider] = ch2;
            spider_ring_id[n_spider] = map_spider_ring_id[10000*cid+100*sid+ch];
            spider_sector_id[n_spider] = map_spider_sector_id[10000*cid+100*sid+ch];
            spider_energy[n_spider] = energy2;
            spider_ts[n_spider] = ts2;
            n_spider++;
          }else{
            if(sid>=11&&sid<=12){
              s3_ring_sid[n_s3_ring] = sid2;
              s3_ring_ch[n_s3_ring] = ch2;
              s3_ring_id[n_s3_ring] = map_s3_ring_id[10000*cid+100*sid+ch];
              s3_ring_energy[n_s3_ring] = energy2;
              s3_ring_ts[n_s3_ring] = ts2;
              n_s3_ring++;
            }else{
              s3_sector_sid[n_s3_sector] = sid2;
              s3_sector_ch[n_s3_sector] = ch2;
              s3_sector_id[n_s3_sector] = map_s3_sector_id[10000*cid+100*sid+ch];
              s3_sector_energy[n_s3_sector] = energy2;
              s3_sector_ts[n_s3_sector] = ts2;
              n_s3_sector++;
            }
          }
        }
      }else break;
    }//while search backward

    while(true){//search forward
      if(i>=tr_in->GetEntries()) break;
      tr_in->GetEntry(i);
      // std::cout << "info forward " << cid << " " << sid << " " << ch << " " << energy << " " << ts << std::endl;
      // std::cout << "time diff " << abs(ts-ts1) << std::endl;

      if(cid==0 && evte<=cut_ge){//if ge data and small energy
        i++;
        continue;
      }
      if(cid==1 && evte<=cut_si){//if si data and small energy
        i++;
        continue;
      }

      sid2 = sid;
      ch2 = ch;
      energy2 = evte;
      ts2 = ts;
    
      if((abs(ts2-ts1))>=abs_time1 && abs(ts2-ts1)<=abs_time2){//this coincidence
        i++;

        if(cid==0){
          ge_sid[n_ge] = sid2; 
          ge_ch[n_ge] = ch2; 
          ge_ring_id[n_ge] = map_ge_ring_id[100*sid2+ch2]; 
          ge_sector_id[n_ge] = map_ge_sector_id[100*sid2+ch2]; 
          ge_energy[n_ge] = energy2; 
          ge_ts[n_ge] = ts2; 
          n_ge++;
        }
        if(cid==1){
          if(!(std::find(v_s3_sid.begin(),v_s3_sid.end(),sid) != v_s3_sid.end())){// spider data
            spider_sid[n_spider] = sid2;
            spider_ch[n_spider] = ch2;
            spider_ring_id[n_spider] = map_spider_ring_id[10000*cid+100*sid+ch];
            spider_sector_id[n_spider] = map_spider_sector_id[10000*cid+100*sid+ch];
            spider_energy[n_spider] = energy2;
            spider_ts[n_spider] = ts2;
            n_spider++;
          }else{
            if(sid>=11&&sid<=12){
              s3_ring_sid[n_s3_ring] = sid2;
              s3_ring_ch[n_s3_ring] = ch2;
              s3_ring_id[n_s3_ring] = map_s3_ring_id[10000*cid+100*sid+ch];
              s3_ring_energy[n_s3_ring] = energy2;
              s3_ring_ts[n_s3_ring] = ts2;
              n_s3_ring++;
            }else{
              s3_sector_sid[n_s3_sector] = sid2;
              s3_sector_ch[n_s3_sector] = ch2;
              s3_sector_id[n_s3_sector] = map_s3_sector_id[10000*cid+100*sid+ch];
              s3_sector_energy[n_s3_sector] = energy2;
              s3_sector_ts[n_s3_sector] = ts2;
              n_s3_sector++;
            }
          }
        }
      }else break;
    } // while search forward

    // std::cout << "n_ge " << n_ge << std::endl;
    // std::cout << " n_spider " << n_spider << std::endl;
    // std::cout << " n_s3_sector " << n_s3_sector << " n_s3_ring " << n_s3_ring << std::endl;
    if(n_ge*(n_spider+n_s3_sector+n_s3_ring) > 0){
      n_evt++;

      file_out->cd();
      tr->Fill();

      if(n_evt%10000==0){
        std::cout << "\r" << n_evt << "  " << i << "/" << tr_in->GetEntries();
        std::cout << std::flush;
      }
    }

    n_ge = 0;
    memset(ge_sid, 0, sizeof(ge_sid));
    memset(ge_ch, 0, sizeof(ge_ch));
    memset(ge_ring_id, 0, sizeof(ge_ring_id));
    memset(ge_sector_id, 0, sizeof(ge_sector_id));
    memset(ge_energy, 0, sizeof(ge_energy));
    memset(ge_ts, 0, sizeof(ge_ts));

    n_spider = 0;
    memset(spider_sid, 0, sizeof(spider_sid));
    memset(spider_ch, 0, sizeof(spider_ch));
    memset(spider_ring_id, 0, sizeof(spider_ring_id));
    memset(spider_sector_id, 0, sizeof(spider_sector_id));
    memset(spider_energy, 0, sizeof(spider_energy));
    memset(spider_ts, 0, sizeof(spider_ts));

    n_s3_sector = 0;
    memset(s3_sector_sid, 0, sizeof(s3_sector_sid));
    memset(s3_sector_ch, 0, sizeof(s3_sector_ch));
    memset(s3_sector_id, 0, sizeof(s3_sector_id));
    memset(s3_sector_energy, 0, sizeof(s3_sector_energy));
    memset(s3_sector_ts, 0, sizeof(s3_sector_ts));

    n_s3_ring = 0;
    memset(s3_ring_sid, 0, sizeof(s3_ring_sid));
    memset(s3_ring_ch, 0, sizeof(s3_ring_ch));
    memset(s3_ring_id, 0, sizeof(s3_ring_id));
    memset(s3_ring_energy, 0, sizeof(s3_ring_energy));
    memset(s3_ring_ts, 0, sizeof(s3_ring_ts));

    while(true){//get next Ge data
      if(i>=tr_in->GetEntries()) break;
      tr_in->GetEntry(i);
      i++;
      if(cid==0 && evte>cut_ge){
        break;
      }
    }
    i_current = i-1;

  }//while

  std::cout << std::endl;

  file_out->cd();
  tr->Write();
}

//
void build::SaveFile()
{
  file_out->cd();
  file_out->Close();
}

//
bool build::InitMapSectorRingID()
{
  std::cout << "start init sector and ring id." << std::endl;

  //
  map_ge_sector_id = {
    {204, 8},
    {205, 9},
    {206, 10},
    {208, 2},
    {209, 3},
    {210, 7},
    {211, 1},
    {212, 1},
    {213, 1},
    {214, 1},
    {215, 1},

    {304, 2},
    {305, 3},
    {306, 5},
    {307, 3},
    {308, 8},
    {309, 6},
    {310, 4},
    {311, 6},

    {400, 3},
    {401, 3},
    {402, 3},
    {403, 3},
    {404, 4},
    {405, 4},
    {406, 4},
    {407, 4},
    {409, 5},
    {410, 5},
    {411, 5},

    {504, 1},
    {505, 3},
    {506, 2},
    {507, 4},
    {508, 6},
    {509, 5},
    {510, 7}
  };

  //
  map_ge_ring_id = {
    {204, 3},
    {205, 3},
    {206, 3},
    {208, 2},
    {209, 5},
    {210, 2},
    {211, 4},
    {212, 3},
    {213, 3},
    {214, 3},
    {215, 3},

    {304, 4},
    {305, 4},
    {306, 4},
    {307, 1},
    {308, 4},
    {309, 1},
    {310, 5},
    {311, 5},

    {400, 3},
    {401, 3},
    {402, 3},
    {403, 3},
    {404, 3},
    {405, 3},
    {406, 3},
    {407, 3},
    {409, 3},
    {410, 3},
    {411, 3},

    {504, 2},
    {505, 2},
    {506, 5},
    {507, 4},
    {508, 4},
    {509, 5},
    {510, 4}
  };

  //
  if((run>=457 && run<=462) || (run>=472 && run<=605) || (run>=680 && run<=718)){
    //
    map_spider_sector_id = {
      {10200, 2},
      {10201, 2},
      {10202, 2},
      {10203, 2},
      {10204, 2},
      {10205, 2},
      {10206, 2},
      {10207, 2},
      {10208, 1},
      {10209, 1},
      {10210, 1},
      {10211, 1},
      {10212, 1},
      {10213, 1},
      {10214, 1},
      {10215, 1},

      {10300, 4},
      {10301, 4},
      {10302, 4},
      {10303, 4},
      {10304, 4},
      {10305, 4},
      {10306, 4},
      {10307, 4},
      {10308, 3},
      {10309, 3},
      {10310, 3},
      {10311, 3},
      {10312, 3},
      {10313, 3},
      {10314, 3},
      {10315, 3},

      {10400, 6},
      {10401, 6},
      {10402, 6},
      {10403, 6},
      {10404, 6},
      {10405, 6},
      {10406, 6},
      {10407, 6},
      {10408, 5},
      {10409, 5},
      {10410, 5},
      {10411, 5},
      {10412, 5},
      {10413, 5},
      {10414, 5},
      {10415, 5},

      {10600, 7},
      {10601, 7},
      {10602, 7},
      {10603, 7},
      {10604, 7},
      {10605, 7},
      {10606, 7},
      {10607, 7},
      {10608, 8},
      {10609, 8},
      {10610, 8},
      {10611, 8},
      {10612, 8},
      {10613, 8},
      {10614, 8},
      {10615, 8},

      {10700, 9},
      {10701, 9},
      {10702, 9},
      {10703, 9},
      {10704, 9},
      {10705, 9},
      {10706, 9},
      {10707, 9},
      {10708, 10},
      {10709, 10},
      {10710, 10},
      {10711, 10},
      {10712, 10},
      {10713, 10},
      {10714, 10},
      {10715, 10},

      {10800, 11},
      {10801, 11},
      {10802, 11},
      {10803, 11},
      {10804, 11},
      {10805, 11},
      {10806, 11},
      {10807, 11},
      {10808, 12},
      {10809, 12},
      {10810, 12},
      {10811, 12},
      {10812, 12},
      {10813, 12},
      {10814, 12},
      {10815, 12}
    };

    //
    map_spider_ring_id = {
      {10200, 8},
      {10201, 7},
      {10202, 6},
      {10203, 5},
      {10204, 4},
      {10205, 3},
      {10206, 2},
      {10207, 1},
      {10208, 8},
      {10209, 7},
      {10210, 6},
      {10211, 5},
      {10212, 4},
      {10213, 3},
      {10214, 2},
      {10215, 1},

      {10300, 1},
      {10301, 2},
      {10302, 3},
      {10303, 4},
      {10304, 5},
      {10305, 6},
      {10306, 7},
      {10307, 8},
      {10308, 1},
      {10309, 2},
      {10310, 3},
      {10311, 4},
      {10312, 5},
      {10313, 6},
      {10314, 7},
      {10315, 8},

      {10400, 8},
      {10401, 7},
      {10402, 6},
      {10403, 5},
      {10404, 4},
      {10405, 3},
      {10406, 2},
      {10407, 1},
      {10408, 8},
      {10409, 7},
      {10410, 6},
      {10411, 5},
      {10412, 4},
      {10413, 3},
      {10414, 2},
      {10415, 1},

      {10600, 1},
      {10601, 2},
      {10602, 3},
      {10603, 4},
      {10604, 5},
      {10605, 6},
      {10606, 7},
      {10607, 8},
      {10608, 1},
      {10609, 2},
      {10610, 3},
      {10611, 4},
      {10612, 5},
      {10613, 6},
      {10614, 7},
      {10615, 8},

      {10700, 1},
      {10701, 2},
      {10702, 3},
      {10703, 4},
      {10704, 5},
      {10705, 6},
      {10706, 7},
      {10707, 8},
      {10708, 1},
      {10709, 2},
      {10710, 3},
      {10711, 4},
      {10712, 5},
      {10713, 6},
      {10714, 7},
      {10715, 8},

      {10800, 1},
      {10801, 2},
      {10802, 3},
      {10803, 4},
      {10804, 5},
      {10805, 6},
      {10806, 7},
      {10807, 8},
      {10808, 1},
      {10809, 2},
      {10810, 3},
      {10811, 4},
      {10812, 5},
      {10813, 6},
      {10814, 7},
      {10815, 8}
    };

    //
    map_s3_sector_id = {
      {10900, 10},
      {10901, 12},
      {10902, 14},
      {10903, 16},
      {10904, 18},
      {10905, 20},
      {10906, 22},
      {10907, 24},
      {10908, 26},
      {10909, 28},
      {10910, 30},
      {10911, 32},
      {10912, 2},
      {10913, 4},
      {10914, 6},
      {10915, 8},

      {11000, 11},
      {11001, 13},
      {11002, 15},
      {11003, 17},
      {11004, 19},
      {11005, 21},
      {11006, 23},
      {11007, 25},
      {11008, 27},
      {11009, 29},
      {11010, 31},
      {11011, 1},
      {11012, 3},
      {11013, 5},
      {11014, 7},
      {11015, 9}
    };
  }else if((run>=616 && run<=674) || (run>=721 && run<=723)){
    //
    map_spider_sector_id = {
      {10900, 2},
      {10901, 2},
      {10902, 2},
      {10903, 2},
      {10904, 2},
      {10905, 2},
      {10906, 2},
      {10907, 2},
      {10908, 1},
      {10909, 1},
      {10910, 1},
      {10911, 1},
      {10912, 1},
      {10913, 1},
      {10914, 1},
      {10915, 1},

      {11000, 4},
      {11001, 4},
      {11002, 4},
      {11003, 4},
      {11004, 4},
      {11005, 4},
      {11006, 4},
      {11007, 4},
      {11008, 3},
      {11009, 3},
      {11010, 3},
      {11011, 3},
      {11012, 3},
      {11013, 3},
      {11014, 3},
      {11015, 3},

      {10400, 6},
      {10401, 6},
      {10402, 6},
      {10403, 6},
      {10404, 6},
      {10405, 6},
      {10406, 6},
      {10407, 6},
      {10408, 5},
      {10409, 5},
      {10410, 5},
      {10411, 5},
      {10412, 5},
      {10413, 5},
      {10414, 5},
      {10415, 5},

      {10600, 7},
      {10601, 7},
      {10602, 7},
      {10603, 7},
      {10604, 7},
      {10605, 7},
      {10606, 7},
      {10607, 7},
      {10608, 8},
      {10609, 8},
      {10610, 8},
      {10611, 8},
      {10612, 8},
      {10613, 8},
      {10614, 8},
      {10615, 8},

      {10700, 9},
      {10701, 9},
      {10702, 9},
      {10703, 9},
      {10704, 9},
      {10705, 9},
      {10706, 9},
      {10707, 9},
      {10708, 10},
      {10709, 10},
      {10710, 10},
      {10711, 10},
      {10712, 10},
      {10713, 10},
      {10714, 10},
      {10715, 10},

      {10800, 11},
      {10801, 11},
      {10802, 11},
      {10803, 11},
      {10804, 11},
      {10805, 11},
      {10806, 11},
      {10807, 11},
      {10808, 12},
      {10809, 12},
      {10810, 12},
      {10811, 12},
      {10812, 12},
      {10813, 12},
      {10814, 12},
      {10815, 12}
    };

    //
    map_spider_ring_id = {
      {10900, 8},
      {10901, 7},
      {10902, 6},
      {10903, 5},
      {10904, 4},
      {10905, 3},
      {10906, 2},
      {10907, 1},
      {10908, 8},
      {10909, 7},
      {10910, 6},
      {10911, 5},
      {10912, 4},
      {10913, 3},
      {10914, 2},
      {10915, 1},

      {11000, 1},
      {11001, 2},
      {11002, 3},
      {11003, 4},
      {11004, 5},
      {11005, 6},
      {11006, 7},
      {11007, 8},
      {11008, 1},
      {11009, 2},
      {11010, 3},
      {11011, 4},
      {11012, 5},
      {11013, 6},
      {11014, 7},
      {11015, 8},

      {10400, 8},
      {10401, 7},
      {10402, 6},
      {10403, 5},
      {10404, 4},
      {10405, 3},
      {10406, 2},
      {10407, 1},
      {10408, 8},
      {10409, 7},
      {10410, 6},
      {10411, 5},
      {10412, 4},
      {10413, 3},
      {10414, 2},
      {10415, 1},

      {10600, 1},
      {10601, 2},
      {10602, 3},
      {10603, 4},
      {10604, 5},
      {10605, 6},
      {10606, 7},
      {10607, 8},
      {10608, 1},
      {10609, 2},
      {10610, 3},
      {10611, 4},
      {10612, 5},
      {10613, 6},
      {10614, 7},
      {10615, 8},

      {10700, 1},
      {10701, 2},
      {10702, 3},
      {10703, 4},
      {10704, 5},
      {10705, 6},
      {10706, 7},
      {10707, 8},
      {10708, 1},
      {10709, 2},
      {10710, 3},
      {10711, 4},
      {10712, 5},
      {10713, 6},
      {10714, 7},
      {10715, 8},

      {10800, 1},
      {10801, 2},
      {10802, 3},
      {10803, 4},
      {10804, 5},
      {10805, 6},
      {10806, 7},
      {10807, 8},
      {10808, 1},
      {10809, 2},
      {10810, 3},
      {10811, 4},
      {10812, 5},
      {10813, 6},
      {10814, 7},
      {10815, 8}
    };

    //
    map_s3_sector_id = {
      {10200, 10},
      {10201, 12},
      {10202, 14},
      {10203, 16},
      {10204, 18},
      {10205, 20},
      {10206, 22},
      {10207, 24},
      {10208, 26},
      {10209, 28},
      {10210, 30},
      {10211, 32},
      {10212, 2},
      {10213, 4},
      {10214, 6},
      {10215, 8},

      {10300, 11},
      {10301, 13},
      {10302, 15},
      {10303, 17},
      {10304, 19},
      {10305, 21},
      {10306, 23},
      {10307, 25},
      {10308, 27},
      {10309, 29},
      {10310, 31},
      {10311, 1},
      {10312, 3},
      {10313, 5},
      {10314, 7},
      {10315, 9}
    };
  }else{
  std::cout << "wrong run number." << std::endl;
    return 0;
  }

  //
  map_s3_ring_id = {
    {11102, 2},
    {11103, 4},
    {11104, 6},
    {11105, 8},
    {11106, 10},
    {11107, 12},
    {11108, 14},
    {11109, 16},
    {11110, 18},
    {11111, 20},
    {11112, 22},
    {11113, 24},

    {11200, 1},
    {11201, 3},
    {11202, 5},
    {11203, 7},
    {11204, 9},
    {11205, 11},
    {11206, 13},
    {11207, 15},
    {11208, 17},
    {11209, 19},
    {11210, 21},
    {11211, 23}
  };

  return 1;
}

//
void build::PrintMapSectorRingID()
{
  std::cout << "start print ring and sector id." << std::endl;

  std::cout << "spider sector id :" << std::endl;
  for(auto &it:map_spider_sector_id){
    std::cout << it.first << " => " << it.second << std::endl;
  }
  std::cout << std::endl;

  std::cout << "spider ring id :" << std::endl;
  for(auto &it:map_spider_ring_id){
    std::cout << it.first << " => " << it.second << std::endl;
  }
  std::cout << std::endl;

  std::cout << "s3 sector id :" << std::endl;
  for(auto &it:map_s3_sector_id){
    std::cout << it.first << " => " << it.second << std::endl;
  }
  std::cout << std::endl;

  std::cout << "s3 ring id :" << std::endl;
  for(auto &it:map_s3_ring_id){
    std::cout << it.first << " => " << it.second << std::endl;
  }
}

