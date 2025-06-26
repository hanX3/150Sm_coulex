#!/usr/bin/bash

max_jobs=120

mkdir -p out

while read run; do
  # ./b_tf1_low $run > out/$run &
  ./build $run > out/$run &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run.txt"

