#!/bin/bash

g++ k6.c || { echo "Compilation failed"; exit 1; }

for ((i = $1; i <= $2; i++)); do
    echo "--- Running iteration $i ---"
    
    ./a.out "$i"

    # Run cadical in background
    ../cadical k6ram.cnf --allsat > "./k6part/p$i.sol" &
    pid=$!

    # Wait for it, but allow Ctrl+C to skip it
    trap "echo 'Interrupted cadical at iteration $i'; kill $pid 2>/dev/null; wait $pid 2>/dev/null; continue" SIGINT
    wait $pid
    trap - SIGINT  # Reset trap after cadical finishes
done
