#include <stdio.h>
#include <string.h>

#define MAP
#include "../map.h"

#ifdef STR
void str_key(MapKeyValue *kv, void *i) {
  if (i != NULL) {
    printf("i: %d\n", *(int *)i);
    *(int *)i += 1;
  }
  if (kv == NULL) {
    printf("(nil)\n");
  } else {
    printf("\"%s\"(%d): \"%s\"\n", (char *)kv->key, (size_t)kv->key_len,
           (char *)kv->value);
  }
}

int main() {
  printf("hey\n");
  Map *map = map_create(NULL, NULL);
  printf("hey\n");

  printf("Map contents: (should be nothing)\n");
  map_list(map, str_key, NULL);

  char *k1 = "key1";
  map = map_insert(map, k1, strlen(k1), "maybe works");
  printf("Map contents:\n");
  map_list(map, str_key, NULL);

  char *k2 = "key2";
  map = map_insert(map, k2, strlen(k2), "another one");
  printf("Map contents:\n");
  map_list(map, str_key, NULL);

  char *k2_val = (char *)map_find(map, k2, strlen(k2));
  printf("searched \"%s\", found \"%s\"\n", k2, k2_val);

  char *k3 = "key3";
  map = map_insert(map, k3, strlen(k3), "another two");
  printf("Map contents:\n");
  map_list(map, str_key, NULL);

  map = map_delete(map, k2, strlen(k2));
  printf("deleted key \"%s\"\n", k2);
  printf("Map contents:\n");
  map_list(map, str_key, NULL);

  int counter = 0;
  // map_list(map, str_key, (void *)&counter);

  map_free(map);
}
#endif // STR

#ifdef INT
void int_key(MapKeyValue *kv, void *i) {
  if (i != NULL) {
    printf("i: %d\n", *(int *)i);
    *(int *)i += 1;
  }
  if (kv == NULL) {
    printf("(nil)\n");
  } else {
    printf("\"%d\"(%d): \"%d\"\n", *(int *)kv->key, (size_t)kv->key_len,
           *(int *)kv->value);
  }
}

int main() {
  Map *map = map_create(NULL, NULL);

  for (int i = 0; i < 100; i += 1) {
    int *j = (int *)arena_alloc(map->arena, 1 * sizeof(int));
    *j = i;
    map_insert(map, (void *)j, sizeof(i) / sizeof(int), (void *)j);
  }
  map_list(map, int_key, NULL);

  // int *f = (int *)arena_alloc(map->arena, 1 * sizeof(int));
  // int *found = (int *)map_find(map, (void *)f, sizeof(*f) / sizeof(int),
  // NULL); *f = 77;
  int f = 77;
  int *found = (int *)map_find(map, (void *)&f, sizeof(f) / sizeof(int));
  printf("found this: %d\n", *found);

  map_free(map);
}
#endif // INT

#ifdef KVS

typedef struct {
  int k;
  int v;
  char *str;
} KV;

void kv_key(MapKeyValue *kv, void *i) {
  if (i != NULL) {
    printf("i: %d\n", *(int *)i);
    *(int *)i += 1;
  }
  if (kv == NULL) {
    printf("(nil)\n");
  } else {
    printf("{\"%d\": \"%d\"} (%d): {\"%d\": \"%d\"}\n", ((KV *)kv->value)->k,
           ((KV *)kv->value)->v, (size_t)kv->key_len, ((KV *)kv->value)->k,
           ((KV *)kv->value)->v);
  }
}

int main() {
  Map *map = map_create(NULL, NULL);

  for (int i = 0; i < 100; i += 1) {
    KV *j = (KV *)arena_alloc(map->arena, 1 * sizeof(KV));
    j->k = i;
    j->v = -1 * i;
    map_insert(map, (void *)j,
               sizeof(j->k) / sizeof(int) + sizeof(j->v) / sizeof(int),
               (void *)j);
  }
  map_list(map, kv_key, NULL);

  KV f = (KV){
      .k = 77,
      .v = -77,
  };
  KV *found = (KV *)map_find(
      map, (void *)&f, sizeof(f.k) / sizeof(int) + sizeof(f.v) / sizeof(int));
  if (found == NULL) {
    printf("not found\n");
  } else {
    printf("found this: {\"%d\": \"%d\"}\n", found->k, found->v);
  }

  KV f2 = (KV){
      .k = 77, // NOTE: mismatching data
      .v = -99,
  };
  KV *found2 =
      (KV *)map_find(map, (void *)&f2,
                     sizeof(f2.k) / sizeof(int) + sizeof(f2.v) / sizeof(int));
  if (found2 == NULL) {
    printf("not found\n");
  } else {
    printf("found2 this: {\"%d\": \"%d\"}\n", found2->k, found2->v);
  }

  map_free(map);
}
#endif // KVS