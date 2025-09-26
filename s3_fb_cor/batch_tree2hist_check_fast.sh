#!/usr/bin/bash

max_jobs=120

while read run; do
  root -b -q "tree2hist_check_fast.cpp($run)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../pars/run.txt"

