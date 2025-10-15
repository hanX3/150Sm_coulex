#!/usr/bin/bash

max_jobs=120

l_phi_offset=(0 30 60 90 120 150 180 210 240 270 300 330)

for phi_offset in "${l_phi_offset[@]}"; do
  ./hadd_hist.sh $phi_offset
done
