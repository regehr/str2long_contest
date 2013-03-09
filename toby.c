#include "str2long.h"

/* check that 'increasing' x by y (taking into account the sign of x) won't
 * cause overflow, where 'increasing' means addition when neg is false and
 * subtraction when neg is true.
 *
 * preconditions:
 * y in {0..9}
 * when neg is true, x in {LONG_MIN..0}
 * when neg is false, x in {0..LONG_MAX}
 */
static unsigned int check_incr(const long x, const long y, 
                               const unsigned int neg) {
  assert(y >= 0L && y <= 9L);
  assert((neg && x <= 0L) || (!neg && x >= 0L));

  if (neg) {
    if ((LONG_MIN + y) > x) {
      return 0;
    }
  } else {
    if ((LONG_MAX - y) < x) {
      return 0;
    }
  }
  
  return 1;
}

/* check that multiplying x by 10 won't cause overflow, taking into account
 *the sign of x. returns 1 iff OK, 0 otherwise
 *
 * preconditions:
 * when neg is true, x in {LONG_MIN..0}
 * when neg is false, x in {0..LONG_MAX}
 */
static unsigned int check_mult(const long x,
                               const unsigned int neg) {

  assert((neg && x <= 0L) || (!neg && x >= 0L));

  if (neg) {
    if (x < (LONG_MIN / 10)) {
      return 0;
    }
  } else {
    if (x > (LONG_MAX / 10)) {
      return 0;
    }
  }
  return 1;
}

/* do like str2long, assuming that we have already parsed the possible
 * minus sign, whose presence is indicated here by neg 
 * (0 iff absent, 1 otherwise). 
 *
 * preconditions:
 * str points to a valid null-termianted string
 */
static long str2long_imp (const char * const str, const unsigned int neg) {
  unsigned int pos = 0;
  long result = 0;
  long c;

  if (str[0] == '\0') {
    // fails regexp
    error = 1;
    return 0;
  }

  while(1) {
    if(!(str[pos] >= '0' && str[pos] <= '9')) {
      error = 1; // fails regexp
      return result;
    }

    c = (long) (str[pos] - '0');  // c in {0..9}

    if (check_incr(result,c,neg)) {
      result = (neg ? result - c : result + c);
    } else {
      error = 1;
      return result;
    }

    pos++;
    if (str[pos] == '\0') {
      return result;
    } else {
      // we expect another character, so result needs to be *= 10
      if (check_mult(result,neg)) {
        result *= 10;
      } else {
        // result*10 would overflow
        error = 1;
        return result;
      }
    }
    // (on next loop iteration) if str[pos] notin [0-9] then fails regexp
  }
}


/* we assume that str points to a valid, null-terminated string. This implies
   that we won't get overflow when incrementing the index pos.
*/
long str2long_toby (const char *str) {
  unsigned int neg = 0;

  if (str[0] == '\0') {
    // fails regexp
    error = 1;
    return 0;
  }

  // since str[0] is not null, and we assume str points to a valid string,
  // we can safey increment str without fear of overflow

  if (str[0] == '-') {
    neg = 1;
    str+=1;
  }

  return str2long_imp(str,neg);
}

