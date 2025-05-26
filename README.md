# 150Sm_coulex

## step1 &rarr; raw2hist
+ `get_hist_raw.cpp`, `macro_raw.cpp`, `batch.sh`: from tree structure rootfile to histogram without calibration parameters, for 2 crates data.
+ `get_hist_cali_ge.cpp,` `macro_cali_ge.cpp`, `batch.sh`: get histogram, add ge calibration parmeters, only for ge crate.
+ `get_hist_cali_si.cpp,` `macro_cali_si.cpp`, `batch.sh`: get histogram, add si calibration parmeters, only for si crate.
+ `get_hist_cali.cpp,` `macro_cali.cpp`, `batch.sh`: get histogram, add ge and si calibration parmeters, only for 2 crates.

## step2 &rarr; cali
### cali_ge
+ `auto_cali.cpp`, `get_par.cpp`, `macro_get_par.cpp`: TSprectrum peak search, and fit each peak, pol2 fit.
+ `compute_k.cpp`, `macro_compute_k.cpp`: get k parameter.

### cali_si
+ `auto_cali.cpp`, `get_par.cpp`, `macro_get_par.cpp`: TSprectrum peak search, and fit each peak, pol1 fit.
+ `compute_average.cpp`: use for spider detector which there maybe some channel can not calibration with alpha source.

### shift_resolution
+ `compute_resolution.cpp`: compute 1406 keV resolution for a run.
+ `shift_source.cpp`, `shift_inbeam.cpp`: check ge detector whether shift for a certain energy.

## step3 &rarr; front and back correlation for s3
### build_s3
`build.h build.cpp set.h main.cpp`: build s3 coincidence events.
`batch.sh`: batch run shell script.

### s3_fb_cor
+ `correction_ring.cpp`, `correction_sector.cpp`, `macro.cpp`, `batch.sh`: from tree get correlation pars, very slow.
+ `tree2gr.cpp`, `batch_tree2gr.sh`: from tree to get graph first.
+ `correction_ring_new.cpp`, `correction_sector_new.cpp`, `macro_new.cpp`, `batch_new.sh`: get correlation pars form graph, faster.

## step4 &rarr; ts offset, time walk
### get_ts_offset
+ `main.cpp analysis.h analysis.cpp raw.h raw.cpp set.h batch.sh`: get time difference histogram.
+ `fit_offset.cpp`: use fit to get time offset, recommand this method.
+ `max_offset.cpp`: use maximum to get time offset.

### ge_time_walk [not that necessary]
+ `main.cpp build.h build.cpp set.h`: from tree using ts.offset to get events. (data%04d_ge_tw_%dns.root)
+ `tree2th.cpp`, `macro.cpp`, `batch_tree2th.sh`: form events to get time difference spectra, and th2d with energy. (../rootfile/data%04d_ts_diff_%s.root)
+ `fit_time_walk.cpp`: from th2d time difference spectra, using ProfileY() and fit to get time walk.

### check_ts_align
+ `main.cpp build.h build.cpp set.h`: from tree using ts.offset and time walk paras to get events.(data%04d_align_%dns.root)
+ `tree2th.cpp`, `macro.cpp`, `batch_tree2th.sh`: form events to get time difference spectra, and th2d with energy. (../rootfile/data%04d_align_ts_diff_%s.root)

## step5 &rarr; real_cali_sort_ge_si
+ `main.cpp sort.h sort.cpp raw.h raw.cpp set.h`: get cali and ts alignment data. (data%04d.root, with cid,sid,ch,evte,ts branch.)
+ `batch.sh`: batch run.

## step6 &rarr; build_all
+ `main.cpp build.h build.cpp set.h`: build events with ge fired. (data%04d_build_%dns.root)
+ `b2hist.cpp`, `macro.cpp`, `batch.sh`: from tree to get some histogram. (data%04d_build_200ns_hist_%s.root)

## step7 &rarr; doppler
### beta_info
+ see readme in beta_info flodar.

### doppler
+ `main.cpp doppler.h doppler.cpp event.h event.cpp angle_info.h angle_info.cpp set.h`: main code for doppler. (data%04d_doppler_%dns_%s.root)
+ `d2hist.cpp`, `macro.cpp`, `batch.sh`: form doppler result to histogram.
+ `draw_sample.cpp`: draw and save figures.
+ `analysi.cpp`, `gate.cpp`: analysis and gate code.


