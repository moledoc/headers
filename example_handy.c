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
	size_t file_count = 0;
	char **dirs = calloc(256, sizeof(char *));
	size_t dir_count = 0;
	ftree ft = {files, 256, dirs, 256};
	walk("/home/utt/c", &ft, &file_count, &dir_count);
	for (int i=0; i<dir_count; ++i) {
		printf("dir: %s\n", dirs[i]);
		free(dirs[i]);
	}
	for (int i=0; i<file_count; ++i) {
		printf("file: %s\n", files[i]);
		free(files[i]);
	}
	free(files);
	free(dirs);
}