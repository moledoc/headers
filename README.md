# headers

Collection of C headers containing useful/cool functions/data structures/algorithms/ideas (`tools` going forward).
Part of this work is to also practice and compose said `tools`.

## General structure

`tools` are separated into header files by some logical grouping.
For example, list related data structures are in `list.h`, trees in `tree.h` etc.
The header file setup is inspired by header-only library style, but does diverge from it slightly.

## Contents

- list.h
	- [x] singly linked list `-DSINGLY_LINKED_LIST`
	- [ ] doubly linked list `-DDOUBLY_LINKED_LIST`
	- [ ] circular singly linked list `-DCIRCULAR_SINGLY_LINKED_LIST`
	- [x] circular doubly linked list `-DCIRCULAR_DOUBLY_LINKED_LIST`
	- [x] stack `-DSTACK`
	- [x] queue `-DQUEUE`
	- [ ] ring buffer/circular buffer (?) `-DRING_BUFFER
- map.h
	- [x] map `-DMAP`
ize
- TODO: trees
	- [ ] binary `-DBINARY_TREE`
	- [ ] trie `-DTRIE_TREE`
	- [ ] b `-DB_TREE`
	- [ ] red-black `-DRED_BLACK_TREE`
	- maybe something else
- TODO: sorting
	- [ ] quick `-DQUICK_SORT`
	- maybe something else, eg: selection, bubble, insertion, merge
- TODO: searching
	- [ ] linear `-DLINEAR_SEARCH`
	- [ ] binary `-DBINARY_SEARCH`
- TODO: strings `-DSTRINGS`
	- [ ] string struct with len
	- [ ] split by char
	- [ ] other utility funcs
- TODO: arena `-DARENA` (i.e. memory arena)
- TODO: MAYBE: really naive garbage collector type thingy
- utils.h `-DUTILS`
	- [x] shift
	- [x] power
- lex.h `-DLEX`
	- [x] tokenizing utilities
- algo_misc.h
	- [x] two sum `-DTWO_SUM`
	- [ ] other interesting algorithms/problems
	- [ ] Run-length encoding (RLE) `-DRLE`
- md5.h `-DMD5`
	- [x] md5
- log.h `-DLOG`
	- [x] logging to stderr
	- [x] logging to specified FILE
	- [x] logging to buffer

## Quick start

NOTE: incorrect at the moment.

```sh
clang main.c -DSINGLY_LINKED_LIST && ./a.out --run all
clang main.c -DCIRCULAR_DOUBLY_LINKED_LIST && ./a.out --run all
clang main.c -DMAP && ./a.out --run all
clang main.c -DTWO_SUM && ./a.out
clang main.c -DMD5 && ./a.out
clang main.c -DLOG && ./a.out
clang main.c -DUTILS && ./a.out
clang main.c -DLEX && ./a.out
clang main.c -DSTACK && ./a.out --run all
clang main.c -DQUEUE && ./a.out --run all
```

## Notes

* use arenas as allocators
	* _create functions should return arenas;
	* _create functions might make data structures less ergonomic atm
* Currently refactor in progress
	* making structs slimmer
	* relying more on an `apply` method
	* more unified and clearer NULL checks
	* malloc error handling
	* update tests to `apply` approach
* TODO: come back to proper testing approach
* This repo is WIP, I`ll be adding things as time goes on.
* Main things from deprecated headers have been ported, but couple interesting ideas was left. Please see this commit `4dcd5527ab18e1d06e7d04d8523c0cbe28f3951f` to see the `depr` dir as it is the last commit (currently) containing it.

## Author

Meelis Utt
