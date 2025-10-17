#!/usr/bin/bash

max_jobs=120

l_cut=(5000 10000 15000 20000 25000 30000 35000 40000 45000 50000)

for cut in "${l_cut[@]}"; do
  ./hadd_hist.sh $cut
done
