#pragma once

#ifndef TRIE_H_
#define TRIE_H_

#include <stdio.h>

enum bool { false=0, true=1};

typedef struct trie_node {
	struct trie_node **children; // [TRIE_CHILDREN_SIZE];
	enum bool is_leaf;
	char *v;
	struct trie_node *parent;
} trie_node;

int mystrcomp(const char *s1, const char *s2);
size_t mystrlen(const char *s1);
void trie_free(trie_node *tn);
void trie_print_rec(trie_node *tn, char ***path, int *path_size, int i);
void trie_print(trie_node *tn);
trie_node *trie_new(trie_node *parent, const char *v, size_t vlen);
int idx_map(char c);
trie_node *trie_add(trie_node *tn, const char *v);
trie_node *trie_find(trie_node *tn, char *v, int *child_list_idx);

#endif // TRIE_H_

#ifdef TRIE_IMPLEMENTATION

#include <stdlib.h>

// A(0)BCDEFGHIJKLMNOPQRSTUVWXYZ(25)abcdefghijklmnopqrstuvwxyz(51)0123456789_-(63)
#define TRIE_CHILDREN_SIZE 64
#define TRIE_PRINT_PATH_SIZE 1024

int mystrcomp(const char *s1, const char *s2) {
	char *s1c = (char *)s1;
	char *s2c = (char *)s2;
	while (*s1c != '\0' && *s2c != '\0') {
		if (*s1c != *s2c) {
			break;
		}
		++s1c;
		++s2c;
	}
	return *s1c == *s2c;
}

size_t mystrlen(const char *s1) {
	char *s1c = (char*)s1;
	size_t size = 0;
	while (*(s1c++) != '\0') {
		++size;
	}
	return size;
}

void trie_free(trie_node *tn) {
	for (int i=0; !tn->is_leaf && i<TRIE_CHILDREN_SIZE; ++i){
		if (tn->children[i] != NULL) {
			trie_free(tn->children[i]);
			tn->children[i] = NULL;
		}
	}
	if (*tn->children) {
		free(tn->children);
		tn->children = NULL;
	}
	if (tn->v) {
		free(tn->v);
		tn->v = NULL;
	}
	tn->parent = NULL;
}

void trie_print_rec(trie_node *tn, char ***path, int *path_size, int i) {
	if (tn == NULL) {
		return;
	}
	if (!mystrcomp("", tn->v)) {
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

int idx_map(char c) {
	int idx = 0;
	if ('A' <= c && c <= 'Z') {
		idx = c-'A';
	} else if ('a' <= c && c <= 'z') {
		idx = c-'a' + 25;
	} else if ('0' <= c && c <= '9') {
		idx = c-'0' + 51;
	} else if (c == '_') {
		idx = 62;
	} else if (c == '-') {
		idx = 63;
	}
	return idx;
}

trie_node *trie_add(trie_node *tn, const char *v) {
	char *vc = (char *)v;
	size_t offset = 0;
	while (*vc != '\0') {
		++offset;
		int idx = idx_map(*vc);
		trie_node *child = tn->children[idx];
		if (child == NULL) {
			child = trie_new(tn, v, offset);
			tn->children[idx] = child;
		}
		if (tn->is_leaf) {
			tn->is_leaf = false;
		}
		tn = child;
		++vc;
	}
	tn->is_leaf = 1;
	return tn;
}

trie_node *trie_find(trie_node *tn, char *v, int *child_list_idx) {
	if (mystrcomp(v, tn->v)) {
		return tn;
	}
	if (!child_list_idx) {
		int tmp;
		child_list_idx = &tmp;
	}
	size_t vlen = mystrlen(v);
	for (int i=0; i<vlen; ++i) {
		if (tn->is_leaf) {
			return NULL;
		}
		*child_list_idx = idx_map(v[i]);
		trie_node *child = tn->children[*child_list_idx];
		if (!child) {
			return NULL;
		}
		tn = child;
	}
	return tn;
}

// return vals
// 1 - successful
// 0 - nothing to delete
// -1 - can't delete - node has children
int trie_delete(trie_node *tn, char *v){
	int idx;
	tn = trie_find(tn, v, &idx);
	if (!tn) {
		return 0;
	}
	if (!tn->is_leaf) {
		return -1;
	}
	trie_node *parent = tn->parent;
	trie_free(tn);
	parent->children[idx] = NULL;
	return 1;
}

#endif // TRIE_IMPLEMENTATION