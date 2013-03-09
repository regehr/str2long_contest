#include "str2long.h"

#if 0

long str2long_jeffrey(const char *str) {
    bool negative = false;
    if (str[0] == '-') {
        negative = true;
        ++str;
    }
    if (*str == '\0')
        return error = 1;
    unsigned long accum = 0;
    char c;
    while (c = *str++) {
        if (c < '0' || c > '9')
            return error = 1;
        unsigned long prev_accum = accum;
        accum = accum * 10 + (c - '0');
        if (accum < prev_accum)  // Overflow.
            return error = 1;
    }

    long result;
    if (negative) {
        result = -accum;  // Implementation-defined, likely 2's complement.
        if (result > 0)  // Overflow.
            return error = 1;
    } else {
        result = accum;  // Implementation-defined, likely 2's complement.
        if (result < 0)  // Overflow.
            return error = 1;
    }
    return result;
}

#else

//This one generates worse code, but is slightly shorter and doesn't
//depend on the unsigned->signed conversion:

#include <limits.h>
long str2long_jeffrey(const char *str) {
    bool positive = true;
    if (str[0] == '-') {
        positive = false;
        ++str;
    }
    if (*str == '\0')
        return error = 1;
    long accum = 0;
    char c;
    while (c = *str++) {
        if (c < '0' || c > '9')
            return error = 1;
        if ((LONG_MIN + (c - '0')) / 10 > accum)
            return error = 1;
        accum = accum * 10 - (c - '0');
    }
    if (positive) {
        if (accum < -LONG_MAX)
            return error = 1;
        return -accum;
    }
    return accum;
}

#endif
