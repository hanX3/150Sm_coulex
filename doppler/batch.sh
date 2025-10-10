#!/usr/bin/bash

max_jobs=60

mkdir -p terminal_cout

l_conf=("b")

while read run; do
  for conf in "${l_conf[@]}"; do
    ./doppler $run 200 300 $conf &
  done
  
  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../pars/run.txt"
