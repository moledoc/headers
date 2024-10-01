// #pragme once - in this scenario we want to allow multiple loading - it might
// be that first time we loaded with no defines, but next time we might have
// needed defines

// {
#if defined(MAP) // HEADER
#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define ARENA
#include "arena.h"
#undef ARENA

typedef struct MapKeyValue {
  void *key;
  size_t key_len;
  void *value;
  struct MapKeyValue *next;
} MapKeyValue;

typedef struct {
  int (*hash)(void *key, size_t key_len, size_t cap);
  Arena *_arena;
  MapKeyValue **kvs;
  size_t len;
  size_t cap;
} Map;

enum {
  MAP_BUCKETS_SIZE_DEFAULT =
      521, // MAP_BUCKETS_SIZE_DEFAULT is the buckets size for Map when
           // creating a new map.
           // Guarantee of no common factor for array size, hash
           // multiplier likely data values;
           // ref: The Practice of Programming, B. Kernighan, R. Pike, p 57

  HASH_MULTIPLIER =
      31, // HASH_MULTIPLIER is multiplier used when calculating
          // hash for string map key.
          // Empirically shown to be good;
          // ref: The Practice of Programming, B. Kernighan, R. Pike, p 56
};

int MAP_BUCKETS_SIZE = MAP_BUCKETS_SIZE_DEFAULT;

// map_hash is default hashing function.
int map_hash(void *key, size_t key_len, size_t cap);

// map_apply executes func fn on each list element with given argument.
void map_apply(Map *map, void (*fn)(MapKeyValue *, void *), void *arg);

// map_free frees the entire map.
// frees memory.
void map_free(Map *map);

// map_create prepares a new map with all the helper functions.
// if NULL is provided for hash, then map_hash is used.
// default buckets size is MAP_BUCKETS_SIZE.
// allocs memory.
Map *map_create(int (*hash)(void *key, size_t key_len, size_t cap));

// map_list prints the entire map.
void map_list(Map *map);

// map_insert adds/updates `value` to/in `map` with `key`.
// differently:
// if `key` doesn't exist, insert the `value`
// if `key` exists, update the `value`
// returns NULL if `map` or `key` is NULL or key_len <= 0
// NULL `value` is allowed.
// allocs memory.
Map *map_insert(Map *map, void *key, size_t key_len, void *value);

// map_find retrieves value corresponding to the key.
// returns NULL if map is NULL or `key` not found.
void *map_find(Map *map, void *key, size_t key_len);

// map_delete removes key-value from `map`.
// frees memory.
Map *map_delete(Map *map, void *key, size_t key_len);

// TODO: reorg - handle up- and down-sizing

#endif // defined(MAP) // HEADER
// }

// {
#if defined(MAP) // IMPLEMENTATION
#pragma once

#include <stdbool.h>

bool _cmp(void *k1, size_t k1_len, void *k2, size_t k2_len) {
  if (k1 == NULL || k2 == NULL || k1_len <= 0 || k2_len == 0) {
    return 0;
  }
  if (k1_len != k2_len) {
    return 0;
  }
  for (int i = 0; i < k1_len; i += 1) {
    if (((uint64_t *)k1)[i] != ((uint64_t *)k2)[i]) {
      return 0;
    }
  }
  return 1;
}

int map_hash(void *key, size_t key_len, size_t cap) {
  char *kkey = (char *)key;
  int h = 0;
  for (int i = 0; i < key_len; i += 1) {
    h += HASH_MULTIPLIER * h + (*kkey)[i];
  }
  if (cap == 0) {
    cap = key_len;
  }
  return h % cap;
}

void map_apply(Map *map, void (*fn)(MapKeyValue *, void *), void *arg) {
  for (int i = 0; i < map->cap; ++i) {
    if (map->kvs[i] == NULL) {
      continue;
    }
    for (MapKeyValue *kv = map->kvs[i]; kv != NULL; kv = kv->next) {
      (*fn)(kv, arg);
    }
  }
}

void map_free(Map *map) {
  if (map == NULL) {
    return NULL;
  }
  arena_destroy(map->_arena);
  if (map->kvs) {
    free(map->kvs);
  }
  if (map) {
    free(map)
  }
}

Map *map_create(int (*hash)(void *key, size_t key_len, size_t cap)) {
  if (hash == NULL) {
    hash = map_hash;
  }

  Map *new = calloc(1, sizeof(Map));
  new->hash = hash;
  new->kvs = calloc(MAP_BUCKETS_SIZE, sizeof(MapKeyValue *));
  _arena = arena_create();
  new->len = 0;
  new->cap = MAP_BUCKETS_SIZE;
  return new;
}

void map_list(Map *map, void (*print)(Map *, void *), void *fmt) {
  map_apply(cursor, print, fmt);
  return;
}

Map *map_insert(Map *map, void *key, size_t key_len, void *value) {
  if (map == NULL || key == NULL || key_len <= 0) {
    return NULL;
  }
  int idx = map->hash(key, key_len, map->cap);

  if (map->kvs[idx] == NULL) {

    MapKeyValue *kv = arena_alloc(map->_arena, sizeof(MapKeyValue));
    kv->key = key;
    kv->key_len = key_len;
    kv->value = value;
    kv->next = NULL;

    map->kvs[idx] = kv;
    map->len += 1;
    return map;
  }

  MapKeyValue *cur = map->kvs[idx];
  MapKeyValue *prev = map->kvs[idx];
  for (; cur != NULL; cur = cur->next) {
    if (_cmp(cur->key, cur->key_len, key, key_len)) {
      cur->value = value;
      return map;
    }
    prev = cur;
  }

  if (cur == NULL && prev != NULL) {
    MapKeyValue *kv = arena_alloc(map->_arena, sizeof(MapKeyValue));
    kv->key = key;
    kv->key_len = key_len;
    kv->value = value;
    kv->next = NULL;

    prev->next = kv;
    map->len += 1;
    return map;
  }

  return map;
}

void *map_find(Map *map, void *key, size_t key_len) {
  if (map == NULL || key == NULL || key_len <= 0) {
    return NULL;
  }
  int idx = map->hash(key, key_len, map->cap);

  MapKeyValue *cur = map->kvs[idx];

  if (cur == NULL) { // not found
    return NULL;
  }

  for (; cur != NULL; cur = cur->next) {
    if (_cmp(cur->key, cur->key_len, key, key_len)) {
      return cur->value;
    }
  }
  return NULL;
}

Map *map_delete(Map *map, void *key, size_t key_len, void *value) {
  if (map == NULL || key == NULL || key_len <= 0) {
    return NULL;
  }
  int idx = map->hash(key, key_len, map->cap);

  if (map->kvs[idx] == NULL) {
    return map;
  }

  if (_cmp(map->kvs[idx]->key, map->kvs[idx]->key_len, key, key_len)) {
    map->kvs[idx] = cur->next;
    return map;
  }

  MapKeyValue *prev = map->kvs[idx];
  MapKeyValue *cur = prev->next;

  for (; cur != NULL; cur = cur->next) {
    if (_cmp(cur->key, cur->key_len, key, key_len)) {
      prev->next = cur->next;
      return map;
    }
    prev = cur;
  }

  return map;
}

#endif // defined(MAP) // IMPLEMENTATION
// }
