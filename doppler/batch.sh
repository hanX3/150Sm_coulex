#!/usr/bin/bash

mkdir -p out
max_jobs=120
l_conf=("a" "b" "c" "d" "e")

while read run; do
  for conf in "${l_conf[@]}"; do
    ./doppler $run 50 300 $conf &
    ./doppler $run 100 300 $conf &
    ./doppler $run 150 300 $conf &
    ./doppler $run 200 300 $conf &
    ./doppler $run 250 300 $conf &

    # root -b -q "macro.cpp($run, 50, 300, \"$conf\")" &
    # root -b -q "macro.cpp($run, 100, 300, \"$conf\")" &
    # root -b -q "macro.cpp($run, 150, 300, \"$conf\")" &
    # root -b -q "macro.cpp($run, 200, 300, \"$conf\")" &
    # root -b -q "macro.cpp($run, 250, 300, \"$conf\")" &
  done
  
  #root -b -q "draw_sample_win_jump.cpp($run, \"a\")" &
  #root -b -q "draw_sample_win_jump.cpp($run, \"b\")" &
  #root -b -q "draw_sample_win_jump.cpp($run, \"c\")" &
  #root -b -q "draw_sample_win_jump.cpp($run, \"d\")" &
  #root -b -q "draw_sample_win_jump.cpp($run, \"e\")" &

  #root -b -q "draw_sample_config.cpp($run, 50, 300)" &
  #root -b -q "draw_sample_config.cpp($run, 100, 300)" &
  #root -b -q "draw_sample_config.cpp($run, 150, 300)" &
  #root -b -q "draw_sample_config.cpp($run, 200, 300)" &
  #root -b -q "draw_sample_config.cpp($run, 250, 300)" &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "./run.txt"

# root -b -q "draw_sample.cpp(1, 100, \"0472_0605\")" &
# root -b -q "draw_sample.cpp(2, 100, \"0616_0674\")" &
# root -b -q "draw_sample.cpp(3, 100, \"0680_0718\")" &
# root -b -q "draw_sample.cpp(4, 100, \"0472_0718\")" &
