#include "str2long.h"

#define CHECK_ERROR(cond) if(cond) { error = 1; return 0; }

static size_t _strlen(const char* s) {
    if(s == NULL) {
        return 0;
    }
    size_t i;
    for(i = 0; s[i] != 0; ++i);
    return i;
}

static unsigned long ilog10_plus1(unsigned long val) {
    unsigned long base = 1;
    unsigned long i = 0;
    while(val >= base) {
        ++i;
        base *= 10;
    }
    return i;
}

long str2long_ryanf (const char* s) {
    error = 0;
    CHECK_ERROR(s == NULL);
    
    int negative = s[0] == '-';
    size_t max_len = ilog10_plus1(LONG_MAX) + negative;
    size_t len = _strlen(s);
    CHECK_ERROR(len == 0);
    CHECK_ERROR(len > max_len);

    long base = (negative ? -1 : 1);
    long value = 0;
    size_t i;
    for(i = 0; i < len - negative; ++i) {
        base *= (i > 0 ? 10 : 1); // base might roll over when multiplying at the end, so do it at the beginning
        unsigned char c = s[len - i - 1] ^ '0';
        long inc = c * base;
        
        CHECK_ERROR(c > 9);
        CHECK_ERROR(value > 0 && (LONG_MAX - value) < inc);
        CHECK_ERROR(value < 0 && (LONG_MIN - value) > inc);

        value += inc;
    }
    return value;
}
