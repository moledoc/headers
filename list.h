// #pragme once - in this scenario we want to allow multiple loading - it might
// be that first time we loaded with no defines, but next time we might have
// needed defines

// {
#if defined(SINGLY_LINKED_LIST) // HEADER
#pragma once

#include <stdbool.h>

typedef struct SLLNode {
  bool (*cmp)(void *, void *);
  void (*print)(struct SLLNode *node);
  void (*free_data)(void *); // frees memory
  struct SLLNode *next;
  void *data;
} SLLNode;

// sll_list prints the full linked list
void sll_list(SLLNode *cursor);

// sll_list_len finds the linked_list length
size_t sll_list_len(SLLNode *cursor);

// sll_free_node frees the current node and returns the next linked list element
// frees memory
SLLNode *sll_node_free(SLLNode *cursor);

// sll_free_nodes frees the entire linked list
// frees memory
void sll_nodes_free(SLLNode *cursor);

// sll_create prepares a new node with all the helper functions
// returns NULL if at least one argument is not properly provided
// allocs memory
SLLNode *sll_create(bool (*cmp)(void *, void *), void (*print)(SLLNode *node),
                    void (*free_data)(void *data), void *data);

// sll_append creates a new node at the end of the linked list
// returns NULL if cursor is NULL
// allocs memory
SLLNode *sll_append(SLLNode *cursor, void *data);

// sll_find searches linked list for provided data and returns the first found
// instance
// returns NULL if cursor is NULL
SLLNode *sll_find(SLLNode *cursor, void *data);

// sll_delete removes the node from the linked list
// linked list is kept intact
// returns NULL if cursor is NULL
// frees memory
SLLNode *sll_delete(SLLNode *cursor, void *data);

// sll_update replaces old_data with new_data on the first found instance
// frees data
// returns NULL if cursor is NULL
SLLNode *sll_update(SLLNode *cursor, void *old_data, void *new_data);

#endif // defined(SINGLY_LINKED_LIST) // HEADER
// }

// {
#ifdef SINGLY_LINKED_LIST // IMPLEMENTATION
#pragma once

#include <stdlib.h>

void sll_list(SLLNode *cursor) {
  for (; cursor != NULL; cursor = cursor->next) {
    printf("-> ");
    cursor->print(cursor);
  }
  printf("-> %p\n", cursor);
  return;
}

size_t sll_list_len(SLLNode *cursor) {
  for (size_t i = 0; cursor != NULL; ++i, cursor = cursor->next) {
    if (!cursor->next) {
      return i + 1;
    }
  }
  return 0;
}

SLLNode *sll_node_free(SLLNode *cursor) {
  if (!cursor) {
    return NULL;
  }
  // comment in for logging: printf("freeing (%p)\n", cursor);
  SLLNode *me = cursor;
  cursor = cursor->next;
  me->free_data(me->data);
  free(me);
  return cursor;
}

void sll_nodes_free(SLLNode *cursor) {
  for (; cursor != NULL;) {
    cursor = sll_node_free(cursor);
  }
  return;
}

SLLNode *sll_create(bool (*cmp)(void *, void *), void (*print)(SLLNode *node),
                    void (*free_data)(void *data), void *data) {
  if (!cmp || !print || !free_data || !data) {
    return NULL;
  }
  SLLNode *new = calloc(1, sizeof(SLLNode));
  new->cmp = cmp;
  new->print = print;
  new->free_data = free_data;
  new->data = data;
  new->next = NULL;
  return new;
}

SLLNode *sll_append(SLLNode *cursor, void *data) {
  if (!cursor) {
    return NULL;
  }
  SLLNode *new =
      sll_create(cursor->cmp, cursor->print, cursor->free_data, data);

  SLLNode *cur = cursor;
  for (; cur->next != NULL; cur = cur->next) {
    ;
  }
  cur->next = new;
  return cursor;
}

SLLNode *sll_find(SLLNode *cursor, void *data) {
  for (; cursor != NULL; cursor = cursor->next) {
    if (cursor->cmp(cursor->data, data)) {
      return cursor;
    }
  }
  return NULL;
}

SLLNode *sll_delete(SLLNode *cursor, void *data) {
  if (!cursor) {
    return NULL;
  }
  SLLNode *cur = cursor;
  SLLNode *prev = NULL;

  for (; cur->next != NULL && !cur->cmp(cur->data, data);) {
    prev = cur;
    cur = cur->next;
  }
  if (!cur->cmp(cur->data, data)) {
    return cursor;
  }
  SLLNode *me = cur;
  if (!prev) {
    cursor = cur->next;
  } else {
    prev->next = cur->next;
  }
  sll_node_free(me);
  return cursor;
}

SLLNode *sll_update(SLLNode *cursor, void *old_data, void *new_data) {
  if (!cursor) {
    return NULL;
  }
  SLLNode *cur = cursor;
  for (; cur != NULL && !cur->cmp(cur->data, old_data); cur = cur->next) {
    ;
  }
  cur->free_data(cur->data);
  cur->data = new_data;
  return cursor;
}

#endif // SINGLY_LINKED_LIST // IMPLEMENTATION
// }

// {
#if defined(MAP) // HEADER
#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct MapKeyValue {
  void *key;
  void *value;
  struct MapKeyValue *next;
} MapKeyValue;

// MapKeyType describes which key types are supported by this Map implementation
enum MapKeyType {
  MapKeyInt,
  MapKeyStr,
  MapKeyCount,
};

typedef struct {
  int (*hash)(enum MapKeyType key_type, void *key, size_t cap);
  enum MapKeyType key_type;
  void (*print_value)(void *value);
  void (*free_value)(void *value);
  MapKeyValue **kvs;
  size_t len;
  size_t cap;
} Map;

// MAP_BUCKETS_SIZE is the buckets size for Map when creating a new map
int MAP_BUCKETS_SIZE = 128;

// map_hash is default hashing function
int map_hash(enum MapKeyType key_type, void *key, size_t cap);

// map_free frees the entire map
// frees memory
void map_free(Map *map);

// map_create prepares a new map with all the helper functions
// returns NULL if at least one argument is not properly provided
// default buckets size is MAP_BUCKETS_SIZE
// allocs memory
Map *map_create(int (*hash)(enum MapKeyType key_type, void *key, size_t cap),
                enum MapKeyType key_type, void (*print_value)(void *value),
                void (*free_value)(void *value));

// map_list prints the entire map
void map_list(Map *map);

// map_insert adds/updates `value` to/in `map` with `key`
// differently:
// if `key` doesn't exist, insert the `value`
// if `key` exists, free the existing value and insert the `value`
// allocs memory
Map *map_insert(Map *map, void *key, void *value);

// map_find retrieves value corresponding to the key
// returns NULL if `key` doesn't exist in `map`
void *map_find(Map *map, void *key);

// map_delete removes key-value pair from `map` that has `key`
// frees memory
Map *map_delete(Map *map, void *key);

// TODO: reorg - handle up- and down-sizing

#endif // defined(MAP) // HEADER
// }

// {
#if defined(MAP) // IMPLEMENTATION
#pragma once

// TODO: reorg - handle up- and down-sizing

int map_hash(enum MapKeyType key_type, void *key, size_t cap) {
  switch (key_type) {
  case MapKeyInt: {
    return (*(int *)key) % cap;
  }
  case MapKeyStr: {
    char *kkey = (char *)key;
    size_t key_len = strlen(kkey);
    int h = 0;
    for (size_t i = 0; i < key_len; ++i) {
      h += *(kkey + i);
    }
    if (cap == 0) {
      cap = key_len;
    }
    return h % cap;
  }
  default:
    assert(0 && "unsupported type");
  }
  assert(0 && "unreachable");
}

void map_free(Map *map) {
  if (!map) {
    return;
  }
  for (int i = 0; i < map->cap; ++i) {
    if (!map->kvs[i]) {
      continue;
    }

    for (MapKeyValue *cur = map->kvs[i]; cur;) {
      map->free_value(map->kvs[i]->value);
      MapKeyValue *me = cur;
      cur = cur->next;
      free(me);
    }

    map->kvs[i] = NULL;
  }
  if (map->kvs) {
    free(map->kvs);
  }
  if (map) {
    free(map);
  }
}

Map *map_create(int (*hash)(enum MapKeyType key_type, void *key, size_t cap),
                enum MapKeyType key_type, void (*print_value)(void *value),
                void (*free_value)(void *value)) {
  if (!hash || !print_value || !free_value) {
    return NULL;
  }
  switch (key_type) {
  case MapKeyInt: {
    break;
  }
  case MapKeyStr: {
    break;
  }
  case MapKeyCount: {
    return NULL;
  }
  default:
    return NULL;
  }

  Map *new = calloc(1, sizeof(Map));
  new->hash = hash;
  new->key_type = key_type;
  new->print_value = print_value;
  new->free_value = free_value;
  new->kvs = calloc(MAP_BUCKETS_SIZE, sizeof(MapKeyValue *));
  new->len = 0;
  new->cap = MAP_BUCKETS_SIZE;
  return new;
}

void map_list(Map *map) {
  if (!map) {
    printf("[ (nil) ]\n");
    return;
  }
  printf("[ ");
  switch (map->key_type) {
  case MapKeyInt: {
    for (int i = 0; i < map->cap; ++i) {
      if (!map->kvs[i]) {
        continue;
      }
      printf("%d:", *(int *)map->kvs[i]->key);
      map->print_value(map->kvs[i]->value);
      putchar(' ');

      for (MapKeyValue *cur = map->kvs[i]->next; cur; cur = cur->next) {
        printf("%d:", *(int *)cur->key);
        map->print_value(cur->value);
        putchar(' ');
      }
    }
    break;
  case MapKeyStr: {
    for (int i = 0; i < map->cap; ++i) {
      if (!map->kvs[i]) {
        continue;
      }
      printf("%s:", (char *)map->kvs[i]->key);
      map->print_value(map->kvs[i]->value);
      putchar(' ');

      // NOTE: handle printing
      for (MapKeyValue *cur = map->kvs[i]->next; cur; cur = cur->next) {
        printf("%s:", (char *)cur->key);
        map->print_value(cur->value);
        putchar(' ');
      }
    }
  } break;
  default:
    assert(0 && "unsupported type");
    break;
  }
  }
  printf("]\n");
}

Map *map_insert(Map *map, void *key, void *value) {
  if (!map) {
    return NULL;
  }

  int idx = map->hash(map->key_type, key, map->cap);

  MapKeyValue *kv = calloc(1, sizeof(MapKeyValue));
  kv->key = key;
  kv->value = value;
  kv->next = NULL;

  if (map->kvs[idx] == NULL) {
    map->kvs[idx] = kv;
    ++map->len;
    return map;
  }

  MapKeyValue *prev = NULL;
  MapKeyValue *cur = map->kvs[idx];

  for (; cur;) {
    switch (map->key_type) {
    case MapKeyInt: {
      if (*(int *)cur->key == *(int *)key) {
        map->free_value(cur->value);
        cur->value = value;
        return map;
      }
      break;
    }
    case MapKeyStr: {
      if (strcmp((char *)cur->key, (char *)key) == 0) {
        map->free_value(cur->value);
        cur->value = value;
        return map;
      }
      break;
    }
    default: {
      assert(0 && "unsupported type");
    }
    }
    prev = cur;
    cur = cur->next;
  }

  if (cur == NULL && prev != NULL) {
    prev->next = kv;
    ++map->len;
  }

  return map;
}

void *map_find(Map *map, void *key) {
  if (!map) {
    return NULL;
  }

  int idx = map->hash(map->key_type, key, map->cap);

  MapKeyValue *cur = map->kvs[idx];

  if (!cur) {
    return NULL;
  }

  for (; cur; cur = cur->next) {
    switch (map->key_type) {
    case MapKeyInt: {
      if (*(int *)cur->key == *(int *)key) {
        return cur->value;
      }
      break;
    }
    case MapKeyStr: {
      if (strcmp((char *)cur->key, (char *)key) == 0) {
        return cur->value;
      }
      break;
    }
    default: {
      assert(0 && "unsupported type");
    }
    }
  }

  return NULL;
}

Map *map_delete(Map *map, void *key) {
  if (!map) {
    return NULL;
  }

  int idx = map->hash(map->key_type, key, map->cap);

  if (map->kvs[idx] == NULL) {
    return NULL;
  }

  MapKeyValue *prev = NULL;
  MapKeyValue *cur = map->kvs[idx];

  for (; cur;) {
    switch (map->key_type) {
    case MapKeyInt: {
      if (*(int *)cur->key == *(int *)key) {
        MapKeyValue *me = cur;
        map->free_value(cur->value);
        if (!prev) {
          map->kvs[idx] = cur->next;
        } else {
          prev->next = cur->next;
        }
        free(me);
        --map->len;
        return map;
      }
      break;
    }
    case MapKeyStr: {
      if (strcmp((char *)cur->key, (char *)key) == 0) {
        MapKeyValue *me = cur;
        map->free_value(cur->value);
        if (!prev) {
          map->kvs[idx] = cur->next;
        } else {
          prev->next = cur->next;
        }
        free(me);
        --map->len;
        return map;
      }
      break;
    }
    default: {
      assert(0 && "unsupported type");
    }
    }
    prev = cur;
    cur = cur->next;
  }

  return map;
}

#endif // defined(MAP) // IMPLEMENTATION
// }

// {
#if defined(STACK) || defined(QUEUE) // HEADER

// print should be able to print `data`
typedef struct SQNode {
  struct SQNode *next;
  void (*print)(struct SQNode *node);
  void *data;
} SQNode;

// sq_list prints the full stack/queue
void sq_list(SQNode *cursor);

// sq_list_len finds the stack/queue length
size_t sq_list_len(SQNode *cursor);

// sq_free_node frees the current node and returns the next linked list element
// frees memory
// freeing of data is left for the user
// as we want to free the node, but return the data when popping
SQNode *sq_node_free(SQNode *cursor);

// sq_free_nodes frees the entire stack/queue
// frees memory
void sq_nodes_free(SQNode *cursor);

// sq_create creates stack/queue
// returns NULL if at least one argument is NULL
// allocs memory
SQNode *sq_create(void (*print)(SQNode *node), void *data);

// sq_push pushes data to
// * top of stack
// * end of queue
// NULL cursor is not allowed
// allocs memory
SQNode *sq_push(SQNode *cursor, void *data);

// sq_pop pops the
// * top of stack
// * head of queue
// NULL cursor is not allowed
// frees memory
// returns new head, data is returned through `out`
SQNode *sq_pop(SQNode *cursor, void **out);

#endif // defined(STACK) || defined(QUEUE) // HEADER

// }

// {
#ifdef STACK // IMPLEMENTATION

#include <stdlib.h>

void sq_list(SQNode *cursor) {
  for (; cursor; cursor = cursor->next) {
    printf("-> ");
    cursor->print(cursor);
  }
  printf("-> %p\n", cursor);
  return;
}

size_t sq_list_len(SQNode *cursor) {
  size_t len = 0;
  for (; cursor; cursor = cursor->next) {
    ++len;
  }
  return len;
}

SQNode *sq_node_free(SQNode *cursor) {
  if (!cursor) {
    return NULL;
  }
  // comment in for logging: printf("freeing (%p)\n", cursor);
  SQNode *me = cursor;
  cursor = cursor->next;
  free(me);
  return cursor;
}

void sq_nodes_free(SQNode *cursor) {
  for (; cursor != NULL;) {
    cursor = sq_node_free(cursor);
  }
  return;
}

SQNode *sq_create(void (*print)(SQNode *node), void *data) {
  if (!print || !data) {
    return NULL;
  }
  SQNode *new = calloc(1, sizeof(SQNode));
  new->print = print;
  new->data = data;
  return new;
}

SQNode *sq_push(SQNode *cursor, void *data) {
  if (!cursor) {
    return NULL;
  }
  SQNode *new = sq_create(cursor->print, data);
  new->next = cursor;
  return new;
}

SQNode *sq_pop(SQNode *cursor, void **out) {
  if (!cursor) {
    return NULL;
  }
  SQNode *new = cursor->next;
  *out = cursor->data;
  sq_node_free(cursor);
  return new;
}

#endif // STACK // IMPLEMENTATION

// }