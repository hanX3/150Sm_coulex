#!/usr/bin/bash

l_cut=(4000 6000 8000 10000 15000 20000 25000 30000)

for cut in "${l_cut[@]}"; do
  ./hadd_hist.sh $cut
done
