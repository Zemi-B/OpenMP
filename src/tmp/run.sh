#!/bin/bash

mkdir -p ../build
  for num_threads in 1 2 4 8 16 24; do
    gcc main.c -Wall -Wextra -fopenmp -O3 -DNUM_THREADS=$num_threads -o ../build/main.out
    echo --- main \($num_threads thread\) ---
    ../build/main.out ../../data/fhoto.pgm ../../data/fhoto_tmp.pgm
  done
done
