#include <assert.h>
#include <stdio.h>
#include <string.h>

#define UTILS
#include "utils.h"

// {
#ifdef SINGLY_LINKED_LIST

#include <string.h>

#include "list.h"

void sll_print_node_int(SLLNode *node) {
  printf("(%p) data:%d\n", node, *(int *)(node->data));
  ; // force uncompressed formatting (ccls)
}

void sll_print_node_str(SLLNode *node) {
  printf("(%p) data:'%s'\n", node, (char *)(node->data));
  ; // force uncompressed formatting (ccls)
}

bool sll_cmp_int(void *a, void *b) { return *(int *)a == *(int *)b; }

bool sll_cmp_str(void *a, void *b) { return strcmp((char *)a, (char *)b) == 0; }

void sll_free_data(void *data) { ; }

SLLNode *sll_assert_next_int(SLLNode *cursor, int d) {
  if (!cursor) {
    return NULL;
  }
  if (*(int *)cursor->data != d) {
    printf("[ERROR]: int data doesn't match: expected '%d', got '%d'\n", d,
           *(int *)cursor->data);
    assert(*(int *)cursor->data == d);
  }
  return cursor->next;
}

void sll_assert_each_int(SLLNode *cursor, int datas[]) {
  int i = 0;
  for (SLLNode *cur = cursor; cur;
       cur = sll_assert_next_int(cur, datas[i]), ++i) {
    ;
  }
  return;
}

SLLNode *sll_assert_next_str(SLLNode *cursor, char *d) {
  if (!cursor) {
    return NULL;
  }
  if (!sll_cmp_str(cursor->data, d)) {
    printf("[ERROR]: str data doesn't match: expected '%s', got '%s'\n", d,
           (char *)cursor->data);
    assert(sll_cmp_str(cursor->data, d));
  }
  return cursor->next;
}

void sll_assert_each_str(SLLNode *cursor, char **datas) {
  int i = 0;
  for (SLLNode *cur = cursor; cur;
       cur = sll_assert_next_str(cur, datas[i]), ++i) {
    ;
  }
  return;
}

void ds_singly_linked_list(int argc, char **argv) {
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  int e = 5;
  int f = 6;
  int g = 7;

  char *s1 = "test";
  char *s1_1 = "test";
  char *s2 = "test2";

  char *run = "";
  for (; argc > 0;) {
    char *arg = shift(&argc, &argv);
    if (strcmp("--run", arg) == 0 && argc > 0) {
      run = shift(&argc, &argv);
    }
  }

  {
    char *cse = "sll_create_validation";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      SLLNode *ll = NULL;
      ll = sll_create(NULL, sll_print_node_int, sll_free_data, (void *)&a);
      assert(!ll);
      ll = sll_create(sll_cmp_int, NULL, sll_free_data, (void *)&a);
      assert(!ll);
      ll = sll_create(sll_cmp_int, sll_print_node_int, NULL, (void *)&a);
      assert(!ll);
      ll = sll_create(sll_cmp_int, sll_print_node_int, sll_free_data, NULL);
      assert(!ll);
      sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_create";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      SLLNode *ll = sll_create(sll_cmp_int, sll_print_node_int, sll_free_data,
                               (void *)&a);
      sll_list(ll);
      sll_assert_next_int(ll, a);
      sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_append_validate";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      SLLNode *ll = NULL;
      ll = sll_append(NULL, (void *)&a);
      assert(!ll && "should be NULL");
      sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_append";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      SLLNode *ll = NULL;

      ll = sll_create(sll_cmp_int, sll_print_node_int, sll_free_data,
                      (void *)&a);
      printf("head: %p\n", ll);
      sll_print_node_int(ll);
      sll_list(ll);
      ll = sll_append(ll, (void *)(&b));
      sll_list(ll);
      ll = sll_append(ll, (void *)(&c));
      ll = sll_append(ll, (void *)(&d));
      ll = sll_append(ll, (void *)(&e));
      sll_list(ll);

      assert(sll_list_len(ll) == 5 && "unexpected list length");
      sll_assert_each_int(ll, (int[]){a, b, c, d, e});
      sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_find_validate";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      SLLNode *n = sll_find(NULL, (void *)(&c));
      assert(!n && "expected NULL");

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_find";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      SLLNode *ll = sll_create(sll_cmp_int, sll_print_node_int, sll_free_data,
                               (void *)&a);
      ll = sll_append(ll, (void *)(&b));
      ll = sll_append(ll, (void *)(&c));
      ll = sll_append(ll, (void *)(&d));
      SLLNode *n = sll_find(ll, (void *)(&c));
      printf("found node: ");
      sll_print_node_int(n);
      sll_assert_next_int(n, c);
      sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_delete_validation";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      SLLNode *ll = sll_delete(NULL, (void *)(&a));
      assert(!ll && "expected NULL");

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_delete";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      SLLNode *ll = sll_create(sll_cmp_int, sll_print_node_int, sll_free_data,
                               (void *)&a);
      ll = sll_append(ll, (void *)(&a));
      ll = sll_append(ll, (void *)(&b));
      ll = sll_append(ll, (void *)(&c));

      printf("deleting %d\n", b);
      ll = sll_delete(ll, (void *)(&b));
      sll_list(ll);
      assert(sll_list_len(ll) == 3 && "list length unexpected");
      sll_assert_each_int(ll, (int[]){a, a, c});

      printf("deleting %d\n", a);
      ll = sll_delete(ll, (void *)(&a));
      sll_list(ll);
      assert(sll_list_len(ll) == 2 && "list length unexpected");
      sll_assert_each_int(ll, (int[]){a, c});
      sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_update_validation";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      SLLNode *ll = sll_update(NULL, (void *)(&a), (void *)(&b));
      assert(!ll && "expected NULL");

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_update";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      SLLNode *ll = sll_create(sll_cmp_int, sll_print_node_int, sll_free_data,
                               (void *)&a);

      ll = sll_append(ll, (void *)(&a));
      ll = sll_append(ll, (void *)(&b));
      ll = sll_append(ll, (void *)(&c));
      ll = sll_append(ll, (void *)(&d));
      printf("updating %d\n", a);

      ll = sll_update(ll, (void *)(&a), (void *)(&e));
      sll_list(ll);
      assert(sll_list_len(ll) == 5 && "list length unexpected");
      sll_assert_each_int(ll, (int[]){e, a, b, c, d});

      ll = sll_update(ll, (void *)(&c), (void *)(&f));
      sll_list(ll);
      assert(sll_list_len(ll) == 5 && "list length unexpected");
      sll_assert_each_int(ll, (int[]){e, a, b, f, d});

      sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_string_data";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      SLLNode *ll = sll_create(sll_cmp_str, sll_print_node_str, sll_free_data,
                               (void *)s1);
      printf("head: %p\n", ll);
      sll_print_node_str(ll);

      ll = sll_append(ll, (void *)s1_1);
      ll = sll_append(ll, (void *)s2);
      sll_list(ll);

      assert(sll_list_len(ll) == 3 && "list length unexpected");
      sll_assert_each_str(ll, (char *[]){s1, s1_1, s2});

      sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_find_diff_ptr";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      SLLNode *ll = sll_create(sll_cmp_int, sll_print_node_int, sll_free_data,
                               (void *)&a);

      int aa = a;
      sll_list(ll);

      SLLNode *n = sll_find(ll, (void *)(&aa));
      assert(n && "unexpected NULL");
      printf("found node: ");
      sll_print_node_int(n);
      sll_assert_next_int(n, a);

      sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  return;
}

#endif // SINGLY_LINKED_LIST
// }

// {
#ifdef MAP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

void map_print_value_int(void *value) { printf("%d", *(int *)value); }

void map_print_value_str(void *value) { printf("'%s'", (char *)value); }

void map_free_data(void *value) { ; }

int map_count_kvs(Map *map) {
  int count = 0;
  for (int i = 0; i < map->cap; ++i) {
    if (!map->kvs[i]) {
      continue;
    }
    for (MapKeyValue *cur = map->kvs[i]; cur; cur = cur->next) {
      ++count;
    }
  }
  return count;
}

void map_assert_each_value_str(Map *map, MapKeyValue expected[],
                               size_t expected_len) {
  MapKeyValue actual[map->len];
  int counter = 0;
  for (int i = 0; i < map->cap; ++i) {
    if (!map->kvs[i]) {
      continue;
    }
    for (MapKeyValue *cur = map->kvs[i]; cur; cur = cur->next) {
      actual[counter].key = cur->key;
      actual[counter].value = cur->value;
      ++counter;
    }
  }
  assert(map->len == counter && counter == expected_len && "unexpected len");

  for (int i = 0; i < counter; ++i) {
    int found = 0;
    for (int j = 0; j < counter; ++j) {
      if (strcmp((char *)expected[i].key, (char *)actual[j].key) == 0 &&
          strcmp((char *)expected[i].value, (char *)actual[j].value) == 0) {
        found = 1;
        break;
      }
    }
    if (!found) {
      printf("key-value '%s:%s' not found\n", (char *)expected[i].key,
             (char *)expected[i].value);
    }
    assert(found && "expected value not found");
  }
  return;
}

void ds_map(int argc, char **argv) {

  char *run = "";
  for (; argc > 0;) {
    char *arg = shift(&argc, &argv);
    if (strcmp("--run", arg) == 0 && argc > 0) {
      run = shift(&argc, &argv);
    }
  }

  {
    char *cse = "map_create";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      Map *map = NULL;
      map = map_create(NULL, MapKeyInt, map_print_value_int, map_free_data);
      assert(!map);
      map = map_create(map_hash, -1, map_print_value_int, map_free_data);
      map =
          map_create(map_hash, MapKeyCount, map_print_value_int, map_free_data);
      map = map_create(map_hash, MapKeyCount + 1, map_print_value_int,
                       map_free_data);
      assert(!map);
      map = map_create(map_hash, MapKeyInt, NULL, map_free_data);
      assert(!map);
      map = map_create(map_hash, MapKeyInt, map_print_value_int, NULL);
      assert(!map);
      map = map_create(map_hash, MapKeyInt, map_print_value_int, map_free_data);
      assert(map);

      map_free(map);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "map_insert";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      Map *map =
          map_create(map_hash, MapKeyStr, map_print_value_str, map_free_data);
      assert(map && "unexpected NULL");

      MapKeyValue kvs[] = (MapKeyValue[]){
          (MapKeyValue){.key = (void *)"hello", .value = (void *)"world"},
          (MapKeyValue){.key = (void *)"helloh", .value = (void *)"holleh"},
          (MapKeyValue){.key = (void *)"earth", .value = (void *)"olleh"},
      };

      int kvs_len = sizeof(kvs) / sizeof(MapKeyValue);

      for (int i = 0; i < kvs_len; ++i) {
        map_insert(map, kvs[i].key, kvs[i].value);
        assert(map && "unexpected NULL");

        printf("idx for '%s:%s': %d\n", (char *)kvs[i].key,
               (char *)kvs[i].value,
               map_hash(MapKeyStr, (char *)kvs[i].key, map->cap));
      }

      map_list(map);

      assert(map->len == kvs_len && map->len == map_count_kvs(map) &&
             "unexpected element count");
      map_assert_each_value_str(map, kvs, kvs_len);

      map_free(map);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "map_update";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      Map *map =
          map_create(map_hash, MapKeyStr, map_print_value_int, map_free_data);
      assert(map && "unexpected NULL");

      int a = 1;
      int b = 2;
      int c = 3;
      int d = 4;
      int e = 5;

      MapKeyValue kvs[] = (MapKeyValue[]){
          (MapKeyValue){.key = (void *)"hello", .value = (void *)&a},
          (MapKeyValue){.key = (void *)"helloh", .value = (void *)&b},
          (MapKeyValue){.key = (void *)"earth", .value = (void *)&c},
      };

      int kvs_len = sizeof(kvs) / sizeof(MapKeyValue);

      for (int i = 0; i < kvs_len; ++i) {
        map_insert(map, kvs[i].key, kvs[i].value);
        assert(map && "unexpected NULL");

        printf("idx for '%s:%d': %d\n", (char *)kvs[i].key,
               *(int *)kvs[i].value,
               map_hash(MapKeyStr, (char *)kvs[i].key, map->cap));
      }

      map_list(map);

      assert(map->len == kvs_len && map->len == map_count_kvs(map) &&
             "unexpected element count");
      map_assert_each_value_str(map, kvs, kvs_len);

      int swapping_vals[3] = (int[]){e, d, a};
      assert(3 == kvs_len && "unexpected length");
      printf("-- swapping values\n");
      for (int i = 0; i < kvs_len; ++i) {
        kvs[i].value = (void *)(&swapping_vals[i]);
        map = map_insert(map, kvs[i].key, kvs[i].value);
        assert(map && "unexpected NULL");
        map_list(map);

        assert(map->len == kvs_len && map->len == map_count_kvs(map) &&
               "unexpected element count");
        map_assert_each_value_str(map, kvs, kvs_len);
      }

      map_free(map);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "map_find";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      int a = 1;
      int b = 2;
      int c = 3;
      int d = 4;
      int e = 5;

      Map *map =
          map_create(map_hash, MapKeyStr, map_print_value_int, map_free_data);
      assert(map && "unexpected NULL");

      MapKeyValue kvs[] = (MapKeyValue[]){
          (MapKeyValue){.key = (void *)"apple", .value = (void *)&a},
          (MapKeyValue){.key = (void *)"banana", .value = (void *)&b},
          (MapKeyValue){.key = (void *)"pear", .value = (void *)&c},
          (MapKeyValue){.key = (void *)"nectarine", .value = (void *)&d},
          (MapKeyValue){.key = (void *)"blue berry", .value = (void *)&e},
      };

      int kvs_len = sizeof(kvs) / sizeof(MapKeyValue);

      for (int i = 0; i < kvs_len; ++i) {
        map = map_insert(map, kvs[i].key, kvs[i].value);
        assert(map && "unexpected NULL");

        printf("idx for '%s:%d': %d\n", (char *)kvs[i].key,
               *(int *)kvs[i].value,
               map_hash(MapKeyStr, (char *)kvs[i].key, map->cap));
      }

      map_list(map);
      assert(map->len == kvs_len && map->len == map_count_kvs(map) &&
             "unexpected element count");
      map_assert_each_value_str(map, kvs, kvs_len);

      for (int i = kvs_len - 1; i >= 0; --i) {
        void *tmp = map_find(map, kvs[i].key);
        assert(tmp && "unexpected NULL");
        int fnd = *(int *)tmp;

        assert(*(int *)kvs[i].value == fnd && "value mismatch");
      }

      map_free(map);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "map_delete";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      int a = 1;
      int b = 2;
      int c = 3;
      int d = 4;
      int e = 5;
      int f = 6;

      Map *map =
          map_create(map_hash, MapKeyStr, map_print_value_int, map_free_data);
      assert(map && "unexpected NULL");

      MapKeyValue kvs[] = (MapKeyValue[]){
          (MapKeyValue){.key = (void *)"apple", .value = (void *)&a},
          (MapKeyValue){.key = (void *)"banana", .value = (void *)&b},
          (MapKeyValue){.key = (void *)"pear", .value = (void *)&c},
          (MapKeyValue){.key = (void *)"nectarine", .value = (void *)&d},
          (MapKeyValue){.key = (void *)"blue berry", .value = (void *)&e},
      };

      int kvs_len = sizeof(kvs) / sizeof(MapKeyValue);

      for (int i = 0; i < kvs_len; ++i) {
        map = map_insert(map, kvs[i].key, kvs[i].value);
        assert(map && "unexpected NULL");

        printf("idx for '%s:%d': %d\n", (char *)kvs[i].key,
               *(int *)kvs[i].value,
               map_hash(MapKeyStr, (char *)kvs[i].key, map->cap));
      }

      for (int i = kvs_len - 1; i >= 0; --i) {
        map_delete(map, kvs[i].key);
        --kvs_len;
        map_list(map);
        assert(map->len == kvs_len && map->len == map_count_kvs(map) &&
               "unexpected element count");
        map_assert_each_value_str(map, kvs, kvs_len);
      }

      map_free(map);

      printf("-- %s: ok\n", cse);
    }
  }

  return;
}

#endif // MAP
// }

// {
#if defined(TWO_SUM)

#include "algo_misc.h"

typedef struct {
  int *nums;
  size_t nums_len;
  int target;
  int res0;
  int res1;
} testcase;

void algo_two_sum() {

  size_t tcs_size = 4;
  testcase tcs[tcs_size];

  // case: small array
  tcs[0] = (testcase){.nums = (int[]){2, 11, 15, 1, 7, 4, 6},
                      .nums_len = (size_t)7,
                      .target = (int)9,
                      .res0 = 2,
                      .res1 = 7};

  // case: smallest possible array
  tcs[1] = (testcase){.nums = (int[]){2, 7},
                      .nums_len = (size_t)2,
                      .target = (int)9,
                      .res0 = 2,
                      .res1 = 7};

  // case: big array with pos/neg numbers, values are at beginning and end, so
  // should be the worst case scenario
  int c1_nums[1000];
  int c1_intermediary = 0;
  for (int i = 8; i < 1000 + 8; ++i) {
    c1_nums[i - 8] = (-1 * (i % 2)) * (c1_intermediary + 10 * i);
  }
  c1_nums[0] = -7;
  c1_nums[999] = -2;
  tcs[2] = (testcase){
      .nums = c1_nums, .nums_len = 1000, .target = -9, .res0 = -7, .res1 = -2};

  // case: bigger array with pos/neg numbers, values are at beginning and end,
  // so should be the worst case scenario
  int c2_nums[10000];
  int c2_intermediary = 0;
  for (int i = 8; i < 10000 + 8; ++i) {
    c2_nums[i - 8] = (-1 * (i % 2)) * (c2_intermediary + 10 * i);
  }
  c2_nums[0] = -7;
  c2_nums[9999] = -2;
  tcs[3] = (testcase){
      .nums = c2_nums, .nums_len = 10000, .target = -9, .res0 = -7, .res1 = -2};

  for (int i = 0; i < tcs_size; ++i) {
    int *ids = two_sum(tcs[i].nums, tcs[i].nums_len, tcs[i].target);
    printf("%d: indeces [%d, %d]\n", i, ids[0], ids[1]);
    assert(ids && ids[0] >= 0 && ids[1] >= 0);
    printf("%d: got [%d, %d], expected [%d, %d]\n", i, tcs[i].nums[ids[0]],
           tcs[i].nums[ids[1]], tcs[i].res0, tcs[i].res1);
    assert(tcs[i].nums[ids[0]] == tcs[i].res0 &&
           tcs[i].nums[ids[1]] == tcs[i].res1);
    free(ids);
  }
  return;
}

#endif // defined(TWO_SUM)
// }

// {
#ifdef MD5
#include <stdlib.h>

#include "md5.h"

void algo_md5() {
  unsigned char digest[16];
  char *str[] = {"", "md5", "The quick brown fox jumps over the lazy dog"};

  for (int i = 0; i < 3; ++i) {
    md5(str[i], digest);
    printf("mine: \n");
    md5_print(digest);
    printf("system: \n");
    char cmd[128];
    memset(cmd, '\0', sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "printf '%s' \"%s\" | md5sum", str[i]);
    system(cmd);
  }
}

#endif // MD5
// }

int main(int argc, char **argv) {
  char *prog_name = shift(&argc, &argv);

#ifdef SINGLY_LINKED_LIST
  ds_singly_linked_list(argc, argv);
#endif // SINGLY_LINKED_LIST

#ifdef MAP
  ds_map(argc, argv);
#endif // MAP

#ifdef TWO_SUM
  algo_two_sum();
#endif // TWO_SUM

#ifdef MD5
  algo_md5();
#endif // MD5

  return 0;
}