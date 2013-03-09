#include "str2long.h"

long str2long_francois(const char* s)
{
  long negative = 0;
  long result = 0;

  error = 0;

  if (*s == '-')
    {
      negative = 1;
      ++s;
    }

  do
    {
      long digit;
      long tmp;

      if (*s < '0' || *s > '9')
        {
	  error = 1;
	  return 0;
        }

      digit = *s - '0';

      tmp = negative ? -result : result;

      if (tmp < LONG_MIN / 10 || tmp > LONG_MAX / 10)
        {
	  error = 1;
	  return 0;
        }

      tmp *= 10;

      if (( negative && tmp < LONG_MIN + digit) ||
	  (!negative && tmp > LONG_MAX - digit))
        {
	  error = 1;
	  return 0;
        }

      result = result * 10 + digit;
    }
  while (*++s);

  return negative ? -result : result;
}

