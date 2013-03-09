#include "str2long.h"

long str2long_yang_2 (const char *s)
{
  unsigned long v = 0;
  unsigned char neg = 0;
  unsigned char c;
  unsigned long max_val;

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

  if (sizeof(long) == 4)
    max_val = 300000000UL;
  else if (sizeof(long) == 8)
    max_val = (unsigned long)1000000000000000000ULL;
  else
    assert(0 && "Unsupported type of long!");

  while ((c = *s++) != '\0') {
    if ((c < '0') || (c > '9'))
      goto err;

    if ((v > max_val))
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
