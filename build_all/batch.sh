#!/usr/bin/bash

max_jobs=120

mkdir -p out

while read run; do
  # ./build $run > out/$run &
  root -b -q "macro.cpp($run, 100)" &
  root -b -q "macro.cpp($run, 200)" &
  root -b -q "macro.cpp($run, 1000)" &
  # root -b -q "draw_sample.cpp($run)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run.txt"

