#include "str2long.h"

long str2long_davec_2(const char *str) {
    int i = 0;
    long n = 0, last_n = 0, m = 1;
    char *p= (char *)str, c;

    if (!p[0]) { goto err; }

    if (p[0] == '-') {
        p++;
        m = -1;
        if (!p[0]) {
            goto err;
        }
    } 

    while((c = *p++)) {
        if (c >= '0' && c <= '9') {
            last_n = n;
            if (i > 0) { n *= 10; }
            n += m * (c - '0'); 
        } else {
            goto err;
        }

        /* detect overflow */
        if (n && ((m == -1 && n > last_n) || (m == 1 && n < last_n))) {
            goto err;
        }

        i++;
    }

    return n;

err:
    error = 1;
    return 0;
}
