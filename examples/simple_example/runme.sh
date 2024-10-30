#!/usr/bin/env bash
LD_PRELOAD=$(dirname "$0")/../../build/src/libsym2obj.so bash -c "gcc a.c -c -o a.o"
