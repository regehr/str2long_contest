#include "str2long.h"

/*
 * if input matches ^-?[0-9]+\0$ and the resulting integer is
 * representable as a long, return the integer; otherwise if
 * the input is a null-terminated string, set error to 1 and
 * return any value; otherwise behavior is undefined
 */
long str2long_davec(const char *str) {
    int i = 0;
    long n = 0, m = 1;
    char *p= (char *)str, c;

    if (p[0] == '-') {
        p++;
        m = -1;
        if (!p[0]) {
            goto err;
        }
    } 

    while((c = *p++)) {
        if (c >= '0' && c <= '9') {
            if (i > 0) { n *= 10; }
            n += m * (c - '0'); 
        } else {
            goto err;
        }

        /* detect overflow */
        if (n && ((m == -1 && n > 0) || (m == 1 && n < 0))) {
            goto err;
        }

        i++;
    }

    return n;

err:
    error = 1;
    return 0;
}
