#!/usr/bin/bash

max_jobs=110

rm correction*.txt

mkdir -p out

while read run; do
  root -b -q "macro.cpp($run)" > out/$run.txt &

  while (( $(jobs -p | wc -l) >= $max_jobs )); do
    wait -n
  done
done < "../pars/run.txt"

