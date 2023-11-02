#include <stdio.h>
#include <stdlib.h>

#define TRIE_CHILDREN 1

// A(0)BCDEFGHIJKLMNOPQRSTUVWXYZ(25)abcdefghijklmnopqrstuvwxyz(51)0123456789_-(63)
#define TRIE_CHILDREN_SIZE 64

enum bool { false=0, true=1};

typedef struct trie_node {
	struct trie_node **children; // [TRIE_CHILDREN_SIZE];
	enum bool is_leaf;
	char *v;
	struct trie_node *parent;
} trie_node;

// TODO: double free or corruption (out) error
void trie_free(trie_node *tn) {
	printf("HERE1 %s %p %p\n", tn->v, tn, tn->children);
	for (int i=0; i<TRIE_CHILDREN_SIZE; ++i){
		if (tn->children[i] != NULL) {
			trie_free(tn->children[i]);
			free(tn->children[i]);
		}
	}
	printf("HERE2 %s %p %p\n", tn->v, tn, tn->children);
	if (tn->children) {
		free(tn->children);
	}
	if (tn->v) {
		free(tn->v);
	}
}

void trie_print_rec(trie_node *tn, char ***path, int *path_size, int i) {
	if (tn == NULL) {
		return;
	}
	if (tn->v != "") {
		(*path)[i] = tn->v;
		++i;
		if (i >= *path_size) {
			(*path_size) *= 2;
			*path = realloc(*path, *path_size);
		}
	}
	if (tn->is_leaf) {
		int j = 0;
		for (; j<i-1; ++j) {
			printf("%s -> ", (*path)[j]);
		}
		printf("%s\n", (*path)[j]);
		return;
	}
	for (int k=0; k<TRIE_CHILDREN_SIZE; ++k) {
		trie_print_rec(tn->children[k], path, path_size, i);
	}
}

#define TRIE_PRINT_PATH_SIZE 1024
void trie_print(trie_node *tn) {
	char **path = calloc(TRIE_PRINT_PATH_SIZE, sizeof(char *));
	int path_size = TRIE_PRINT_PATH_SIZE;
	trie_print_rec(tn, &path, &path_size, 0);
	free(path);
}

trie_node *trie_new(trie_node *parent, const char *v, size_t vlen) {
	trie_node *new = calloc(1, sizeof(trie_node *));
	trie_node **children = calloc(TRIE_CHILDREN_SIZE, sizeof(trie_node *));
	new->children = children;
	new->parent = parent;
	char *vc = calloc(vlen, sizeof(char)); // TODO: try without allocation
	for (int i=0;i<vlen; ++i) {
		vc[i] = v[i];
	}
	new->v = vc;
	return new;
}

int trie_add(trie_node *tn, const char *v) {
	trie_node *tnc = tn;
	char *vc = (char *)v;
	size_t offset = 0;
	while (*vc != '\0') {
		++offset;
		int idx = 0;
		if ('A' <= *vc && *vc <= 'Z') {
			idx = *vc-'A';
		} else if ('a' <= *vc && *vc <= 'z') {
			idx = *vc-'a' + 25;
		} else if ('0' <= *vc && *vc <= '9') {
			idx = *vc-'0' + 51;
		} else if (*vc == '_') {
			idx = 62;
		} else if (*vc == '-') {
			idx = 63;
		}
		trie_node *child = tnc->children[idx];
		if (child == NULL) {
			printf("HERE\n");
			child = trie_new(tnc, v, offset);
			tnc->children[idx] = child;
		}
		tnc = child;
		++vc;
	}
	tnc->is_leaf = 1;
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
	trie_node *root = trie_new(NULL, "root", 4);
	printf("%p %s %p %p\n", root, root->v, root->children, NULL);
	trie_add(root, "String");
	trie_add(root, "Street");
	trie_print(root);
	trie_free(root);
}