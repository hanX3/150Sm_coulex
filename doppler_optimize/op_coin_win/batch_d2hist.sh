#!/usr/bin/bash

max_jobs=160

l_win=(25 50 100 150 200 250 300)

while read run; do
  for win in "${l_win[@]}"; do
    root -b -q "d2hist.cpp($run, $win)" &
  done

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done < "../../pars/run.txt"
