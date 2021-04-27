#!bin/bash

gcc sequential.c -o seq
gcc parallel.c -o par -fopenmp

for i in {1..10}; do
    for j in 500 1000 1500 2000 3000; do
        ./seq $j 1
    done
done

for i in {1..10}; do
    for k in 2 4 10; do
        for j in 500 1000 1500 2000 3000; do
            ./par $j $k
        done
    done
done
