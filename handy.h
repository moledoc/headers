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
	size_t cur_files_count; // current file count
	//
	char **dirs;
	size_t *dp_lens; // directory path lengths
	size_t dsize; // dirs size
	size_t cur_dirs_count; // current directory count
} ftree;

// char *shift(int *argc, char ***argv);
// void print_str_list(char **lst, size_t lst_size);
// void free_str_list(char **lst, size_t lst_size);
// size_t mystrlen(const char *s1);
// int mystrcomp(const char *s1, const char *s2);
// void mymemset(char *buf, int v, size_t size);
// void mymemcpy(char *dest, char *src, size_t size);
// int contains(char **ss, size_t sslen, char *s);
// // char **split(char *s, size_t slen, char sep, size_t *elem_count);
// void split(char *s, size_t slen, char sep, char ***filter_elems, size_t *elem_count);
// char *ctob(const char c1); // character to binary representation
// char btoc(const char *b1); // binary representation to character
// void _walk_counter(char *path, size_t path_size, size_t *files_count, size_t *dirs_count, filter flt);
// void _walker(char *path, size_t path_size, ftree *ft, filter flt);
// void walk(char *path, char* filter_str, ftree *); // '|' is used as sep in filter_str
// void ftree_free(ftree *ft);
// void ftree_print(ftree *ft);

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
		if (!lst[i]) {
			continue;
		}
		free(lst[i]);
		lst[i] = NULL;
	}
	if (lst) {
		free(lst);
		lst = NULL;
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

size_t split(char *s, char sep, char ***elems) {
	size_t slen = mystrlen(s);
	char *ss = calloc(slen+2, sizeof(char));
	mymemcpy(ss, s, slen);
	ss[slen] = sep;
	ss[slen+1] = '\0';
	size_t elems_count = 0;
	char **es = calloc(elems_count, sizeof(char *));
	int offset = 0;
	for (int i=0; i<slen+1; ++i) {
		if (ss[i] == sep) {
			int len = i-offset;
			es[elems_count] = calloc(len + 1, sizeof(char));
			mymemcpy(es[elems_count], ss + offset, len);
			es[elems_count][len] = '\0';
			offset = i+1;
			++elems_count;
		}
	}
	free(ss);
	*elems = es;
	return elems_count;
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

// void _walk_counter(char *path, size_t path_size, size_t *files_count, size_t *dirs_count, filter flt) {
// 	DIR *dp;
// 	struct dirent *ep;
// 	dp = opendir(path);
// 	if (!dp) {
// 		return;
// 	}
// 	while (ep = readdir(dp)) {
// 		if (mystrcomp(".", ep->d_name) || mystrcomp("..", ep->d_name) || 
// 			contains(flt.elems, flt.size, ep->d_name)) {
// 			continue;
// 		}
// 		size_t ep_name_len = mystrlen(ep->d_name);
// 		size_t new_path_size = path_size+ep_name_len+1;
// 		char new_path[new_path_size+1];
// 		new_path[new_path_size] = '\0';
// 
// 		mymemcpy(new_path, path, path_size);
// 		new_path[path_size] = '/';
// 		mymemcpy(new_path + (path_size+1), ep->d_name, ep_name_len);
// 
// 		switch (ep->d_type) {
// 		case DT_DIR:
// 			++(*dirs_count);
// 			_walk_counter(new_path, new_path_size, files_count, dirs_count, flt);
// 			break;
// 		case DT_REG:
// 			++(*files_count);
// 			break;
// 		}
// 	}
// 	(void) closedir(dp);
// }
// 
// void _walker(char *path, size_t path_size, ftree *ft, filter flt) {
// 	DIR *dp;
// 	struct dirent *ep;
// 	dp = opendir(path);
// 	if (!dp) {
// 		return;
// 	}
// 	while (ep = readdir(dp)) {
// 		if (mystrcomp(".", ep->d_name) || mystrcomp("..", ep->d_name) || 
// 			contains(flt.elems, flt.size, ep->d_name)) {
// 			continue;
// 		}
// 		size_t ep_name_len = mystrlen(ep->d_name);
// 		size_t new_path_size = path_size+ep_name_len+1;
// 		char new_path[new_path_size+1];
// 		new_path[new_path_size] = '\0';
// 
// 		mymemcpy(new_path, path, path_size);
// 		new_path[path_size] = '/';
// 		mymemcpy(new_path + (path_size+1), ep->d_name, ep_name_len);
// 
// 		switch (ep->d_type) {
// 		case DT_DIR:
// 			ft->dirs[ft->cur_dirs_count] = calloc(new_path_size+1, sizeof(char));
// 			ft->dirs[ft->cur_dirs_count][new_path_size] = '\0';
// 			for (int i=0; i<new_path_size; ++i) {
// 				ft->dirs[ft->cur_dirs_count][i] = new_path[i];
// 			}
// 			ft->dp_lens[ft->cur_dirs_count] = new_path_size;
// 			++(ft->cur_dirs_count);
// 			_walker(new_path, new_path_size, ft, flt);
// 			break;
// 		case DT_REG:
// 			ft->files[ft->cur_files_count] = calloc(new_path_size+1, sizeof(char));
// 			ft->files[ft->cur_files_count][new_path_size] = '\0';
// 			for (int i=0; i<new_path_size; ++i) {
// 				ft->files[ft->cur_files_count][i] = new_path[i];
// 			}
// 			ft->fp_lens[ft->cur_files_count] = new_path_size;
// 			++(ft->cur_files_count);
// 			break;
// 		}
// 	}
// 	(void) closedir(dp);
// }
// 
// void walk(char *path, char* filter_str, ftree *ft) { // '|' is used as sep in filter_str
// 	size_t filter_elems_count = 0;
// 	// char **filter_elems = split(filter_str, mystrlen(filter_str), '|', &filter_elems_count);
// 	char ***filter_elems;
// 	split(filter_str, mystrlen(filter_str), '|', filter_elems, &filter_elems_count)
// 	filter flt = {filter_elems, filter_elems_count};
// 
// 	size_t files_count = 0;
// 	size_t dirs_count = 0;
// 	size_t path_len = mystrlen(path);
// 	_walk_counter(path, path_len, &files_count, &dirs_count, flt);
// 
// 	char *files[files_count];
// 	char *dirs[files_count];
// 	size_t fp_lens[files_count];
// 	size_t dp_lens[dirs_count];
// 	ft->files = files;
// 	ft->fp_lens = fp_lens;
// 	ft->fsize = files_count;
// 	ft->cur_files_count = 0;
// 	ft->dirs = dirs;
// 	ft->dp_lens = dp_lens;
// 	ft->dsize = dirs_count;
// 	ft->cur_dirs_count = 0;
// 	_walker(path, path_len, ft, flt);
// 	free_str_list(filter_elems, filter_elems_count);
// }

void ftree_free(ftree *ft) {
	free_str_list(ft->dirs, ft->cur_dirs_count);
	free_str_list(ft->files, ft->cur_files_count);
	free(ft->fp_lens); 
	ft->fp_lens = NULL;
	free(ft->dp_lens);
	ft->dp_lens = NULL;
}

void ftree_print(ftree *ft) {
	print_str_list(ft->dirs, ft->cur_dirs_count);
	print_str_list(ft->files, ft->cur_files_count);
}

#endif // TOOLBOX_IMPLEMENTATION