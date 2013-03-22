// Copyright 2013 David Eisenstat
#include "str2long.h"

#include <limits.h>

int error;

static inline long longfromulong(unsigned long n) {
  return n > (unsigned long)LONG_MAX ? ~(long)~n : (long)n;
}

long str2long_davide_3(char const *s) {
  switch (1) {
  case 0:
  case LONG_MIN != -LONG_MAX:
    break;
  }
  char const *t = s + (*s == '-');
  unsigned long n = 0;
  do {
    unsigned int d = (unsigned int)*t - '0';
    // (n >> 60) != 0 trades speed for space
    if (d > 9 || n > (-(unsigned long)LONG_MIN) / 10) goto err;
    n = n * 10 + d;
    t++;
  } while (*t != '\0');
  if (*s == '-') {
    n = -n;
    if (longfromulong(n) <= 0) goto out;
  } else {
    if (longfromulong(n) >= 0) goto out;
  }
 err:
  error = 1;
 out:
  return longfromulong(n);
}
