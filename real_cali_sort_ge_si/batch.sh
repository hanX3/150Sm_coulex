#!/usr/bin/bash

max_jobs=120

mkdir -p terminal_out

while read run; do
  ./sort $run > terminal_out/$run &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../pars/run.txt"

