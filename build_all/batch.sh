#!/usr/bin/bash

max_jobs=120

mkdir -p out

while read run; do
  ./b100 $run > out/$run &
  ./b200 $run > out/$run &
  ./b1000 $run > out/$run &
  # root -b -q "macro.cpp($run, 100)" > ./out/$run.100 &
  # root -b -q "macro.cpp($run, 200)" > ./out/$run.200 &
  # root -b -q "macro.cpp($run, 1000)" > ./out/$run.1000 &
  # root -b -q "draw_sample.cpp($run, 100)" &
  # root -b -q "draw_sample.cpp($run, 200)" &
  # root -b -q "draw_sample.cpp($run, 1000)" &
  # root -b -q "draw_sample_no_bg.cpp($run, 100)" &
  # root -b -q "draw_sample_no_bg.cpp($run, 200)" &
  # root -b -q "draw_sample_no_bg.cpp($run, 1000)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run.txt"

