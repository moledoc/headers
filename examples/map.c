#include <stdio.h>
#include <string.h>

#define MAP
#include "../map.h"

void str_key(MapKeyValue *kv, void *_) {
  if (kv == NULL) {
    printf("(nil)\n");
  } else {
    printf("\"%s\"(%d): \"%s\"\n", (char *)kv->key, (size_t)kv->key_len,
           (char *)kv->value);
  }
}

int main() {
  printf("hey\n");
  Map *map = map_create(NULL);

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

  map_free(map);
}