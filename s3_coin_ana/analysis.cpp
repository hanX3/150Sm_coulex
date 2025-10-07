#include "analysis.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <ctime>

#include "TH1D.h"

#include "set.h"

//
analysis::analysis(int r, const std::string &filename_in, const std::string &filename_out)
{
  benchmark = new TBenchmark;

  file_in = TFile::Open(filename_in.c_str());
  if(file_in->IsZombie()){
    std::cout << "open file " << filename_in.c_str() << " error!" << std::endl;
    delete file_in;

    return ;
  }
  tree = (TTree*)file_in->Get("tr_event");

  file_out = new TFile(filename_out.c_str(), "recreate");
  if(!file_out){
    std::cout << "can not create " << filename_out << std::endl;

    delete file_out;
    return ;
  }

  st = new s3_tree(tree); 

  if(!ReadS3AngleData()){
    throw std::invalid_argument("can not read s3 angle data.");
  }
  PrintS3AngleData();

  //
  Double_t m_p = APro;
  Double_t m_r = ATar;
  Double_t e_p = EBeam;

  Double_t delta_e = 2.; // MeV
  Double_t e_tilde = e_p - delta_e*(1.+m_p/m_r);
  tau_tilde = TMath::Sqrt(e_p/e_tilde);
  tau = m_p/m_r*tau_tilde;
  std::cout << "tau " << tau << std::endl;
  std::cout << "tau_tilde " << tau_tilde << std::endl;

  //
  run = r;

  rndm = new TRandom3((Long64_t)time(0));
}

//
analysis::~analysis()
{
  file_in->Close();
  delete st;
}

//
void analysis::Process()
{
  benchmark->Start("analysis");

  //
  double tp_lab, tr_lab, pp_lab, pr_lab;
  double tp_cm, tr_cm, pp_cm, pr_cm;

  TH2D *hh_e_ring_id = new TH2D("hh_e_ring_id", "", 24, 1, 25, 10000, 0, 100000); 
  TH2D *hh_e_theta = new TH2D("hh_e_theta", "", 30, 0, 90, 10000, 0, 100000); 
  TH2D *hh_e_theta_coin = new TH2D("hh_e_theta_coin", "", 30, 0, 90, 10000, 0, 100000); 
  TH1D *h_phi_diff = new TH1D("h_phi_diff", "", 12, 0, 360);
  TH1D *h_theta_sum = new TH1D("h_theta_sum", "", 120, 0, 360);
  TH2D *hh_theta_pr = new TH2D("hh_theta_pr", "", 45, 0, 90, 45, 0, 90);
  TH1D *h_p = new TH1D("h_p", "", 1000, 0, 100000);
  TH1D *h_r = new TH1D("h_r", "", 1000, 0, 100000);

  for(Long64_t i=0;i<st->GetEntries();++i){
    if(i%10000==0){ 
      std::cout << "\r" << i << "/" << st->GetEntries();
      std::cout << std::flush;
    }

    st->GetEntry(i);

    if(st->n_s3_ring==1 && st->n_s3_sector==1){
      hh_e_ring_id->Fill(st->s3_ring_id[0], st->s3_ring_energy[0]);
      hh_e_theta->Fill(m_r2theta[st->s3_ring_id[0]], st->s3_ring_energy[0]);
    }

    if(st->n_s3_ring==2 && st->n_s3_sector==2){
      hh_e_theta->Fill(m_r2theta[st->s3_ring_id[0]], st->s3_ring_energy[0]);
      hh_e_theta->Fill(m_r2theta[st->s3_ring_id[1]], st->s3_ring_energy[1]);

      tr_lab = m_r2theta[st->s3_ring_id[0]];    
      pr_lab = m_s2phi[st->s3_sector_id[0]];
      tp_lab = m_r2theta[st->s3_ring_id[1]];
      pp_lab = m_s2phi[st->s3_sector_id[1]];

      tp_cm = ThetaLab2CMProjectile(tp_lab);
      tr_cm = ThetaLab2CMRecoil(tr_lab);
      pp_cm = pp_lab;
      pr_cm = pr_lab;

      h_phi_diff->Fill(std::abs(pp_cm-pr_cm));

      if(std::abs(pp_cm-pr_cm)>160 && std::abs(pp_cm-pr_cm)<200){  
        h_theta_sum->Fill(tp_cm+tr_cm);
      }

      if(std::abs(pp_cm-pr_cm)>160 && std::abs(pp_cm-pr_cm)<200 && (tp_cm+tr_cm)>160 && (tp_cm+tr_cm)<200){
        hh_e_theta_coin->Fill(m_r2theta[st->s3_ring_id[0]], st->s3_ring_energy[0]);
        hh_e_theta_coin->Fill(m_r2theta[st->s3_ring_id[1]], st->s3_ring_energy[1]);
        
        hh_theta_pr->Fill(tp_lab, tr_lab);

        h_p->Fill(st->s3_ring_energy[1]);
        h_r->Fill(st->s3_ring_energy[0]);
      }
    }
  }
  
  file_out->cd();

  hh_e_ring_id->Write();
  hh_e_theta->Write();
  hh_e_theta_coin->Write();
  h_phi_diff->Write();
  h_theta_sum->Write();
  hh_theta_pr->Write();
  h_p->Write();
  h_r->Write();

  file_out->Close();

  benchmark->Show("analysis");
}

//
double analysis::ThetaLab2CMProjectile(double t)
{
  double t_cm = TMath::ASin(tau*TMath::Sin(t/180.*TMath::Pi()))/TMath::Pi()*180. + t;
  
  return t_cm;
}

//
double analysis::ThetaLab2CMRecoil(double t)
{
  double t_cm = TMath::ASin(tau_tilde*TMath::Sin(t/180.*TMath::Pi()))/TMath::Pi()*180. + t;
  
  return t_cm;
}

//
bool analysis::ReadS3AngleData()
{
  int id;
  double par;

  std::string line;

  //
  std::ifstream fi_s3_ring_angle("../pars/angle_info/s3_ring_angle.txt");
  if(!fi_s3_ring_angle){
    std::cout << "can not open s3 ring angle file." << std::endl;
    return 0;
  }else{
    std::getline(fi_s3_ring_angle, line);
    
    while(fi_s3_ring_angle >> id >> par){
      m_r2theta[id] = par;
    }
    fi_s3_ring_angle.close();
  }

  //
  std::ifstream fi_s3_sector_angle("../pars/angle_info/s3_sector_angle.txt");
  if(!fi_s3_sector_angle){
    std::cout << "can not open s3 ring angle file." << std::endl;
    return 0;
  }else{
    std::getline(fi_s3_sector_angle, line);
    
    while(fi_s3_sector_angle >> id >> par){
      m_s2phi[id] = par;
    }
    fi_s3_sector_angle.close();
  }

  return 1;
}

//
void analysis::PrintS3AngleData()
{
  std::cout << "\nstart print s3 angle data" << std::endl;

  //
  for(auto &[r,t]:m_r2theta){
    std::cout << "ring id " << r << " --> " << t << std::endl;
  }
  std::cout << std::endl;

  //
  for(auto &[s,p]:m_s2phi){
    std::cout << "sector id " << s << " --> " << p << std::endl;
  }
  std::cout << std::endl;
}
