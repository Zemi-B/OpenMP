#!/bin/bash

rm out.txt
mkdir -p ../build
for program_number in "1" "2" "3"; do
  for num_threads in 1 2 4 8; do
    gcc main$program_number.c -Wall -Wextra -fopenmp -O3 -DNUM_THREADS=$num_threads -DDEBUG -o ../build/main$program_number.out
    ../build/main$program_number.out ../data/fhoto.pgm ../data/fhoto$program_number.pgm >>out.txt
  done
done

mkdir -p ../img
python3 fig.py
