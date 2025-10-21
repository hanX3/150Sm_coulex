#!/usr/bin/bash

l_theta_offset=(0 30 60 90 120 150 180 210 240 270 300 330)

for theta_offset in "${l_theta_offset[@]}"; do
  ./hadd_hist.sh $theta_offset
done
