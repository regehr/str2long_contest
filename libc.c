#include "str2long.h"


long str2long_libc(const char * str)
{
  // strtol allows leading whitespace and a plus sign,
  // which we must reject. This test also rejects the
  // empty string, so later we don't need to check for
  // (endptr == str).
  if ((*str != '-') && (*str < '0' || *str > '9'))
  {
    error = 1;
    return 0;
  }

  errno = 0;
  char * endptr;
  long result = strtol(str, &endptr, 10);
  if ((*endptr != '\0') || (errno != 0))
    error = 1;
  return result;
}

