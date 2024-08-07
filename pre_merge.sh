#!/bin/sh

set -xe

clang main.c -DSINGLY_LINKED_LIST -DSTACK -DQUEUE -DLOG -DUTILS && ./a.out
# -DMAP -DTWO_SUM # NOTE: memory leak in MAP
# -DLEX # NOTE: double free
# -DMD5 # NOTE: still 1 reacable memory, but I don't allocate anything - investigate
# -DCIRCULAR_DOUBLY_LINKED_LIST # NOTE: needs refactor, before doesn't make sense

# target: clang main.c -DSINGLY_LINKED_LIST -DCIRCULAR_DOUBLY_LINKED_LIST -DSTACK -DQUEUE -DMAP -DTWO_SUM -DMD5 -DLOG -DUTILS -DLEX && ./a.out

valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./a.out
