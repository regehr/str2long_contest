#include "str2long.h"

/* I assume sizeof(long) is 4 or 8,  the code could be extended vor other values */
int check_sensible_long[-!(sizeof(long) == 4 || sizeof(long) == 8)];

static long long strlen_xx(const char *s) {
    const char *i = s;
    while (*i) ++i;
    return i - s;
}

/*
 * if input matches ^-?[0-9]+\0$ and the resulting integer is
 * representable as a long, return the integer; otherwise if
 * the input is a null-terminated string, set error to 1 and
 * return any value; otherwise behavior is undefined
 */
long str2long_olivier (const char *s)
{
    const char *ref = (sizeof(long) == 4) ? "2147483647" : "9223372036854775807";
    int isNeg = *s == '-';
    if (isNeg) ++s;
    if (*s == '0') {
        while (*s == '0') ++s;
        if (!*s) return 0;
    }

    long long refLen = strlen_xx(ref);
    long long sLen = strlen_xx(s);
    if (sLen == 0 || sLen > refLen) { error = 1; return 0; }
    if (sLen == refLen) {
        int i;
        for (i = 0; s[i] != 0; ++i)  {
            if (s[i] < ref[i]) break;
            if (s[i] == ref[i]) continue;
            if (isNeg && !ref[i+1] && s[i] == ref[i] + 1)
                return (sizeof(long) == 4) ? -2147483647 - 1 : -9223372036854775807LL - 1;
            error = 1;
            return 0;
        }
    }
    long result = 0;
    int i;
    for (i = 0; i < sLen; ++i)  {
        char c = s[i];
        if (c < '0' || c > '9') { error = 1; return 0; }
        result *= 10;
        result += c - '0';
    }
    return isNeg ? -result : result;
}

