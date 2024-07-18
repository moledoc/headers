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
	- [ ] queue
	- [ ] stack
	- [x] map
- utils.h
	- [x] shift
	- [x] power
	- [x] lexer
- algo_misc.h
	- [x] two sum
- md5.h
	- [x] md5
- log.h
	- [x] logging to stderr
	- [x] logging to specified FILE
	- [x] logging to buffer

## Quick start

```sh
clang main.c -DSINGLY_LINKED_LIST && ./a.out --run all
clang main.c -DMAP && ./a.out --run all
clang main.c -DTWO_SUM && ./a.out
clang main.c -DMD5 && ./a.out
clang main.c -DLOG && ./a.out
clang main.c -DUTILS && ./a.out
clang main.c -DLEX && ./a.out
```

## Notes

* This repo is WIP, I`ll be adding things as time goes on.
* idea: try using sym-linking headers to other projects
* TODO: convert cool stuff from `depr` to new way

## Author

Meelis Utt