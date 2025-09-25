# s3 fb cor

## 1. tree2gr.cpp / tree2gr_fast.cpp

### How to run

+ `./batch_tree2gr.sh` / `./batch_tree2gr_fast.sh`

### info

+ from s3 coincidence event to get 2D graph of ring and sector. can be used to get front-back correlation parameters.
+ input file: data%04d_build_s3_200ns_no_s3cor.root
+ output file: data%04d_gr2.root / data%04d_gr_fast.root
+ parmeter file: run.txt

## 2. draw_sample

### How to run

#### draw_sample_single

```
> root
> .L draw_sample.cpp
> draw_sample_single()
```

#### draw_sample

`./batch_draw_sample.sh`

### info

+ input file: data%04d_gr_fast.root
+ output file: some figures

### How to run

+ `./batch_new.sh`

### info

+ from ring and sector 2D graph, using pol1 fit to get correction parameters. `correction_sector_new.cpp`, `correction_ring_new.cpp`, `macro_new.cpp`, `batch_new.sh`
+ input file: data%04d_gr.root
+ output file: correction_ring_%04d.txt, correction_ring_%04d.txt, figures

## 2. correction

### How to run

+ `./batch_new.sh`

### info

+ from ring and sector 2D graph, using pol1 fit to get correction parameters. `correction_sector_new.cpp`, `correction_ring_new.cpp`, `macro_new.cpp`, `batch_new.sh`
+ input file: data%04d_gr.root
+ output file: correction_ring_%04d.txt, correction_ring_%04d.txt, figures
  

