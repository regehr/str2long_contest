#include "str2long.h"

long str2long_davidl(const char *s) {
    int d, negate = 0;
    long x = 0;

    if (*s == '-') {
        s++;
    } else {
        negate = 1;
    }

    do {
        d = *s - '0';
        if (!(d >= 0 && d < 10 && x >= LONG_MIN / 10 && 10 * x >= LONG_MIN + d))
            goto error;

        x = x * 10 - d;
    } while (*++s);

    if (negate) {
        if (x < -LONG_MAX)
            goto error;
        x *= -1;
    }

    return x;

error:
    error = 1;
    return 0;
}
