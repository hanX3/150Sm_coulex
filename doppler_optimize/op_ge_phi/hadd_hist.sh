#!/usr/bin/bash

if [ $# -ne 1 ]; then
  echo "need phi_offset"
  exit 1
fi

#
#rm -f ./try/$(printf "%03d" "${1}")deg/rootfile/data0472_0549_doppler_200ns_jump_300ns_hist.root 
#hadd ./try/$(printf "%03d" "${1}")deg/rootfile/data0472_0549_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data04??_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data05[0-4]?_doppler_200ns_jump_300ns_hist.root      
#
#rm -f ./try/$(printf "%03d" "${1}")deg/rootfile/data0550_0605_doppler_200ns_jump_300ns_hist.root
#hadd ./try/$(printf "%03d" "${1}")deg/rootfile/data0550_0605_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data05[5-9]?_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data060[0-5]_doppler_200ns_jump_300ns_hist.root
#
#rm -f ./try/$(printf "%03d" "${1}")deg/rootfile/data0472_0605_doppler_200ns_jump_300ns_hist.root
#hadd ./try/$(printf "%03d" "${1}")deg/rootfile/data0472_0605_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data0472_0549_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data0550_0605_doppler_200ns_jump_300ns_hist.root

rm -f ./try/$(printf "%03d" "${1}")deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root 
hadd ./try/$(printf "%03d" "${1}")deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data061[6-9]_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data06[2-6]?_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data067[1-4]_doppler_200ns_jump_300ns_hist.root
#
#rm -f ./try/$(printf "%03d" "${1}")deg/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root
#hadd ./try/$(printf "%03d" "${1}")deg/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data06[8-9]?_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data070?_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data071?_doppler_200ns_jump_300ns_hist.root
#
#rm -f ./try/$(printf "%03d" "${1}")deg/rootfile/data0472_0718_doppler_200ns_jump_300ns_hist.root
#hadd ./try/$(printf "%03d" "${1}")deg/rootfile/data0472_0718_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data0472_0605_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data0616_0674_doppler_200ns_jump_300ns_hist.root ./try/$(printf "%03d" "${1}")deg/rootfile/data0680_0718_doppler_200ns_jump_300ns_hist.root
#
##
#rm -f ./try/$(printf "%03d" "${1}")deg/rootfile/data0472_0549_doppler_200ns_jump_300ns_hist.root
#rm -f ./try/$(printf "%03d" "${1}")deg/rootfile/data0550_0605_doppler_200ns_jump_300ns_hist.root

