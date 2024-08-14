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
	- [ ] circular doubly linked list `-DCIRCULAR_DOUBLY_LINKED_LIST`
		- WIP
		- TODO: refactor to apply
	- [x] stack `-DSTACK`
	- [x] queue `-DQUEUE`
	- [x] map `-DMAP`
		- TODO: refactor to apply
			- also allow NULL as value, to enable analog to go's eg `map[string]struct{}` construct
		- TODO: implement array resizing (up/down) and re-org on data after resize
	- [ ] ring buffer/circular buffer (?) `-DRING_BUFFER
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

* Currently refactor in progress
	* making structs slimmer
	* relying more on an `apply` method
	* more unified and clearer NULL checks
	* malloc error handling
	* update tests to `apply` approach
* This repo is WIP, I`ll be adding things as time goes on.
* Main things from deprecated headers have been ported, but couple interesting ideas was left. Please see this commit `4dcd5527ab18e1d06e7d04d8523c0cbe28f3951f` to see the `depr` dir as it is the last commit (currently) containing it.

## TODOs

- Make main runnable, when all define flags are provided
```sh
clang main.c -DSINGLY_LINKED_LIST -DCIRCULAR_DOUBLY_LINKED_LIST -DMAP -DTWO_SUM -DMD5 -DLOG -DUTILS -DLEX -DSTACK -DQUEUE && ./a.out
```
- NOTE: the following is working, so need to look into `-DLEX`, `-DCIRCULAR_DOUBLY_LINKED_LIST`, `-DTWO_SUM`
```sh
clang main.c -DSINGLY_LINKED_LIST -DMAP -DMD5 -DLOG -DUTILS -DSTACK -DQUEUE && ./a.out
```
- NOTE: there is a memory leak according to `valgrind`, when the above command is ran - need to look into that.


## Author

Meelis Utt
