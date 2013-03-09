#include "str2long.h"

long str2long_patrick(const char *s) {
  long ret = 0;
  int neg = -1;
  if (*s == '-') { neg = 1; s++; }
  if (!*s) return (error=1);
  
  for (; *s; s++) {
    if (!isdigit(*s)) return (error=1);
    if (ret < LONG_MIN/10) return (error=1);
    ret = ret * 10;
    if (ret < LONG_MIN + (*s - '0')) return (error=1);
    ret -= (*s - '0');
  }
  if (neg == -1 && ret == LONG_MIN) return (error=1);
  return neg * ret;
}

