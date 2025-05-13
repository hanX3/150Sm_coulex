#!/usr/bin/bash

max_jobs=120

mkdir -p out

while read run; do
  # root -b -q "macro_raw.cpp($run)" &
  # root -b -q "macro_cali_ge.cpp($run)" &
  # root -b -q "macro_cali_si.cpp($run)" &
  # root -b -q "macro_cali.cpp($run)" &
  root -b -q "draw_sample.cpp($run)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../pars/run.txt"

