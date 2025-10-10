# build_all

## 1. build

### How to run

+ `./build run_number` / `./batch.sh`

### info

+ from calibration data build gamma-particle coincidence events. coincidence window can be setted in set.h.
+ input file: data%04d.root
+ output file: data$04d_build_200ns_jump_300ns.root
+ parmeter file: run.txt

## 2. evaluation/b2hist

### How to run

#### single

```
> root
> .L macro_b2hist.cpp
> macro_b2hist(472, 200, 300)
```

#### batch

`./batch_b2hist.sh`


### info

+ from build rootfile get histogram, including beam spot, hits number, particle spectra, and gamma spectra.
+ input file: data$04d_build_200ns_jump_300ns.root
+ output file: data$04d_build_200ns_jump_300ns_event.root, data$04d_build_200ns_jump_300ns_bg.root

## 2. evaluation/draw

### How to run

`./batch_draw_sample.sh`, `batch_draw_sample_no_bg.sh`

### info

+ from histgoram get some figures.
+ input file: data$04d_build_200ns_jump_300ns_event.root, data$04d_build_200ns_jump_300ns_bg.root
+ output file: some figures.

## 2. evaluation/draw_addback

### How to run

```
> root
> .L draw_addback.cpp
> draw_addback()
```

