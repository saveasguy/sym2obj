#!/usr/bin/env bash

for c in $(ls *.c); do
    gcc -c $c -o $c.o
done
