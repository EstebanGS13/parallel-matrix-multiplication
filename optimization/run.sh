#!bin/bash

gcc sequential.c -o seq
gcc -Wall -O0 sequential.c -o o0
gcc -Wall -O1 sequential.c -o o1
gcc -Wall -O2 sequential.c -o o2
gcc -Wall -O3 sequential.c -o o3
gcc -Wall -O3 -funroll-loops sequential.c -o o3fun

# Normal mult
for i in {1..10}; do
    for j in 500 1000 1500 2000 3000; do
        ./seq $j 0
    done
done

# Row wise mult
for i in {1..10}; do
    for j in 500 1000 1500 2000 3000; do
        ./seq $j 1
    done
done
mv row_wise_mult.csv row_mult.csv

# Optimizations O0 to O3
for opt_num in {0..3}; do
    for i in {1..10}; do
        for j in 500 1000 1500 2000 3000; do
            ./o$opt_num $j 1
        done
    done
    mv row_wise_mult.csv o$opt_num.csv
done

# Optimization 03 with funroll
for i in {1..10}; do
    for j in 500 1000 1500 2000 3000; do
        ./o3fun $j 1
    done
done
mv row_wise_mult.csv o3fun.csv
