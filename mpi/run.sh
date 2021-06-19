#!bin/bash

gcc sequential.c -o seq
mpicc parallel.c -o par

scp -r /home/cluser/seq cluser@wn1:/home/cluser/
scp -r /home/cluser/par cluser@wn1:/home/cluser/

scp -r /home/cluser/seq cluser@wn2:/home/cluser/
scp -r /home/cluser/par cluser@wn2:/home/cluser/

scp -r /home/cluser/seq cluser@wn3:/home/cluser/
scp -r /home/cluser/par cluser@wn3:/home/cluser/


for i in {1..10}; do
    for j in 500 1000 1500 2000 3000; do
        ./seq $j 1
    done
done

for i in {1..10}; do
    for k in 2 4 10; do
        for j in 500 1000 1500 2000 3000; do
            mpirun -np $k -hosts head,wn1,wn2,wn3 ./par $j 1
        done
    done
done
