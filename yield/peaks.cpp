//
// pars = 9
// Gaussian + Left-Skew
Double_t gpeak(Double_t *v, Double_t *par);

// pars = 5
// Step + Pol1
Double_t bgstep(Double_t *v, Double_t *par);

// pars = 12
// Gaussian + Left-Skew + Step + Background
Double_t gpeakexregion(Double_t *v, Double_t *par);

// pars = 18
// two Gaussian + two Left-Skew + two Step + Background
Double_t g2peakexregion(Double_t *v, Double_t *par);

// pars = 24
// three Gaussian + three Left-Skew + three Step + Background
Double_t g3peakexregion(Double_t *v, Double_t *par);

//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

// pars = 5
// Gaussian + Left-Skew
Double_t gpeak(Double_t *v, Double_t *par)
{
  Double_t energy  = par[0];
  Double_t gamma  = par[1];

  Double_t delta = par[2]; // Gaussian
  Double_t a = par[3]; // Left-Skew
  Double_t beta = par[4]; // Left-Skew
                          //
  Double_t gaussian = gamma*exp(-pow((v[0]-energy)/delta, 2.));
  Double_t left_skew = gamma*1/2.*a*exp((v[0]-energy)/beta);
  left_skew *= erfc(delta/2./beta + (v[0]-energy)/delta);

  //
  Double_t result = gaussian+left_skew; 

  return result;
}

// pars 5
// Step + Pol1
Double_t bgstep(Double_t *v, Double_t *par)
{
  Double_t p0 = par[0];
  Double_t p1 = par[1];
  Double_t energy = par[2];
  Double_t delta  = par[3];
  Double_t step   = par[4];

  Double_t result = p0 + p1*v[0];
  result += step*1/2.*erfc((v[0]-energy)/delta);
   
  return result;
}

// pars = 12
// Gaussian + Left-Skew + Step + Background
Double_t gpeakexregion(Double_t *v, Double_t *par)
{
  if(v[0]>par[8] && v[0]<par[9]){// low exclude region
    TF1::RejectPoint();
    return 0;
  }
  if(v[0]>par[10] && v[0]<par[11]){// high exclude region
    TF1::RejectPoint();
    return 0;
  }
  
  Double_t energy  = par[0];
  Double_t gamma  = par[1];

  Double_t delta = par[2]; // Gaussian
  Double_t a = par[3]; // Left-Skew
  Double_t beta = par[4]; // Left-Skew
  Double_t s = par[5]; // Step
  Double_t bg0 = par[6]; // Background
  Double_t bg1 = par[7]; // Background

  Double_t gaussian = gamma*exp(-pow((v[0]-energy)/delta, 2.));
  Double_t left_skew = gamma*1/2.*a*exp((v[0]-energy)/beta);
  left_skew *= erfc(delta/2./beta + (v[0]-energy)/delta);
  Double_t step = gamma*s*1/2.*erfc((v[0]-energy)/delta);
  Double_t background = bg0+bg1*v[0];

  //
  Double_t result = gaussian+left_skew+step+background; 

  return result;
}

//
// pars = 18
// two Gaussian + two Left-Skew + two Step + Background
Double_t g2peakexregion(Double_t *v, Double_t *par)
{
  if(v[0]>par[14] && v[0]<par[15]){// low exclude region
    TF1::RejectPoint();
    return 0;
  }
  if(v[0]>par[16] && v[0]<par[17]){// high exclude region
    TF1::RejectPoint();
    return 0;
  }
  
  // peak1
  Double_t energy1  = par[0];
  Double_t gamma1  = par[1];
  Double_t delta1 = par[2]; // Gaussian
  Double_t a1 = par[3]; // Left-Skew
  Double_t beta1 = par[4]; // Left-Skew
  Double_t s1 = par[5]; // Left-Skew

  // peak2
  Double_t energy2  = par[6];
  Double_t gamma2  = par[7];
  Double_t delta2 = par[8]; // Gaussian
  Double_t a2 = par[9]; // Left-Skew
  Double_t beta2 = par[10]; // Left-Skew
  Double_t s2 = par[11]; // Left-Skew
  
  // Background
  Double_t bg0 = par[12];
  Double_t bg1 = par[13];

  // peak1
  Double_t gaussian1 = gamma1*exp(-pow((v[0]-energy1)/delta1, 2.));
  Double_t left_skew1 = gamma1*1/2.*a1*exp((v[0]-energy1)/beta1);
  left_skew1 *= erfc(delta1/2./beta1 + (v[0]-energy1)/delta1);
  Double_t step1 = gamma1*s1*1/2.*erfc((v[0]-energy1)/delta1);

  // peak2
  Double_t gaussian2 = gamma2*exp(-pow((v[0]-energy2)/delta2, 2.));
  Double_t left_skew2 = gamma2*1/2.*a2*exp((v[0]-energy2)/beta2);
  left_skew2 *= erfc(delta2/2./beta2 + (v[0]-energy2)/delta2);
  Double_t step2 = gamma2*s2*1/2.*erfc((v[0]-energy2)/delta2);

  // Background
  Double_t background = bg0+bg1*v[0];

  //
  Double_t result = gaussian1+left_skew1+step1
                  + gaussian2+left_skew2+step2
                  + background; 

  return result;
}

//
// pars = 24
// three Gaussian + three Left-Skew + three Step + Background
Double_t g3peakexregion(Double_t *v, Double_t *par)
{
  if(v[0]>par[20] && v[0]<par[21]){// low exclude region
    TF1::RejectPoint();
    return 0;
  }
  if(v[0]>par[22] && v[0]<par[23]){// high exclude region
    TF1::RejectPoint();
    return 0;
  }
  
  // peak1
  Double_t energy1  = par[0];
  Double_t gamma1  = par[1];
  Double_t delta1 = par[2]; // Gaussian
  Double_t a1 = par[3]; // Left-Skew
  Double_t beta1 = par[4]; // Left-Skew
  Double_t s1 = par[5]; // Left-Skew

  // peak2
  Double_t energy2  = par[6];
  Double_t gamma2  = par[7];
  Double_t delta2 = par[8]; // Gaussian
  Double_t a2 = par[9]; // Left-Skew
  Double_t beta2 = par[10]; // Left-Skew
  Double_t s2 = par[11]; // Left-Skew
 
  // peak3
  Double_t energy3  = par[12];
  Double_t gamma3  = par[13];
  Double_t delta3 = par[14]; // Gaussian
  Double_t a3 = par[15]; // Left-Skew
  Double_t beta3 = par[16]; // Left-Skew
  Double_t s3 = par[17]; // Left-Skew
  
  // Background
  Double_t bg0 = par[18];
  Double_t bg1 = par[19];

  // peak1
  Double_t gaussian1 = gamma1*exp(-pow((v[0]-energy1)/delta1, 2.));
  Double_t left_skew1 = gamma1*1/2.*a1*exp((v[0]-energy1)/beta1);
  left_skew1 *= erfc(delta1/2./beta1 + (v[0]-energy1)/delta1);
  Double_t step1 = gamma1*s1*1/2.*erfc((v[0]-energy1)/delta1);

  // peak2
  Double_t gaussian2 = gamma2*exp(-pow((v[0]-energy2)/delta2, 2.));
  Double_t left_skew2 = gamma2*1/2.*a2*exp((v[0]-energy2)/beta2);
  left_skew2 *= erfc(delta2/2./beta2 + (v[0]-energy2)/delta2);
  Double_t step2 = gamma2*s2*1/2.*erfc((v[0]-energy2)/delta2);

  // peak3
  Double_t gaussian3 = gamma3*exp(-pow((v[0]-energy3)/delta3, 2.));
  Double_t left_skew3 = gamma3*1/2.*a3*exp((v[0]-energy3)/beta3);
  left_skew3 *= erfc(delta3/2./beta3 + (v[0]-energy3)/delta3);
  Double_t step3 = gamma3*s3*1/2.*erfc((v[0]-energy3)/delta3);

  // Background
  Double_t background = bg0+bg1*v[0];

  //
  Double_t result = gaussian1+left_skew1+step1
                  + gaussian2+left_skew2+step2
                  + gaussian3+left_skew3+step3
                  + background; 

  return result;
}
