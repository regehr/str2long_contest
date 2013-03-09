#include "str2long.h"

/*
 * if input matches ^-?[0-9]+\0$ and the resulting integer is
 * representable as a long, return the integer; otherwise if
 * the input is a null-terminated string, set error to 1 and 
 * return any value; otherwise behavior is undefined
 */
long str2long_francois_2(const char* s)
{
    long sign = 1, result = 0;

    if (*s == '-')
    {
        sign = -sign;
        ++s;
    }

    do
    {
        long digit;

        if (*s < '0' || *s > '9')
        {
            error = 1;
            break;
        }

        digit = *s - '0';

        if (result < (LONG_MIN + digit) / 10 ||
            result > (LONG_MAX - digit) / 10)
        {
            error = 1;
            break;
        }

        result = result * 10 + digit * sign;
    }
    while (*++s);

    return result;
}

