#!/usr/bin/bash

max_jobs=60

mkdir -p terminal_cout

while read run; do
  ./build $run 200 300 &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../pars/run.txt"

