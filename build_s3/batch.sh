#!/usr/bin/bash

max_jobs=120

mkdir -p out

while read run; do
  ./build_no_s3att_no_s3cor $run > out/$run.txt &
  ./build_no_s3cor $run > out/$run.txt &
  # ./build $run > out/$run.txt &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run.txt"

