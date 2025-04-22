#ifndef SET_H
#define SET_H

constexpr int nMaxS3Ring = 24;
constexpr int nMaxS3Sector = 32;

constexpr int TimeWindow = 200; // ns
constexpr int TimeJump = 500; // ns

constexpr double CutSiEnergy = 50.; // 

constexpr int n_trace = 200;
constexpr int n_sinc = 12; // The number of interpolation points
constexpr int t_sinc = 64; // decay velocity of gaussian function
constexpr int l_sinc = 16; // number of lobe

constexpr int BaselineLength = 30;
constexpr double S3Threshold = 30; 
constexpr int MoveAverageLength = 8;

#endif
