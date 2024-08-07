#!/bin/sh

set -xe

clang main.c -DSINGLY_LINKED_LIST -DSTACK -DQUEUE -DMAP -DTWO_SUM && ./a.out

// target: clang main.c -DSINGLY_LINKED_LIST -DCIRCULAR_DOUBLY_LINKED_LIST -DMAP -DTWO_SUM -DMD5 -DLOG -DUTILS -DLEX -DSTACK -DQUEUE && ./a.out

valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./a.out
