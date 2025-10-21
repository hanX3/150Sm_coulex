#!/usr/bin/bash

max_jobs=160

l_theta_offset=(0 30 60 90 120 150 180 210 240 270 300 330)

for theta_offset in "${l_theta_offset[@]}"; do
  ./run.sh ${theta_offset} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
