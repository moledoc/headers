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

typedef struct {
  int k1;
  int v1;
  int k2;
  int v2;
  int k3;
  int v3;
  int k4;
  int v4;
} kv2;

char *kv_fmt = "HE3RE -- {%c:%d}\n";
char *kv2_fmt = "HE4RE -- {%c:%d, %c:%d, %c:%d, %c:%d}\n";

int main() {
  MEMREG_CAPACITY = 10;

  MemReg *region = memreg_create();

  char *tst_str = "test";
  char *tst = memreg_alloc(region, strlen(tst_str) * sizeof(char ));
  memcpy(tst, tst_str, str_len(tst_str));

  int *tst2 = memreg_alloc(region, 1 * sizeof(int ));
  *tst2 = 'A';

  kv *tst3 = memreg_alloc(region, 1 * sizeof(kv ));
  tst3->k = 98;
  tst3->v = 99;

  kv2 *tst4 = memreg_alloc(region, 1 * sizeof(kv2 ));
  tst4->k1 = 98;
  tst4->v1 = 99;
  // tst4->k2 = 99;
  // tst4->v2 = 98;
  // tst4->k3 = 99;
  // tst4->v3 = 98;
  tst4->k4 = 99;
  tst4->v4 = 98;

  memreg_print(4, "HE1RE -- %s\n", tst);
  memreg_print(1, "HE2RE -- %s\n", tst2);
  memreg_print(1 * sizeof(kv), kv_fmt, tst3->k, tst3->v);
  memreg_print(1 * sizeof(kv2), kv2_fmt, tst4->k1, tst4->v1, tst4->k2,
               tst4->v2, tst4->k3, tst4->v3, tst4->k4, tst4->v4);

  memreg_dump(region);

  memreg_clear(region, tst);
  memreg_clear(region, tst2);
  memreg_clear(region, tst3);
  memreg_clear(region, tst4);

  tst3 = memreg_alloc(region, 1 * sizeof(kv *));
  tst3->k = 97;
  tst3->v = 97;
  memreg_print(1 * sizeof(kv), kv_fmt, tst3->k, tst3->v);

  memreg_dump(region);
  region = memreg_delete(region);
  memreg_dump(region);

  return 0;
}
