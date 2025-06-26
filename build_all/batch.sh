#!/usr/bin/bash

max_jobs=120

mkdir -p out

while read run; do
  ./build $run 50 300 &
  ./build $run 100 300 &
  ./build $run 150 300 &
  ./build $run 200 300 &
  ./build $run 250 300 &

  # root -b -q "macro.cpp($run, 50, 300)" &
  # root -b -q "macro.cpp($run, 100, 300)" &
  # root -b -q "macro.cpp($run, 150, 300)" &
  # root -b -q "macro.cpp($run, 200, 300)" &
  # root -b -q "macro.cpp($run, 250, 300)" &
  
  # root -b -q "draw_sample.cpp($run, 50, 50)" &
  # root -b -q "draw_sample.cpp($run, 100, 100)" &
  # root -b -q "draw_sample.cpp($run, 150, 150)" &
  # root -b -q "draw_sample.cpp($run, 200, 200)" &
  # root -b -q "draw_sample.cpp($run, 250, 250)" &

  # root -b -q "draw_sample_no_bg.cpp($run, 50, 50)" &
  # root -b -q "draw_sample_no_bg.cpp($run, 100, 100)" &
  # root -b -q "draw_sample_no_bg.cpp($run, 150, 150)" &
  # root -b -q "draw_sample_no_bg.cpp($run, 200, 200)" &
  # root -b -q "draw_sample_no_bg.cpp($run, 250, 250)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run.txt"

