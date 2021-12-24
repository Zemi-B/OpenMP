#!/bin/bash

for program_name in "1" "2" "3"; do
  ../build/main$program_name.out ../data/fhoto.pgm ../data/fhoto${program_name}.pgm
done
