#pragma once

// {
#ifdef TWO_SUM // HEADER

// Given an array of integers and an integer target,
// return indices of the two numbers such that they add up to target.
// exists exactly 1 solution
// no repeated number allowed
// array.len in[2, 10 ^ 4]
// array.vals in[-10 ^ 9, 10 ^ 9]
// target in[-10 ^ 9, 10 ^ 9]

int *two_sum(int *nums, size_t nums_len, int target); // allocs memory

#endif // TWO_SUM // HEADER
// }

#ifdef TWO_SUM // IMPLEMENTATION

#include <stdlib.h>

#define MAP
#include "list.h"

// allocs memory
// O(n)

// let x_0,x_1,x_2, x_(n-1) be list elements
// let i in [0,..,n-1]
// loop over list: check if target-x_i in hash table
// if yes, then return i and hash table's value
// if no, then add {x_i:i} to hash table

void algo_two_sum_print_value(void *value) {
  printf("'%d'", *(int *)value);
  return;
}

void algo_two_sum_free_value(void *value) { if(value) free(value); }

int *two_sum(int *nums, size_t nums_len, int target) {
  int *res = malloc(2 * sizeof(int));
  res[0] = res[1] = -1;

  MAP_BUCKETS_SIZE = 2048;
  Map *map = map_create(map_hash, MapKeyInt, algo_two_sum_print_value,
                        algo_two_sum_free_value);
  for (int i = 0; i < nums_len; ++i) {
    int tmp = target - nums[i];
    void *found = map_find(map, (void *)&tmp);
    if (!found) {
      int *ins = calloc(1, sizeof(int));
      (*ins) = i;
      map_insert(map, (void *)&nums[i], (void *)ins);
    } else {
      res[0] = *(int *)found;
      res[1] = i;
      break;
    }
  }
  map_free(map);
  return res;
}

#undef MAP

#endif // TWO_SUM // IMPLEMENTATION