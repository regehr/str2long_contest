#include "str2long.h"

long str2long_matthewf(const char *s) {
    long val = 0;
    int negative = 0;

    if (*s == '-') {
        negative = 1;
        ++s;
    }

    for (;*s != '\0'; ++s) {
        if (*s < '0' || *s > '9') {
            /* Non-numeric character; bail out. */
            error = 1;
            return -1;
        } else {
            long d = *s - '0'; /* digit value */

            if (negative) {
                if ((LONG_MIN + d) / 10 > val) {
                    /* We're about to underflow. */
                    error = 1;
                    return -1;
                }
                val = val * 10 - d;
            } else {
                if ((LONG_MAX - d) / 10 < val) {
                    /* We're about to overflow. */
                    error = 1;
                    return -1;
                }
                val = val * 10 + d;
            }
        }
    }
    return val;
}
