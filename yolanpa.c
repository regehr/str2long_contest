#include "str2long.h"

long str2long_yolanpa (const char * a)
{
    long s=0;
    unsigned long v=0;
    if (*a=='-') { s=1; a++; }
    do {
        if (*a >= '0' &&
            *a <= '9' &&
           (v < LONG_MAX/10 ||
           (v == LONG_MAX/10 && *a-'0' <= LONG_MAX%10+s)))
            v = v*10 + (*a-'0');
        else
            return error = 1;
    } while (*++a);
    return s ? (long)(~v+1) : (long)v;
}

#if 0
long str2long_yolanpa(const char * a)
{
  unsigned long s=0;
  unsigned long v=0;
  if (*a=='-') { s=1; a++; }
  for (;;) {
    if (*a >= '0' &&
	*a <= '9' &&
	(v < LONG_MAX/10 ||
	 (v == LONG_MAX/10 && *a-'0' <= LONG_MAX%10+s))) {
      v = v*10 + (*a-'0');
      a++;
    } else {
      error = 1;
      return 0;
    }
    if (*a == 0) {
      error = 0;
      return s ? (long)(~v+1) : (long)v;
    }
  }
}
#endif
