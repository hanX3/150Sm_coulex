#!/usr/bin/bash

l_theta_offset=(0 2 4 6 8 10 350 352 354 356 358)

for theta_offset in "${l_theta_offset[@]}"; do
  ./hadd_hist.sh $theta_offset
done
