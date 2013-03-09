#include "str2long.h"

long str2long_andrew (const char *str) {
    static const long TENTH_MAX = LONG_MAX / 10;
    int minus = (*str == '-');
    const char *parse = str + minus;
    char c = *parse++;
    long value = 0;
    int max_digit = 9;
    int digit;

    if (!c)
        goto reterr;

    digit = c - '0';

    while ((c = *parse++)) {
        if (digit < 0 || digit > max_digit)
            goto reterr;

        value = value * 10 + digit;
        if (value > TENTH_MAX)
            goto reterr;

        digit = c - '0';

        if (value == TENTH_MAX) {
            if (*parse)
                goto reterr;
            max_digit = LONG_MAX - TENTH_MAX * 10 + minus;
            break;
        }
    }

    if (digit < 0 || digit > max_digit)
        goto reterr;

    if (minus && value == TENTH_MAX && digit == max_digit)
        return LONG_MIN;

    value = value * 10 + digit;

    return minus ? -value : value;

reterr:
    error = 1;
    return 0;
}

