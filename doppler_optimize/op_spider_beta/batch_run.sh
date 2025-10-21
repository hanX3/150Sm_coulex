#!/usr/bin/bash

max_jobs=160

l_beta_par=(0.2 0.4 0.6 0.8 1.0 1.2 1.4 1.6 1.8 2.0)

for beta_par in "${l_beta_par[@]}"; do
  ./run.sh ${beta_par} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
