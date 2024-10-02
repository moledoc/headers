#include <stdio.h>
#include <string.h>

#define MAP
#include "../map.h"

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

int main1() {
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

  map_free(map);
}