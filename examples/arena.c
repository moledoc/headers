#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ARENA
#include "arena.h"

typedef struct {
  int k1;
  int v1;
  int k2;
  int v2;
} KV;

int main(int argc, char **argv) {
  ARENA_CAP = 2;
  Arena *arena = arena_create();
  char *test = (char *)arena_alloc(arena, strlen("test") + 1);
  memcpy(test, "test", strlen("test"));

  KV *kv = (KV *)arena_alloc(arena, sizeof(KV));
  kv->k1 = 1;
  kv->v1 = 2;
  kv->k2 = 3;
  kv->v2 = 4;
  printf("%s\n", test);
  KV *kv2 = (KV *)arena_alloc(arena, sizeof(KV));
  kv2->k1 = 5;
  kv2->v1 = 6;
  kv2->k2 = 7;
  kv2->v2 = 8;
  printf("%s\n", test);

  printf("data size: %d\n", arena_data_size(test));
  printf("data size: %d\n", arena_data_size(kv));
  printf("data size: %d\n", arena_data_size(kv2));

  kv = (KV *)arena_free(arena, kv);
  test = (char *)arena_free(arena, test);
  kv = (KV *)arena_alloc(arena, sizeof(KV));
  kv2 = (KV *)arena_free(arena, kv2);

  arena = arena_destroy(arena);
}