#!/usr/bin/bash

l_win=(50 100 150 200 250)
l_conf=("a" "b" "c" "d" "e")

for win in "${l_win[@]}"; do
  for conf in "${l_conf[@]}"; do
    rm -f ../rootfile/data0472_0549_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root 
    hadd ../rootfile/data0472_0549_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data04??_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data05[0-4]?_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root      

    rm -f ../rootfile/data0550_0605_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root
    hadd ../rootfile/data0550_0605_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data05[5-9]?_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data060[0-5]_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root

    rm -f ../rootfile/data0472_0605_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root
    hadd ../rootfile/data0472_0605_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data0472_0549_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data0550_0605_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root

    rm -f ../rootfile/data0616_0674_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root 
    hadd ../rootfile/data0616_0674_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data061[6-9]_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data06[2-6]?_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data067[1-4]_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root

    rm -f ../rootfile/data0680_0718_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root
    hadd ../rootfile/data0680_0718_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data06[8-9]?_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data070?_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data071?_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root

    rm -f ../rootfile/data0472_0718_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root
    hadd ../rootfile/data0472_0718_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data0472_0605_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data0616_0674_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root ../rootfile/data0680_0718_tw_doppler_${win}ns_jump_300ns_${conf}_hist.root
  done
done

