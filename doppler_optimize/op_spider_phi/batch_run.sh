#!/usr/bin/bash

max_jobs=120

l_phi_rot=(0 30 60 90 120 150 180 210 240 270 300 330)

for phi_rot in "${l_phi_rot[@]}"; do
  ./run.sh ${phi_rot} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
