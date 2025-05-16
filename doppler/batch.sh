#!/usr/bin/bash

max_jobs=60

mkdir -p out

while read run; do
  # ./de $run > out/$run &
  # root -b -q "macro.cpp($run)" &
  root -b -q "draw_sample.cpp($run)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run.txt"

