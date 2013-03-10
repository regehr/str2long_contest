#include "str2long.h"

long str2long_sidney (const char *in) {
    int negative = (*in == '-');
    long out = 0;

    if (negative) in++;

    if (!*in) goto failure;

    for (; *in; in++) {
        if (*in < '0' || *in > '9') goto failure;
        int next = *in - '0';
        if (negative) {
            if (out < LONG_MIN / 10 || LONG_MIN - (out *= 10) > -next
            ) goto failure;
            out -= next;
        } else {
            if (out > LONG_MAX / 10 || LONG_MAX - (out *= 10) < next
            ) goto failure;
            out += next;
        }
    }

    return out;

failure:
    error = 1;
    return 0;
}
