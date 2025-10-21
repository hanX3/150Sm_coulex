#!/usr/bin/bash

max_jobs=160

l_theta_offset=(0 2 4 6 8 10 350 352 354 356 358)

while read run; do
  for theta_offset in "${l_theta_offset[@]}"; do
    root -b -q "d2hist.cpp($run, $theta_offset)" &
  done

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done < "../../pars/run.txt"
