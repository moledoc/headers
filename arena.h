#pragma once

// {
#ifdef ARENA // HEADER

#include <stdint.h>

typedef struct Arena {
  struct Arena *next;
  uint32_t cap;
  uint32_t offset;
  uint32_t ref_count;
  uintptr_t data[];
} Arena;

// arena_create is a function that creates an memory arena that can be used to
// allocate objects.
// returns NULL if arena_cap is zero.
// returns pointer to the memory arena.
// allocs memory.
Arena *arena_create(uint32_t arena_cap);

// arena_destroy destroys the memory arena.
// returns NULL.
// recommended to assign return value to avoid use after free.
// frees memory.
Arena *arena_destroy(Arena *arena);

// arena_alloc allocates requested size in the arena and returns it.
uintptr_t *arena_alloc(Arena *arena, uint32_t size);

// arena_free deallocates memory in the arena.
uintptr_t *arena_free(Arena *arena, void *data);

// arena_data_size returns size of the data.
uint32_t arena_data_size(void *data);

#endif // ARENA // HEADER

// }

// {
#ifdef ARENA // IMPLEMENTATION

#include <alloca.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

// ARENA_HELPERS

uintptr_t arena_norm(uint32_t data_size) {
  return (data_size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
}

uint32_t arena_denorm(uintptr_t size) { return size * sizeof(uintptr_t); }

uint32_t arena_data_offset(Arena *arena, void *data) {
  return (uintptr_t *)data - arena->data;
}

void arena_zero_page(Arena *arena) {
  if (arena == NULL) {
    return;
  }
  for (int i = 0; i < arena->cap; i += 1) {
    arena->data[i] = 0;
  }
  return;
}

// ARENA_HELPERS

Arena *arena_create(uint32_t arena_cap) {
  if (arena_cap == 0) {
    return NULL;
  }
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_ANONYMOUS | MAP_PRIVATE;
  int fd = -1;
  off_t offset = 0;
  size_t length = sizeof(Arena) + sizeof(uintptr_t) * arena_cap;
  Arena *arena = mmap(NULL, length, prot, flags, fd, offset);
  if (arena == MAP_FAILED) {
    return NULL;
  }
  arena->next = NULL;
  arena->cap = arena_cap;
  arena->offset = 0;
  arena->ref_count = 0;
  arena_zero_page(arena);
  return arena;
}

Arena *arena_destroy(Arena *arena) {
  for (; arena != NULL;) {
    Arena *me = arena;
    arena = arena->next;
    int ret = munmap(me, sizeof(Arena) + sizeof(uintptr_t) * me->cap);
    if (ret != 0) {
      fprintf(stderr, "[WARNING]: failed to unmap arena: %p\n", me);
    }
  }
  return NULL;
}

uintptr_t *arena_alloc(Arena *arena, uint32_t size) {
  if (arena == NULL) {
    return NULL;
  }
  uintptr_t *ret = NULL;
  uintptr_t norm_size = arena_norm(size);
  if ((arena->offset + norm_size + 1) <= arena->cap) {
    arena->data[arena->offset] = size;
    ret = arena->data + (arena->offset + 1);
    arena->offset += norm_size + 1;
    arena->ref_count += 1;
  } else {
    uint32_t arena_cap = arena->cap;
    if (arena->cap < norm_size + 1) {
      arena_cap = norm_size + 1;
    }
    // NOTE: find last arena page that fits the data
    Arena *a = NULL;
    for (;; arena = arena->next) {
      if (arena->next == NULL) {
        arena->next = arena_create(arena_cap);
        a = arena->next;
        break;
      }
      if ((arena->offset + norm_size + 1) <= arena->cap) {
        a = arena;
        break;
      }
    }
    ret = arena_alloc(a, size);
  }
  return ret;
}

uintptr_t *arena_free(Arena *arena, void *data) {
  if (data == NULL) {
    return NULL;
  }

  // NOTE: find correct arena page
  uint32_t offs_in_arena;
  for (;
       arena != NULL && (offs_in_arena = arena_data_offset(arena, data)) > 0 &&
       offs_in_arena >= arena->cap;
       arena = arena->next) {
    ;
  }

  if (arena == NULL) {
    return NULL;
  }

  uintptr_t norm_size = arena_norm(*(uint32_t *)(data - sizeof(uintptr_t)));

  for (int i = offs_in_arena - 1; i < norm_size + 1; i += 1) {
    arena->data[i] = 0;
  }
  arena->ref_count -= 1;
  if (arena->ref_count == 0) {
    arena->offset = 0;
  }
  return NULL;
}

uint32_t arena_data_size(void *data) {
  if (data == NULL) {
    return 0;
  }
  return *(uint32_t *)(data - sizeof(uintptr_t));
}

#endif // ARENA // IMPLEMENTATION

// }