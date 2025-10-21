#!/usr/bin/bash

max_jobs=160

l_phi_offset=(0 30 60 90 120 150 180 210 240 270 300 330)

while read run; do
  for phi_offset in "${l_phi_offset[@]}"; do
    root -b -q "d2hist.cpp($run, $phi_offset)" &
  done

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done < "../../pars/run.txt"
