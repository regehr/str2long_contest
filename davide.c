#include "str2long.h"

static unsigned long str2ulong(char const *str, unsigned long bound) {
  unsigned long n = 0;
  do {
    // this also catches an empty string on the first iteration
    if (!isdigit(*str)) {
      error = 1;
      break;
    }
    unsigned int digit = (unsigned int)(*str - '0');
    // is the multiply/add going to exceed bound?
    if (n > bound / 10 || (n == bound / 10 && digit > bound % 10)) {
      error = 1;
      break;
    }
    n = n * 10 + digit;
    str++;
  } while (*str != '\0');
  return n;
}

long str2long_davide(char const *str) {
  if (*str == '-') {
    unsigned long n = str2ulong(str + 1, -(unsigned long)LONG_MIN);
    // this avoids overflowing a long on negative zero
    // clang and gcc are smart enough to compile this to a single negq
    return n != 0 ? -1 - (long)(n - 1) : -(long)n;
  } else {
    return (long)str2ulong(str, LONG_MAX);
  }
}
