#include "doppler.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "TH1D.h"

#include "set.h"

//
doppler::doppler(const std::string &filename_in, const std::string &filename_out, std::string config)
{
  configure = config;

  benchmark = new TBenchmark;

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "open file " << filename_in.c_str() << " error!" << std::endl;
    delete file_in;

    throw std::invalid_argument("can not open rootfile to read.");
  }

  file_out = new TFile(filename_out.c_str(), "recreate");
  if(!file_out){
    std::cout << "can not create " << filename_out << std::endl;
    delete file_out;
    
    throw std::invalid_argument("can not open rootfile to write.");
  }
  
  tr_event_out = new TTree("tr_event", "event doppler data");
  tr_event_out->Branch("hits", &hits, "hits/I");
  tr_event_out->Branch("flag_spider_p", &flag_spider_p, "flag_spider_p/O");
  tr_event_out->Branch("flag_spider_r", &flag_spider_r, "flag_spider_r/O");
  tr_event_out->Branch("flag_s3_p", &flag_s3_p, "flag_s3_p/O");
  tr_event_out->Branch("flag_s3_r", &flag_s3_r, "flag_s3_r/O");
  tr_event_out->Branch("rp", &rp, "rp/I");
  tr_event_out->Branch("sp", &sp, "sp/I");
  tr_event_out->Branch("rr", &rr, "rr/I");
  tr_event_out->Branch("sr", &sr, "sr/I");
  tr_event_out->Branch("rg", rg, "rg[hits]/I");
  tr_event_out->Branch("sg", sg, "sg[hits]/I");

  tr_event_out->Branch("e_raw", e_raw, "e_raw[hits]/D");
  tr_event_out->Branch("e_dc_p", e_dc_p, "e_dc_p[hits]/D");
  tr_event_out->Branch("e_dc_r", e_dc_r, "e_dc_r[hits]/D");

  tr_bg_out = new TTree("tr_bg", "bg doppler data");
  tr_bg_out->Branch("hits", &hits, "hits/I");
  tr_bg_out->Branch("flag_spider_p", &flag_spider_p, "flag_spider_p/O");
  tr_bg_out->Branch("flag_spider_r", &flag_spider_r, "flag_spider_r/O");
  tr_bg_out->Branch("flag_s3_p", &flag_s3_p, "flag_s3_p/O");
  tr_bg_out->Branch("flag_s3_r", &flag_s3_r, "flag_s3_r/O");
  tr_bg_out->Branch("rp", &rp, "rp/I");
  tr_bg_out->Branch("sp", &sp, "sp/I");
  tr_bg_out->Branch("rr", &rr, "rr/I");
  tr_bg_out->Branch("sr", &sr, "sr/I");
  tr_bg_out->Branch("rg", rg, "rg[hits]/I");
  tr_bg_out->Branch("sg", sg, "sg[hits]/I");

  tr_bg_out->Branch("e_raw", e_raw, "e_raw[hits]/D");
  tr_bg_out->Branch("e_dc_p", e_dc_p, "e_dc_p[hits]/D");
  tr_bg_out->Branch("e_dc_r", e_dc_r, "e_dc_r[hits]/D");

  //
  tr_event = (TTree*)file_in->Get("tr_event");
  tr_bg = (TTree*)file_in->Get("tr_bg");
  event_coin = new event(tr_event); 
  event_bg = new event(tr_bg); 
  
  //angle info
  angle = new angle_info();

  //
  if(!ReadGeConfigInfo()){
    throw std::invalid_argument("can not read ge configure info data.");
  }
  PrintGeConfigInfo();

  // cut info
  if(!ReadCutInfo()){
    throw std::invalid_argument("can not read cut info data.");
  }
  PrintCutInfo();
}

//
doppler::~doppler()
{
  delete file_in;
  delete event_coin;
  delete event_bg;
  delete file_out;
}

//
void doppler::Process()
{
  benchmark->Start("build");

  ProcessDoppler(event_coin, tr_event_out);
  ProcessDoppler(event_bg, tr_bg_out);

  benchmark->Show("doppler");
}

//
void doppler::ProcessDoppler(event *ed, TTree *tr)
{
  file_out->cd();
  Clear();

  std::vector<int> v_ge_index;
  std::vector<int> v_spider_p_index;
  std::vector<int> v_spider_r_index;
  std::vector<int> v_s3_sector_p_index;
  std::vector<int> v_s3_sector_r_index;
  std::vector<int> v_s3_ring_p_index;
  std::vector<int> v_s3_ring_r_index;

  //
  int num_spider_p = 0, num_spider_r = 0;
  int num_s3_sector_p = 0, num_s3_sector_r = 0;
  int num_s3_ring_p = 0, num_s3_ring_r = 0;

  int key = 0;

  double tg, pg, tp_p, pp_p, tp_r, pp_r;
  double cospg_p, cospg_r;
  double beta_p, beta_r;

  std::cout << "\ntotal entries " << ed->GetEntries() << std::endl;
  for(Long64_t i=0;i<ed->GetEntries();++i){
    if(i%10000==0){ 
      std::cout << "\r" << i << "/" << ed->GetEntries();
      std::cout << std::flush;
    }

    Clear();

    v_ge_index.clear();
    v_spider_p_index.clear();
    v_spider_r_index.clear();
    v_s3_sector_p_index.clear();
    v_s3_sector_r_index.clear();
    v_s3_ring_p_index.clear();
    v_s3_ring_r_index.clear();

    num_spider_p = 0; num_spider_r = 0; 
    num_s3_sector_p = 0; num_s3_sector_r = 0;
    num_s3_ring_p = 0; num_s3_ring_r = 0;
    flag_spider_p = 0; flag_spider_r = 0;
    flag_s3_p = 0; flag_s3_r = 0;

    key = 0;
    ed->GetEntry(i);

    // ge
    for(int j=0;j<ed->n_ge;j++){
      if(map_ge_flag[100*ed->ge_sid[j]+ed->ge_ch[j]]){
        v_ge_index.push_back(j);
      }
    }

    // spider
    for(int j=0;j<ed->n_spider;j++){
      key = 100*ed->spider_ring_id[j]+ed->spider_sector_id[j];
      if(ed->spider_energy[j]>map_spider_cut_p[key].first && ed->spider_energy[j]<map_spider_cut_p[key].second){
        num_spider_p++;
        v_spider_p_index.push_back(j);
      }
      if(ed->spider_energy[j]>map_spider_cut_r[key].first && ed->spider_energy[j]<map_spider_cut_r[key].second){
        num_spider_r++;
        v_spider_r_index.push_back(j);
      }
    }

    // s3 sector
    for(int j=0;j<ed->n_s3_sector;j++){
      key = ed->s3_sector_id[j];
      if(ed->s3_sector_energy[j]>map_s3_sector_cut_p[key].first && ed->s3_sector_energy[j]<map_s3_sector_cut_p[key].second){
        num_s3_sector_p++;
        v_s3_sector_p_index.push_back(j);
      }
      if(ed->s3_sector_energy[j]>map_s3_sector_cut_r[key].first && ed->s3_sector_energy[j]<map_s3_sector_cut_r[key].second){
        num_s3_sector_r++;
        v_s3_sector_r_index.push_back(j);
      }
    }

    // s3 ring
    for(int j=0;j<ed->n_s3_ring;j++){
      key = ed->s3_ring_id[j];
      if(ed->s3_ring_energy[j]>map_s3_ring_cut_p[key].first && ed->s3_ring_energy[j]<map_s3_ring_cut_p[key].second){
        num_s3_ring_p++;
        v_s3_ring_p_index.push_back(j);
      }
      if(ed->s3_ring_energy[j]>map_s3_ring_cut_r[key].first && ed->s3_ring_energy[j]<map_s3_ring_cut_r[key].second){
        num_s3_ring_r++;
        v_s3_ring_r_index.push_back(j);
      }
    }

    // std::cout << "\ni " << i 
    //           << " \nge enrgy             " << ed->ge_energy[0]
    //           << " \nnumber_spider_p      " << num_spider_p
    //           << " \nnumber_spider_r      " << num_spider_r
    //           << " \nnumber_s3_sector_p   " << num_s3_sector_p
    //           << " \nnumber_s3_sector_r   " << num_s3_sector_r
    //           << " \nnumber_s3_ring_p     " << num_s3_ring_p
    //           << " \nnumber_s3_ring_r     " << num_s3_ring_r
    //           << " \nflag_ge size          " << v_ge_index.size()
    //           << " \nflag_spider_p size    " << v_spider_p_index.size()
    //           << " flag_spider_r size      " << v_spider_r_index.size()
    //           << " \nflag_s3_sector_p size " << v_s3_sector_p_index.size()
    //           << " flag_s3_sector_r size   " << v_s3_sector_r_index.size()
    //           << " \nflag_s3_ring_p size   " << v_s3_ring_p_index.size()
    //           << " flag_s3_ring_r size     " << v_s3_ring_r_index.size()
    //           << "\n" << std::endl;
    // for(int j=0;j<ed->n_ge;j++){
    //   std::cout << "ge energy " << ed->ge_energy[j] << std::endl;
    // }
    // for(int j=0;j<ed->n_spider;j++){
    //   std::cout << "spider energy " << ed->spider_energy[j] << std::endl;
    // }
    // for(int j=0;j<ed->n_s3_ring;j++){
    //   std::cout << "s3 ring energy " << ed->s3_ring_energy[j] << std::endl;
    // }
    // for(int j=0;j<ed->n_s3_sector;j++){
    //   std::cout << "s3 sector energy " << ed->s3_sector_energy[j] << std::endl;
    // }
    // std::cout << "\n\n";

    // spider p
    if(num_spider_p>1){
      int max_index = v_spider_p_index[0];
      double max_e = ed->spider_energy[max_index];

      for(int j=1;j<num_spider_p;j++){
        int idx = v_spider_p_index[j];
        if(ed->spider_energy[idx]>max_e){
          max_index = idx;
          max_e = ed->spider_energy[idx];
        }
      }

      v_spider_p_index.clear();
      v_spider_p_index.push_back(max_index);
      num_spider_p = 1;
    }
    if(num_spider_p==1) flag_spider_p = 1;

    // spider r
    if(num_spider_r>1){
      int max_index = v_spider_r_index[0];
      double max_e = ed->spider_energy[max_index];

      for(int j=1;j<num_spider_r;j++){
        int idx = v_spider_r_index[j];
        if(ed->spider_energy[idx]>max_e){
          max_index = idx;
          max_e = ed->spider_energy[idx];
        }
      }

      v_spider_r_index.clear();
      v_spider_r_index.push_back(max_index);
      num_spider_r = 1;
    }
    if(num_spider_r==1) flag_spider_r = 1;

    // s3 p
    if(num_s3_sector_p==0 || num_s3_ring_p==0){
      flag_s3_p = 0;
      num_s3_sector_p = 0;
      num_s3_ring_p = 0;
    }else if (num_s3_sector_p==1 && num_s3_ring_p==1){
      flag_s3_p = 1;
    }else if (num_s3_sector_p==1 && num_s3_ring_p==2){
      flag_s3_p = 1;
      num_s3_sector_p = 1;
      num_s3_ring_p = 1;

      double e1 = ed->s3_sector_energy[v_s3_sector_p_index[0]];
      double e2 = ed->s3_ring_energy[v_s3_ring_p_index[0]];
      double e3 = ed->s3_ring_energy[v_s3_ring_p_index[1]];

      if(std::abs(e1-e2) < (std::abs(e1-e3))){
        v_s3_ring_p_index.erase(v_s3_ring_p_index.begin()+1);
      }else{
        v_s3_ring_p_index.erase(v_s3_ring_p_index.begin());
      }
    }else if(num_s3_sector_p==2 && num_s3_ring_p==1){
      flag_s3_p = 1;
      num_s3_sector_p = 1;
      num_s3_ring_p = 1;

      double e1 = ed->s3_ring_energy[v_s3_ring_p_index[0]];
      double e2 = ed->s3_sector_energy[v_s3_sector_p_index[0]];
      double e3 = ed->s3_sector_energy[v_s3_sector_p_index[1]];

      if(std::abs(e1-e2) < (std::abs(e1-e3))){
        v_s3_sector_p_index.erase(v_s3_sector_p_index.begin()+1);
      }else{
        v_s3_sector_p_index.erase(v_s3_sector_p_index.begin());
      }
    }else{
      flag_s3_p = 0;
      num_s3_sector_p = 0;
      num_s3_ring_p = 0;

      v_s3_sector_p_index.clear();
      v_s3_ring_p_index.clear();
    }

    // s3 r
    if(num_s3_sector_r==0 || num_s3_ring_r==0){
      flag_s3_r = 0;
      num_s3_sector_r = 0;
      num_s3_ring_r = 0;
    }else if (num_s3_sector_r==1 && num_s3_ring_r==1){
      flag_s3_r = 1;
    }else if (num_s3_sector_r==1 && num_s3_ring_r==2){
      flag_s3_r = 1;
      num_s3_sector_r = 1;
      num_s3_ring_r = 1;

      double e1 = ed->s3_sector_energy[v_s3_sector_r_index[0]];
      double e2 = ed->s3_ring_energy[v_s3_ring_r_index[0]];
      double e3 = ed->s3_ring_energy[v_s3_ring_r_index[1]];

      if(std::abs(e1-e2) < (std::abs(e1-e3))){
        v_s3_ring_r_index.erase(v_s3_ring_r_index.begin()+1);
      }else{
        v_s3_ring_r_index.erase(v_s3_ring_r_index.begin());
      }
    }else if(num_s3_sector_r==2 && num_s3_ring_r==1){
      flag_s3_r = 1;
      num_s3_sector_r = 1;
      num_s3_ring_r = 1;

      double e1 = ed->s3_ring_energy[v_s3_ring_r_index[0]];
      double e2 = ed->s3_sector_energy[v_s3_sector_r_index[0]];
      double e3 = ed->s3_sector_energy[v_s3_sector_r_index[1]];

      if(std::abs(e1-e2) < (std::abs(e1-e3))){
        v_s3_sector_r_index.erase(v_s3_sector_r_index.begin()+1);
      }else{
        v_s3_sector_r_index.erase(v_s3_sector_r_index.begin());
      }
    }else{
      flag_s3_r = 0;
      num_s3_sector_r = 0;
      num_s3_ring_r = 0;

      v_s3_sector_r_index.clear();
      v_s3_ring_r_index.clear();
    }

    // std::cout << "i " << i 
    //          << " \nge enrgy             " << ed->ge_energy[0]
    //          << " \nnumber_spider_p      " << num_spider_p
    //          << " \nnumber_spider_r      " << num_spider_r
    //          << " \nnumber_s3_sector_p   " << num_s3_sector_p
    //          << " \nnumber_s3_sector_r   " << num_s3_sector_r
    //          << " \nnumber_s3_ring_p     " << num_s3_ring_p
    //          << " \nnumber_s3_ring_r     " << num_s3_ring_r
    //          << " \nflag_ge size          " << v_ge_index.size()
    //          << " \nflag_spider_p size    " << v_spider_p_index.size()
    //          << " \nflag_spider_r size      " << v_spider_r_index.size()
    //          << " \nflag_s3_sector_p size " << v_s3_sector_p_index.size()
    //          << " \nflag_s3_sector_r size   " << v_s3_sector_r_index.size()
    //          << " \nflag_s3_ring_p size   " << v_s3_ring_p_index.size()
    //          << " \nflag_s3_ring_r size     " << v_s3_ring_r_index.size()
    //          << "\n" << std::endl;

    // for(int j=0;j<ed->n_ge;j++){
    //   std::cout << "ge energy " << ed->ge_energy[j] << std::endl;
    // }
    // if(flag_spider_p){
    //   for(int j=0;j<num_spider_p;j++){
    //     std::cout << "spider p energy " << ed->spider_energy[v_spider_p_index[j]] << std::endl;
    //   }
    // }
    // if(flag_spider_r){
    //   for(int j=0;j<num_spider_r;j++){
    //     std::cout << "spider r energy " << ed->spider_energy[v_spider_r_index[j]] << std::endl;
    //   }
    // }
    // if(flag_s3_p){
    //   for(int j=0;j<num_s3_sector_p;j++){
    //     std::cout << "s3 sector p energy " << ed->s3_sector_energy[v_s3_sector_p_index[j]] << std::endl;
    //   }
    //   for(int j=0;j<num_s3_ring_p;j++){
    //     std::cout << "s3 ring p energy " << ed->s3_ring_energy[v_s3_ring_p_index[j]] << std::endl;
    //   }
    // }
    // if(flag_s3_r){
    //   for(int j=0;j<num_s3_sector_r;j++){
    //     std::cout << "s3 sector r energy " << ed->s3_sector_energy[v_s3_sector_r_index[j]] << std::endl;
    //   }
    //   for(int j=0;j<num_s3_ring_p;j++){
    //     std::cout << "s3 ring r energy " << ed->s3_ring_energy[v_s3_ring_r_index[j]] << std::endl;
    //   }
    // }
    // std::cout << "\n\n";

    // spider p
    if(flag_spider_p && !flag_spider_r && !flag_s3_p && !flag_s3_r){
      int ii = v_spider_p_index[0];
      
      for(size_t j=0;j<v_ge_index.size();j++){
        int jj = v_ge_index[j];
        rg[j] = ed->ge_ring_id[jj];
        sg[j] = ed->ge_sector_id[jj];

        e_raw[j] = ed->ge_energy[jj];

        tg = angle->GetGammaTheta(ed->ge_ring_id[jj], ed->ge_sector_id[jj]);
        pg = angle->GetGammaPhi(ed->ge_ring_id[jj], ed->ge_sector_id[jj]);

        tp_p = angle->GetProjectileSpiderTheta(ed->spider_ring_id[ii], ed->spider_sector_id[ii]);
        pp_p = angle->GetProjectileSpiderPhi(ed->spider_ring_id[ii], ed->spider_sector_id[ii]);
        cospg_p = angle->ComputeCosThetaParticleGamma(tg, pg, tp_p, pp_p);
        beta_p = angle->ComputeProjectileBeta(tp_p, 1000); // 1000fs means stopped
        e_dc_p[j] = ComputeEnergyWithDoppler(ed->ge_energy[jj], beta_p, cospg_p);

        tp_r = angle->GetRecoilTheta(tp_p);
        pp_r = angle->GetRecoilPhi(pp_p);
        cospg_r = angle->ComputeCosThetaParticleGamma(tg, pg, tp_r, pp_r);
        beta_r = angle->ComputeRecoilBeta(tp_r, 1000); // 1000fs means stopped
        e_dc_r[j] = ComputeEnergyWithDoppler(ed->ge_energy[jj], beta_r, cospg_r);

        // std::cout << "ge ring " << rg[j] << " sector " << sg[j] << std::endl;   
        // std::cout << "ge ring " << ed->ge_ring_id[j] << " sector " << ed->ge_sector_id[j] << " energy " << ed->ge_energy[j] << " keV " << std::endl;
        // std::cout << "tg " << tg << " pg " << pg << std::endl;
        // std::cout << "beta_p " << beta_p << " cospg_p " << cospg_p << std::endl;
        // std::cout << "beta_r " << beta_r << " cospg_r " << cospg_r << std::endl;
        // std::cout << "e_raw " << e_raw[j] << std::endl;
        // std::cout << "e_dc_p " << e_dc_p[j] << std::endl;
        // std::cout << "e_dc_r " << e_dc_r[j] << std::endl;
      }

      hits = v_ge_index.size();
      rp = ed->spider_ring_id[ii];
      sp = ed->spider_sector_id[ii];

      if(hits>0) tr->Fill();
      Clear();
    }

    // s3 p
    if(!flag_spider_p && !flag_spider_r && flag_s3_p && !flag_s3_r){
      int ii_sector = v_s3_sector_p_index[0];
      int ii_ring = v_s3_ring_p_index[0];

      for(size_t j=0;j<v_ge_index.size();j++){
        int jj = v_ge_index[j];
        rg[j] = ed->ge_ring_id[jj];
        sg[j] = ed->ge_sector_id[jj];

        e_raw[j] = ed->ge_energy[jj];

        tg = angle->GetGammaTheta(ed->ge_ring_id[jj], ed->ge_sector_id[jj]);
        pg = angle->GetGammaPhi(ed->ge_ring_id[jj], ed->ge_sector_id[jj]);

        tp_p = angle->GetProjectileS3Theta(ed->s3_ring_id[ii_ring]);
        pp_p = angle->GetProjectileS3Phi(ed->spider_sector_id[ii_sector]);
        cospg_p = angle->ComputeCosThetaParticleGamma(tg, pg, tp_p, pp_p);
        beta_p = angle->ComputeProjectileBeta(tp_p, 1000); // 1000fs means stopped
        e_dc_p[j] = ComputeEnergyWithDoppler(ed->ge_energy[jj], beta_p, cospg_p);

        tp_r = angle->GetRecoilTheta(tp_p);
        pp_r = angle->GetRecoilPhi(pp_p);
        cospg_r = angle->ComputeCosThetaParticleGamma(tg, pg, tp_r, pp_r);
        beta_r = angle->ComputeRecoilBeta(tp_r, 1000); // 1000fs means stopped
        e_dc_r[j] = ComputeEnergyWithDoppler(ed->ge_energy[jj], beta_r, cospg_r);

        // std::cout << "ge ring " << rg[j] << " sector " << sg[j] << std::endl;   
        // std::cout << "ge ring " << ed->ge_ring_id[j] << " sector " << ed->ge_sector_id[j] << " energy " << ed->ge_energy[j] << " keV " << std::endl;
        // std::cout << "tg " << tg << " pg " << pg << std::endl;
        // std::cout << "beta_p " << beta_p << " cospg_p " << cospg_p << std::endl;
        // std::cout << "beta_r " << beta_r << " cospg_r " << cospg_r << std::endl;
        // std::cout << "e_raw " << e_raw[j] << std::endl;
        // std::cout << "e_dc_p " << e_dc_p[j] << std::endl;
        // std::cout << "e_dc_r " << e_dc_r[j] << std::endl;
      }

      hits = v_ge_index.size();
      rp = ed->s3_ring_id[ii_ring];
      sp = ed->s3_sector_id[ii_ring];

      if(hits>0) tr->Fill();
      Clear();
    }

    // std::cout << "\nooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n";
  }
    
  std::cout << std::endl;
}

//
void doppler::SaveFile()
{
  file_out->cd();
  tr_event_out->Write();
  tr_bg_out->Write();
  file_out->Close();
}

//
double doppler::ComputeEnergyWithDoppler(double energy, double beta, double tg, double pg, double tp, double pp)
{
  tg = tg/180.*PI;
  pg = pg/180.*PI;
  tp = tp/180.*PI;
  pp = pp/180.*PI;

  double tmp1 = sin(tp)*sin(tg)*cos(pp-pg);
  double tmp2 = cos(tp)*cos(tg);

  double energy_doppler = energy*(1-beta*(tmp1+tmp2))/sqrt(1-beta*beta);

  return energy_doppler;
}

//
double doppler::ComputeEnergyWithDoppler(double energy, double beta, double cospg)
{
  //std::cout << "new ... " << angle->GetCosThetaParticleGamma(mod_ge, ch_ge, si_ring_id, si_sector_id) << " si " << angle->GetSiTheta(si_ring_id) << " " << angle->GetSiPhi(si_sector_id) << " ge " << angle->GetGeTheta(mod_ge, ch_ge) << " " << angle->GetGePhi(mod_ge, ch_ge) << std::endl;
  double energy_doppler = energy*(1-beta*cospg)/sqrt(1-beta*beta);

  return energy_doppler; 
}

//
bool doppler::ReadCutInfo()
{
  std::cout << "read cut info" << std::endl;

  std::string line;
  int ring = 0, sector = 0;
  double min = 0, max = 0;
  int key = 0;

  // spider cut p
  std::ifstream fi_spider_cut_p;
  fi_spider_cut_p.open("../../../../../pars/cut_info/spider_cut_p.txt");
  if(!fi_spider_cut_p){
    std::cout << "can not open spider_cut_p.txt." << std::endl;
    return 0;
  }
  std::getline(fi_spider_cut_p, line);
  while(1){
    fi_spider_cut_p >> ring >> sector >> min >> max;
    if(!fi_spider_cut_p)  break;

    key = 100*ring + sector;
    map_spider_cut_p[key] = std::make_pair(min, max);
  }
  fi_spider_cut_p.close();

  // spider cut r
  std::ifstream fi_spider_cut_r;
  fi_spider_cut_r.open("../../../../../pars/cut_info/spider_cut_r.txt");
  if(!fi_spider_cut_r){
    std::cout << "can not open spider_cut_r.txt." << std::endl;
    return 0;
  }
  std::getline(fi_spider_cut_r, line);
  while(1){
    fi_spider_cut_r >> ring >> sector >> min >> max;
    if(!fi_spider_cut_r)  break;

    key = 100*ring + sector;
    map_spider_cut_r[key] = std::make_pair(min, max);
  }
  fi_spider_cut_r.close();

  // s3 sector cut p
  std::ifstream fi_s3_sector_cut_p;
  fi_s3_sector_cut_p.open("../../../../../pars/cut_info/s3_sector_cut_p.txt");
  if(!fi_s3_sector_cut_p){
    std::cout << "can not open s3_sector_cut_p.txt." << std::endl;
    return 0;
  }
  std::getline(fi_s3_sector_cut_p, line);
  while(1){
    fi_s3_sector_cut_p >> sector >> min >> max;
    if(!fi_s3_sector_cut_p)  break;

    key = sector;
    // map_s3_sector_cut_p[key] = std::make_pair(min, max);
    map_s3_sector_cut_p[key] = std::make_pair(CUTS3PMIN, max);
  }
  fi_s3_sector_cut_p.close();

  // s3 sector cut r
  std::ifstream fi_s3_sector_cut_r;
  fi_s3_sector_cut_r.open("../../../../../pars/cut_info/s3_sector_cut_r.txt");
  if(!fi_s3_sector_cut_r){
    std::cout << "can not open s3_sector_cut_r.txt." << std::endl;
    return 0;
  }
  std::getline(fi_s3_sector_cut_r, line);
  while(1){
    fi_s3_sector_cut_r >> sector >> min >> max;
    if(!fi_s3_sector_cut_r)  break;

    key = sector;
    map_s3_sector_cut_r[key] = std::make_pair(min, max);
  }
  fi_s3_sector_cut_p.close();

  // s3 ring cut p
  std::ifstream fi_s3_ring_cut_p;
  fi_s3_ring_cut_p.open("../../../../../pars/cut_info/s3_ring_cut_p.txt");
  if(!fi_s3_ring_cut_p){
    std::cout << "can not open s3_ring_cut_p.txt." << std::endl;
    return 0;
  }
  std::getline(fi_s3_ring_cut_p, line);
  while(1){
    fi_s3_ring_cut_p >> ring >> min >> max;
    if(!fi_s3_ring_cut_p)  break;

    key = ring;
    // map_s3_ring_cut_p[key] = std::make_pair(min, max);
    map_s3_ring_cut_p[key] = std::make_pair(CUTS3PMIN, max);
  }
  fi_s3_ring_cut_p.close();

  // s3 ring cut r
  std::ifstream fi_s3_ring_cut_r;
  fi_s3_ring_cut_r.open("../../../../../pars/cut_info/s3_ring_cut_r.txt");
  if(!fi_s3_ring_cut_r){
    std::cout << "can not open s3_ring_cut_r.txt." << std::endl;
    return 0;
  }
  std::getline(fi_s3_ring_cut_r, line);
  while(1){
    fi_s3_ring_cut_r >> ring >> min >> max;
    if(!fi_s3_ring_cut_r)  break;

    key = ring;
    map_s3_ring_cut_r[key] = std::make_pair(min, max);
  }
  fi_s3_ring_cut_r.close();

  return 1;
}

//
void doppler::PrintCutInfo()
{
  // spider cut p
  std::cout << "spider cut p cut info " << std::endl; 
  for(const auto &it : map_spider_cut_p){
    std::cout << "ring " << it.first/100 << " sector " << it.first%100 << " e_min " << it.second.first << " e_max " << it.second.second << std::endl;
  }

  // spider cut r
  std::cout << "spider cut r cut info " << std::endl; 
  for(const auto &it : map_spider_cut_r){
    std::cout << "ring " << it.first/100 << " sector " << it.first%100 << " e_min " << it.second.first << " e_max " << it.second.second << std::endl;
  }

  // s3 ring cut p
  std::cout << "s3 ring cut p cut info " << std::endl; 
  for(const auto &it : map_s3_ring_cut_p){
    std::cout << "ring " << it.first << " e_min " << it.second.first << " e_max " << it.second.second << std::endl;
  }

  // s3 ring cut r
  std::cout << "s3 ring cut r cut info " << std::endl; 
  for(const auto &it : map_s3_ring_cut_r){
    std::cout << "ring " << it.first << " e_min " << it.second.first << " e_max " << it.second.second << std::endl;
  }

  // s3 sector cut p
  std::cout << "s3 sector cut p cut info " << std::endl; 
  for(const auto &it : map_s3_sector_cut_p){
    std::cout << "sector " << it.first << " e_min " << it.second.first << " e_max " << it.second.second << std::endl;
  }

  // s3 sector cut r
  std::cout << "s3 sector cut r cut info " << std::endl; 
  for(const auto &it : map_s3_sector_cut_r){
    std::cout << "sector " << it.first << " e_min " << it.second.first << " e_max " << it.second.second << std::endl;
  }
}

//
bool doppler::ReadGeConfigInfo()
{
  std::cout << "read ge configure info" << std::endl;

  std::string line;
  int sid = 0, ch = 0;
  bool gb = 0;
  int key = 0;

  // ge configure
  std::ifstream fi;
  fi.open(TString::Format("../../../../../pars/config/%s.txt", configure.c_str()));
  if(!fi){
    std::cout << "can not open configure.txt." << std::endl;
    return 0;
  }
  std::getline(fi, line);
  while(1){
    fi >> sid >> ch >> gb;
    if(!fi)  break;

    key = 100*sid + ch;
    map_ge_flag[key] = gb;
  }
  fi.close();

  return 1;
}

//
void doppler::PrintGeConfigInfo()
{
  //
  std::cout << "ge configure info " << std::endl; 
  for(const auto &it : map_ge_flag){
    std::cout << "sid " << it.first/100 << " ch " << it.first%100 << " " << it.second << std::endl;
  }
}

//
void doppler::Clear()
{
  hits = 0;
  flag_spider_p = 0; flag_spider_r = 0;
  flag_s3_p = 0; flag_s3_r = 0;
  rp = 0; sp = 0;
  rr = 0; sr = 0;
  memset(rg, 0, sizeof(rg));
  memset(sg, 0, sizeof(sg));

  memset(e_raw, 0, sizeof(e_raw));
  memset(e_dc_p, 0, sizeof(e_dc_p));
  memset(e_dc_r, 0, sizeof(e_dc_r));
}
