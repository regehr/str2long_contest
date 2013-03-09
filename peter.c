#include "str2long.h"
#include <stdbool.h>

long str2long_peter(const char * str)
{
  bool negative = (*str == '-');
  if (negative)
    ++str;

  unsigned long result = 0;
  do
  {
    if (*str < '0' || *str > '9')
      goto fail;

    if (result > LONG_MAX / 10)
      goto fail;

    unsigned digit = *str - '0';
    result = result * 10 + digit;
  }
  while (*++str);

  if (result > (unsigned long)LONG_MAX + negative)
    goto fail;

  return negative ? -result : result;

fail:
  error = 1;
  return 0;
}
