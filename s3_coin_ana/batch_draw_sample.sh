#!/usr/bin/bash

max_jobs=120

while read run; do
  mkdir -p fig/$(printf "%04d" $run) 

  root -b -q "draw_sample.cpp($run)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../pars/run.txt"

