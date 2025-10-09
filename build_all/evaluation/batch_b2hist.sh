#!/usr/bin/bash

max_jobs=120

mkdir -p terminal_out

while read run; do
  root -b -q "macro_b2hist.cpp($run, 200, 300)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../../pars/run.txt"

