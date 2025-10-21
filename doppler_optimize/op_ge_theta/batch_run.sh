#!/usr/bin/bash

max_jobs=160

l_theta_offset=(0 2 4 6 8 10 350 352 354 356 358)

for theta_offset in "${l_theta_offset[@]}"; do
  ./run.sh ${theta_offset} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
