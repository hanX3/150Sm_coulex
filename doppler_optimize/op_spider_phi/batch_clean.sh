#!/usr/bin/bash

max_jobs=120

l_phi_offset=(0 5 10 330 335 340 345 350 355)

for phi_offset in "${l_phi_offset[@]}"; do
  name=$(printf "%03ddeg" ${phi_offset}) 
  rm -rf try/${name}
done
