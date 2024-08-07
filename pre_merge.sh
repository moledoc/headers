#!/bin/sh

set -xe

clang main.c -DSINGLY_LINKED_LIST -DMAP -DMD5 -DLOG -DUTILS -DSTACK -DQUEUE && ./a.out
valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./a.out
