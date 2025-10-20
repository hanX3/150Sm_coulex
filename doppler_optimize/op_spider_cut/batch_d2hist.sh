#!/usr/bin/bash

max_jobs=160

l_cut=(4000 6000 8000 10000 15000 20000 25000 30000)

while read run; do
  for cut in "${l_cut[@]}"; do
    root -b -q "d2hist.cpp($run, $cut)" &
  done

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done < "../../pars/run.txt"
