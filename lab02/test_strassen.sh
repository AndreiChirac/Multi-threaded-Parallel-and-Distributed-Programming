#!/bin/bash

N=1000
P=2

if [ ! -f "strassen" ]
then
    echo "Nu exista binarul strassen"
    exit
fi

if [ ! -f "strassen_par" ]
then
    echo "Nu exista binarul strassen_par"
    exit
fi

./strassen $N > seq.txt
./strassen_par $N > par.txt
./strassen_par_v2 $N > par2.txt

diff seq.txt par.txt
diff seq.txt par2.txt

rm -rf seq.txt par.txt par2.txt