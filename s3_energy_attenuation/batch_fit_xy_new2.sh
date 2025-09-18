#!/usr/bin/bash

max_jobs=120

for i in $(seq 1 24);
do
  root -b -q "fit_xy_new2.cpp(${i}, 0)" &
done

for i in $(seq 1 32);
do
  root -b -q "fit_xy_new2.cpp(0, ${i})" &
done

