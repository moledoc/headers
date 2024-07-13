#define TRIE_IMPLEMENTATION
#include "./trie.h"

int main(void) {
	trie_node *root = trie_new(NULL, "root", 4);
	printf("Adding nodes to root\n");
	trie_add(root, "Str");
	trie_add(root, "Stra");
	trie_add(root, "str");
	trie_add(root, "String");
	trie_add(root, "Street");
	trie_print(root);

	int idx;
	trie_node *nd;

	char *str = "Strin";
	printf("\nFinding '%s':\n", str);
	nd = trie_find(root, str, &idx);
	if (!nd) {
		printf("%s:%d: Didn't find node '%s', but should've\n", __FILE__, __LINE__, str);
	}
	trie_print(nd);

	str = "St";
	printf("\nFinding '%s':\n",str);
	nd = trie_find(root, str, NULL);
	if (!nd) {
		printf("%s:%d: Didn't find node '%s', but should've\n", __FILE__, __LINE__, str);
	}
	trie_print(nd);

	int is_successful = 0;
	str = "Strin";
	printf("\nDeleting '%s'\n", str);
	is_successful = trie_delete(root, str);
	printf("Deleting '%s' return state: %d\n", str, is_successful);
	if (is_successful == -1) {
		printf("Node '%s' still has children - nothing was deleted\n", str);
	}
	printf("Node '%s' should still exist\n", str);
	nd = trie_find(root, str, &idx);
	if (!nd) {
		printf("%s:%d: Didn't find node '%s', but should've\n", __FILE__, __LINE__, str);
	}
	trie_print(nd);

	str = "String";
	printf("\nDeleting '%s'\n", str);
	is_successful = trie_delete(root, str);
	printf("Node '%s' return state: %d\n", str, is_successful);
	if (is_successful == 1) {
		printf("Node '%s' was deleted successfully\n", str);
	}
	printf("Node '%s' shouldn't exist\n", str);
	nd = trie_find(root, str, &idx);
	if (!nd) {
		printf("Didn't find node '%s' as expected\n", str);
	}
	trie_print(nd);

	str = "Tree";
	printf("\nDeleting '%s'\n", str);
	is_successful = trie_delete(root, str);
	printf("Node '%s' return state: %d\n", str, is_successful);
	if (is_successful == 0) {
		printf("Node '%s' was not found, nothing deleted\n", str);
	}

	trie_free(root);
}