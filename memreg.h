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
  MEMREG_STREAM_STDOUT = 1,
  MEMREG_STREAM_STDERR = 2,
  MEMREG_STREAM_DEFAULT = MEMREG_STREAM_STDERR,
};

uint32_t MEMREG_CAPACITY = MEMREG_CAPACITY_DEFAULT;
uint32_t MEMREG_STREAM = MEMREG_STREAM_DEFAULT;

// memreg_create requests memory and returns a memory region
// that can be used by the program
// allocs memory (techincally maps)
MemReg *memreg_create();

// memreg_delete releases requested memory
// frees memory (technically unmaps)
void *memreg_delete(MemReg *region);

// memreg_zero zeros the given region
void memreg_zero(MemReg *region);

// memreg_zero_data zeros `data` in `region`
void *memreg_zero_data(MemReg *region, void *data);

// memreg_norm converts data size to uintptr_t size
uintptr_t memreg_norm(uint32_t data_size);

// memreg_data_size returns size of allocated data
uint32_t memreg_data_size(void *data);

// memreg_alloced_size returns size allocated in the region
uintptr_t memreg_alloced_size(void *data);

// memreg_alloc allocates memory from region
// doesn't allocate additional memory,
// unless current region doesn't have enough to store data
void *memreg_alloc(MemReg *region, uint32_t data_size);

// memreg_clear decrements the ref count and if it reaches zero, clears the
// memory region, so it could be reused.
void memreg_clear(MemReg *region, void *data, uint32_t data_size);

// memreg_print prints data with format (fmt) to MEMREG_STREAM
void memreg_print(void *data, char *fmt, int argscount, ...);

// memreg_dump dumps each region's metadata and data to MEMREG_STREAM
// data is represented as ptr addresses
void memreg_dump(MemReg *region);

#endif // MEMREG // HEADER

// }

// {
#ifdef MEMREG // IMPLEMENTATION

#include <stdarg.h>
#include <sys/mman.h>
#include <unistd.h>
// #include <pthread.h> // TODO: add concurrency safety

void memreg_zero(MemReg *region) {
  for (int i = 0; i < region->capacity; ++i) {
    region->data[i] = 0;
  }
}

void *memreg_zero_data(MemReg *region, void *data) {
  if (data == NULL) {
    return NULL;
  }
  uint32_t size = memreg_data_size(data);
  uintptr_t size_norm = memreg_alloced_size(data);
  uintptr_t offs = (uintptr_t *)data - region->data;

  for (uint32_t i = offs; i < offs + size_norm; ++i) {
    region->data[i] = 0;
  }
  region->data[offs - 1] = 0;

  return NULL;
}

MemReg *memreg_create() {
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_ANONYMOUS | MAP_PRIVATE;
  int fd = -1;
  off_t offset = 0;
  size_t length = sizeof(MemReg) + sizeof(uintptr_t) * MEMREG_CAPACITY;
  MemReg *region = mmap(NULL, length, prot, flags, fd, offset);
  if (region == MAP_FAILED) {
    return NULL;
  }
  memreg_zero(region);
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

uintptr_t memreg_norm(uint32_t data_size) {
  return (data_size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
}

uint32_t memreg_data_size(void *data) {
  if (data == NULL) {
    return 0;
  }
  return *(uint32_t *)(data - sizeof(uintptr_t));
}

uintptr_t memreg_alloced_size(void *data) {
  if (data == NULL) {
    return 0;
  }
  uint32_t data_size = memreg_data_size(data);
  return (uintptr_t)memreg_norm(data_size);
}

void *memreg_alloc(MemReg *region, uint32_t data_size) {
  if (region == NULL) {
    return NULL;
  }
  uintptr_t data_size_norm = memreg_norm(data_size);

  if (region->offset + data_size_norm + 1 <=
      region->capacity) { // NOTE: +1 for data size
    uintptr_t *me = region->data + region->offset;
    region->offset += data_size_norm + 1;
    region->ref_count += 1;
    *me = data_size;
    ++me;
    return me;
  }

  if (region->next == NULL) {
    uint32_t bu = MEMREG_CAPACITY;
    if (data_size > MEMREG_CAPACITY) {
      MEMREG_CAPACITY = data_size + 1; // NOTE: +1 for data size
    }
    MemReg *new = memreg_create();
    region->next = new;
    MEMREG_CAPACITY = bu;
  }

  return memreg_alloc(region->next, data_size);
}

void memreg_clear(MemReg *region, void *data, uint32_t data_size) {

  int offs_in_reg;
  for (; region != NULL &&
         (offs_in_reg =
              ((uintptr_t *)data - region->data) / sizeof(uintptr_t)) > 0 &&
         offs_in_reg >= region->capacity;
       region = region->next) {
    ;
  }

  if (region == NULL) {
    return;
  }

  region->ref_count -= 1;
  memreg_zero_data(region, region->data + offs_in_reg);

  if (region->ref_count == 0) {
    memreg_zero(region);
    region->offset = 0;
  }

  return;
}

void str_zero(char *dest, size_t size) {
  if (dest == NULL) {
    return;
  }
  for (int i = 0; i < size; ++i) {
    dest[i] = '\0';
  }
}

void memreg_dump(MemReg *region) {
  if (region == NULL) {
    return;
  }

  char data_buf[(sizeof(unsigned long) + sizeof(char)) * region->capacity + 1];
  str_zero(data_buf, sizeof(data_buf));

  int offset = 0;
  for (int i = 0; i < region->capacity; ++i) {
    offset +=
        snprintf(data_buf + offset, 32 + 1, "%p ", (void *)*(region->data + i));
  }

  char *fmt = "----------------\n"
              "memory dump:\n"
              "\tcurrent: %p\n"
              "\tnext: %p\n"
              "\tcapacity: %u\n"
              "\toffset: %u\n"
              "\tdata: %s\n"
              "----------------\n";
  size_t fmt_size = strlen(fmt);
  char buf[fmt_size + sizeof(uintptr_t) + 2 * sizeof(unsigned int) +
           (sizeof(data_buf) - 1) + 1];
  str_zero(buf, sizeof(buf));
  snprintf(buf, sizeof(buf) - 1, fmt, region, region->next, region->capacity,
           region->offset, data_buf);

  write(MEMREG_STREAM, buf, sizeof(buf));

  return memreg_dump(region->next);
}

size_t str_len(const char *s1) {
  if (s1 == NULL) {
    return 0;
  }
  size_t len = 0;
  for (; (*s1++) != '\0';) {
    ++len;
  }
  return len;
}

void memreg_print(void *data, char *fmt, int argscount, ...) {
  if (data == NULL) {
    printf("%p\n", NULL);
    return;
  }
  va_list args;
  va_start(args, argscount);
  uint32_t data_size = *(uint32_t *)(data - sizeof(uintptr_t));
  char buf[str_len(fmt) + data_size + 1];
  str_zero(buf, sizeof(buf));
  if (argscount == 0) {
    snprintf(buf, sizeof(buf) - 1, fmt, data);
  } else {
    vsnprintf(buf, sizeof(buf) - 1, fmt, args);
  }
  va_end(args);
  write(MEMREG_STREAM, buf, sizeof(buf));
  return;
}

#endif // MEMREG // IMPLEMENTATION

// }