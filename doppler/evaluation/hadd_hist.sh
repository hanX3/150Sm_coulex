#!/usr/bin/bash

if [ $# -ne 1 ]; then
    echo "need coincedence window"
    exit 1
fi

#
rm -f ../../rootfile/data0472_0549_doppler_${1}ns_jump_300ns_hist.root 
hadd ../../rootfile/data0472_0549_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data04??_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data05[0-4]?_doppler_${1}ns_jump_300ns_hist.root      

rm -f ../../rootfile/data0550_0605_doppler_${1}ns_jump_300ns_hist.root
hadd ../../rootfile/data0550_0605_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data05[5-9]?_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data060[0-5]_doppler_${1}ns_jump_300ns_hist.root

rm -f ../../rootfile/data0472_0605_doppler_${1}ns_jump_300ns_hist.root
hadd ../../rootfile/data0472_0605_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data0472_0549_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data0550_0605_doppler_${1}ns_jump_300ns_hist.root

rm -f ../../rootfile/data0616_0674_doppler_${1}ns_jump_300ns_hist.root 
hadd ../../rootfile/data0616_0674_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data061[6-9]_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data06[2-6]?_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data067[1-4]_doppler_${1}ns_jump_300ns_hist.root

rm -f ../../rootfile/data0680_0718_doppler_${1}ns_jump_300ns_hist.root
hadd ../../rootfile/data0680_0718_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data06[8-9]?_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data070?_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data071?_doppler_${1}ns_jump_300ns_hist.root

rm -f ../../rootfile/data0472_0718_doppler_${1}ns_jump_300ns_hist.root
hadd ../../rootfile/data0472_0718_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data0472_0605_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data0616_0674_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data0680_0718_doppler_${1}ns_jump_300ns_hist.root

# low
rm -f ../../rootfile/doppler_${1}ns_p1_hist.root
mv ../../rootfile/data0616_0674_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/doppler_${1}ns_p1_hist.root

# high
rm -f ../../rootfile/doppler_${1}ns_p2_hist.root
hadd ../../rootfile/doppler_${1}ns_p2_hist.root ../../rootfile/data0472_0605_doppler_${1}ns_jump_300ns_hist.root ../../rootfile/data0680_0718_doppler_${1}ns_jump_300ns_hist.root
