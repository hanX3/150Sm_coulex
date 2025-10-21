#!/usr/bin/bash

max_jobs=160

l_beta_par=(0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.2 1.4)

for beta_par in "${l_beta_par[@]}"; do
  ./run.sh ${beta_par} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
