#!/usr/bin/env bash

EXAMPLE_DIR=$(dirname "$0" 2> /dev/null)
gcc -v
gcc $EXAMPLE_DIR/a.c -c -o $EXAMPLE_DIR/a.o
