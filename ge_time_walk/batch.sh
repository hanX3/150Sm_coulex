#!/usr/bin/bash

max_jobs=120

mkdir -p out

while read run; do
  ./build $run > out/$run &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run_472_605.txt"

