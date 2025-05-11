#!/usr/bin/bash

max_jobs=90

while read run; do
  ./analysis $run > out/$run &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run.txt"

