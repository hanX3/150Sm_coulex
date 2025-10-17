#!/usr/bin/bash

if [ $# -ne 1 ]; then
  echo "need cut"
  exit 1
fi

#
rm -f ./try/$(printf "%05d" "${1}")keV/rootfile/data0472_0549_doppler_200ns_jump_300ns_hist.root 
hadd ./try/$(printf "%05d" "${1}")keV/rootfile/data0472_0549_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data04??_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data05[0-4]?_doppler_200ns_jump_300ns_hist.root      

rm -f ./try/$(printf "%05d" "${1}")keV/rootfile/data0550_0605_doppler_200ns_jump_300ns_hist.root
hadd ./try/$(printf "%05d" "${1}")keV/rootfile/data0550_0605_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data05[5-9]?_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data060[0-5]_doppler_200ns_jump_300ns_hist.root

rm -f ./try/$(printf "%05d" "${1}")keV/rootfile/data0472_0605_doppler_200ns_jump_300ns_hist.root
hadd ./try/$(printf "%05d" "${1}")keV/rootfile/data0472_0605_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data0472_0549_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data0550_0605_doppler_200ns_jump_300ns_hist.root

rm -f ./try/$(printf "%05d" "${1}")keV/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root 
hadd ./try/$(printf "%05d" "${1}")keV/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data061[6-9]_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data06[2-6]?_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data067[1-4]_doppler_200ns_jump_300ns_hist.root

rm -f ./try/$(printf "%05d" "${1}")keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root
hadd ./try/$(printf "%05d" "${1}")keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data06[8-9]?_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data070?_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data071?_doppler_200ns_jump_300ns_hist.root

rm -f ./try/$(printf "%05d" "${1}")keV/rootfile/data0472_0718_doppler_200ns_jump_300ns_hist.root
hadd ./try/$(printf "%05d" "${1}")keV/rootfile/data0472_0718_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data0472_0605_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%05d" "${1}")keV/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root

rm -f ./try/$(printf "%05d" "${1}")keV/rootfile/data0472_0549_doppler_200ns_jump_300ns_hist.root
rm -f ./try/$(printf "%05d" "${1}")keV/rootfile/data0550_0605_doppler_200ns_jump_300ns_hist.root

