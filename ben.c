#include "str2long.h"

static int is_digit(char c) { return c >= '0' && c <= '9'; }

static int long_bits() {
  int rv = 0;
  unsigned long v = ULONG_MAX;
  while (v != 0) { v >>= 1; ++rv; }
  return rv;
}


long str2long_ben(const char* s) {
  int is_neg = 0;
  long rv, i, radix = 1, s_len;
  if (!s) goto err;

  // does input match ^-?[0-9]+ not including null terminator?
  {
    // first char must be - or a digit
    if (s[0] != '-' && !is_digit(s[0])) goto err;
    if (s[0] == '-') { is_neg = 1; ++s; }
    if (!is_digit(s[0])) goto err; // in particular, "-" alone is invalid.

    // trim leading zeros before doing length-based calculations.
    while (s[0] == '0') { ++s; }
        s_len  = strlen(s);
    if (s_len == 0) return 0;

    // make sure all the remaining chars are digits.
    int i;
    for (i = 0; i < s_len; ++i) { if (!is_digit(s[i])) goto err; }
  }

  // to be more portable, we could use sprintf(buf, "%ld", LONG_MAX),
  // at the cost of some code clarity (for getting the right buf size).
  const char* long_max_str = (long_bits() == 32)
                               ? "2147483647"
                               : "9223372036854775807";
  int long_max_len = strlen(long_max_str);
  if (s_len > long_max_len || s_len < 0) goto err;
  if (s_len == long_max_len) {
    // may or may not be OK, let's ask strncmp to compare everything
    // except the last digit:
    int r = strncmp(s, long_max_str, s_len - 1);
    if (r < 0) goto ok; // s was definitely smaller
    if (r > 0) goto err; // s was definitely larger

    // same length, need to consider the last digit.
    char last = s[s_len - 1];
    if (last <= '7') goto ok; // both trailing digits are '7'.
    if (last == '8' && is_neg) return LONG_MIN;
    goto err;
  }

ok: // time to go ahead, now that our preconditions are taken care of.
  i     = s_len - 1;
  rv    = s[i] - '0';
  radix = 1;
  while (i --> 0) {
    radix *= 10;
    rv    += (s[i] - '0') * radix;
  }
  return is_neg ? -rv : rv;

err:
  error = 1;
  return 0;
}
