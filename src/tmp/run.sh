#!/bin/bash

gcc main.c -Wall -Wextra -fopenmp -O3 -o main.out
./main.out
