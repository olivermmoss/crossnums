#!/bin/bash

g++ addvtx.c || { echo "Compilation failed"; exit 1; }

for ((i = 0; i <= 24; i++)); do
    echo "--- Running iteration $i ---"
    
    ./a.out 

done
