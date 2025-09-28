#!/usr/bin/bash

max_jobs=60

while read run; do
  root -b -q "tree2hist.cpp($run)" &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done < "../pars/run.txt"
