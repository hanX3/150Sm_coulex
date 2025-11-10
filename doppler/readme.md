# doppler

## how to run ?

```
./doppler run_number coin_win random_win / ./batch.sh
```

## info

+ form build rootfile get doppler correction results;
+ input file: ../rootfile/data%04d_build_%dns_jump_%dns.root
+ output file: ../rootfile/data%04d_doppler_%dns_jump_%dns.root
+ * parmeter file: run.txt, angle information and beta information.

## evaluation/d2hist

```
./batch_d2hist.sh
```

### info

+ from doppler rootfile to get all histograms, very slow.

## evaluation/d2hist_simple

```
./batch_d2hist_simple.sh
```

### info

+ from doppler rootfile to get part of histograms, such as `ge all different spider rings`, `ge ring3,4,5 different spider rings`,`ge all different s3 rings` and `ge ring3,4,5 different s3 rings`.

## evaluation/d2th2

```
> root
> .L d2th2.cpp
> d2th2("data0472_0605_doppler_100ns_jump_300ns")
```

### info

+ from doppler rootfile to get $\gamma$-$\gamma$ coincidenve th2 histogram.

## evaluation/get_no_bg_hist

```
> root
> .L get_no_bg_hist.cpp
> get_no_bg_hist("doppler_100ns_p1")
```

### info

+ from `doppler_100ns_p1_hist.root/doppler_100ns_p2_hist.root` to get `doppler_100ns_p1_no_bg_hist.root/doppler_100ns_p2_no_bg_hist.root`
+ include event* and bg* histograms

## evaluation/get_rebin_hist

```
> root
> .L get_rebin_hist.cpp
> get_rebin_hist("doppler_100ns_p1_no_bg")
```

### info

+ add 4 s3 rings together.

## evaluation/get_hist_aa

```
> root
> .L get_hist_aa.cpp
> get_hist_aa("doppler_100ns_p1_no_bg")
```

### info

+ add all spider and s3 together to seewhere I can get.

