#include "str2long.h"

long str2long_bernd(const char *s)
{
  int plus;
  // LONG_(MAX|MIN) w/o LSD; equal but the optimizer should do his job
  unsigned long nmax;
  // LSD of LONG_(MAX|MIN); used for possible overflow detection
  unsigned cmax;
  unsigned long n = 0;
  // return value
  long r;

  // ^-?
  if (*s == '-') {
    ++s;
    plus = 0;
    nmax = -(LONG_MIN / 10);
    cmax = -(LONG_MIN + LONG_MIN / -10 * 10);
  } else {
    plus = 1;
    nmax = LONG_MAX / 10;
    cmax = LONG_MAX - LONG_MAX / 10 * 10;
  }
  // [0-9]
  if (*s < '0' || *s > '9') {
    goto invalid;
  }
  for (;;) {
    unsigned c = *s - '0';

    // resulting integer is representable as a long
    if (n < nmax || (n == nmax && c <= cmax)) {
      n = n * 10 + c;
    } else {
      goto invalid;
    }
    ++s;
    // \0$
    if (*s == '\0') {
      break;
    }
    // [0-9]+
    if (*s < '0' || *s > '9') {
      goto invalid;
    }
  }
  r = n;
  return plus ? r : -r;
  // otherwise set error to 1 and return any value
 invalid:
  error = 1;
  return 0;
}

