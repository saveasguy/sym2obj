#!/usr/bin/env bash

EXAMPLE_DIR=$(dirname "$0")
SYM2OBJ_DIR=$EXAMPLE_DIR LD_PRELOAD=$EXAMPLE_DIR/../../build/src/libsym2obj.so bash -c "gcc $EXAMPLE_DIR/a.c -c -o $EXAMPLE_DIR/a.o"
