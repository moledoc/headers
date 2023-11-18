#define TOOLBOX_IMPLEMENTATION
#include "./handy.h"
#include <string.h>

int main(int argc, char **argv) {
	// shift
	const char *pname = shift(&argc, &argv); // shift program name
	printf("program name='%s'\n",pname);

	// mystrlen
	printf("program name length=%d, <string.h>.strlen=%d\n", mystrlen(pname), strlen(pname));

	// mystrcomp
	printf("program name is 'incorrect_name'=%d (0 is false, 1 is true)\n", mystrcomp("incorrect_name", pname));

	// ctob and btoc
	for (char c='a'; c < 'c'; ++c){
		char *cb = ctob(c);
		char bc = btoc(cb);
		printf("character '%c' (as int '%d') as binary representation: '%s'\n", c, c, cb);
		printf("binary representation of '%s' as char '%c' (as int '%d')\n", cb, bc, bc);
	}

	// split
	size_t elem_count = 0;
	char *filter = ".git|test";
	char **elems = split(filter, mystrlen(filter), '|', &elem_count);
	printf("splitting '%s':\n", filter);
	print_str_list(elems, elem_count);
	free_str_list(elems, elem_count);

	elem_count = 0;
	filter = ".git";
	elems = split(filter, mystrlen(filter), '|', &elem_count);
	printf("splitting '%s':\n", filter);
	print_str_list(elems, elem_count);
	free_str_list(elems, elem_count);

	elem_count = 0;
	filter = "";
	elems = split(filter, mystrlen(filter), '|', &elem_count);
	printf("splitting '%s':\n", filter);
	print_str_list(elems, elem_count);
	free_str_list(elems, elem_count);

	// walk
	char *root = ".";
	printf("Walking directory '%s':\n", root);
	char **files = calloc(256, sizeof(char *));
	char **dirs = calloc(256, sizeof(char *));
	size_t *fp_lens = calloc(256, sizeof(size_t));
	size_t *dp_lens = calloc(256, sizeof(size_t));
	ftree ft = {files, fp_lens, 256, 0, dirs, dp_lens, 256, 0};
	walk(root, &ft, ".git");
	ftree_print(ft);
	// check that path length are correct
	for (int i=0; i<ft.file_count; ++i) {
		printf("file: %s, stored length: %d, calculated length: %d, <string.h>.strlen=%d\n", ft.files[i], ft.fp_lens[i], mystrlen(ft.files[i]), strlen(ft.files[i]));
	}
	ftree_free(ft);
}