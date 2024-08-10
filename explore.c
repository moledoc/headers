#include <stdio.h>
#include <string.h>

#ifndef MEMREG
#define MEMREG
#endif // MEMREG

#include "memreg.h"

typedef struct {
  int k;
  int v;
} kv;

char *kv_fmt = "HE3RE -- {%c:%d}\n";

int main() {
  MEMREG_CAPACITY = 10;

  MemReg *region = memreg_create();

  char *tst_str = "test";
  char *tst = memreg_alloc(region, 1 * sizeof(char *));
  memcpy(tst, tst_str, str_len(tst_str));

  int *tst2 = memreg_alloc(region, 1 * sizeof(int *));
  *tst2 = 'A';

  kv *tst3 = memreg_alloc(region, 1 * sizeof(kv *));
  tst3->k = 98;
  tst3->v = 99;

  memreg_print(4, "HE1RE -- %s\n", tst);
  memreg_print(1, "HE2RE -- %s\n", tst2);
  memreg_print(1 * sizeof(kv), kv_fmt, tst3->k, tst3->v);

  memreg_dump(region);

  memreg_clear(region, tst);
  memreg_clear(region, tst2);
  memreg_clear(region, tst3);

  tst3 = memreg_alloc(region, 1 * sizeof(kv *));
  tst3->k = 98;
  tst3->v = 99;
  memreg_print(1 * sizeof(kv), kv_fmt, tst3->k, tst3->v);

  memreg_dump(region);
  region = memreg_delete(region);
  memreg_dump(region);

  return 0;
}
