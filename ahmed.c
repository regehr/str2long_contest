#include "str2long.h"

long str2long_ahmed(const char *s) {
        long ret = 0;
        _Bool negative = *s == '-';
        if (negative) ++s;
        while (*s) {
                if (*s < '0' || *s > '9') {
                        error = 1;
                        return 0;
                }
                long i = *s - '0';
                if (((LONG_MAX - i) / 10) < ret) {
                        if (negative && i != 0 && (((LONG_MAX - i) +
1) / 10) == ret && !s[1]) {
                                return LONG_MIN;
                        }
                        error = 1;
                        return 0;
                }
                ret *= 10;
                ret += i;
                ++s;
        }
        if (negative) ret = -ret;
        return ret;
}
