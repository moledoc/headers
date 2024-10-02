#include <stdio.h>

#define CIRCULAR_DOUBLY_LINKED_LIST
#include "../list.h"

bool cdll_cmp_int(void *a, void *b) { return *(int *)a == *(int *)b; }

void cdll_print_node_int(CDLLNode *node, void *_) {
  if (node == NULL) {
    printf("-> (%p) data: NULL\n", node);
    return;
  }
  printf("-> (%p) data:%d prev: %p next: %p\n", node, *(int *)(node->data),
         node->prev, node->next);
  return;
}

int main() {
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  int e = 5;
  int f = 6;
  int g = 7;

  CDLLNode *cdll = cdll_create(cdll_cmp_int, (void *)&a);
  cdll_list(cdll, cdll_print_node_int, NULL);
  printf("-----\n");
  cdll = cdll_append(cdll, (void *)(&b));
  cdll_list(cdll, cdll_print_node_int, NULL);
  printf("-----\n");
  cdll = cdll_add(cdll, (void *)(&c));
  cdll = cdll_append(cdll, (void *)(&d));
  cdll = cdll_add(cdll, (void *)(&e));
  cdll_list(cdll, cdll_print_node_int, NULL);

  printf("list len: %d\n", cdll_list_len(cdll));

  printf("-----\n");
  cdll_print_node_int(cdll_find(cdll, &b), NULL);
  printf("-----\n");
  cdll_print_node_int(cdll_update(cdll, &b, &g), NULL);
  printf("-----\n");
  cdll_print_node_int(cdll_find(cdll, &b), NULL);
  printf("-----\n");
  cdll_delete(cdll, &b);
  printf("-----\n");
  cdll_list(cdll, cdll_print_node_int, NULL);

  cdll_nodes_free(cdll);
}