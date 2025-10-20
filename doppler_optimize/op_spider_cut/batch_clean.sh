#!/usr/bin/bash

max_jobs=120

l_cut=(4000 6000 8000 10000 15000 20000 25000 30000)

for cut in "${l_cut[@]}"; do
  name=$(printf "%05dkeV" ${cut}) 
  rm -rf try/${name}
done
