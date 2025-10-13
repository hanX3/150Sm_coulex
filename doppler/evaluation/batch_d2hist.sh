#!/usr/bin/bash

max_jobs=160

mkdir -p terminal_cout

while read run; do
  root -b -q "d2hist.cpp($run, 50, 300)" &
  
  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../../pars/run.txt"
