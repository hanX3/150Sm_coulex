#!/usr/bin/bash

if [ $# -ne 1 ]; then
  echo "need win"
  exit 1
fi

rm -f ./try/$(printf "%03d" "${1}")ns/rootfile/data0616_0674_doppler_$(printf "%d" "${1}")ns_jump_300ns_hist.root 
hadd ./try/$(printf "%03d" "${1}")ns/rootfile/data0616_0674_doppler_$(printf "%d" "${1}")ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")ns/rootfile/data061[6-9]_doppler_$(printf "%d" "${1}")ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")ns/rootfile/data06[2-6]?_doppler_$(printf "%d" "${1}")ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")ns/rootfile/data067[1-4]_doppler_$(printf "%d" "${1}")ns_jump_300ns_hist.root
