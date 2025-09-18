#ifndef angle_info_h
#define angle_info_h

#define PI 3.14159265

#include <map>
#include <vector>
#include <string>

#include "TString.h"

//
class angle_info{
public:
  angle_info();
  virtual ~angle_info();

private:
  std::map<int, std::pair<double, double>> map_ge_angle; // theta,phi
  std::map<int, std::pair<double, double>> map_spider_angle; // theta,phi
  std::map<int, double> map_s3_sector_phi;
  std::map<int, double> map_s3_ring_theta;

  std::map<std::string, std::pair<double, double>> map_theta_ein_eout_p;
  std::map<std::string, std::pair<double, double>> map_theta_ein_eout_r;
  std::map<std::string, double> map_theta_p2r;

public:
  bool InitAngleInfo();
  void PrintAngleInfo();

  bool InitEinEoutInfo();
  void PrintEinEoutInfo();
  void PrintThetaP2RInfo();

public:
  double GetGammaTheta(int ring, int sector) { return map_ge_angle[100*ring+sector].first; }
  double GetGammaPhi(int ring, int sector) { return map_ge_angle[100*ring+sector].second; }
  
  double GetProjectileSpiderTheta(int ring, int sector) { return map_spider_angle[100*ring+sector].first; } 
  double GetProjectileSpiderPhi(int ring, int sector) { return map_spider_angle[100*ring+sector].second; }
  double GetProjectileS3Theta(int ring) { return map_s3_ring_theta[ring]; } 
  double GetProjectileS3Phi(int sector) { return map_s3_sector_phi[sector]; }

  double GetRecoilTheta(double theta_p);
  double GetRecoilPhi(double phi_p);

  double ComputeProjectileBeta(double theta, double dt);
  double ComputeRecoilBeta(double theta, double dt); // theta

public:
  // tg: theta gamma
  // pg: phi gamma
  // tp: theta particle
  // pp: phi particle
  double ComputeCosThetaParticleGamma(double tg, double pg, double tp, double pp);
};

#endif
