#pragma once

#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <stdio.h>

char *shift(int *argc, char ***argv);
size_t mystrlen(const char *s1);
int mystrcomp(const char *s1, const char *s2);
void mymemset(char *buf, int v, size_t size);
char *ctob(const char c1); // character to binary representation
char btoc(const char *b1); // binary representation to character

typedef struct {
	char **files;
	size_t fsize;
	size_t file_count;
	char **dirs;
	size_t dsize;
	size_t dir_count;
} ftree;

int walk(char *path, ftree *ft);
void ftree_free_list(char **lst, size_t lst_size);
void ftree_free(ftree ft);
void ftree_print_list(char **lst, size_t lst_size);
void ftree_print(ftree ft);

#endif // TOOLBOX_H_

#ifdef TOOLBOX_IMPLEMENTATION

#include <stdlib.h>
#include <dirent.h>

char *shift(int *argc, char ***argv) {
	if (*argc <= 0) {
		return NULL;
	}
	char *result = **argv;
	*argc -= 1;
	*argv += 1;
	return result;
}

size_t mystrlen(const char *s1) {
	char *s1c = (char*)s1;
	size_t size = 0;
	while (*(s1c++) != '\0') {
		++size;
	}
	return size;
}

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

void mymemset(char *buf, int v, size_t size) {
	for (size_t i=0; i<size; ++i) {
		buf[i] = v;
	}
}

char *ctob(const char c1) {
	char c1c = (char)c1;
	size_t blen = 0;
	while ( c1c >>= 1 > 0) {
		++blen;
	}
	blen += 1; // +1 for index 0;
	c1c = (char)c1; 
	char *b = calloc(blen, sizeof(char));
	for (int i=0; i<blen;++i) {
		b[blen-i-1] = c1c & 1 ? '1' : '0';
		c1c >>= 1;
	}
	return b;	
}

char btoc(const char *b1) {
	char *b1c = (char *)b1;
	int blen = mystrlen(b1c);
	int pow = 2;
	char c = 0;
	for (int i=blen-1-1; i>=0; --i) { // -1: len-1; -1: handle bin repr index 0 separately
		c += (b1c[i] == '1') * pow;
		pow *= 2;
	}
	c += (b1c[blen-1] == '1');
	return c;
}

int walk(char *path, ftree *ft) {
	DIR *dp;
	struct dirent *ep;
	dp = opendir(path);
	if (!dp) {
		return 0;
	}
	size_t path_size = 0;
	while (path[path_size] != '\0') {
		++path_size;
	}
	while (ep = readdir(dp)) {
		if (mystrcomp(".", ep->d_name) || mystrcomp("..", ep->d_name)) {
			continue;
		}
		// MAYBE: extract to something like strcat
		size_t new_path_size = path_size+ep->d_reclen+1;
		char new_path[new_path_size];
		for (int i=0; i<new_path_size; ++i) {
			if (i < path_size) {
				new_path[i] = path[i];
			} else if (i == path_size) {
				new_path[i] = '/';
			} else {
				new_path[i] = ep->d_name[i-1-path_size];
			}
		}
		switch (ep->d_type) {
		case DT_DIR:
			walk(new_path, ft);
			// MAYBE: can do without calloc?
			ft->dirs[ft->dir_count] = calloc(new_path_size, sizeof(char));
			for (int i=0; i<new_path_size; ++i) {
				ft->dirs[ft->dir_count][i] = new_path[i];
			}
			++(ft->dir_count);
			if (ft->dir_count > ft->dsize) {
				ft->dsize *= 2;
				ft->dirs = realloc(ft->dirs, ft->dsize);
			}
			break;
		case DT_REG:
			// MAYBE: can do without calloc?
			ft->files[ft->file_count] = calloc(new_path_size, sizeof(char));
			for (int i=0; i<new_path_size; ++i) {
				ft->files[ft->file_count][i] = new_path[i];
			}
			++(ft->file_count);
			if (ft->file_count > ft->fsize) {
				ft->fsize *= 2;
				ft->files = realloc(ft->files, ft->fsize);
			}
			break;
		}
	}
	(void) closedir(dp);
}

void ftree_free_list(char **lst, size_t lst_size) {
	for (int i=0; i<lst_size; ++i) {
		free(lst[i]);
	}
}

void ftree_free(ftree ft) {
	ftree_free_list(ft.dirs, ft.dir_count);
	ftree_free_list(ft.files, ft.file_count);
	free(ft.files);
	free(ft.dirs);
}

void ftree_print_list(char **lst, size_t lst_size) {
	for (int i=0; i<lst_size; ++i) {
		printf("%s\n", lst[i]);
	}
}

void ftree_print(ftree ft) {
	ftree_print_list(ft.dirs, ft.dir_count);
	ftree_print_list(ft.files, ft.file_count);
}

#endif // TOOLBOX_IMPLEMENTATION