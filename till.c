#include "str2long.h"

// This is used to hint clang/gcc as to which branch is likely
// to be taken.
// These builtins are not part of the c specs #define PURE_C to
// avoid using them.
#ifdef PURE_C
#  define likely(x)    (x)
#  define unlikely(x)  (x)
#else
#  define likely(x)    __builtin_expect (!!(x), 1)
#  define unlikely(x)  __builtin_expect (!!(x), 0)
#endif

// Both set to 1 to respect the specs.
#define ERR_OVERFLOW 1
#define ERR_PARSE 1

// We can't import stdlib.h.
inline int abs(int x) { return (x<0)?-x:x; }

// Parsing a positive number
static inline long str2long_pos(const char *in) {
  long res = 0;
  do {
    const unsigned char digit =  *(in++) - '0';

    // Overflows wrap around on unsigned ints.
    if (unlikely(digit > 9)) {
      error = ERR_PARSE;
      return res;
    }

    if (unlikely(res >= (LONG_MAX / 10)) &&
        ((res > (LONG_MAX / 10)) ||
         (digit > abs(LONG_MAX % 10)))) {
      error = ERR_OVERFLOW;
      return LONG_MAX;
    }
    res = res * 10 + digit;
  } while (*in);
  error = 0;
  return res;
}

// Parsing a negative number
static inline long str2long_neg(const char *in) {
  long res = 0;
  do {
    const unsigned char digit =  *(in++) - '0';

    if (unlikely(digit > 9)) {
      error = ERR_PARSE;
      return res;
    }

    if (unlikely(res <= (LONG_MIN / 10)) &&
        ((res < (LONG_MIN / 10)) ||
         (digit > (abs(LONG_MIN % 10))))) {
      error = ERR_OVERFLOW;
      return LONG_MIN;
    }
    res = res * 10 - digit;
  } while (*in);
  error = 0;
  return res;
}

long str2long_till (const char *in){
  if (*in == '-') {
    return str2long_neg(in+1);
  } else {
    return str2long_pos(in);
  }
}
