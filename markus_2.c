#include "str2long.h"

long str2long_markus_2 (const char* str) {
    unsigned int idx;
    long out = 0;

    if (str[0] == '-') {
        for (idx=1; ((int)str[idx] >= (int)'0') && ((int)str[idx] <= (int)'9'); ++idx) {
            long digit = (long)str[idx] - (long)'0';
            out *= 10;

            if ((LONG_MIN - out) > -digit) {
                error = 1;
                return 0;
            }

            out -= digit;
        }
        if (idx == 1)
        { /* Loop didn't loop -> No digit or empty string. */
            error = 1;
            return 0;
        }
    }
    else {
        for (idx=0; ((int)str[idx] >= (int)'0') && ((int)str[idx] <= (int)'9'); ++idx) {
            long digit = (long)str[idx] - (long)'0';
            out *= 10;

            if ((LONG_MAX - out) < digit) {
                error = 1;
                return 0;
            }

            out += digit;
        }
        if (idx == 0)
        { /* Loop didn't loop -> No digit or empty string. */
            error = 1;
            return 0;
        }
    }

    return out;
}
