#!/usr/bin/bash

l_beta_par=(0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.2 1.4)

for beta_par in "${l_beta_par[@]}"; do
  ./hadd_hist.sh $(echo "${beta_par}*10" | bc | cut -d'.' -f1)
done
