#!bin/bash

for i in {1..10}
do
    for j in 100 200 400 600 800 1000 1500 2000
    do
        ./seq $j
    done
done

for i in {1..10}
do
    for k in 2 4 10
    do
        for j in 100 200 400 600 800 1000 1500 2000
        do
            ./par $j $k
        done
    done
done