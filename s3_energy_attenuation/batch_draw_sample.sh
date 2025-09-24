#!/usr/bin/bash

max_jobs=120

for i in $(seq -w 1 24);
do
  mkdir -p fig/ring${i}
done

for i in $(seq -w 1 32);
do
  mkdir -p fig/sector${i}
done

#
while read run; do
  root -b -q "draw_sample.cpp($run)" &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done < "../pars/run.txt"
