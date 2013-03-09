#include "str2long.h"

long str2long_daniel (const char *str)
{
#define ULONG_MSB (1ul << (sizeof(long) * CHAR_BIT - 1))
    int neg = 0;
    unsigned long val = 0;

    if (*str == '-') {
        neg = 1;
        str++;
    }

    do {
        unsigned c = *str++;
        unsigned long newval;

        /* not decimal digit */
        if (c < '0' || c > '9') goto err;

        /* this might overflow, but unsigned overflow is defined as modulo arithmetic */
        newval = val * 10ul + (c - '0');

        /* larger than max positive representable value */
        if (newval > ULONG_MSB) goto err;

        /* wraparound */
        if (val > newval) goto err;

        /* largest negative, but more input left */
        if (val == ULONG_MSB && *str) goto err;

        val = newval;
    } while (*str);

    /* largest representable negative - must have negative sign */
    if (val == ULONG_MSB && !neg) goto err;

    /* negate if necessary; assume 2's complement */
    if (neg) val = -val;

    error = 0;

    /* conversion from unsigned to signed is implementation defined; assume 2's complement */
    return val;

err:
    error = 1;
    return 0;
}
