#include "str2long.h"

/*
 * if input matches ^-?[0-9]+\0$ and the resulting integer is
 * representable as a long, return the integer; otherwise if
 * the input is a null-terminated string, set error to 1 and 
 * return any value; otherwise behavior is undefined
 */
long str2long_phil (const char *str)
{
  long l = 0;
  long sign = 1;
  long digit;

  if (*str == '-') {
    sign = -1;
    str++;
  }

  if (!(*str))
    goto fail;

  while (*str) {
    if (*str < '0' || '9' < *str)
      goto fail;
    digit = sign * (*str - '0');

    if (l > LONG_MAX / 10 || l < LONG_MIN / 10)
      goto fail;
    l *= 10;

    if ((sign == 1 && digit > LONG_MAX - l) || (sign == -1 && digit < LONG_MIN - l))
      goto fail;
    l += digit;

    str++;
  }

  return l;

 fail:
  error = 1;
  return l;
}
