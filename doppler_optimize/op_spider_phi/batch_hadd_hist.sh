#!/usr/bin/bash

l_phi_offset=(0 5 10 330 335 340 345 350 355)

for phi_offset in "${l_phi_offset[@]}"; do
  ./hadd_hist.sh $phi_offset
done
