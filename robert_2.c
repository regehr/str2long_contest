#include "str2long.h"

long str2long_robert_2(const char *str)
{
    long result = 0;
    int i;

    if (str[0] == '-') {
        if (str[1] == '\0')
            error = 1;

        for (i=1; str[i]; i++) {
            int c = str[i];

            if (c < '0' || '9' < c)
                error = 1;
            else if (result < LONG_MIN/10)
                error = 1;
            else if (result * 10 < (LONG_MIN + (c - '0')))
                error = 1;
            else
                result = result * 10 - (c - '0');
        }
    } else {
        if (str[0] == '\0')
            error = 1;

        for (i=0; str[i]; i++) {
            int c = str[i];

            if (c < '0' || '9' < c)
                error = 1;
            else if (result > LONG_MAX/10)
                error = 1;
            else if (result * 10 > (LONG_MAX - (c - '0')))
                error = 1;
            else
                result = result * 10 + (c - '0');
        }
    }
    return result;
}

