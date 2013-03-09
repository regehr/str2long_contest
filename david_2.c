#include "str2long.h"

#include <stdbool.h>

// Assumption: ULONG_MAX is greater than or equal to LONG_MAX
// and the absolute value of LONG_MIN.  That's what allows us to
// use an unsigned long to keep track of the magnitude.

// Assumption: LONG_MAX + LONG_MIN == -1
// That is used when we compute the mag_limit and also in the
// last block when we figure out how to negate the magnitude. 

long str2long_david_2(const char * str)
{
  bool negative = false;
  if (*str == '-')
  {
    negative = true;
    str++;
  }

  if (*str == 0)
  {
    error = 1; // No digits.
    return 0;
  }

  unsigned long mag = 0;  // Absolute value of the number.
  unsigned long mag_limit = (unsigned long)LONG_MAX + negative;
  while(*str)
  {
    char c = *str++;
    if (c < '0' || c > '9')
    {
      error = 1; // Invalid character.
      return 0;
    }

    unsigned char digit = c - '0';
    if (mag > (mag_limit - digit)/10)
    {
      error = 1; // Number not expressible as long.
      return 0;
    }

    mag = mag * 10 + digit;
  }

  if(!negative)
  {
    return mag;
  }
  else if (mag == mag_limit)
  {
    // Cannot do (long)mag because mag is too big.
    return LONG_MIN;
  }
  else
  {
    return -(long)mag;
  }
}

