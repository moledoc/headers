#define TOOLBOX_IMPLEMENTATION
#include "./handy.h"

int main(int argc, char **argv) {
	const char *pname = shift(&argc, &argv); // shift program name
	printf("program name='%s'\n",pname);
	printf("program name length=%d\n", mystrlen(pname));
	printf("program name is 'incorrect_name'=%d (0 is false, 1 is true)\n", mystrcomp("incorrect_name", pname));
	char c = 'b';
	printf("character '%c' (as int '%d') as binary representation: '%s'\n", c, c, ctob(c));
	c = 'c';
	printf("character '%c' (as int '%d') as binary representation: '%s'\n", c, c, ctob(c));

}