#include "str2long.h"

long str2long_adam (const char *s) {
  long result = error = 0;
  long negative = *s == '-' ? ++s,-1 : 1;
  while (*s) {
    result *= 10;
    result += *s++ - 0x30;
    if ((result >> 1) > result) {
      if (!*s && negative == -1 && result == LONG_MIN) {
        return result;
      }
      return error = 1;
    }
  }
  return negative * result;
}
