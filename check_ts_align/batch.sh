#!/usr/bin/bash

max_jobs=150

mkdir -p out

while read run; do
  ./btf1 $run > out/$run &
  ./btf2 $run > out/$run &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run.txt"

