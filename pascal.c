#include "str2long.h"

#define lim (-(unsigned long)LONG_MIN)

long str2long_pascal (const char *p)
{
  unsigned long l = 0;
  int neg = 0;
  if (!*p) goto err;
  if (*p == '-')
    {
      p++;
      neg = 1;
    }
 loop:
  if (*p < '0' || *p > '9') goto err;
  if (l > lim / 10) goto err;  
  l = 10 * l + (*p - '0');
  if (l > lim) goto err;
  p++;
  if (!*p) goto end;
  goto loop;
 end:
  if (neg)
    return -l;
  if (l > (unsigned long) LONG_MAX)
    goto err;
  return l;
 err:
  error = 1;
  return 0;
}

