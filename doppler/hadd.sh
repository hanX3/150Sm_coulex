#!/usr/bin/bash

l_win=(50 100 150 200 250)
l_conf=("a" "b" "c" "d" "e")

for win in "${l_win[@]}"; do
  for conf in "${l_conf[@]}"; do
    rm -f ../rootfile/data0472_0549_doppler_${win}ns_jump_300ns_${conf}.root 
    hadd ../rootfile/data0472_0549_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data04??_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data05[0-4]?_doppler_${win}ns_jump_300ns_${conf}.root      

    rm -f ../rootfile/data0550_0605_doppler_${win}ns_jump_300ns_${conf}.root
    hadd ../rootfile/data0550_0605_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data05[5-9]?_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data060[0-5]_doppler_${win}ns_jump_300ns_${conf}.root

    rm -f ../rootfile/data0472_0605_doppler_${win}ns_jump_300ns_${conf}.root
    hadd ../rootfile/data0472_0605_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data0472_0549_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data0550_0605_doppler_${win}ns_jump_300ns_${conf}.root

    rm -f ../rootfile/data0616_0674_doppler_${win}ns_jump_300ns_${conf}.root 
    hadd ../rootfile/data0616_0674_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data061[6-9]_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data06[2-6]?_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data067[1-4]_doppler_${win}ns_jump_300ns_${conf}.root

    rm -f ../rootfile/data0680_0718_doppler_${win}ns_jump_300ns_${conf}.root
    hadd ../rootfile/data0680_0718_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data06[8-9]?_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data070?_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data071?_doppler_${win}ns_jump_300ns_${conf}.root

    rm -f ../rootfile/data0472_0718_doppler_${win}ns_jump_300ns_${conf}.root
    hadd ../rootfile/data0472_0718_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data0472_0605_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data0616_0674_doppler_${win}ns_jump_300ns_${conf}.root ../rootfile/data0680_0718_doppler_${win}ns_jump_300ns_${conf}.root
  done
done

