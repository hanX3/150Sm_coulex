#!/usr/bin/bash

max_jobs=160

l_cut=(5000 10000 15000 20000 25000 30000 35000 40000 45000 50000)

for cut in "${l_cut[@]}"; do
  ./run.sh ${cut} &

  while [ "$(jobs -p | wc -l)" -ge "$max_jobs" ]; do
    wait -n
  done
done
