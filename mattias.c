#include "str2long.h"

long str2long_mattias (const char *s)
{
  int sign = 1;
  if (*s == '-') {
    sign = -1;
    s++;
  }
  /* It is tempting to use unsigned long to collect the result, but it
     is not clear from the standard whether it can represent the
     magnitude of a signed long. C99 6.2.6.2 implies that it is
     possible to have -LONG_MIN > ULONG_MAX. */
  long r = 0;
  /* Digit values are contiguous in the source and execution character sets
     (C99 5.2.1 p3). Works even with EBCDIC. */
  const char *t;
  for (t = s; *t >= '0' && *t <= '9'; t++) {
    /* Loop invariant: r = sign * string_to_number(*s..*(t-1)) if t > s,
                           0                                   if t = s. */
    int digit = *t - '0';
    if (r < LONG_MIN / 10 || r > LONG_MAX / 10)
      break;
    r *= 10;
    if (r < LONG_MIN + digit || r > LONG_MAX - digit)
      break;
    r += digit * sign;
  }
  if (t == s || *t != '\0')
    error = 1;  /* No digits at all, or a trailing non-digit, or overflow. */
  return r;
}
