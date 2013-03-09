#include "str2long.h"

long str2long_greg (const char *str)
{
    long sign = 1;
    long acc = 0;
    if (*str != '\0' && *str == '-') {
        sign = -1;
        str++;
    }

    if (*str == '\0')
        error = 1;

    while (*str != '\0') {
        long c = *str++ - '0';
        if (c < 0 || c > 9 ||
                (sign > 0 && acc > (LONG_MAX - sign*c) / 10L) ||
                (sign < 0 && acc < (LONG_MIN - sign*c) / 10L)) {
            error = 1;
            return 0;
        }
        acc = acc*10L + sign*c;
    }

    return acc;
}
