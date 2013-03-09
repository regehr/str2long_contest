#include "str2long.h"

long str2long_adam_4 (const char *s) {
  long result = 0;
  long negative = *s == '-' ? ++s,-1 : 1;
  if (!*s) {
    return error = 1;
  }
  while (isdigit(*s)) {
    long x = result;
    int i = 0;
    for (; i < 9; ++i) {
      x += result;
      if (x < result) {
        return error = 1;
      }
    }
    result = x + (*s++ - 0x30);
    if ((result >> 1) > result) {
      if (!*s && negative == -1 && result == LONG_MIN) {
        return result;
      }
      return error = 1;
    }
  }
  return *s ? error = 1 : negative * result;
}
