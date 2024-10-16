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
    printf("\"%s\": \"%s\"\n", (char *)kv->key, (char *)kv->value);
  }
}

int main() {
  Map *map = map_create(NULL, NULL);
  printf("hey\n");

  printf("Map contents: (should be nothing)\n");
  map_list(map, str_key, NULL);

  char *k1 = "key1";
  map = map_insert(map, k1, "maybe works");
  printf("Map contents:\n");
  map_list(map, str_key, NULL);

  char *k2 = "key2";
  map = map_insert(map, k2, "another one");
  printf("Map contents:\n");
  map_list(map, str_key, NULL);

  char *k2_val = (char *)map_find(map, k2);
  printf("searched \"%s\", found \"%s\"\n", k2, k2_val);

  char *k3 = "key3";
  map = map_insert(map, k3, "another two");
  printf("Map contents:\n");
  map_list(map, str_key, NULL);

  map = map_delete(map, k2);
  printf("deleted key \"%s\"\n", k2);
  printf("Map contents:\n");
  map_list(map, str_key, NULL);

  int counter = 0;
  // map_list(map, str_key, (void *)&counter);

  char *k1_1 = "k1_1";
  map = map_insert(map, k1_1, "k1_1");
  char *k1_2 = "k1_2";
  map = map_insert(map, k1_2, "k1_2");
  char *k1_3 = "k1_3";
  map = map_insert(map, k1_3, "k1_3");
  char *k1_4 = "k1_4";
  map = map_insert(map, k1_4, "k1_4");

  printf("\n\nbefore finding:\n");
  map_list(map, str_key, NULL);

  char *f1 = (char *)map_find(map, k1_1);
  char *f2 = (char *)map_find(map, k1_2);
  printf("searched \"%s\", found \"%s\"\n", k1_1, f1);
  printf("searched \"%s\", found \"%s\"\n", k1_2, f2);

  char *f3 = (char *)map_find(map, k1);
  char *f4 = (char *)map_find(map, k3);
  printf("searched \"%s\", found \"%s\"\n", k1, f3);
  printf("searched \"%s\", found \"%s\"\n", k3, f4);

  char *k1_5 = "k1_5 555555555555555555555555555555555555555555555555555555";
  map = map_insert(map, k1_5, "k1_5");
  char *f5 = (char *)map_find(map, k1_5);
  printf("searched \"%s\", found \"%s\"\n", k1_5, f5);
  char *k1_6 = "k1_5 555555555555555555555555555555555555555555555555555555";
  map = map_insert(map, k1_6, "k1_6");
  char *f6 = (char *)map_find(map, k1_6);
  printf("searched \"%s\" with 1, found \"%s\"\n", k1_6, f6);

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
    printf("\"%d\": \"%d\"\n", *(int *)kv->key, *(int *)kv->value);
  }
}

int main() {
  Map *map = map_create(NULL, NULL);

  for (int i = 10000; i < 11000; i += 1) {
    int *j = (int *)arena_alloc(map->arena, 1 * sizeof(int));
    *j = i;
    map_insert(map, (void *)j, (void *)j);
  }
  int ff = 10459;
  int *ffound = (int *)map_find(map, (void *)&ff);
  if (ffound == NULL) {
    printf("ffound: not found\n");
  } else {
    printf("ffound this: %d\n", *ffound);
  }

  for (int i = 0; i < 100; i += 1) {
    int *j = (int *)arena_alloc(map->arena, 1 * sizeof(int));
    *j = i;
    map_insert(map, (void *)j, (void *)j);
  }
  // map_list(map, int_key, NULL);

  int *f = (int *)arena_alloc(map->arena, 1 * sizeof(int));
  *f = 77;
  int *found = (int *)map_find(map, (void *)f);

  if (found == NULL) {
    printf("found: not found\n");
  } else {
    printf("found this: %d\n", *found);
  }

  int f2 = *f;
  int *found2 = (int *)map_find(map, (void *)&f2);

  if (found2 == NULL) {
    printf("found2: not found\n");
  } else {
    printf("found2 this: %d\n", *found2);
  }

  int *f3 = (int *)arena_alloc(map->arena, 1 * sizeof(int));
  *f3 = 0;
  int *found3 = (int *)map_find(map, (void *)&f3);
  if (found3 == NULL) {
    printf("found3: not found\n");
  } else {
    printf("found3 this: %d\n", *found3);
  }

  map_free(map);
}
#endif // INT

#ifdef NUL_VAL
void int_key(MapKeyValue *kv, void *i) {
  if (i != NULL) {
    printf("i: %d\n", *(int *)i);
    *(int *)i += 1;
  }
  if (kv == NULL) {
    printf("(nil)\n");
  } else {
    printf("\"%d\": \"%p\"\n", *(int *)kv->key, kv->value);
  }
}

int main() {
  Map *map = map_create(NULL, NULL);

  for (int i = 0; i < 100; i += 1) {
    int *j = (int *)arena_alloc(map->arena, 1 * sizeof(int));
    *j = i;
    map_insert(map, (void *)j, NULL);
  }
  // map_list(map, int_key, NULL);

  int *f1 = (int *)arena_alloc(map->arena, 1 * sizeof(int));
  *f1 = 77; // vs `f = 177`
  int *found1 = (int *)map_find(map, (void *)f1);
  printf("found1 this: %p (should exist)\n", found1);

  int f2 = 77;
  int *found2 = (int *)map_find(map, (void *)&f2);
  printf("found2 this: %p (should exist)\n", found2);

  int f3 = 177;
  int *found3 = (int *)map_find(map, (void *)&f3);
  printf("found3 this: %p (shouldn't exist)\n", found3);

  map_free(map);
}
#endif // NUL_VAL

#ifdef KVS

typedef struct {
  int k;
  int v;
  char *str;
  unsigned long long big;
} KV;

void kv_key(MapKeyValue *kv, void *i) {
  if (i != NULL) {
    printf("i: %d\n", *(int *)i);
    *(int *)i += 1;
  }
  if (kv == NULL) {
    printf("(nil)\n");
  } else {
    printf("{\"%d\", \"%d\", \"%s\"}: {\"%d\", \"%d\", \"%s\", %lu}\n",
           ((KV *)kv->value)->k, ((KV *)kv->value)->v, ((KV *)kv->value)->str,
           ((KV *)kv->value)->k, ((KV *)kv->value)->v, ((KV *)kv->value)->str,
           ((KV *)kv->value)->big);
  }
}

void k77(MapKeyValue *kv, void *i) {
  if (i != NULL) {
    printf("i: %d\n", *(int *)i);
    *(int *)i += 1;
  }
  if (kv == NULL) {
    printf("(nil)\n");
  } else {
    if (*(int *)kv->key != 77) {
      return;
    }
    printf("{\"%d\", \"%d\", \"%s\"}: {\"%d\", \"%d\", \"%s\", %lu}\n",
           ((KV *)kv->value)->k, ((KV *)kv->value)->v, ((KV *)kv->value)->str,
           ((KV *)kv->value)->k, ((KV *)kv->value)->v, ((KV *)kv->value)->str,
           ((KV *)kv->value)->big);
  }
}

int main() {
  Map *map = map_create(NULL, NULL);

  for (int i = 0; i < 100; i += 1) {
    KV *j = (KV *)arena_alloc(map->arena, 1 * sizeof(KV));
    j->k = i;
    j->v = -1 * i;
    map_insert(map, (void *)j, (void *)j);
  }
  map_list(map, kv_key, NULL);

  KV f = (KV){
      .k = 77,
      .v = -77,
  };
  KV *found = (KV *)map_find(map, (void *)&f);
  if (found == NULL) {
    printf("not found\n");
  } else {
    printf("found this: {\"%d\", \"%d\", \"%s\"}\n", found->k, found->v,
           found->str);
  }

  KV f2 = (KV){
      .k = 77, // NOTE: mismatching data
      .v = -99,
  };
  KV *found2 = (KV *)map_find(map, (void *)&f2);
  if (found2 == NULL) {
    printf("found2: not found\n");
  } else {
    printf("found2 this: {\"%d\", \"%d\", \"%s\"}\n", found2->k, found2->v,
           found2->str);
  }

  for (int i = 100; i < 110; i += 1) {
    KV *j = (KV *)arena_alloc(map->arena, 1 * sizeof(KV));
    j->k = i;
    j->v = -1 * i;
    j->str = "test";
    map_insert(map, (void *)j, (void *)j);
  }
  // map_list(map, kv_key, NULL);

  KV f3 = (KV){
      .k = 100,
      .v = -100,
      .str = "test",
  };
  KV *found3 = (KV *)map_find(map, (void *)&f3);
  if (found3 == NULL) {
    printf("found3: not found\n");
  } else {
    printf("found3 this: {\"%d\", \"%d\", \"%s\"}\n", found3->k, found3->v,
           found3->str);
  }

  int i = 236;
  KV *j = (KV *)arena_alloc(map->arena, 1 * sizeof(KV));
  j->k = 77;
  j->v = 0;
  j->str = "test2";
  j->big = 1234567890;
  map_insert(map, (void *)j, (void *)j);
  map_list(map, k77, NULL);

  KV *f4 = (KV *)arena_alloc(map->arena, 1 * sizeof(KV));

  f4->k = 77;
  f4->v = 0;
  f4->str = "test2";
  f4->big = 1234567890;

  KV *found4 = (KV *)map_find(map, (void *)f4);
  if (found4 == NULL) {
    printf("found4: not found\n");
  } else {
    printf("found4 this: {\"%d\", \"%d\", \"%s\", %lu}\n", found4->k, found4->v,
           found4->str, found4->big);
  }

  f4->big = 1234567891;
  found4 = (KV *)map_find(map, (void *)f4);
  if (found4 == NULL) {
    printf("found4: not found\n");
  } else {
    printf("found4 this: {\"%d\", \"%d\", \"%s\", %lu}\n", found4->k, found4->v,
           found4->str, found4->big);
  }

  KV *search6 = &(KV){.k = 50, .v = -50};
  KV *found6 = (KV *)map_find(map, (void *)search6);
  if (found6 == NULL) {
    printf("found6: not found\n");
  } else {
    printf("found6 this: {\"%d\", \"%d\", \"%s\", %lu}, ", found6->k, found6->v,
           found6->str, found6->big);
    printf("searched with key: {\"%d\", \"%d\", \"%s\", %lu}\n", search6->k,
           search6->v, search6->str, search6->big);
  }

  KV *search7 = &(KV){.k = 50, .v = 1000};
  KV *found7 = (KV *)map_find(map, (void *)search7);
  if (found7 == NULL) {
    printf("found7: not found\n");
  } else {
    printf("found7 this: {\"%d\", \"%d\", \"%s\", %lu}, ", found7->k, found7->v,
           found7->str, found7->big);
    printf("searched with key: {\"%d\", \"%d\", \"%s\", %lu}\n", search7->k,
           search7->v, search7->str, search7->big);
  }

  map_free(map);
}
#endif // KVS