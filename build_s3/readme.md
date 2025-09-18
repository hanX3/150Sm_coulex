# build s3

## 1. build

### How to run

+ `./build run_number`
+ `./batch.sh`

### info

+ from s3 raw data to get coincidence event. can be used for s3 energy attenuation and s3 front-back correlation correction. **need change `define` parameters in `set.h` file.**
+ input file: data_C1_%04d.root
+ output file: data%04d_build_s3_%dns.root, data%04d_build_s3_%dns_nos3cor.root, data%04d_build_s3_%dns_no_s3att_no_s3cor.root
+ parmeter file: run.txt, foldar run_s3_att, foldar run_fb_cor




