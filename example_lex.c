#define LEX_IMPLEMENTATION 
#include "./lex.h"

int main(int argc, char **argv) {
	size_t token_counter = 0;
	char *fname = "test_lex.txt";
	FILE *fptr = fopen(fname, "r");
	LEX_FNAME=fname;
	LEX_SKIP_WHITESPACE=0;
	lex_token **tokens = lex_tokenize(fptr, &token_counter);
	fclose(fptr);
	lex_print(tokens, token_counter);
	lex_free(tokens, token_counter);
}

