#pragma once

// {
#ifdef UTILS // HEADER
// shift consumes one argv elem
// returns NULL if all elems are consumed
char *shift(int *argc, char ***argv);

// power calculates a^n
// if n < 0 or a < 0, then return 0 for now
long long int power(long long a, int n);

#endif // UTILS // HEADER
// }

// {
#ifdef UTILS // IMPLEMENTATION

char *shift(int *argc, char ***argv) {
  if (*argc <= 0) {
    return NULL;
  }
  char *result = **argv;
  *argc -= 1;
  *argv += 1;
  return result;
}

long long int power(long long a, int n) {
  if (n < 0 || a < 0) {
    return 0;
  }
  long long int res = 1;
  for (; n > 0; a *= a, n >>= 1) {
    if (n & 1) {
      res *= a;
    }
  }
  return res;
}

#endif // UTILS // IMPLEMENTATION

// }