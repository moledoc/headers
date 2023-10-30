#include <stdio.h>

typedef struct trie_node {
	char children[256];
	int is_lead;
	char *v;
	struct trie_node *parent;
} trie_node;

void trie_free(trie_node *tn) {
}

void trie_print(trie_node *tn) {
}

trie_node *trie_new_node(trie_node *parent, char *v) {
	// UNIMPLEMENTED
	return NULL;
}

int trie_add(trie_node *tn, char *v) {
	// UNIMPLEMETED
	return 0;
}

int trie_find(trie_node *tn, char *v) {
	// UNIMPLEMENTED
	return 0;
}

int trie_delete(trie_node *tn, char *v){
	// UNIMPLEMENTED
	return 0;
}

int main(int argc, char **argv) {
}