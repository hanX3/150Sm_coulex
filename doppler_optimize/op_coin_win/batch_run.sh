#!/usr/bin/bash

max_jobs=160

l_win=(25 50 100 150 200 250 300)

for win in "${l_win[@]}"; do
  ./run.sh ${win} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
