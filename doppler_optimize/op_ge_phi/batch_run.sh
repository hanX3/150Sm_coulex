#!/usr/bin/bash

max_jobs=160

l_phi_offset=(0 30 60 90 120 150 180 210 240 270 300 330)

for phi_offset in "${l_phi_offset[@]}"; do
  ./run.sh ${phi_offset} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
