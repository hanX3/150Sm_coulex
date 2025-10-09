#!/usr/bin/bash

max_jobs=120

while read run; do
  root -b -q "draw_sample.cpp($run, 200, 300)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../../pars/run.txt"

