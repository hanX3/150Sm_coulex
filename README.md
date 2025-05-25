# 150Sm_coulex

## raw2hist
+ `get_hist_raw.cpp`, `macro_raw.cpp`, `batch.sh`: from tree structure rootfile to histogram without calibration parameters, for 2 crates data.
+ `get_hist_cali_ge.cpp,` `macro_cali_ge.cpp`, `batch.sh`: get histogram, add ge calibration parmeters, only for ge crate.
+ `get_hist_cali_si.cpp,` `macro_cali_si.cpp`, `batch.sh`: get histogram, add si calibration parmeters, only for si crate.
+ `get_hist_cali.cpp,` `macro_cali.cpp`, `batch.sh`: get histogram, add ge and si calibration parmeters, only for 2 crates.
