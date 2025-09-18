# s3 energy attenuation

## 1. tree2hist.cpp

### How to run

`./batch_tree2hist.sh`

### info

+ from s3 coincidence data to get histogram of each ring and sector in S3 Si detector
+ input file: data????_build_s3_200ns_no_s3cor.root
+ output file: data????_ring_hist_no_s3cor.root data????_sector_hist_no_s3cor.root
+ parmeter file: run.txt

## 2. draw_sample.cpp

### How to run

`./batch_draw_sample.sh`

### info

+ draw histogram from hist rootfile.
+ input file: data????_ring_hist.root / data????_ring_hist_no_s3cor.root  data????_sector_hist.root / data????_sector_hist_no_s3cor.root
  + ouput file: figures in flodar `fig`

## 3. fit_xy_new2.cpp

### How to run

`batch_fit_xy_new2.sh`

### info

+ get energy attenuation pars between reference run and certain run.
+ input file: data????_ring_hist_no_s3cor.root data????_sector_hist_no_s3cor.root
+ out putfile: lots of run0680_0718_ring01.txt files in foldar `pars`; lots of figures in `fig`



