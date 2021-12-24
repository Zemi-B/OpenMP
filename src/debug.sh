#!/bin/bash

rm out.txt
for program_name in "1" "2" "3"; do
  for parallel in "X"; do
    for num_threads in 1 2 4 8; do
      gcc main$program_name.c -Wall -Wextra -fopenmp -O3 -DPARALLEL_$parallel -DNUM_THREADS=$num_threads -DDEBUG -o ../build/main$program_name.out
      echo --- main$program_name \($parallel $num_threads thread\) ---
      ../build/main$program_name.out ../data/fhoto.pgm ../data/fhoto${program_name}_${parallel}.pgm >>out.txt
    done
  done
done
