#include "str2long.h"
#include <stdbool.h>

/*
 * if input matches ^-?[0-9]+\0$ and the resulting integer is
 * representable as a long, return the integer; otherwise if
 * the input is a null-terminated string, set error to 1 and
 * return any value; otherwise behavior is undefined
 */
long str2long_ryan (const char *str)
{
    bool negative = false;
    if (*str == '-') {
        negative = true;
        str++;
    }
    const unsigned long max = (unsigned long)LONG_MAX + negative;
    unsigned long accum = 0;
    do {
        const char ch = *str;
        if (ch < '0' || ch > '9') {
            /* Invalid character (or end-of-string). */
            error = 1;
            return 0;
        }
        if (accum > max / 10) {
            error = 1;
            return 0;
        }
        accum = accum * 10 + (ch - '0');
    } while (*++str != '\0');
    if (accum > max) {
        error = 1;
        return 0;
    }
    return (long)(negative ? -accum : accum);
} 
