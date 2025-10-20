#!/usr/bin/bash

max_jobs=160

l_cut=(4000 6000 8000 10000 15000 20000 25000 30000)

for cut in "${l_cut[@]}"; do
  ./run.sh ${cut} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
