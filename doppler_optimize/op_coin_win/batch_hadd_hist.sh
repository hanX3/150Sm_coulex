#!/usr/bin/bash

l_win=(25 50 100 150 200 250 300)

for win in "${l_win[@]}"; do
  ./hadd_hist.sh $win
done
