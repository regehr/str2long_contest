#include "str2long.h"

long str2long_mikael_2(const char* str) {
    unsigned long buf = 0;
    long ret = 0;
    unsigned char negative = 0;
    unsigned char digit;
    unsigned long maxval = (unsigned long) LONG_MAX;

    if(!str) {
        error = 1;
        return 0;
    }

    if(*str == '-') {
        negative = 1;
        str++;
    }

    if(*str < '0' || *str > '9') {
        error = 1;
        return 0;
    }

    maxval += negative;
    for(; *str; str++) {
        if(*str < '0') {
            error = 1;
            return 0;
        }

        digit = *str - '0';    

        if(digit > 9) {
            error = 1;
            return 0;
        }

        if(buf > maxval/10) {
            error = 1;
            return 0;
        }
        buf *= 10;

        if(buf > maxval-digit) {
            error = 1;
            return 0;
        }
        buf += digit;
    }

    if(buf > maxval) {
        error = 1;
        return 0;
    }

    if(buf > LONG_MAX) {
        // Here, the code assumes LONG_MAX == -(LONG_MIN+1)
        return LONG_MIN;
    }
    ret = (long) buf;

    if(negative) 
        return -ret;

    return ret;
}

