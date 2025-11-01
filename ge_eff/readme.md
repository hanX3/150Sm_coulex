# ge eff

Efficiency calibration using 133Ba and 152Eu $\gamma$ source.

## energy and intensity

### 133Ba

| energy [keV] | intensity |
| --- | --- |
| 81.00 |34.06 | 
| 302.85|18.33 |
|356.02|62.05|
|383.85|8.94|

### 152Eu

| energy [keV] | intensity |
| --- | --- |
|121.78 |28.58| 
|244.70|7.58|
|344.28|26.5|
|411.12|2.234|
|443.96|3.148|
|778.90|12.94|
|867.37|4.245|
|964.08|14.60|
|1112.1|13.64|
|1408.0|21.00|

## info

+ input file: data0722_build_200ns_jump_300ns.root, fit_range.txt
+ output file: terminal out of some certain peak effciency and error, some figure with fit results.

### How to run

#### eff_cali_ring_ge.cpp

```
> root
> .L eff_cali_ring_ge.cpp
> eff_cali_ring_ge(3,2) // (3,2) is one example, need to change
```

#### eff_cali_all_ge.cpp

```
> root
> .L eff_cali_all_ge.cpp
> eff_cali_all_ge()
```

### details

+ `eff_cali_ring_ge.cpp` and `eff_cali_all_ge.cpp` code.
+ In each `cpp` file, including the folowing functions.
  + Double_t gpeakexregion(Double_t *v, Double_t *par), Gaussian + Left-Skew + Step + Background.
  + Double_t bgstep(Double_t *v, Double_t *par), Step + Pol1.
  + Double_t gpeak(Double_t *v, Double_t *par), Gaussian + Left-Skew.
  + double eff_fun(double *x, double *p), radware Ge detector efficiency function.
  + double *eff_fit(map<double, double> &m,  map<double, double> &m_err), fitting.
  + void get_ring_ge_h(int r), get histgoram of certain ge detector.
  + double eff_cali_ring_ge_single_peak(TH1D *h, double energy, double l_bg, double l_peak, double u_peak, double u_bg, double ll_ex, double lr_ex, double hl_ex, double hr_ex), get peak yield used for fitting.
  + void eff_cali_ring_ge(int r), fit certain ge detector.



 
