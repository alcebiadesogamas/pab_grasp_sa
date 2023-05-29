#!/bin/bash
DELAY_SECONDS=1
g++ -o main 
for ((i=1; i<=5; i++))
do
    echo "Running program $i"
    ./main
    sleep $DELAY_SECONDS
done