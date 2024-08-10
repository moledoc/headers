#pragma once

// {
#ifdef MEMREG // HEADER

#include <stdint.h>

typedef struct MemReg {
  struct MemReg *next;
  uint32_t capacity;
  uint32_t offset;
  uint32_t ref_count;
  uintptr_t data[];
} MemReg;

enum {
  MEMREG_CAPACITY_DEFAULT = 1 * 1024,
};

uint32_t MEMREG_CAPACITY = MEMREG_CAPACITY_DEFAULT;

// memreg_zero zeros the region
void memreg_zero_data(MemReg *region);

// maps memory
MemReg *memreg_create();

// unmaps memory
void *memreg_delete(MemReg *region);

// memreg_alloc allocates memory
void *memreg_alloc(MemReg *region, uint32_t data_size);

void memreg_print(uint32_t data_size, char *fmt, ...);

// memreg_dump dumps memory region metadata and data to stderr
void memreg_dump(MemReg *region);

// memreg_clear decrements the ref count and if it reaches zero, clears the
// memory region for reuse
void memreg_clear(MemReg *region, void *data);

#endif // MEMREG // HEADER

// }

// {
#ifdef MEMREG // IMPLEMENTATION

#include <stdarg.h>
#include <sys/mman.h>
#include <unistd.h>
// #include <pthread.h> // TODO: add concurrency safety

void memreg_zero_data(MemReg *region) {
  for (int i = 0; i < region->capacity; ++i) {
    region->data[i] = 0;
  }
}

MemReg *memreg_create() {
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_ANONYMOUS | MAP_PRIVATE;
  int fd = -1;
  off_t offset = 0;
  size_t length = sizeof(MemReg) + sizeof(uintptr_t) * MEMREG_CAPACITY;
  MemReg *region = mmap(NULL, length, prot, flags, fd, offset);
  if (region == MAP_FAILED) {
    // TODO: handle mmap failure
  }
  memreg_zero_data(region);
  region->next = NULL;
  region->capacity = MEMREG_CAPACITY;
  region->offset = 0;
  region->ref_count = 0;
  return region;
}

void *memreg_delete(MemReg *region) {
  for (; region != NULL;) {
    MemReg *me = region;
    region = region->next;
    int ret = munmap(me, sizeof(MemReg) + sizeof(uintptr_t) * me->capacity);
    if (ret != 0) {
      // TODO: handle failure
    }
  }
  return NULL;
}

void *memreg_alloc(MemReg *region, uint32_t data_size) {
  // check if fits in current region
  // if yes, then return
  // if no, then
  // // if not exist, create
  // check next region

  data_size = (data_size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

  if (region->offset + data_size < region->capacity) {
    uintptr_t *me = region->data + region->offset;
    region->offset += 1; // data_size;
    region->ref_count += 1;
    return me;
  }

  if (region->next == NULL) {
    uint32_t bu = MEMREG_CAPACITY;
    if (data_size > MEMREG_CAPACITY) {
      MEMREG_CAPACITY = data_size;
    }
    MemReg *new = memreg_create();
    region->next = new;
    MEMREG_CAPACITY = bu;
  }

  return memreg_alloc(region->next, data_size);
}

void str_zero(char *dest, char val, size_t size) {
  for (int i = 0; i < size; ++i) {
    dest[i] = val;
  }
}

void memreg_dump(MemReg *region) {
  if (region == NULL) {
    return;
  }

  FILE *stream = stdout;
  fputs("----------------\n", stream);

  char buf[65];
  str_zero(buf, '\0', sizeof(buf));
  snprintf(buf, sizeof(buf) - 1,
           "next: %p\ncapacity: %u\noffset: %u\ndata: ", region->next,
           region->capacity, region->offset);
  write(fileno(stream), buf, sizeof(buf));

  for (int i = 0; i < region->capacity; ++i) {
    printf("%lu ", *(region->data + i));
  }
  putchar('\n');

  return memreg_dump(region->next);
}

size_t str_len(const char *s1) {
  size_t len = 0;
  for (; (*s1++) != '\0';) {
    ++len;
  }
  return len;
}

void memreg_print(uint32_t data_size, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buf[str_len(fmt) + data_size + 1];
  str_zero(buf, '\0', sizeof(buf));
  vsnprintf(buf, sizeof(buf) - 1, fmt, args);
  va_end(args);
  write(fileno(stdout), buf, sizeof(buf));
  return;
}

void memreg_clear(MemReg *region, void *data) {
  int tmp;
  for (; (tmp = ((uintptr_t *)data - region->data) / sizeof(uintptr_t)) > 0 &&
         tmp >= region->capacity;
       region = region->next) {
    ;
  }

  region->ref_count -= 1;

  if (region->ref_count == 0) {
    memreg_zero_data(region);
    region->offset = 0;
  }

  return;
}

#endif // MEMREG // IMPLEMENTATION

// }