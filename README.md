# headers

Collection of C headers containing useful/cool functions/data structures/algorithms/ideas (`tools` going forward).
Part of this work is to also practice and compose said `tools`.

## General structure

`tools` are separated into header files by some logical grouping.
For example, list related data structures are in `list.h`.
The header file setup is inspired by header-only library style, but does diverge from it slightly.

## TOC

- list.h
	- [x] singly linked list
	- [ ] doubly linked list
	- [ ] singly circular linked list
	- [ ] doubly circular linked list
		- WIP
	- [x] queue
	- [x] stack
	- [x] map
- TODO: trees
	- [ ] binary
	- [ ] trie
	- [ ] b
	- maybe something else
- TODO: sorting
	- [ ] quick
	- maybe something else, eg: selection, bubble, insertion, merge
- TODO: searching
	- [ ] linear
	- [ ] binary
- TODO: strings
	- [ ] string struct with len
	- [ ] split by char
	- [ ] other utility funcs
- utils.h
	- [x] shift
	- [x] power
	- [x] lexer
- algo_misc.h
	- [x] two sum
	- [ ] other interesting algorithms/problems
- md5.h
	- [x] md5
- log.h
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

* This repo is WIP, I`ll be adding things as time goes on.
* Main things from deprecated headers have been ported, but couple interesting ideas was left. Please see this commit `4dcd5527ab18e1d06e7d04d8523c0cbe28f3951f` to see the `depr` dir as it is the last commit (currently) containing it.

## Author

Meelis Utt
