#!bin/bash

sudo apt-get install gcc

gcc sequential.c -o seq
gcc parallel.c -lpthread -o par