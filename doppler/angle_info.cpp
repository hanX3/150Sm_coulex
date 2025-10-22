#include "angle_info.h"
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <cmath>

#include "set.h"

angle_info::angle_info()
{
  if(!InitAngleInfo()){
    throw std::invalid_argument("can not init angle info.");
  }
  PrintAngleInfo();
  
  if(!InitEinEoutInfo()){
    throw std::invalid_argument("can not init e_in and e_out info.");
  }

  PrintEinEoutInfo();
  PrintThetaP2RInfo();

  PrintBetaInfo();
}

//
angle_info::~angle_info()
{
  
}

//  
bool angle_info::InitAngleInfo()
{
  std::cout << "start read angle info data" << std::endl;

  std::string line;
  int ring, sector;
  double theta, phi;
  int key = 0;

  //
  std::ifstream fi_ge;
  fi_ge.open("../pars/angle_info/ge_angle.txt");
  if(!fi_ge){
    std::cout << "can not open ge_angle.txt." << std::endl;
    return 0;
  }

  std::getline(fi_ge, line);
  while(1){
    fi_ge >> ring  >> sector >> theta >> phi;
    if(!fi_ge.good()) break;

    key = 100*ring+sector;
    map_ge_angle[key] = std::make_pair(theta, phi);
  }
  fi_ge.close();

  //
  std::ifstream fi_spider;
  fi_spider.open("../pars/angle_info/spider_angle.txt");
  if(!fi_spider){
    std::cout << "can not open spider_angle.txt." << std::endl;
    return 0;
  }

  std::getline(fi_spider, line);
  while(1){
    fi_spider >> ring  >> sector >> theta >> phi;
    if(!fi_spider.good()) break;

    key = 100*ring+sector;
    map_spider_angle[key] = std::make_pair(theta, phi);
  }
  fi_spider.close();

  //
  std::ifstream fi_s3_sector;
  fi_s3_sector.open("../pars/angle_info/s3_sector_angle.txt");
  if(!fi_s3_sector){
    std::cout << "can not open s3_sector_angle.txt." << std::endl;
    return 0;
  }

  std::getline(fi_s3_sector, line);
  while(1){
    fi_s3_sector >> sector >> phi;
    if(!fi_s3_sector.good()) break;

    key = sector;
    map_s3_sector_phi[key] = phi;
  }
  fi_s3_sector.close();

  //
  std::ifstream fi_s3_ring;
  fi_s3_ring.open("../pars/angle_info/s3_ring_angle.txt");
  if(!fi_s3_ring){
    std::cout << "can not open s3_ring_angle.txt." << std::endl;
    return 0;
  }

  std::getline(fi_s3_ring, line);
  while(1){
    fi_s3_ring >> ring >> theta;
    if(!fi_s3_ring.good()) break;

    key = ring;
    map_s3_ring_theta[key] = theta;
  }
  fi_s3_ring.close();

  return 1;
}

//
void angle_info::PrintAngleInfo()
{
  std::cout << "print angle info ..." << std::endl;
  
  //
  std::cout << std::endl;
  std::cout << "print ge angle info ..." << std::endl;
  std::map<int, std::pair<double, double>>::iterator it_ge = map_ge_angle.begin();
  for(;it_ge!=map_ge_angle.end();it_ge++){
    std::cout << "ring " << it_ge->first/100 << " sector " << it_ge->first%100<< " => " << it_ge->second.first << " " << it_ge->second.second << '\n';
  }

  //
  std::cout << std::endl;
  std::cout << "print spider angle info ..." << std::endl;
  std::map<int, std::pair<double, double>>::iterator it_spider = map_spider_angle.begin();
  for(;it_spider!=map_spider_angle.end();it_spider++){
    std::cout << "ring " << it_spider->first/100 << " sector " << it_spider->first%100<< " => " << it_spider->second.first << " " << it_spider->second.second << '\n';
  }

  //
  std::cout << std::endl;
  std::cout << "print s3 sector phi info ..." << std::endl;
  std::map<int, double>::iterator it_s3_sector = map_s3_sector_phi.begin();
  for(;it_s3_sector!=map_s3_sector_phi.end();it_s3_sector++){
    std::cout << "sector " << it_s3_sector->first << " => " << it_s3_sector->second << '\n';
  }

  //
  std::cout << std::endl;
  std::cout << "print s3 ring theta info ..." << std::endl;
  std::map<int, double>::iterator it_s3_ring = map_s3_ring_theta.begin();
  for(;it_s3_ring!=map_s3_ring_theta.end();it_s3_ring++){
    std::cout << "ring " << it_s3_ring->first << " => " << it_s3_ring->second << '\n';
  }
}

//
bool angle_info::InitEinEoutInfo()
{
  std::cout << "start read Ein Eout info data" << std::endl;

  //
  std::ifstream fi_p;
  fi_p.open("../pars/beta_info/theta_ein_eout_p.txt");
  if(!fi_p){
    std::cout << "can not open theta_ein_eout_p.txt." << std::endl;
    return 0;
  }

  char c_theta_p[32];
  double theta_p, e_in, e_out;
  while(1){
    fi_p >> theta_p >> e_in >> e_out;
    if(!fi_p.good()) break;

    theta_p = std::round(theta_p * 100.0) / 100.0;
    std::sprintf(c_theta_p, "%.2f", theta_p);
    map_theta_ein_eout_p[c_theta_p] = std::make_pair(e_in, e_out);
  }
  fi_p.close();

  //
  std::ifstream fi_r;
  fi_r.open("../pars/beta_info/theta_ein_eout_r.txt");
  if(!fi_r){
    std::cout << "can not open theta_ein_eout_r.txt." << std::endl;
    return 0;
  }

  char c_theta_r[32];
  double theta_r;
  while(1){
    fi_r >> theta_p >> theta_r >> e_in >> e_out;
    if(!fi_r.good()) break;

    theta_p = std::round(theta_p * 100.0) / 100.0;
    theta_r = std::round(theta_r * 100.0) / 100.0;
    std::sprintf(c_theta_p, "%.2f", theta_p);
    std::sprintf(c_theta_r, "%.2f", theta_r);
    if(theta_p<90){
      map_theta_ein_eout_r[c_theta_r] = std::make_pair(e_in, e_out*0.5);
    }else{
      map_theta_ein_eout_r[c_theta_r] = std::make_pair(e_in, e_out);
    }
    map_theta_p2r[c_theta_p] = theta_r;
  }
  fi_r.close();

  return 1;
}

//
void angle_info::PrintEinEoutInfo()
{
  std::cout << "print ein eout info ..." << std::endl;
  
  //
  std::cout << std::endl;
  std::map<std::string, std::pair<double, double>>::iterator it_p = map_theta_ein_eout_p.begin();
  for(;it_p!=map_theta_ein_eout_p.end();it_p++){
    std::cout << it_p->first << " => " << it_p->second.first << " " << it_p->second.second << '\n';
  }

  //
  std::cout << std::endl;
  std::map<std::string, std::pair<double, double>>::iterator it_r = map_theta_ein_eout_r.begin();
  for(;it_r!=map_theta_ein_eout_r.end();it_r++){
    std::cout << it_r->first << " => " << it_r->second.first << " " << it_r->second.second << '\n';
  }
}

//
void angle_info::PrintThetaP2RInfo()
{
  std::cout << "print theta_p to theta_r info ..." << std::endl;

  //
  std::cout << std::endl;
  std::map<std::string, double>::iterator it = map_theta_p2r.begin();
  for(;it!=map_theta_p2r.end();it++){
    std::cout << it->first << " => " << it->second << '\n';
  }
}

//
void angle_info::PrintBetaInfo()
{
  std::cout << "print projectile beta info ..." << std::endl;

  //
  std::map<std::string, double>::iterator it = map_theta_p2r.begin();
  for(;it!=map_theta_p2r.end();it++){
    std::cout << "p ==> " << it->first << "deg, beta " << ComputeProjectileBeta(std::stod(it->first), 1000.) << std::endl;
    std::cout << "r ==> " << it->second << "deg, beta " << ComputeRecoilBeta(it->second, 1000.) << std::endl;
    std::cout << std::endl;
  }

  std::cout << std::endl;
}

//
double angle_info::GetRecoilTheta(double theta_p)
{
  char c_theta_p[32];
  theta_p = std::round(theta_p * 100.0) / 100.0;
  std::sprintf(c_theta_p, "%.2f", theta_p);

  return map_theta_p2r[c_theta_p];
}

//
double angle_info::GetRecoilPhi(double phi_p)
{ 
  return phi_p+180; 
}

// dt unit fs
double angle_info::ComputeProjectileBeta(double theta, double dt)
{
  double half_target_thickness = TARGETTHICKNESS/2.;

  char c_theta[32];
  theta = std::round(theta * 100.0) / 100.0;
  std::sprintf(c_theta, "%.2f", theta);

  double e_in = map_theta_ein_eout_p[c_theta].first;
  double e_out = map_theta_ein_eout_p[c_theta].second;

  double d = 0;
  if(theta>90)  d = half_target_thickness/std::cos((180.-theta)/180.*PI); // half target thickness = 0.8 um.
  else d = half_target_thickness/std::cos(theta/180.*PI); // half target thickness = 0.8 um.

  double m_p = 16*938.3 + 16*931.5 - 32*8.493;
  double beta_in = std::sqrt(2.*e_in/m_p);
  double beta_out = std::sqrt(2.*e_out/m_p);

  double v_in = beta_in*300; // d/v = time with unit ps
  double v_out = beta_out*300;

  double a = (v_out*v_out-v_in*v_in)/2./d;
  double v_now = v_in + a*dt/1000.;
  double beta_now = 0;
  
  if(v_now<v_out){
    beta_now = beta_out;
  }else{
    beta_now = v_now/300.;
  }

  // std::cout << "projectile a " << a << " t " << (v_out-v_in)/a << "ns " << v_now << " " << beta_now << std::endl;
  return beta_now;
}

// dt unit fs
double angle_info::ComputeRecoilBeta(double theta, double dt)
{
  double half_target_thickness = TARGETTHICKNESS/2.;

  char c_theta[32];
  theta = std::round(theta * 100.0) / 100.0;
  std::sprintf(c_theta, "%.2f", theta);

  double e_in = map_theta_ein_eout_r[c_theta].first;
  double e_out = map_theta_ein_eout_r[c_theta].second;

  double d = 0;
  if(theta>90)  d = half_target_thickness/std::cos((180.-theta)/180.*PI); // half target thickness = 0.8 um.
  else d = half_target_thickness/std::cos(theta/180.*PI); // half target thickness = 0.8 um.

  double m_r = 62*938.3 + 88*931.5 - 150*8.262;
  double beta_in = std::sqrt(2.*e_in/m_r);
  double beta_out = std::sqrt(2.*e_out/m_r);

  double v_in = beta_in*300; // d/v = time with unit ps
  double v_out = beta_out*300;

  double a = (v_out*v_out-v_in*v_in)/2./d;
  double v_now = v_in + a*dt/1000.;
  double beta_now = 0;
  
  if(v_now<v_out){
    beta_now = beta_out;
  }else{
    beta_now = v_now/300.;
  }

  // std::cout << "recoil a " << a << " t " << (v_out-v_in)/a << "ns " << v_now << " " << beta_now << std::endl;
  return beta_now;
}

//
double angle_info::ComputeCosThetaParticleGamma(double tg, double pg, double tp, double pp)
{
  tg = tg/180.*PI;
  pg = pg/180.*PI;
  tp = tp/180.*PI;
  pp = pp/180.*PI;

  double tmp1 = std::sin(tp)*std::sin(tg)*std::cos(pp-pg);
  double tmp2 = std::cos(tp)*std::cos(tg);

  return tmp1+tmp2;
}

