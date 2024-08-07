#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

// {
#ifdef SINGLY_LINKED_LIST

#include <string.h>

#ifndef UTILS
#define UTILS
#include "utils.h"
#undef UTILS
#endif // UTILS

#include "list.h"

bool sll_cmp_int(void *a, void *b) { return *(int *)a == *(int *)b; }
bool sll_cmp_str(void *a, void *b) { return strcmp((char *)a, (char *)b) == 0; }

void sll_print_node_int(SLLNode *node, void *_) {
  printf("-> (%p) data:%d\n", node, *(int *)(node->data));
  return;
}

void sll_print_node_str(SLLNode *node, void *_) {
  printf("-> (%p) data:'%s'\n", node, (char *)(node->data));
  return;
}

void sll_free_data(SLLNode *node, void *_) {
  if (node != NULL && node->data != NULL) {
    free(node->data);
  }
  return;
}

typedef struct {
  int *ds;
  size_t size;
} DatasInt;

void sll_assert_node_int(SLLNode *node, void *datas) {
  size_t nodes_count = sll_list_len(node);
  if (nodes_count == 0) {
    return;
  }

  int *ds = ((DatasInt *)datas)->ds;
  size_t ds_size = ((DatasInt *)datas)->size;
  size_t offset = ds_size - nodes_count;
  assert(*(int *)node->data == ds[ds_size - nodes_count] && "data mismatch");
  // NOTE: can't use memmove, as it moves data between mem addresses
  return;
}

void sll_assert_node_int_reverse(SLLNode *node, void *datas) {
  if (((DatasInt *)datas)->size == 0) {
    return;
  }
  int *ds = ((DatasInt *)datas)->ds;
  assert(*(int *)node->data == (ds[((DatasInt *)datas)->size - 1]) &&
         "data mismatch");
  // NOTE: memmove in this case is ok, as we're not actually moving data to
  // different mem address
  memmove(((DatasInt *)datas)->ds, ds, ((DatasInt *)datas)->size - 1);
  --((DatasInt *)datas)->size;
  return;
}

typedef struct {
  char **ds;
  size_t size;
} DatasStr;

void sll_assert_node_str(SLLNode *node, void *datas) {
  if (((DatasStr *)datas)->size == 0) {
    return;
  }
  char **ds = ((DatasStr *)datas)->ds;
  assert(strcmp((char *)node->data, *ds) == 0 && "data mismatch");
  memmove(((DatasStr *)datas)->ds, ds + 1, ((DatasStr *)datas)->size - 1);
  --((DatasStr *)datas)->size;
  return;
}

void sll_apply_print_node_int(SLLNode *cursor, void *fmt) {
  printf(fmt, cursor);
  return;
}

void sll_apply_inc_int(SLLNode *cursor, void *d) {
  if (!cursor) {
    return;
  }
  *(int *)cursor->data += *(int *)d;
  return;
}

// NOTE: run valgrind to make sure there are no leaks
void ds_singly_linked_list(int argc, char **argv) {
  char *prog_name = shift(&argc, &argv);

  char *run = "";
  for (; argc > 0;) {
    char *arg = shift(&argc, &argv);
    if (strcmp("--run", arg) == 0 && argc > 0) {
      run = shift(&argc, &argv);
    }
  }

  {
    char *cse = "sll_create";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      SLLNode *ll = NULL;
      DatasInt *expected_int = NULL;
      DatasStr *expected_str = NULL;

      int a = 1;

      // input validation
      ll = sll_create(NULL, NULL);
      assert(ll == NULL && "unexpected non-NULL");
      ll = sll_create(NULL, (void *)&a);
      assert(ll == NULL && "unexpected non-NULL");
      ll = sll_create(sll_cmp_int, NULL);
      assert(ll == NULL && "unexpected non-NULL");

      // functionality int
      ll = sll_create(sll_cmp_int, (void *)&a);
      sll_list(ll, sll_print_node_int, NULL);

      assert(sll_list_len(ll) == 1 && "unexpected list length");
      expected_int = malloc(1 * sizeof(DatasInt));
      expected_int->ds = (int[]){a};
      expected_int->size = 1; // TODO: non-hardcoded size
      sll_apply(ll, sll_assert_node_int, expected_int);
      free(expected_int);

      ll = sll_nodes_free(ll);
      ll = sll_nodes_free(ll); // duplicate free should work if we use returned
                               // ll from sll_nodes_free

      // functionality int alloced
      int *a_alloced = malloc(1 * sizeof(int));
      *a_alloced = a;
      ll = sll_create(sll_cmp_int, (void *)a_alloced);
      sll_list(ll, sll_print_node_int, NULL);

      assert(sll_list_len(ll) == 1 && "unexpected list length");
      expected_int = malloc(1 * sizeof(DatasInt));
      expected_int->ds = (int[]){a};
      expected_int->size = 1; // TODO: non-hardcoded size
      sll_apply(ll, sll_assert_node_int, expected_int);
      free(expected_int);

      sll_apply(ll, sll_free_data, NULL);
      ll = sll_nodes_free(ll);

      // functionality str
      char *a_str = "a";
      ll = sll_create(sll_cmp_str, (void *)a_str);
      sll_list(ll, sll_print_node_str, NULL);

      assert(sll_list_len(ll) == 1 && "unexpected list length");
      expected_str = malloc(1 * sizeof(DatasInt));
      expected_str->ds = (char *[]){a_str};
      expected_str->size =
          sizeof(expected_str->ds) / sizeof(expected_str->ds[0]);
      sll_apply(ll, sll_assert_node_str, expected_str);
      free(expected_str);

      ll = sll_nodes_free(ll);
      ll = sll_nodes_free(ll); // duplicate free should work if we use returned
                               // ll from sll_nodes_free

      // functionality str alloced
      char *a_str_alloced = calloc((strlen(a_str) + 1), sizeof(char));
      memcpy(a_str_alloced, a_str, strlen(a_str));
      ll = sll_create(sll_cmp_str, (void *)a_str_alloced);
      sll_list(ll, sll_print_node_str, NULL);

      assert(sll_list_len(ll) == 1 && "unexpected list length");
      expected_str = malloc(1 * sizeof(DatasStr));
      expected_str->ds = (char *[]){a_str};
      expected_str->size =
          sizeof(expected_str->ds) / sizeof(expected_str->ds[0]);
      sll_apply(ll, sll_assert_node_str, expected_str);
      free(expected_str);

      sll_apply(ll, sll_free_data, NULL);
      ll = sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_add";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      size_t expected_int_size = 10;
      DatasInt *expected_int = malloc(1 * sizeof(DatasInt));
      expected_int->ds = calloc(expected_int_size, sizeof(int));
      expected_int->size = expected_int_size;
      for (int i = 0; i < expected_int->size; ++i) {
        expected_int->ds[i] = i;
      }

      SLLNode *ll = sll_create(sll_cmp_int, (void *)&expected_int->ds[0]);

      // validation
      SLLNode *val_node = sll_add(NULL, (void *)&expected_int->ds[0]);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_add(NULL, NULL);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_add(ll, NULL);
      assert(val_node == ll && "unexpected linked list ptr mismatch");
      assert(sll_list_len(ll) == 1 && "unexpected length mismatch");

      // functionality
      for (int i = 1; i < expected_int->size; ++i) {
        SLLNode *tmp = sll_add(ll, (void *)&expected_int->ds[i]);
        assert(errno == 0 && "memory alloc failed");
        assert(tmp != ll && "unexpected eqaulity of ptrs");
        ll = tmp;
      }
      sll_list(ll, sll_print_node_int, NULL);
      assert(sll_list_len(ll) == expected_int->size &&
             "unexpected length mismatch");
      sll_apply(ll, sll_assert_node_int_reverse, expected_int);
      sll_apply(ll, sll_assert_node_int_reverse,
                expected_int); // NOTE: duplicate apply to show memmove is safe
                               // with add

      if (expected_int != NULL && expected_int->ds != NULL) {
        free(expected_int->ds);
      }
      if (expected_int != NULL) {
        free(expected_int);
      }
      ll = sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_apply";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      size_t expected_int_size = 10;
      DatasInt *expected_int = malloc(1 * sizeof(DatasInt));
      expected_int->ds = calloc(expected_int_size, sizeof(int));
      expected_int->size = expected_int_size;
      for (int i = 0; i < expected_int->size; ++i) {
        expected_int->ds[i] = i;
      }

      SLLNode *ll = sll_create(sll_cmp_int, (void *)&expected_int->ds[0]);
      for (int i = 1; i < expected_int->size; ++i) {
        SLLNode *tmp = sll_add(ll, (void *)&expected_int->ds[i]);
        assert(errno == 0 && "memory alloc failed");
        assert(tmp != ll && "unexpected eqaulity of ptrs");
        ll = tmp;
      }

      int inc_coef = 5;
      DatasInt *expected_int_assert = malloc(1 * sizeof(DatasInt));
      expected_int_assert->ds = calloc(expected_int_size, sizeof(int));
      expected_int_assert->size = expected_int_size;
      for (int i = 0; i < expected_int_assert->size; ++i) {
        expected_int_assert->ds[i] = i + inc_coef;
      }

      sll_apply(ll, sll_apply_print_node_int, (void *)"node ptr -- %p\n");

      printf("before incrementing\n");
      sll_list(ll, sll_print_node_int, NULL);
      sll_apply(ll, sll_apply_inc_int, (void *)&inc_coef);
      sll_apply(ll, sll_assert_node_int_reverse, expected_int_assert);
      printf("after incrementing\n");
      sll_list(ll, sll_print_node_int, NULL);

      if (expected_int != NULL && expected_int->ds != NULL) {
        free(expected_int->ds);
      }
      if (expected_int != NULL) {
        free(expected_int);
      }
      if (expected_int_assert != NULL && expected_int_assert->ds != NULL) {
        free(expected_int_assert->ds);
      }
      if (expected_int_assert != NULL) {
        free(expected_int_assert);
      }
      ll = sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_append";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      size_t expected_int_size = 10;
      DatasInt *expected_int = malloc(1 * sizeof(DatasInt));
      expected_int->ds = calloc(expected_int_size, sizeof(int));
      expected_int->size = expected_int_size;
      for (int i = 0; i < expected_int->size; ++i) {
        expected_int->ds[i] = i;
      }

      SLLNode *ll = sll_create(sll_cmp_int, (void *)&expected_int->ds[0]);

      // validation
      SLLNode *val_node = sll_append(NULL, (void *)&expected_int->ds[0]);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_append(NULL, NULL);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_append(ll, NULL);
      assert(val_node == ll && "unexpected linked list ptr mismatch");
      assert(sll_list_len(ll) == 1 && "unexpected length mismatch");

      // functionality
      for (int i = 1; i < expected_int->size; ++i) {
        SLLNode *tmp = sll_append(ll, (void *)&expected_int->ds[i]);
        assert(errno == 0 && "memory alloc failed");
        assert(tmp == ll && "unexpected eqaulity of ptrs");
      }
      sll_list(ll, sll_print_node_int, NULL);
      assert(sll_list_len(ll) == expected_int->size &&
             "unexpected length mismatch");
      sll_apply(ll, sll_assert_node_int, expected_int);

      if (expected_int != NULL && expected_int->ds != NULL) {
        free(expected_int->ds);
      }
      if (expected_int != NULL) {
        free(expected_int);
      }
      ll = sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_find";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      size_t expected_int_size = 10;
      DatasInt *expected_int = malloc(1 * sizeof(DatasInt));
      expected_int->ds = calloc(expected_int_size, sizeof(int));
      expected_int->size = expected_int_size;
      for (int i = 0; i < expected_int->size; ++i) {
        expected_int->ds[i] = i;
      }

      SLLNode *ll = sll_create(sll_cmp_int, (void *)&expected_int->ds[0]);

      // validation
      SLLNode *val_node = sll_find(NULL, (void *)&expected_int->ds[0]);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_find(NULL, NULL);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_find(ll, NULL);
      assert(val_node == NULL && "unexpected non-NULL");
      assert(sll_list_len(ll) == 1 && "unexpected length mismatch");

      // data prep
      for (int i = 1; i < expected_int->size; ++i) {
        SLLNode *tmp = sll_add(ll, (void *)&expected_int->ds[i]);
        assert(errno == 0 && "memory alloc failed");
        assert(tmp != ll && "unexpected eqaulity of ptrs");
        ll = tmp;
      }

      // functionality
      for (int i = 0; i < expected_int->size; ++i) {
        SLLNode *n = sll_find(ll, (void *)&expected_int->ds[i]);
        assert(n != NULL && "unexpected NULL");
        printf("searched for '%d', found '%d'\n", expected_int->ds[i],
               *(int *)n->data);
        assert(*(int *)n->data == expected_int->ds[i] && "data mismatch");
      }

      if (expected_int != NULL && expected_int->ds != NULL) {
        free(expected_int->ds);
      }
      if (expected_int != NULL) {
        free(expected_int);
      }
      ll = sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_update";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      size_t expected_int_size = 10;
      DatasInt *expected_int = malloc(1 * sizeof(DatasInt));
      expected_int->ds = calloc(expected_int_size, sizeof(int));
      expected_int->size = expected_int_size;
      for (int i = 0; i < expected_int->size; ++i) {
        expected_int->ds[i] = i;
      }

      SLLNode *ll = sll_create(sll_cmp_int, (void *)&expected_int->ds[0]);

      // validation
      SLLNode *val_node = sll_update(NULL, (void *)&expected_int->ds[0],
                                     (void *)&expected_int->ds[1]);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_update(NULL, NULL, NULL);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_update(ll, NULL, (void *)&expected_int->ds[0]);
      assert(val_node == ll && "list ptrs differ");
      val_node = sll_update(ll, (void *)&expected_int->ds[0], NULL);
      assert(val_node == ll && "list ptrs differ");
      assert(sll_list_len(ll) == 1 && "unexpected length mismatch");

      // data prep
      for (int i = 1; i < expected_int->size; ++i) {
        SLLNode *tmp = sll_add(ll, (void *)&expected_int->ds[i]);
        assert(errno == 0 && "memory alloc failed");
        assert(tmp != ll && "unexpected eqaulity of ptrs");
        ll = tmp;
      }
      printf("before update\n");
      sll_list(ll, sll_print_node_int, NULL);

      // functionality - reverse list by updating
      for (int i = 0; i < expected_int->size; ++i) {
        ll = sll_update(ll, (void *)&expected_int->ds[i],
                        (void *)&expected_int->ds[expected_int->size - 1 - i]);
        assert(ll != NULL && "unexpected NULL");
      }
      printf("after update\n");
      sll_list(ll, sll_print_node_int, NULL);
      assert(sll_list_len(ll) == expected_int->size &&
             "unexpected length mismatch");
      sll_apply(
          ll, sll_assert_node_int,
          expected_int); // NOTE: use sll_assert_node_int instead of _reverse,
                         // because we modified data in underlying mem addresses
                         // i.e. data in expected_int and linked list has the
                         // same orientation

      if (expected_int != NULL && expected_int->ds != NULL) {
        free(expected_int->ds);
      }
      if (expected_int != NULL) {
        free(expected_int);
      }
      ll = sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "sll_delete";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      size_t expected_int_size = 10;
      DatasInt *expected_int = malloc(1 * sizeof(DatasInt));
      expected_int->ds = calloc(expected_int_size, sizeof(int));
      expected_int->size = expected_int_size;
      for (int i = 0; i < expected_int->size; ++i) {
        expected_int->ds[i] = i;
      }

      SLLNode *ll = sll_create(sll_cmp_int, (void *)&expected_int->ds[0]);

      // validation
      SLLNode *val_node = sll_delete(NULL, (void *)&expected_int->ds[0]);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_delete(NULL, NULL);
      assert(val_node == NULL && "unexpected non-NULL");
      val_node = sll_delete(ll, NULL);
      assert(val_node == ll && "list ptrs differ");
      assert(sll_list_len(ll) == 1 && "unexpected length mismatch");

      // data prep
      for (int i = 1; i < expected_int->size; ++i) {
        SLLNode *tmp = sll_add(ll, (void *)&expected_int->ds[i]);
        assert(errno == 0 && "memory alloc failed");
        assert(tmp != ll && "unexpected eqaulity of ptrs");
        ll = tmp;
      }

      // functionality
      size_t orig_ll_len = sll_list_len(ll);
      for (int i = 0, iters_count = 0; iters_count < expected_int->size;
           i = (i + 3) % expected_int->size, ++iters_count) {
        printf("list before deleting '%d'\n", expected_int->ds[i]);
        sll_list(ll, sll_print_node_int, NULL);
        ll = sll_delete(ll, (void *)&expected_int->ds[i]);
        assert(sll_list_len(ll) == orig_ll_len - iters_count - 1 &&
               "unexpected list length");
        SLLNode *n = sll_find(ll, (void *)&expected_int->ds[i]);
        assert(n == NULL && "expected not found, but data was found");
        printf("list after deleting '%d'\n", expected_int->ds[i]);
        sll_list(ll, sll_print_node_int, NULL);
      }

      if (expected_int != NULL && expected_int->ds != NULL) {
        free(expected_int->ds);
      }
      if (expected_int != NULL) {
        free(expected_int);
      }
      ll = sll_nodes_free(ll);

      printf("-- %s: ok\n", cse);
    }
  }

  return;
}

#endif // SINGLY_LINKED_LIST
// }

// {
#ifdef CIRCULAR_DOUBLY_LINKED_LIST

#include <string.h>

#ifndef UTILS
#define UTILS
#include "utils.h"
#undef UTILS
#endif // UTILS

#define CDLL_TESTS
#include "list.h"

void cdll_print_node_int(CDLLNode *node) {
  printf("(%p) data:%d prev:%p next:%p\n", node, *(int *)(node->data),
         node->prev, node->next);
  ; // force uncompressed formatting (ccls)
}

void cdll_print_node_str(CDLLNode *node) {
  printf("(%p) data:'%s' prev:%p next:%p\n", node, (char *)(node->data),
         node->prev, node->next);
  ; // force uncompressed formatting (ccls)
}

bool cdll_cmp_int(void *a, void *b) { return *(int *)a == *(int *)b; }

bool cdll_cmp_str(void *a, void *b) {
  return strcmp((char *)a, (char *)b) == 0;
}

void cdll_free_data(void *data) { ; }

CDLLNode *cdll_assert_next_int(CDLLNode *cursor, int d) {
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

void cdll_assert_each_int(CDLLNode *cursor, int datas[]) {
  int i = 0;
  for (CDLLNode *cur = cursor; cur && cur != cursor;
       cur = cdll_assert_next_int(cur, datas[i]), ++i) {
    assert(cursor->prev && cursor->prev->next == cursor &&
           "prev->next != current");
    assert(cursor->next && cursor->next->prev == cursor &&
           "next->prev != current");
    ;
  }
  return;
}

CDLLNode *cdll_assert_next_str(CDLLNode *cursor, char *d) {
  if (!cursor) {
    return NULL;
  }
  if (!cdll_cmp_str(cursor->data, d)) {
    printf("[ERROR]: str data doesn't match: expected '%s', got '%s'\n", d,
           (char *)cursor->data);
    assert(cdll_cmp_str(cursor->data, d));
  }
  return cursor->next;
}

void cdll_assert_each_str(CDLLNode *cursor, char **datas) {
  int i = 0;
  for (CDLLNode *cur = cursor; cur && cur != cursor;
       cur = cdll_assert_next_str(cur, datas[i]), ++i) {
    assert(cursor->prev && cursor->prev->next == cursor &&
           "prev->next != current");
    assert(cursor->next && cursor->next->prev == cursor &&
           "next->prev != current");
    ;
  }
  return;
}

void ds_circular_doubly_linked_list(int argc, char **argv) {
  char *prog_name = shift(&argc, &argv);

  int a = 1;
  int aa = a;
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
    char *cse = "cdll_create_validation";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      CDLLNode *cdll = NULL;
      cdll = cdll_create(NULL, cdll_print_node_int, cdll_free_data, NULL, NULL,
                         (void *)&a);
      assert(!cdll);
      cdll = cdll_create(cdll_cmp_int, NULL, cdll_free_data, NULL, NULL,
                         (void *)&a);
      assert(!cdll);
      cdll = cdll_create(cdll_cmp_int, cdll_print_node_int, NULL, NULL, NULL,
                         (void *)&a);
      assert(!cdll);
      cdll = cdll_create(cdll_cmp_int, cdll_print_node_int, cdll_free_data,
                         NULL, NULL, NULL);
      assert(!cdll);
      cdll_nodes_free(cdll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "cdll_create";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      CDLLNode *cdll = cdll_create(cdll_cmp_int, cdll_print_node_int,
                                   cdll_free_data, NULL, NULL, (void *)&a);
      cdll_list(cdll);
      cdll_assert_next_int(cdll, a);
      cdll_nodes_free(cdll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "cdll_append_validate";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      CDLLNode *cdll = NULL;
      cdll = cdll_append(NULL, (void *)&a);
      assert(!cdll && "should be NULL");
      cdll_nodes_free(cdll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "cdll_append";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      CDLLNode *cdll = NULL;

      cdll = cdll_create(cdll_cmp_int, cdll_print_node_int, cdll_free_data,
                         NULL, NULL, (void *)&a);
      printf("head: %p\n", cdll);
      cdll_print_node_int(cdll);
      cdll_list(cdll);
      printf("-----\n");
      cdll = cdll_append(cdll, (void *)(&b));
      cdll_list(cdll);
      printf("-----\n");
      cdll = cdll_append(cdll, (void *)(&c));
      cdll = cdll_append(cdll, (void *)(&d));
      cdll = cdll_append(cdll, (void *)(&e));
      cdll_list(cdll);

      assert(cdll_list_len(cdll) == 5 && "unexpected list length");
      cdll_assert_each_int(cdll, (int[]){a, b, c, d, e});

      printf("-----\n");
      cdll = cdll_append(cdll, (void *)(&c));
      cdll = cdll_append(cdll, (void *)(&d));
      cdll = cdll_append(cdll, (void *)(&e));
      cdll_list(cdll);

      assert(cdll_list_len(cdll) == 8 && "unexpected list length");
      cdll_assert_each_int(cdll, (int[]){a, b, c, d, e, c, d, e});

      cdll_nodes_free(cdll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "cdll_string_data";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      CDLLNode *cdll = cdll_create(cdll_cmp_str, cdll_print_node_str,
                                   cdll_free_data, NULL, NULL, (void *)s1);
      printf("head: %p\n", cdll);
      cdll_print_node_str(cdll);

      cdll = cdll_append(cdll, (void *)s1_1);
      cdll = cdll_append(cdll, (void *)s2);
      cdll_list(cdll);

      assert(cdll_list_len(cdll) == 3 && "list length unexpected");
      cdll_assert_each_str(cdll, (char *[]){s1, s1_1, s2});

      cdll_nodes_free(cdll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "cdll_find_validate";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      CDLLNode *cdll = NULL;
      cdll = cdll_find(NULL, (void *)&a);
      assert(!cdll && "should be NULL");
      cdll_nodes_free(cdll);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "cdll_find";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      // single elem list find
      {
        CDLLNode *cdll = cdll_create(cdll_cmp_int, cdll_print_node_int,
                                     cdll_free_data, NULL, NULL, (void *)&a);
        CDLLNode *fnd = cdll_find(cdll, (void *)&a);
        assert(fnd && "unexpected NULL");
        assert(*(int *)fnd->data == a && "data mismatch");
#ifdef CDLL_TESTS
        printf("single iters %d\n", iters);
#endif // CDLL_TESTS

        fnd = cdll_find(cdll, (void *)&aa);
        assert(fnd && "unexpected NULL");
        assert(*(int *)fnd->data == a &&
               "data mismatch"); // NOTE: cmp with `a`, because aa == a;
#ifdef CDLL_TESTS
        printf("single iters %d\n", iters);
#endif // CDLL_TESTS

        cdll_nodes_free(cdll);
      }

      // two elem list find
      {
        int datas[2] = {a, b};
        CDLLNode *cdll = cdll_create(cdll_cmp_int, cdll_print_node_int,
                                     cdll_free_data, NULL, NULL, (void *)&a);
        CDLLNode *fnd = NULL;
        for (int i = 1; i < sizeof(datas) / sizeof(*datas); ++i) {
          cdll = cdll_append(cdll, (void *)(&datas[i]));
        }

        for (int i = 0; i < sizeof(datas) / sizeof(*datas); ++i) {
          fnd = cdll_find(cdll, (void *)&datas[i]);
          assert(fnd && "unexpected NULL");
          cdll_assert_next_int(fnd, datas[i]);
#ifdef CDLL_TESTS
          printf("two -- iters %d\n", iters);
#endif // CDLL_TESTS
        }
        cdll_nodes_free(cdll);
      }

      // three elem list find
      {
        int datas[3] = {a, b, c};
        CDLLNode *cdll = cdll_create(cdll_cmp_int, cdll_print_node_int,
                                     cdll_free_data, NULL, NULL, (void *)&a);
        CDLLNode *fnd = NULL;
        for (int i = 1; i < sizeof(datas) / sizeof(*datas); ++i) {
          cdll = cdll_append(cdll, (void *)(&datas[i]));
        }

        for (int i = 0; i < sizeof(datas) / sizeof(*datas); ++i) {
          fnd = cdll_find(cdll, (void *)&datas[i]);
          assert(fnd && "unexpected NULL");
          cdll_assert_next_int(fnd, datas[i]);
#ifdef CDLL_TESTS
          printf("three -- iters %d\n", iters);
#endif // CDLL_TESTS
        }
        cdll_nodes_free(cdll);
      }

      // more elem list find
      {
        int datas[7] = {a, b, c, d, e, f, g};
        CDLLNode *cdll = cdll_create(cdll_cmp_int, cdll_print_node_int,
                                     cdll_free_data, NULL, NULL, (void *)&a);
        CDLLNode *fnd = NULL;
        for (int i = 1; i < sizeof(datas) / sizeof(*datas); ++i) {
          cdll = cdll_append(cdll, (void *)(&datas[i]));
        }

        for (int i = 0; i < sizeof(datas) / sizeof(*datas); ++i) {
          fnd = cdll_find(cdll, (void *)&datas[i]);
          assert(fnd && "unexpected NULL");
          cdll_assert_next_int(fnd, datas[i]);
#ifdef CDLL_TESTS
          printf("more -- iters %d\n", iters);
#endif // CDLL_TESTS
        }
        cdll_nodes_free(cdll);
      }

      // not found
      {
        int datas[7] = {a, b, c, d, e, f, g};
        CDLLNode *cdll = cdll_create(cdll_cmp_int, cdll_print_node_int,
                                     cdll_free_data, NULL, NULL, (void *)&a);
        CDLLNode *fnd = NULL;
        for (int i = 1; i < sizeof(datas) / sizeof(*datas); ++i) {
          cdll = cdll_append(cdll, (void *)(&datas[i]));
        }

        int not_found = 10000;
        fnd = cdll_find(cdll, (void *)&not_found);
        assert(!fnd && "unexpected non-NULL");
#ifdef CDLL_TESTS
        printf("not found -- iters %d\n", iters);
#endif // CDLL_TESTS
        cdll_nodes_free(cdll);
      }

      printf("-- %s: ok\n", cse);
    }
  }

  // TODO: implement
  // * update
  // * delete

  return;
}

#undef CDLL_TESTS

#endif // CIRCULAR_DOUBLY_LINKED_LIST
// }

// {
#ifdef MAP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef UTILS
#define UTILS
#include "utils.h"
#undef UTILS
#endif // UTILS

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

void map_apply_print_node_int(MapKeyValue *kv, void *fmt) {
  printf(fmt, kv);
  return;
}

void map_apply_inc_int(MapKeyValue *kv, void *d) {
  if (kv == NULL) {
    return;
  }
  *(int *)kv->value += *(int *)d;
  return;
}

void ds_map(int argc, char **argv) {
  char *prog_name = shift(&argc, &argv);

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

  {
    char *cse = "map_apply";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      Map *map =
          map_create(map_hash, MapKeyInt, map_print_value_int, map_free_data);
      assert(map && "unexpected NULL");

      int a = 1;
      int b = 2;
      int c = 3;
      int d = 4;
      int e = 5;

      int a_cpy = a;
      int b_cpy = b;
      int c_cpy = c;
      int d_cpy = d;
      int e_cpy = e;

      MapKeyValue kvs[] = (MapKeyValue[]){
          (MapKeyValue){.key = (void *)&a, .value = (void *)&a_cpy},
          (MapKeyValue){.key = (void *)&b, .value = (void *)&b_cpy},
          (MapKeyValue){.key = (void *)&c, .value = (void *)&c_cpy},
          (MapKeyValue){.key = (void *)&d, .value = (void *)&d_cpy},
          (MapKeyValue){.key = (void *)&e, .value = (void *)&e_cpy},
      };

      int kvs_len = sizeof(kvs) / sizeof(MapKeyValue);

      for (int i = 0; i < kvs_len; ++i) {
        map = map_insert(map, kvs[i].key, kvs[i].value);
        assert(map && "unexpected NULL");

        printf("idx for '%d:%d': %d\n", *(int *)kvs[i].key,
               *(int *)kvs[i].value,
               map_hash(MapKeyStr, (char *)kvs[i].key, map->cap));
      }

      int inc_coef = 5;
      map_apply(map, map_apply_print_node_int,
                (void *)"apply -- map kv ptr '%p'\n");
      map_apply(map, map_apply_inc_int, (void *)&inc_coef);

      int exp_keys[] = (int[]){a, b, c, d, e};
      int exp_values[] = (int[]){a + inc_coef, b + inc_coef, c + inc_coef,
                                 d + inc_coef, e + inc_coef};
      for (int i = 0; i < sizeof(exp_keys) / sizeof(exp_keys[0]); ++i) {
        void *tmp = map_find(map, (void *)&exp_keys[i]);
        assert(tmp && "unexpected NULL");
        int fnd = *(int *)tmp;

        assert(exp_values[i] == fnd && "value mismatch");
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
#ifdef STACK

#include <string.h>

#ifndef UTILS
#define UTILS
#include "utils.h"
#undef UTILS
#endif // UTILS

#include "list.h"

void stack_print_node_int(StackNode *node) {
  printf("(%p) data:%d\n", node, *(int *)(node->data));
  ; // force uncompressed formatting (ccls)
}

void stack_print_node_str(StackNode *node) {
  printf("(%p) data:'%s'\n", node, (char *)(node->data));
  ; // force uncompressed formatting (ccls)
}

void stack_apply_print_node_int(StackNode *node, void *fmt) {
  printf(fmt, node);
  return;
}

void stack_apply_inc_int(StackNode *node, void *d) {
  if (!node) {
    return;
  }
  *(int *)node->data += *(int *)d;
  return;
}

void ds_stack(int argc, char **argv) {
  char *prog_name = shift(&argc, &argv);

  int a = 1;
  int aa = a;
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
    char *cse = "stack_create";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      StackNode *stack = NULL;
      stack = stack_create(NULL, (void *)&a);
      assert(!stack && "unexpected non-NULL");
      stack = stack_create(stack_print_node_int, NULL);
      assert(!stack && "unexpected non-NULL");
      stack = stack_create(stack_print_node_int, (void *)&a);
      assert(stack && "unexpected NULL");
      stack_nodes_free(stack);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "stack_push";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      StackNode *stack = NULL;
      assert(!stack_push(stack, (void *)&a) && "unexpected non-NULL");

      stack = stack_create(stack_print_node_int, (void *)&a);
      stack_list(stack);
      stack = stack_push(stack, (void *)&a);
      stack = stack_push(stack, (void *)&aa);
      stack = stack_push(stack, (void *)&b);
      stack = stack_push(stack, (void *)&c);
      stack = stack_push(stack, (void *)&d);
      stack_list(stack);

      assert(stack_list_len(stack) == 6 && "unexpected list length");
      assert(*(int *)stack->data == d);
      stack_nodes_free(stack);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "stack_pop";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      StackNode *stack = NULL;
      assert(!stack_push(stack, (void *)&a) && "unexpected non-NULL");

      stack = stack_create(stack_print_node_int, (void *)&a);
      stack = stack_push(stack, (void *)&a);
      stack = stack_push(stack, (void *)&aa);
      stack = stack_push(stack, (void *)&b);
      stack = stack_push(stack, (void *)&c);
      stack = stack_push(stack, (void *)&d);
      stack_list(stack);

      assert(stack_list_len(stack) == 6 && "unexpected list length");
      size_t orig_len = stack_list_len(stack);
      int exp_stack[] = (int[]){a, a, aa, b, c, d};
      for (int i = 0; i < orig_len; ++i) {
        void *out;
        stack = stack_pop(stack, &out);
        assert(stack_list_len(stack) == orig_len - i - 1 &&
               "unexpected list length");
        if (stack) {
          assert(*(int *)stack->data == exp_stack[orig_len - i - 1 - 1]);
        }
        assert(*(int *)out == exp_stack[orig_len - i - 1] &&
               "unexpected value in `out`");
        printf("popped '%d'\n", *(int *)out);
      }
      stack_nodes_free(stack);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "string stack";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      StackNode *stack = NULL;

      stack = stack_create(stack_print_node_str, (void *)s1);
      stack = stack_push(stack, (void *)s1_1);
      stack = stack_push(stack, (void *)s2);
      stack = stack_push(stack, (void *)s2);
      stack_list(stack);

      assert(stack_list_len(stack) == 4 && "unexpected list length");
      size_t orig_len = stack_list_len(stack);
      char *exp_stack[] = (char *[]){s1, s1_1, s2, s2};
      for (int i = 0; i < orig_len; ++i) {
        void *out;
        stack = stack_pop(stack, &out);
        assert(stack_list_len(stack) == orig_len - i - 1 &&
               "unexpected list length");
        if (stack) {
          assert(strcmp((char *)stack->data, exp_stack[orig_len - i - 1 - 1]) ==
                 0);
        }
        assert(strcmp((char *)out, exp_stack[orig_len - i - 1]) == 0 &&
               "unexpected value in `out`");
        printf("popped '%s'\n", (char *)out);
      }
      stack_nodes_free(stack);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "stack_apply";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      StackNode *stack = NULL;

      int a_cpy = a;
      int b_cpy = b;
      int c_cpy = c;
      int d_cpy = d;
      int e_cpy = e;

      stack = stack_create(stack_print_node_int, (void *)&a_cpy);
      stack = stack_push(stack, (void *)&b_cpy);
      stack = stack_push(stack, (void *)&c_cpy);
      stack = stack_push(stack, (void *)&d_cpy);
      stack = stack_push(stack, (void *)&e_cpy);

      int inc_coef = 5;
      stack_apply(stack, stack_apply_print_node_int,
                  (void *)"apply -- node ptr '%p'\n");
      stack_apply(stack, stack_apply_inc_int, (void *)&inc_coef);

      int exp_stack[] = (int[]){a + inc_coef, b + inc_coef, c + inc_coef,
                                d + inc_coef, e + inc_coef};
      size_t orig_len = stack_list_len(stack);
      for (int i = 0; i < orig_len; ++i) {
        void *out;
        stack = stack_pop(stack, &out);
        assert(stack_list_len(stack) == orig_len - i - 1 &&
               "unexpected list length");
        if (stack) {
          assert(*(int *)stack->data == exp_stack[orig_len - i - 1 - 1]);
        }
        assert(*(int *)out == exp_stack[orig_len - i - 1] &&
               "unexpected value in `out`");
        printf("popped '%d'\n", *(int *)out);
      }

      stack_nodes_free(stack);

      printf("-- %s: ok\n", cse);
    }
  }

  return;
}

#endif // STACK
// }

// {
#ifdef QUEUE

#include <string.h>

#ifndef UTILS
#define UTILS
#include "utils.h"
#undef UTILS
#endif // UTILS

#include "list.h"

void queue_print_node_int(QueueNode *node) {
  printf("(%p) data:%d\n", node, *(int *)(node->data));
  ; // force uncompressed formatting (ccls)
}

void queue_print_node_str(QueueNode *node) {
  printf("(%p) data:'%s'\n", node, (char *)(node->data));
  ; // force uncompressed formatting (ccls)
}

void queue_apply_print_node_int(QueueNode *node, void *fmt) {
  printf(fmt, node);
  return;
}

void queue_apply_inc_int(QueueNode *node, void *d) {
  if (!node) {
    return;
  }
  *(int *)node->data += *(int *)d;
  return;
}

void ds_queue(int argc, char **argv) {
  char *prog_name = shift(&argc, &argv);

  int a = 1;
  int aa = a;
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
    char *cse = "queue_create";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);
      QueueNode *queue = NULL;
      queue = queue_create(NULL, (void *)&a);
      assert(!queue && "unexpected non-NULL");
      queue = queue_create(queue_print_node_int, NULL);
      assert(!queue && "unexpected non-NULL");
      queue = queue_create(queue_print_node_int, (void *)&a);
      assert(queue && "unexpected NULL");
      queue_nodes_free(queue);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "queue_push";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      QueueNode *queue = NULL;
      assert(!queue_push(queue, (void *)&a) && "unexpected non-NULL");

      queue = queue_create(queue_print_node_int, (void *)&a);
      queue_list(queue);
      queue = queue_push(queue, (void *)&a);
      queue = queue_push(queue, (void *)&aa);
      queue = queue_push(queue, (void *)&b);
      queue = queue_push(queue, (void *)&c);
      queue = queue_push(queue, (void *)&d);
      queue_list(queue);

      assert(queue_list_len(queue) == 6 && "unexpected list length");
      assert(*(int *)queue->data == a);
      queue_nodes_free(queue);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "queue_pop";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      QueueNode *queue = NULL;
      assert(!queue_push(queue, (void *)&a) && "unexpected non-NULL");

      queue = queue_create(queue_print_node_int, (void *)&a);
      queue = queue_push(queue, (void *)&a);
      queue = queue_push(queue, (void *)&aa);
      queue = queue_push(queue, (void *)&b);
      queue = queue_push(queue, (void *)&c);
      queue = queue_push(queue, (void *)&d);
      queue_list(queue);

      assert(queue_list_len(queue) == 6 && "unexpected list length");
      size_t orig_len = queue_list_len(queue);
      int exp_queue[] = (int[]){a, a, aa, b, c, d};
      for (int i = 0; i < orig_len; ++i) {
        void *out;
        queue = queue_pop(queue, &out);
        assert(queue_list_len(queue) == orig_len - i - 1 &&
               "unexpected list length");
        if (queue) {
          assert(*(int *)queue->data == exp_queue[i + 1]);
        }
        assert(*(int *)out == exp_queue[i] && "unexpected value in `out`");
        printf("popped '%d'\n", *(int *)out);
      }
      queue_nodes_free(queue);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "string queue";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      QueueNode *queue = NULL;

      queue = queue_create(queue_print_node_str, (void *)s1);
      queue = queue_push(queue, (void *)s1_1);
      queue = queue_push(queue, (void *)s2);
      queue = queue_push(queue, (void *)s2);
      queue_list(queue);

      assert(queue_list_len(queue) == 4 && "unexpected list length");
      size_t orig_len = queue_list_len(queue);
      char *exp_queue[] = (char *[]){s1, s1_1, s2, s2};
      for (int i = 0; i < orig_len; ++i) {
        void *out;
        queue = queue_pop(queue, &out);
        assert(queue_list_len(queue) == orig_len - i - 1 &&
               "unexpected list length");
        if (queue) {
          assert(strcmp((char *)queue->data, exp_queue[i + 1]) == 0);
        }
        assert(strcmp((char *)out, exp_queue[i]) == 0 &&
               "unexpected value in `out`");
        printf("popped '%s'\n", (char *)out);
      }
      queue_nodes_free(queue);

      printf("-- %s: ok\n", cse);
    }
  }

  {
    char *cse = "queue_apply";
    if (strcmp(run, cse) == 0 || strcmp(run, "all") == 0 || strlen(run) == 0) {
      printf("---- %s\n", cse);

      QueueNode *queue = NULL;

      int a_cpy = a;
      int b_cpy = b;
      int c_cpy = c;
      int d_cpy = d;
      int e_cpy = e;

      queue = queue_create(queue_print_node_int, (void *)&a_cpy);
      queue = queue_push(queue, (void *)&b_cpy);
      queue = queue_push(queue, (void *)&c_cpy);
      queue = queue_push(queue, (void *)&d_cpy);
      queue = queue_push(queue, (void *)&e_cpy);

      int inc_coef = 5;
      queue_apply(queue, queue_apply_print_node_int,
                  (void *)"apply -- node ptr '%p'\n");
      queue_apply(queue, queue_apply_inc_int, (void *)&inc_coef);

      int exp_queue[] = (int[]){e + inc_coef, d + inc_coef, c + inc_coef,
                                b + inc_coef, a + inc_coef};
      size_t orig_len = queue_list_len(queue);
      for (int i = 0; i < orig_len; ++i) {
        void *out;
        queue = queue_pop(queue, &out);
        assert(queue_list_len(queue) == orig_len - i - 1 &&
               "unexpected list length");
        if (queue) {
          assert(*(int *)queue->data == exp_queue[orig_len - i - 1 - 1]);
        }
        assert(*(int *)out == exp_queue[orig_len - i - 1] &&
               "unexpected value in `out`");
        printf("popped '%d'\n", *(int *)out);
      }

      queue_nodes_free(queue);

      printf("-- %s: ok\n", cse);
    }
  }

  return;
}

#endif // QUEUE
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

// {
#ifdef LOG

#include <string.h>

#include "log.h"

void utils_log() {

  printf("standard logging---------------\n");
  log_level = FATAL;
  log_fatal("%s: test test test: %d %c %s\n", __FILE__, 1, '2', "3");
  log_level = INFO;
  log_fatal("fatal log: should be shown\n");
  log_info("info log: should be shown\n");
  log_debug("debug log: shouldn't be shown\n");
  log_log(INFO, "lower lvl logging func, %d %c %s\n", 1, '2', "3");

  printf("stream logging------------------\n");
  log_finfo(stdout, "stream logging to stdout\n");
  log_finfo(stderr, "stream logging to stderr\n");
  log_flog(stderr, INFO, "lower lvl logging func, %d %c %s\n", 1, '2', "3");

  char *filename = "example_log.out";
  FILE *fptr = fopen(filename, "w");
  log_level = TRACE;
  log_ftrace(fptr, "%s:%d: trace should be shown\n", __FILE__, __LINE__);
  log_fwarn(fptr, "warning should be shown\n");
  log_flog(fptr, INFO, "lower lvl logging func, %d %c %s\n", 1, '2', "3");
  fclose(fptr);

  fptr = fopen(filename, "r");
  fseek(fptr, 0, SEEK_END);
  long read_buf_len = ftell(fptr);
  rewind(fptr);
  char read_buf[read_buf_len + 1];
  memset(read_buf, '\0', sizeof(read_buf));
  int n = fread(read_buf, sizeof(char), sizeof(read_buf), fptr);
  fclose(fptr);
  read_buf[n] = '\0';
  printf("logs read from '%s' (between lines):\n\n----\n%s----\n\n", filename,
         read_buf);
  remove(filename);

  printf("buffered logging-----------------------\n");
  size_t bsize = 1024 * 5;
  int len = 0;
  char buf[bsize];
  log_level = INFO;
  len += log_binfo(buf + len, bsize, "should be shown\n");
  len += log_bdebug(buf + len, bsize, "shouldn't be shown\n");
  len += log_berr(buf + len, bsize, "should be shown: %d %c %s\n", 1, '2', "3");
  len += log_blog(buf + len, bsize, INFO, "lower lvl logging func, %d %c %s\n",
                  1, '2', "3");
  printf("%s", buf);

  return;
}

#endif // LOG
// }

// {
#ifdef UTILS

#include "utils.h"

long long int naive_power(long long a, int n) {
  long long a_orig = a;
  for (int i = 1; i < n; ++i) {
    a *= a_orig;
  }
  return a;
}

void utils_power() {
  assert(power(2, 0) == 1);
  assert(power(2, 1) == 2);
  assert(power(1, 0) == 1);
  assert(power(1, 1) == 1);
  assert(power(1, 10) == 1);
  assert(power(4, 3) == 64);
  assert(power(2, 10) == 1024);
  assert(power(5, 6) == naive_power(5, 6));
  assert(power(10, 10) == naive_power(10, 10));

  // for now power doesn't support neg numbers
  assert(power(-3, 3) == 0);
  assert(power(3, -3) == 0);
  assert(power(-3, -3) == 0);

  printf("-- power: ok\n");
}
#endif // UTILS
// }

// {
#ifdef LEX

#include <string.h>

#include "lex.h"

void utils_lex() {

  char *buf =
      "\n\t - symbols \\ \n \t - / !@#$%^&*()[]{}_+="
      "\n\t - words test test.test test_test TeSt TEST tEsT test123 T35t "
      "test-test"
      "\n\t - ints 0 1 12 -12 -0 -12 12.12 12_12 12-12"
      "\n\t - chars a b c ab"
      "\n\t - strings \"test\" \"test test test\" \"test-test\" \"test_test\" "
      "'test' 't' '1' "
      "\n";

  size_t token_counter;
  size_t buf_counter;
  lex_token **tokens;
  char *buffer;
  FILE *fptr;

  printf("----------- every buf token --------------\n");
  token_counter = 0;
  tokens = lex_tokenize_buf(buf, strlen(buf), &token_counter);
  lex_print(tokens, token_counter);
  lex_free(tokens, token_counter);

  printf("---------- no whitespace buf tokens ---------------\n");
  token_counter = 0;
  LEX_SKIP_WHITESPACE_TOKEN = 1;
  tokens = lex_tokenize_buf(buf, strlen(buf), &token_counter);
  lex_print(tokens, token_counter);
  lex_free(tokens, token_counter);

  printf("--------- bufferize stream ----------------\n");
  fptr = fopen("README.md", "r");
  buf_counter = 0;
  buffer = lex_bufferize(fptr, &buf_counter);
  printf("buffer byte count:%d\n%s\n", buf_counter, buffer);
  if (buffer) {
    free(buffer);
  }
  fclose(fptr);

  printf("--------- every stream token ----------------\n");
  fptr = fopen("README.md", "r");
  token_counter = 0;
  tokens = lex_tokenize(fptr, &token_counter);
  lex_print(tokens, token_counter);
  lex_free(tokens, token_counter);
  fclose(fptr);

  printf("--------- no whitespace stream chars ----------------\n");
  fptr = fopen("README.md", "r");
  token_counter = 0;
  LEX_SKIP_WHITESPACE_CHAR = 1;
  tokens = lex_tokenize(fptr, &token_counter);
  lex_print(tokens, token_counter);
  lex_free(tokens, token_counter);
  fclose(fptr);
}

#endif // LEX
// }

int main(int argc, char **argv) {

#ifdef SINGLY_LINKED_LIST
  ds_singly_linked_list(argc, argv);
#endif // SINGLY_LINKED_LIST

#ifdef CIRCULAR_DOUBLY_LINKED_LIST
  ds_circular_doubly_linked_list(argc, argv);
#endif // CIRCULAR_DOUBLY_LINKED_LIST

#ifdef MAP
  ds_map(argc, argv);
#endif // MAP

#ifdef TWO_SUM
  algo_two_sum();
#endif // TWO_SUM

#ifdef MD5
  algo_md5();
#endif // MD5

#ifdef LOG
  utils_log();
#endif // LOG

#ifdef UTILS
  utils_power();
#endif // UTILS

#ifdef LEX
  utils_lex();
#endif // LEX

#ifdef STACK
  ds_stack(argc, argv);
#endif // STACK

#ifdef QUEUE
  ds_queue(argc, argv);
#endif // QUEUE

  return 0;
}