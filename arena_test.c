#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ARENA
#include "arena.h"

typedef struct {
  int v1;
  int v2;
} Vector2;

typedef struct {
  int v1;
  int v2;
  int v3;
  int v4;
} Vector4;

// allocs memory
int *vec2_to_arr(Vector2 vec) {
  int *arr = calloc(2, sizeof(int));
  arr[0] = vec2.v1;
  arr[1] = vec2.v2;
  return arr;
}

// allocs memory
int *vec4_to_arr(Vector4 vec) {
  int *arr = calloc(4, sizeof(int));
  arr[0] = vec.v1;
  arr[1] = vec.v2;
  arr[2] = vec.v3;
  arr[3] = vec.v4;
  return arr;
}

bool elemwise_cmp(uintptr_t *expected, uint32_t expected_size,
                  uintptr_t *actual, uint32_t actual_size) {
  if (expected_size != actual_size) {
    return false;
  }
  for (int i = 0; i < expected_size; i += 1) {
    if (expected[i] != actual[i]) {
      return false;
    }
  }
  return true;
}

int skip(int argc, char **argv, char *target) {
  char *run = "";
  for (int i = 0; i < argc; i += 1) {
    char *arg = argv[i];
    if (strcmp("--run", arg) == 0 && i + 1 < argc) {
      run = argv[i + 1];
      break;
    }
  }
  return strlen(run) != 0 && strcmp(run, target) != 0;
}

void arena_create_test(int argc, char **argv) {
  char *target = "create";
  if (skip(argc, argv, target)) {
    return;
  }

  // create - default ARENA_CAP
  {
    Arena *arena = arena_create();
    assert(arena != NULL && "unexpected NULL");
    assert(arena->next == NULL && "unexpected non-NULL");
    assert(arena->cap == ARENA_CAP && "unexpected size");
    assert(arena->ref_count == 0 && "unexpected count");
    assert(arena->data != NULL && "unexpected NULL");

    uintptr_t *expected_data = alloca(sizeof(uintptr_t) * ARENA_CAP);
    memset(expected_data, 0, ARENA_CAP);
    elemwise_cmp(expected_data, ARENA_CAP, arena->data, arena->cap);

    arena = arena_destroy(arena);
  }

  // create - custom ARENA_CAP
  {
    uint32_t expected_cap = 24;
    ARENA_CAP = expected_cap;
    Arena *arena = arena_create();
    assert(arena != NULL && "unexpected NULL");
    assert(arena->next == NULL && "unexpected non-NULL");
    assert(arena->cap == expected_cap && "unexpected size");
    assert(arena->ref_count == 0 && "unexpected count");
    assert(arena->data != NULL && "unexpected NULL");

    uintptr_t *expected_data = alloca(sizeof(uintptr_t) * ARENA_CAP);
    memset(expected_data, 0, ARENA_CAP);
    elemwise_cmp(expected_data, ARENA_CAP, arena->data, arena->cap);

    arena = arena_destroy(arena);
  }

  // alloc - nothing alloced, fits
  // alloc - nothing alloced, doesn't fit
  // alloc - something alloced, room on page, fits
  // alloc - something alloced, no room on page, no next page, fits
  // alloc - something alloced, no room on page, no next page, doesn't fit
  // alloc - something alloced, no room on page, next page, fits

  {
    uint32_t cap = 8;
    ARENA_CAP = cap;

    uintptr_t *expected_page_1_data = alloca(sizeof(uintptr_t) * ARENA_CAP);
    memset(expected_data, 0, ARENA_CAP);

    elemwise_cmp(expected_data, ARENA_CAP, arena->data, arena->cap);

    arena = arena_destroy(arena);
  }

  printf("%s - ok\n", target);
  return;
}

int main(int argc, char **argv) {
  arena_create_test(argc, argv);
  /*
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

    printf("hey\n");
  */
}