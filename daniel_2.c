#include "str2long.h"

long str2long_daniel_2(const char *str)
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

        /* check for decimal digit */
        if (c < '0' || c > '9') goto err;
        c -= '0';

        /* check for overflow when shifting over a digit */
        if (val && ULONG_MSB / val < 10ul) goto err;
        val *= 10ul;

        /* check for overflow when adding new digit */
        if (ULONG_MSB - val < c) goto err;
        val += c;
    } while (*str);

    /* largest representable negative - must have negative sign */
    if (val == ULONG_MSB && !neg) goto err;

    /* negate if necessary; assume 2's complement */
    if (neg) val = -val;

    /* conversion from unsigned to signed is implementation defined; assume 2's complement */
    return val;

err:
    error = 1;
    return 0;
}
