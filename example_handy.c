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
		free(cb);
	}

	// split
	char split_me[3][32];
	mymemcpy(split_me[0], "", 0);
	mymemcpy(split_me[1], ".git", 4);
	mymemcpy(split_me[2], ".git|test", 9);
	mymemcpy(split_me[3], ".git|test|tst2", 14);
	for (int i=0; i<4; ++i) {
		printf("Spliting '%s'\n", split_me[i]);
		char **elems = NULL;
		size_t elems_count = split(split_me[i], '|', &elems);
		for (int j=0; j<elems_count; ++j) {
			printf("\t'%s'\n", elems[j]);
			free(elems[j]);
		}
		free(elems);
	}
// 	size_t elem_count = 0;
// 	char *split_me = ".git|test";
// 	char **elems = split(split_me, mystrlen(split_me), '|', &elem_count);
// 	printf("splitting '%s':\n", split_me);
// 	print_str_list(elems, elem_count);
// 	free_str_list(elems, elem_count);
// 
// 	elem_count = 0;
// 	split_me = ".git";
// 	elems = split(split_me, mystrlen(split_me), '|', &elem_count);
// 	printf("splitting '%s':\n", split_me);
// 	print_str_list(elems, elem_count);
// 	free_str_list(elems, elem_count);
// 
// 	elem_count = 0;
// 	split_me = "";
// 	elems = split(split_me, mystrlen(split_me), '|', &elem_count);
// 	printf("splitting '%s':\n", split_me);
// 	print_str_list(elems, elem_count);
// 	free_str_list(elems, elem_count);

// 	// walk
// 	char *root = "/home/utt/go"; // FIXME: prints junk at the end, even though walker contains correct data
// 	printf("Walking directory '%s':\n", root);
// 	ftree *ft = malloc(sizeof(ft));
// 	walk(root, ".git", ft);
// 
// 	ftree_print(ft);
// 	// check that path length are correct
// 	for (int i=0; i<ft->cur_files_count; ++i) {
// 		printf("file: %s, stored length: %d, calculated length: %d, <string.h>.strlen=%d\n", ft->files[i], ft->fp_lens[i], mystrlen(ft->files[i]), strlen(ft->files[i]));
// 	}
// 	free(ft);
}