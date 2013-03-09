#include "str2long.h"

long str2long_yang (const char *s)
{
  unsigned long v = 0;
  unsigned num_of_elem = 0;
  unsigned char neg = 0;
  unsigned char c;

  assert(s && "NULL string!");
  error = 0;

  if (*s == '-') {
    neg = 1;
    s++;
  }
  if (*s == '\0')
    goto err;

  /* skip the leading 0s, thus accept strings like
     0000000000000000000000000000000123 */
  while (*s == '0')
    s++;

  while ((c = *s++) != '\0') {
    if ((c < '0') || (c > '9'))
      goto err;

    num_of_elem++;
    if (num_of_elem > 19)
      goto err;

    c -= '0';
    v = v * 10 + c;
  }

  if (neg) {
    if (v > (-(unsigned long)(LONG_MIN)))
      goto err;
    else if (v == (unsigned long)(LONG_MIN))
      return v;
    return -((long)v);
  }
  else {
    if (v > (unsigned long)(LONG_MAX))
      goto err;
    return (long)v;
  }

err:
  error = 1;
  return -1;
}

