#include "str2long.h"

long str2long_andrewh (const char *s) {
  char c = *s++;
  unsigned long safe;
  long sign;
  unsigned long lim;
  if (c == '-') {
    c = *s++;
    safe = (LONG_MAX - (9 - 1)) / 10;
    lim = ((unsigned long)LONG_MAX) + 1;
    sign = -1;
  } else {
    safe = (LONG_MAX - 9) / 10 ;
    lim = LONG_MAX;
    sign = 0;
  }
  unsigned long curr = 0;
  // don't check overflow until it's a concern
  while (isdigit(c) && curr <= safe) {
    curr = 10 * curr + (c - '0');
    c = *s++;
  }

  if (c != '\0') {
    if (!isdigit(c)) {
      error = 1;
      return 0;
    }
    int digit = c - '0';
    if ((lim - digit) / 10 < curr) {
      error = 1;
      return 0;
    }
    curr = 10 * curr + digit;
  }

  // curr is correct and in range for its sign.  Now we just want to
  // negate it (if we need to.)  Rely on "usual implementation defined
  // behavior" and 2s-complement, i.e. either long -> unsigned long or
  // unsigned long -> long just keeps the same bits.
  // (This can incur unsigned overflow but that's defined to be OK.)
  long ret = ((curr ^ sign) - sign);
  return ret;
}
