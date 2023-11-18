#define TOOLBOX_IMPLEMENTATION
#include "./handy.h"

int main(int argc, char **argv) {
	// shift
	const char *pname = shift(&argc, &argv); // shift program name
	printf("program name='%s'\n",pname);

	// mystrlen
	printf("program name length=%d\n", mystrlen(pname));

	// mystrcomp
	printf("program name is 'incorrect_name'=%d (0 is false, 1 is true)\n", mystrcomp("incorrect_name", pname));

	// ctob and btoc
	for (char c='a'; c < 'c'; ++c){
		char *cb = ctob(c);
		char bc = btoc(cb);
		printf("character '%c' (as int '%d') as binary representation: '%s'\n", c, c, cb);
		printf("binary representation of '%s' as char '%c' (as int '%d')\n", cb, bc, bc);
	}

	// walk
	char **files = calloc(256, sizeof(char *));
	char **dirs = calloc(256, sizeof(char *));
	ftree ft = {files, 256, 0, dirs, 256, 0};
	walk(".", &ft);
	ftree_print(ft);
	//ftree_free(ft);
}