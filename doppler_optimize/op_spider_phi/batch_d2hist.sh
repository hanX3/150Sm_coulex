#!/usr/bin/bash

max_jobs=160

l_phi_offset=(0 5 10 330 335 340 345 350 355)

while read run; do
  for phi_offset in "${l_phi_offset[@]}"; do
    root -b -q "d2hist.cpp($run, $phi_offset)" &
  done

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done < "../../pars/run.txt"
