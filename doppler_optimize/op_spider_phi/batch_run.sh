#!/usr/bin/bash

max_jobs=160

l_phi_offset=(0 5 10 330 335 340 345 350 355)

for phi_offset in "${l_phi_offset[@]}"; do
  ./run.sh ${phi_offset} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
