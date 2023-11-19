#pragma once

#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <stdio.h>

typedef struct {
	char **elems;
	size_t size;
} filter;

typedef struct {
	char **files;
	size_t *fp_lens; // file path lengths
	size_t fsize; // files size
	size_t file_count; // current file count
	//
	char **dirs;
	size_t *dp_lens; // directory path lengths
	size_t dsize; // dirs size
	size_t dir_count; // current directory count
} ftree;

char *shift(int *argc, char ***argv);
void print_str_list(char **lst, size_t lst_size);
void free_str_list(char **lst, size_t lst_size);
size_t mystrlen(const char *s1);
int mystrcomp(const char *s1, const char *s2);
void mymemset(char *buf, int v, size_t size);
void mymemcpy(char *dest, char *src, size_t size);
int contains(char **ss, size_t sslen, char *s);
char **split(char *s, size_t slen, char sep, size_t *elem_count);
char *ctob(const char c1); // character to binary representation
char btoc(const char *b1); // binary representation to character
char **reloc(char **src, size_t *size); // REVIEWME: (and it's usages)
int walk(char *path, size_t path_size, ftree *ft, filter flt);
void ftree_free(ftree ft);
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

void print_str_list(char **lst, size_t lst_size) {
	for (int i=0; i<lst_size; ++i) {
		printf("%s\n", lst[i]);
	}
}

void free_str_list(char **lst, size_t lst_size) {
	for (int i=0; i<lst_size; ++i) {
		if (lst[i] != NULL) {
			continue;
		}
		free(lst[i]);
	}
	if (lst) {
		free(lst);
	}
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

void mymemcpy(char *dest, char *src, size_t size) {
	for (int i=0; i<size; ++i) {
		dest[i] = src[i];
	}
}

int contains(char **ss, size_t sslen, char *s) {
	for (int i=0; i<sslen; ++i) {
		if (!mystrcomp(ss[i], s)) {
			return 0;
		}
	}
	return 1;
}

char **split(char *s, size_t slen, char sep, size_t *elem_count) {
	int split_size = 256;
	int *split_ids = calloc(split_size, sizeof(int));
	int split_count = 0;
	for (int i=0; i<slen; ++i) {
		if (s[i] == sep) {
			split_ids[split_count] = i;
			++split_count;
			if (split_count > split_size) {
				split_size *= 2;
				split_ids = realloc(split_ids, split_size);
			}
		}
	}
	*elem_count = split_count + 1;
	char **elems = calloc(*elem_count, sizeof(char *));
	if (*elem_count == 1) {
		elems[0] = calloc(slen, sizeof(char));
		mymemcpy(elems[0], s, slen);
		return elems;
	}
	for (int i=0; i<split_count+1; ++i) {
		int len;
		int offset;
		if (i == 0) {
			len = split_ids[i];
			offset = 0;
		} else if (i == split_count) {
			len = slen - split_ids[i] - 1; // -1 to exclude sep (lower bound)
			offset = split_ids[i-1] + 1;
		} else {
			len = split_ids[i] -1 - split_ids[i-1]; // -1 to exclude sep from the range (lower bound)
			offset = split_ids[i-1] + 1; // +1 to exclude sep from the range (lower bound)
		}
		elems[i] = calloc(len, sizeof(char));
		for (int j=0; j<len; ++j) {
			elems[i][j] = s[offset+j];
		}
	}
	free(split_ids);
	return elems;		
}

char *ctob(const char c1) { // character to binary representation
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

char btoc(const char *b1) { // binary representation to character
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

char **reloc(char **src, size_t *size) { // REVIEWME: (and it's usages)
	char **dest = malloc(*size * 2 * sizeof(char *));
	for (int i=0; i<*size; ++i) {
		dest[i] = src[i];
	}
	free_str_list(src, *size);
	*size *= 2;
	return dest;
}

int walk(char *path, size_t path_size, ftree *ft, filter flt) {
	DIR *dp;
	struct dirent *ep;
	dp = opendir(path);
	if (!dp) {
		return 0;
	}
	size_t filter_elem_count = 0;
	while (ep = readdir(dp)) {
		if (mystrcomp(".", ep->d_name) || mystrcomp("..", ep->d_name) || 
			contains(flt.elems, flt.size, ep->d_name)) {
			continue;
		}
		size_t ep_name_len = mystrlen(ep->d_name);
		size_t new_path_size = path_size+ep_name_len+1;
		char *new_path = calloc(new_path_size, sizeof(char));

		mymemcpy(new_path, path, path_size);
		mymemcpy(new_path + (path_size+1), ep->d_name, ep_name_len);
		new_path[path_size] = '/';

		switch (ep->d_type) {
		case DT_DIR:
			walk(new_path, new_path_size, ft, flt);
			// MAYBE: can do without calloc?
			ft->dirs[ft->dir_count] = new_path;
			ft->dp_lens[ft->dir_count] = new_path_size;
			++(ft->dir_count);
			if (ft->dir_count >= ft->dsize) {
				ft->dirs = reloc(ft->dirs, &ft->dsize);
				ft->dp_lens = realloc(ft->dp_lens, ft->dsize);
			}
			break;
		case DT_REG:
			// MAYBE: can do without calloc?
			ft->files[ft->file_count] = new_path;
			ft->fp_lens[ft->file_count] = new_path_size;
			++(ft->file_count);
			if (ft->file_count >= ft->fsize) {
				ft->files = reloc(ft->files, &ft->fsize);
				ft->fp_lens = realloc(ft->fp_lens, ft->fsize);
			}
			break;
		}
	}
	(void) closedir(dp);
}

void ftree_free(ftree ft) {
	free_str_list(ft.dirs, ft.dir_count);
	free_str_list(ft.files, ft.file_count);
	free(ft.fp_lens);
	free(ft.dp_lens);
}

void ftree_print(ftree ft) {
	print_str_list(ft.dirs, ft.dir_count);
	print_str_list(ft.files, ft.file_count);
}

#endif // TOOLBOX_IMPLEMENTATION