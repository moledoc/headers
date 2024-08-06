// #pragme once - in this scenario we want to allow multiple loading - it might
// be that first time we loaded with no defines, but next time we might have
// needed defines

// {
#if defined(SINGLY_LINKED_LIST) // HEADER
#pragma once

#include <stdbool.h>

typedef struct SLLNode {
  bool (*cmp)(void *, void *);
  struct SLLNode *next;
  void *data; // NOTE: freeing data is not handled; please handle it yourself
} SLLNode;

// sll_apply executes func fn to each list elements with given argument
void sll_apply(SLLNode *cursor, void (*fn)(SLLNode *, void *), void *arg);

// sll_list prints the full linked list
void sll_list(SLLNode *cursor, void (*print)(SLLNode *, void *), void *fmt);

// sll_list_len finds the linked_list length
size_t sll_list_len(SLLNode *cursor);

// sll_free_node frees the current node and returns the next linked list element
// frees memory
SLLNode *sll_node_free(SLLNode *cursor);

// sll_free_nodes frees the entire linked list
// frees memory
void *sll_nodes_free(SLLNode *cursor);

// sll_create prepares a new node with all the helper functions
// returns NULL if at least one argument is not properly provided
// allocs memory
SLLNode *sll_create(bool (*cmp)(void *, void *), void *data);

// sll_append creates a new node at the end of the linked list
// returns NULL if cursor is NULL
// allows duplicates
// allocs memory
SLLNode *sll_append(SLLNode *cursor, void *data);

// sll_add creates a new node at the beginning of the linked list and returns
// that node returns NULL if cursor is NULL allows duplicates allocs memory
SLLNode *sll_add(SLLNode *cursor, void *data);

// sll_find searches linked list for provided data and returns the first found
// instance
// returns NULL if cursor is NULL
SLLNode *sll_find(SLLNode *cursor, void *data);

// sll_update replaces old_data with new_data on the first found instance
// frees data
// returns NULL if cursor is NULL
SLLNode *sll_update(SLLNode *cursor, void *old_data, void *new_data);

// sll_delete removes the node from the linked list
// linked list is kept intact
// returns NULL if cursor is NULL
// frees memory
SLLNode *sll_delete(SLLNode *cursor, void *data);

#endif // defined(SINGLY_LINKED_LIST) // HEADER
// }

// {
#ifdef SINGLY_LINKED_LIST // IMPLEMENTATION
#pragma once

#include <stdlib.h>

void sll_apply(SLLNode *cursor, void (*fn)(SLLNode *, void *), void *arg) {
  for (; cursor != NULL; cursor = cursor->next) {
    (*fn)(cursor, arg);
  }
}

void sll_list(SLLNode *cursor, void (*print)(SLLNode *, void *), void *fmt) {
  sll_apply(cursor, print, fmt);
  return;
}

void sll_list_count(SLLNode *cursor, void *count) { (*(int *)count)++; }

size_t sll_list_len(SLLNode *cursor) {
  int count = 0;
  sll_apply(cursor, sll_list_count, (void *)&count);
  return count;
}

SLLNode *sll_node_free(SLLNode *cursor) {
  if (cursor == NULL) {
    return NULL;
  }
  SLLNode *me = cursor;
  cursor = cursor->next;
  free(me);
  return cursor;
}

void *sll_nodes_free(SLLNode *cursor) {
  for (; cursor != NULL;) {
    cursor = sll_node_free(cursor);
  }
  return NULL;
}

SLLNode *sll_create(bool (*cmp)(void *, void *), void *data) {
  if (cmp == NULL || data == NULL) {
    return NULL;
  }
  SLLNode *new = calloc(1, sizeof(SLLNode));
  new->cmp = cmp;
  new->data = data;
  new->next = NULL;
  return new;
}

SLLNode *sll_append(SLLNode *cursor, void *data) {
  if (cursor == NULL) {
    return NULL;
  }
  if (data == NULL) {
    return cursor;
  }
  SLLNode *new = sll_create(cursor->cmp, data);

  SLLNode *cur = cursor;
  for (; cur->next != NULL; cur = cur->next) {
    ;
  }
  cur->next = new;
  return cursor;
}

SLLNode *sll_add(SLLNode *cursor, void *data) {
  if (cursor == NULL) {
    return NULL;
  }
  if (data == NULL) {
    return cursor;
  }
  SLLNode *new = sll_create(cursor->cmp, data);
  new->next = cursor;
  return new;
}

SLLNode *sll_find(SLLNode *cursor, void *data) {
  if (data == NULL) {
    return NULL;
  }
  for (; cursor != NULL; cursor = cursor->next) {
    if (cursor->cmp(cursor->data, data)) {
      return cursor;
    }
  }
  return NULL;
}

SLLNode *sll_update(SLLNode *cursor, void *old_data, void *new_data) {
  if (cursor == NULL) {
    return NULL;
  }
  if (new_data == NULL) {
    return cursor;
  }
  SLLNode *cur = cursor;
  for (; cur != NULL && !cur->cmp(cur->data, old_data); cur = cur->next) {
    ;
  }
  cur->data = new_data;
  return cursor;
}

SLLNode *sll_delete(SLLNode *cursor, void *data) {
  if (cursor == NULL) {
    return NULL;
  }
  if (data == NULL) {
    return cursor;
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

#endif // SINGLY_LINKED_LIST // IMPLEMENTATION
// }

// {
#ifdef CIRCULAR_DOUBLY_LINKED_LIST // HEADER
#pragma once

#include <stdbool.h>

typedef struct CDLLNode {
  bool (*cmp)(void *, void *);
  void (*print)(struct CDLLNode *node);
  void (*free_data)(void *); // frees memory
  struct CDLLNode *prev;
  struct CDLLNode *next;
  void *data;
} CDLLNode;

// cdll_list prints the full linked list
void cdll_list(CDLLNode *cursor);

// cdll_list_len finds the linked_list length
size_t cdll_list_len(CDLLNode *cursor);

// cdll_free_node frees the current node and returns the next linked list
// element frees memory
CDLLNode *cdll_node_free(CDLLNode *cursor);

// cdll_free_nodes frees the entire linked list
// frees memory
void cdll_nodes_free(CDLLNode *cursor);

// cdll_create prepares a new node with all the helper functions
// returns NULL if at least one argument is not properly provided
// allocs memory
CDLLNode *cdll_create(bool (*cmp)(void *, void *),
                      void (*print)(CDLLNode *node),
                      void (*free_data)(void *data), CDLLNode *prev,
                      CDLLNode *next, void *data);

// cdll_append creates a new node at the end of the linked list
// returns NULL if cursor is NULL
// allows duplicates
// allocs memory
CDLLNode *cdll_append(CDLLNode *cursor, void *data);

// cdll_find searches linked list for provided data and returns the first found
// instance
// returns NULL if cursor is NULL
CDLLNode *cdll_find(CDLLNode *cursor, void *data);

// cdll_update replaces old_data with new_data on the first found instance
// frees data
// returns NULL if cursor is NULL
CDLLNode *cdll_update(CDLLNode *cursor, void *old_data, void *new_data);

// cdll_delete removes the node from the linked list
// linked list is kept intact
// returns NULL if cursor is NULL
// frees memory
CDLLNode *cdll_delete(CDLLNode *cursor, void *data);

#endif // CIRCULAR_DOUBLY_LINKED_LIST // HEADER
// }

// {
#ifdef CIRCULAR_DOUBLY_LINKED_LIST // IMPLEMENTATION
#pragma once

#include <stdlib.h>

void cdll_list(CDLLNode *cursor) {
  if (!cursor) {
    printf("-> %p\n", (void *)cursor);
    return;
  }
  CDLLNode *cur = cursor;
  printf("-> ");
  cur->print(cur);
  for (cur = cur->next; cur && cur != cursor; cur = cur->next) {
    printf("-> ");
    cur->print(cur);
  }
  return;
}

size_t cdll_list_len(CDLLNode *cursor) {
  if (!cursor) {
    return 0;
  }
  CDLLNode *cur = cursor;
  cur = cur->next;
  for (size_t i = 1; cur; ++i, cur = cur->next) {
    if (cur == cursor) {
      return i;
    }
  }
  return 0;
}

CDLLNode *cdll_node_free(CDLLNode *cursor) {
  if (!cursor) {
    return NULL;
  }
  // comment in for logging: printf("freeing (%p)\n", cursor);
  CDLLNode *me = cursor;
  if (me->prev) {
    me->prev->next = NULL;
  }
  me->prev = NULL;
  cursor = cursor->next;
  me->free_data(me->data);
  free(me);
  if (cursor) {
    cursor->prev = NULL;
  }
  return cursor;
}

void cdll_nodes_free(CDLLNode *cursor) {
  for (; cursor != NULL;) {
    cursor = cdll_node_free(cursor);
  }
  return;
}

CDLLNode *cdll_create(bool (*cmp)(void *, void *),
                      void (*print)(CDLLNode *node),
                      void (*free_data)(void *data), CDLLNode *prev,
                      CDLLNode *next, void *data) {
  if (!cmp || !print || !free_data || !data) {
    return NULL;
  }
  CDLLNode *new = calloc(1, sizeof(CDLLNode));
  if (!prev) {
    prev = new;
  }
  if (!next) {
    next = new;
  }
  new->cmp = cmp;
  new->print = print;
  new->free_data = free_data;
  new->data = data;
  new->next = next;
  new->prev = prev;
  return new;
}

CDLLNode *cdll_append(CDLLNode *cursor, void *data) {
  if (!cursor || !cursor->prev) {
    return NULL;
  }

  CDLLNode *new = cdll_create(cursor->cmp, cursor->print, cursor->free_data,
                              cursor->prev, cursor, data);
  if (cursor->next == cursor) { // NOTE: if is 1 elem list
    cursor->next = new;
  }
  cursor->prev->next = new;
  cursor->prev = new;
  return cursor;
}

#ifdef CDLL_TESTS
int iters = 0;
#endif // CDLL_TESTS

// cdll_find searches linked list for provided data and returns the first found
// instance
// returns NULL if cursor is NULL
CDLLNode *cdll_find(CDLLNode *cursor, void *data) {
  if (!cursor) {
    return NULL;
  }
#ifdef CDLL_TESTS
  iters = 0;
#endif // CDLL_TESTS
  if (cursor->cmp(cursor->data, data)) {
    return cursor;
  }
  CDLLNode *prev = cursor->prev;
  CDLLNode *next = cursor->next;
  for (; next != cursor && prev != cursor;) {
#ifdef CDLL_TESTS
    ++iters;
#endif // CDLL_TESTS
    if (next->cmp(next->data, data)) {
      return next;
    }
    if (prev->cmp(prev->data, data)) {
      return prev;
    }
    if (prev == next || prev->prev == next || next->next == prev) {
      break;
    }
    prev = prev->prev;
    next = next->next;
  }
  return NULL;
}

// TODO: below this point

// cdll_update replaces old_data with new_data on the first found instance
// frees data
// returns NULL if cursor is NULL
CDLLNode *cdll_update(CDLLNode *cursor, void *old_data, void *new_data);

// cdll_delete removes the node from the linked list
// linked list is kept intact
// returns NULL if cursor is NULL
// frees memory
CDLLNode *cdll_delete(CDLLNode *cursor, void *data);

#endif // CIRCULAR_DOUBLY_LINKED_LIST // IMPLEMENTATION
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

enum {
  MAP_BUCKETS_SIZE =
      521, // MAP_BUCKETS_SIZE is the buckets size for Map when
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

// map_apply executes func fn to each list elements with given argument
void map_apply(Map *map, void (*fn)(MapKeyValue *, void *), void *arg);

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
    size_t key_len = 0;

    int h = 0;
    for (unsigned char *p = (unsigned char *)kkey; *p != '\0'; ++p, ++key_len) {
      h += HASH_MULTIPLIER * h + *p;
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

// map_apply executes func fn to each list elements with given argument
void map_apply(Map *map, void (*fn)(MapKeyValue *, void *), void *arg) {
  for (int i = 0; i < map->cap; ++i) {
    if (map->kvs[i] == NULL) {
      continue;
    }
    (*fn)(map->kvs[i], arg);
  }
}

#endif // defined(MAP) // IMPLEMENTATION
// }

// {
#ifdef STACK // HEADER
#pragma once

// print should be able to print `data`
typedef struct StackNode {
  struct StackNode *next;
  void (*print)(struct StackNode *node);
  void *data;
} StackNode;

// stack_list prints the full stack/queue
void stack_list(StackNode *cursor);

// stack_list_len finds the stack/queue length
size_t stack_list_len(StackNode *cursor);

// stack_free_node frees the current node and returns the next linked list
// element frees memory freeing of data is left for the user as we want to free
// the node, but return the data when popping
StackNode *stack_node_free(StackNode *cursor);

// stack_free_nodes frees the entire stack/queue
// frees memory
void stack_nodes_free(StackNode *cursor);

// stack_create creates stack/queue
// returns NULL if at least one argument is NULL
// allocs memory
StackNode *stack_create(void (*print)(StackNode *node), void *data);

// stack_push pushes data to top of stack
// NULL cursor is not allowed
// allocs memory
StackNode *stack_push(StackNode *cursor, void *data);

// stack_pop pops the top of stack
// NULL cursor is not allowed
// frees memory
// returns new head, data is returned through `out`
StackNode *stack_pop(StackNode *cursor, void **out);

// stack_apply executes func fn to each list elements with given argument
void stack_apply(StackNode *cursor, void (*fn)(StackNode *, void *), void *arg);

#endif // STACK // HEADER

// }

// {
#ifdef STACK // IMPLEMENTATION

#include <stdlib.h>

void stack_list(StackNode *cursor) {
  for (; cursor; cursor = cursor->next) {
    printf("-> ");
    cursor->print(cursor);
  }
  printf("-> %p\n", cursor);
  return;
}

size_t stack_list_len(StackNode *cursor) {
  size_t len = 0;
  for (; cursor; cursor = cursor->next) {
    ++len;
  }
  return len;
}

StackNode *stack_node_free(StackNode *cursor) {
  if (!cursor) {
    return NULL;
  }
  // comment in for logging: printf("freeing (%p)\n", cursor);
  StackNode *me = cursor;
  cursor = cursor->next;
  free(me);
  return cursor;
}

void stack_nodes_free(StackNode *cursor) {
  for (; cursor != NULL;) {
    cursor = stack_node_free(cursor);
  }
  return;
}

StackNode *stack_create(void (*print)(StackNode *node), void *data) {
  if (!print || !data) {
    return NULL;
  }
  StackNode *new = calloc(1, sizeof(StackNode));
  new->print = print;
  new->data = data;
  return new;
}

StackNode *stack_push(StackNode *cursor, void *data) {
  if (!cursor) {
    return NULL;
  }
  StackNode *new = stack_create(cursor->print, data);
  new->next = cursor;
  return new;
}

StackNode *stack_pop(StackNode *cursor, void **out) {
  if (!cursor) {
    return NULL;
  }
  StackNode *new = cursor->next;
  *out = cursor->data;
  stack_node_free(cursor);
  return new;
}

void stack_apply(StackNode *cursor, void (*fn)(StackNode *, void *),
                 void *arg) {
  for (; cursor != NULL; cursor = cursor->next) {
    (*fn)(cursor, arg);
  }
}

#endif // STACK // IMPLEMENTATION

// }

// {
#ifdef QUEUE // HEADER
#pragma once

// print should be able to print `data`
typedef struct QueueNode {
  struct QueueNode *next;
  void (*print)(struct QueueNode *node);
  void *data;
} QueueNode;

// queue_list prints the full stack/queue
void queue_list(QueueNode *cursor);

// queue_list_len finds the stack/queue length
size_t queue_list_len(QueueNode *cursor);

// queue_free_node frees the current node and returns the next linked list
// element frees memory freeing of data is left for the user as we want to free
// the node, but return the data when popping
QueueNode *queue_node_free(QueueNode *cursor);

// queue_free_nodes frees the entire stack/queue
// frees memory
void queue_nodes_free(QueueNode *cursor);

// queue_create creates stack/queue
// returns NULL if at least one argument is NULL
// allocs memory
QueueNode *queue_create(void (*print)(QueueNode *node), void *data);

// queue_push pushes data to end of queue
// NULL cursor is not allowed
// allocs memory
QueueNode *queue_push(QueueNode *cursor, void *data);

// queue_pop pops the head of queue
// NULL cursor is not allowed
// frees memory
// returns new head, data is returned through `out`
QueueNode *queue_pop(QueueNode *cursor, void **out);

// queue_apply executes func fn to each list elements with given argument
void queue_apply(QueueNode *cursor, void (*fn)(QueueNode *, void *), void *arg);

#endif // QUEUE // HEADER

// }

// {
#ifdef QUEUE // IMPLEMENTATION

#include <stdlib.h>

void queue_list(QueueNode *cursor) {
  for (; cursor; cursor = cursor->next) {
    printf("-> ");
    cursor->print(cursor);
  }
  printf("-> %p\n", cursor);
  return;
}

size_t queue_list_len(QueueNode *cursor) {
  size_t len = 0;
  for (; cursor; cursor = cursor->next) {
    ++len;
  }
  return len;
}

QueueNode *queue_node_free(QueueNode *cursor) {
  if (!cursor) {
    return NULL;
  }
  // comment in for logging: printf("freeing (%p)\n", cursor);
  QueueNode *me = cursor;
  cursor = cursor->next;
  free(me);
  return cursor;
}

void queue_nodes_free(QueueNode *cursor) {
  for (; cursor != NULL;) {
    cursor = queue_node_free(cursor);
  }
  return;
}

QueueNode *queue_create(void (*print)(QueueNode *node), void *data) {
  if (!print || !data) {
    return NULL;
  }
  QueueNode *new = calloc(1, sizeof(QueueNode));
  new->print = print;
  new->data = data;
  return new;
}

QueueNode *queue_push(QueueNode *cursor, void *data) {
  if (!cursor) {
    return NULL;
  }
  QueueNode *new = queue_create(cursor->print, data);
  QueueNode *cur = cursor;
  for (; cur->next; cur = cur->next) {
    ;
  }
  cur->next = new;
  return cursor;
}

QueueNode *queue_pop(QueueNode *cursor, void **out) {
  if (!cursor) {
    return NULL;
  }
  QueueNode *new = cursor->next;
  *out = cursor->data;
  queue_node_free(cursor);
  return new;
}

void queue_apply(QueueNode *cursor, void (*fn)(QueueNode *, void *),
                 void *arg) {
  for (; cursor != NULL; cursor = cursor->next) {
    (*fn)(cursor, arg);
  }
}

#endif // QUEUE // IMPLEMENTATION

// }