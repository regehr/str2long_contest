#include "str2long.h"

long str2long_john (const char *s)
{
  int sign = 1;
  if  (s[0] == '-') {
    sign = -1;
    s++;
  } 
  const char *pos = s;
  while (*pos != 0) { 
    if (*pos < '0' || *pos > '9') goto err;
    pos++;
  }
  if (pos == s)  goto err;
  pos--;
  long result = 0;
  long mul = 1;
  int overflow = 0;
  while (1) {
    long num = *pos - '0';
    if (num != 0 && overflow) goto err;
    unsigned long inc = (unsigned long) num * (unsigned long) mul;
    if (((long)inc / mul) != num) goto err;
    result = (unsigned long) result + (unsigned long)(sign * inc);
    if (result < 0 && sign == 1) goto err;
    if (result > 0 && sign == -1) goto err;
    unsigned long umul = mul;
    umul *= 10;
    if (((long)umul / 10) != mul) overflow = 1;
    mul = (long) umul;
    if (pos==s) break;
    pos--;    
  } 
  return result;
 err:
  error = 1;
  return 0;
}

