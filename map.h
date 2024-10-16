// #pragme once - in this scenario we want to allow multiple loading - it might
// be that first time we loaded with no defines, but next time we might have
// needed defines

// {
#if defined(MAP) // HEADER
#pragma once

#include <stdbool.h>

#define ARENA
#include "arena.h"
#undef ARENA

typedef struct MapKeyValue {
  void *key;
  void *value;
  struct MapKeyValue *next;
} MapKeyValue;

typedef struct {
  int (*hash)(void *key, size_t cap);
  Arena *arena;
  MapKeyValue **kvs;
  size_t len;
  size_t cap;
} Map;

// map_hash is default hashing function.
int map_hash(void *key, size_t cap);

// map_apply executes func fn on each list element with given argument.
void map_apply(Map *map, void (*fn)(MapKeyValue *, void *), void *arg);

// map_free frees the entire map.
// frees memory.
void map_free(Map *map);

// map_create prepares a new map with all the helper functions.
// if NULL is provided for hash, then map_hash is used.
// if NULL is provided for pbucket_size then a default of 521 is used:
// * bucket_size is the buckets size for Map when
// * creating a new map.
// * Guarantee of no common factor for array size, hash
// * multiplier likely data values;
// * ref: The Practice of Programming, B. Kernighan, R. Pike, p 57
// allocs memory.
Map *map_create(int (*hash)(void *key, size_t cap), size_t *pbucket_size);

// map_list prints the entire map.
void map_list(Map *map, void (*print)(MapKeyValue *, void *), void *fmt);

// map_insert adds/updates `value` to/in `map` with `key`.
// differently:
// if `key` doesn't exist, insert the `value`
// if `key` exists, update the `value`
// returns NULL if `map` or `key` is NULL.
// NULL `value` is allowed,
// in which case internal empty struct is used
// (i.e. all values will point to same object).
// NULL value is useful, when only key is important.
// allocs memory.
Map *map_insert(Map *map, void *key, void *value);

// map_find retrieves value corresponding to the key.
// returns NULL if map is NULL or `key` not found.
void *map_find(Map *map, void *key);

// map_delete removes key-value from `map`.
// frees memory.
Map *map_delete(Map *map, void *key);

#endif // defined(MAP) // HEADER
// }

// {
#if defined(MAP) // IMPLEMENTATION
#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
} _empty;

typedef struct {
  size_t offset;
  MapKeyValue **kvs;
} _reorgCollector;

enum _reorgAction { _REORG_INC, _REORG_DEC };

bool _cmp(void *k1, void *k2) {
  if (k1 == NULL || k2 == NULL) {
    return false;
  }

  return *((int *)k1) == *((int *)k2);
}

void _reorg_collection(MapKeyValue *kv, void *collector) {
  ((_reorgCollector *)collector)->kvs[((_reorgCollector *)collector)->offset] =
      kv;
  ((_reorgCollector *)collector)->offset += 1;
  return;
}

_reorgCollector *_reorg_extract_kvs(Map *map) {
  _reorgCollector *collector =
      (_reorgCollector *)arena_alloc(map->arena, 1 * sizeof(_reorgCollector));
  collector->kvs =
      (MapKeyValue **)arena_alloc(map->arena, map->len * sizeof(MapKeyValue *));
  map_apply(map, _reorg_collection, (void *)collector);
  return collector;
}

void _recompute_idx(Map *map, _reorgCollector *collected) {
  for (int i = 0; i < collected->offset; i += 1) {
    collected->kvs[i]->next = NULL; // NOTE: avoid pointing to old address
    map_insert(map, collected->kvs[i]->key, collected->kvs[i]->value);
  }
}

void _reorg(Map *map, size_t factor, enum _reorgAction action) {
  if (map == NULL || factor <= 1) {
    return;
  }

  size_t new_cap;
  if (action == _REORG_INC) {
    new_cap = factor * map->cap;
  } else if (action == _REORG_DEC) {
    new_cap = map->cap / factor;
  }

  _reorgCollector *collector = _reorg_extract_kvs(map);

  MapKeyValue **old_kvs = map->kvs;
  map->kvs = calloc(new_cap, sizeof(MapKeyValue *));

  // NOTE: set map->cap after collection and before recomputing indices
  // map_apply will segfault otherwise in collection
  // and recomputing uses hash, which uses map->cap.
  map->cap = new_cap;

  // NOTE: set len to zero, because insert will increase map->len
  map->len = 0;

  _recompute_idx(map, collector);

  if (old_kvs != NULL) {
    free(old_kvs);
  }
}

// NOTE: I'm not 100% sure of the correctness of this, but thus far it has been
// working the best.
// Need further testing to make sure.
int map_hash(void *key, size_t cap) {
  if (key == NULL) {
    return 0;
  }
  int h = 0;

  // hash_multiplier is multiplier used when calculating
  // hash for string map key.
  // Value 31 is empirically shown to be good;
  // ref: The Practice of Programming, B. Kernighan, R. Pike, p 56
  // int hash_multiplier = 31;
  // NOTE: not used due to current hash calculation
  // approach, but this could change in the future.

  h = *(int *)key;

  if (h < 0) {
    h *= -1;
  }
  return cap > 0 ? h % cap : h;
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
    return;
  }
  if (map->kvs) {
    free(map->kvs);
  }
  arena_destroy(map->arena);
  if (map) {
    free(map);
  }
}

Map *map_create(int (*hash)(void *key, size_t cap), size_t *pbucket_size) {
  if (hash == NULL) {
    hash = map_hash;
  }

  // bucket_size is the buckets size for Map when
  // creating a new map.
  // Guarantee of no common factor for array size, hash
  // multiplier likely data values;
  // ref: The Practice of Programming, B. Kernighan, R. Pike, p 57
  size_t bucket_size = 521;
  if (pbucket_size != NULL) {
    bucket_size = *pbucket_size;
  }

  Map *new = calloc(1, sizeof(Map));
  new->hash = hash;
  new->kvs = calloc(bucket_size, sizeof(MapKeyValue *));
  new->arena = arena_create(
      2 * bucket_size); // NOTE: half is for size, half for actual data
  new->len = 0;
  new->cap = bucket_size;
  return new;
}

void map_list(Map *map, void (*print)(MapKeyValue *, void *), void *fmt) {
  map_apply(map, print, fmt);
  return;
}

void *map_find(Map *map, void *key) {
  if (map == NULL || key == NULL) {
    return NULL;
  }

  int idx = map->hash(key, map->cap);

  MapKeyValue *cur = map->kvs[idx];

  if (cur == NULL) { // not found
    return NULL;
  }

  for (; cur != NULL; cur = cur->next) {
    if (_cmp(cur->key, key)) {
      return cur->value;
    }
  }
  return NULL;
}

Map *map_insert(Map *map, void *key, void *value) {
  if (map == NULL || key == NULL) {
    return NULL;
  }
  int idx = map->hash(key, map->cap);

  if (map->kvs[idx] == NULL) {

    MapKeyValue *kv =
        (MapKeyValue *)arena_alloc(map->arena, sizeof(MapKeyValue));
    kv->key = key;
    kv->value = value != NULL ? value : &((_empty){});
    kv->next = NULL;

    map->kvs[idx] = kv;
    map->len += 1;
    if (map->len >= map->cap * 80 / 100) {
      _reorg(map, 2, _REORG_INC);
    }
    return map;
  }

  MapKeyValue *cur = map->kvs[idx];
  MapKeyValue *prev = map->kvs[idx];
  for (; cur != NULL; cur = cur->next) {
    if (_cmp(cur->key, key)) {
      cur->value = value;
      return map;
    }
    prev = cur;
  }

  if (cur == NULL && prev != NULL) {
    MapKeyValue *kv =
        (MapKeyValue *)arena_alloc(map->arena, sizeof(MapKeyValue));
    kv->key = key;
    kv->value = value != NULL ? value : &((_empty){});
    kv->next = NULL;

    prev->next = kv;
    map->len += 1;
    if (map->len >= map->cap * 80 / 100) {
      _reorg(map, 2, _REORG_INC);
    }
    return map;
  }

  return map;
}

Map *map_delete(Map *map, void *key) {
  if (map == NULL || key == NULL) {
    return NULL;
  }
  int idx = map->hash(key, map->cap);

  if (map->kvs[idx] == NULL) {
    return map;
  }

  if (_cmp(map->kvs[idx]->key, key)) {
    MapKeyValue *new_head = map->kvs[idx]->next;
    arena_free(map->arena, map->kvs[idx]);
    map->kvs[idx] = new_head;
    map->len -= 1;
    // MAYBE: TODO: _reorg, but need to think about the condition properly
    // if (map->len <= map->cap * 20 / 100) {
    //  _reorg(map, 2, _REORG_DEC); // TODO: NOTE: this is a good place to test
    //                              // _reorg, down or up;
    // }
    return map;
  }

  MapKeyValue *prev = map->kvs[idx];
  MapKeyValue *cur = prev->next;

  for (; cur != NULL; cur = cur->next) {
    if (_cmp(cur->key, key)) {
      prev->next = cur->next;
      arena_free(map->arena, cur);
      map->len -= 1;
      // MAYBE: TODO: _reorg, but need to think about the condition properly
      // if (map->len <= map->cap * 20 / 100) {
      //  _reorg(map, 2, _REORG_DEC);
      // }
      return map;
    }
    prev = cur;
  }

  return map;
}

#endif // defined(MAP) // IMPLEMENTATION
// }
