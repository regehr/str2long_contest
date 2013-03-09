#include "str2long.h"

#if 0

enum {
    ERR_NULL_POINTER = 1,
    ERR_WRAP_AROUND,
    ERR_TOO_BIG,
    ERR_TRAILING_JUNK,
    ERR_NO_DIGITS,
};
#define ERR(e) ((error = 1), (long)ERR_##e)

long str2long_kevin (const char *s)
{
    if (!s)
        return ERR(NULL_POINTER);

    unsigned long is_negative = 0;
    if (*s == '-') {
        ++s;
        is_negative = 1;
    }

    unsigned long n = 0;
    char c;
    const char *t;
    // C11 Â§5.2.1 promises digits are consecutive. Take that, Ã‰mile.
    for (t = s; ((c = *t) >= '0') && (c <= '9'); ++t) {
        unsigned long m = 10 * n + (unsigned long)(c - '0');
        // Check for wrap; if it's too big for an unsigned long,
        // it's certainly too big for a long.
        if (m < n)
            return ERR(WRAP_AROUND);
        // Check whether the current magnitude would fit in a signed long.
        // Implementation-defined bound (C11 Â§6.2.6.2):
        // assumes two's complement.
        if (m > is_negative + (unsigned long)LONG_MAX)
            return ERR(TOO_BIG);
        // Joy.
        n = m;
    }

    if (*t)
        return ERR(TRAILING_JUNK);
    if (s == t)
        return ERR(NO_DIGITS);

    // Implementation-defined representation (C11 Â§6.2.6.2):
    // assumes two's complement.
    if (is_negative)
        n = ~n + 1;

    // Implementation-defined conversion (C11Â§6.3.1.3):
    // assumes bit reinterpretation.
    return (long) n;
}

#else

enum {
    ERR_NULL_POINTER = 1,
    ERR_TIMES_10,
    ERR_TOO_BIG,
    ERR_TRAILING_JUNK,
    ERR_NO_DIGITS,
};
#define ERR(e) ((error = 1), (long)ERR_##e)

long str2long_kevin(const char *s)
{
    if (!s)
        return ERR(NULL_POINTER);

    long n = 0;
    char c;
    const char *t;
    if (*s == '-') {
        ++s;
        // C11 §5.2.1 promises digits are consecutive. Take that, Émile.
        for (t = s; ((c = *t) >= '0') && (c <= '9'); ++t) {
            long digit = c - '0';
            // If no non-zero digits have yet been seen, skip the other tests,
            // to avoid the (0 - LONG_MIN) calculation that would overflow in
            // the two's complement case. Fortunately C11 §5.2.4.2.1 guarantees
            // |LONG_MIN| > 9.
            if (n) {
                // Verify that it is safe to multiply the current value by 10.
                if (n < LONG_MIN / 10)
                    return ERR(TIMES_10);
                n *= 10;
                // Verify that it is safe to add the new digit.
                if (digit > n - LONG_MIN)
                    return ERR(TOO_BIG);
            }
            // Joy.
            n -= digit;
        }
    } else {
        for (t = s; ((c = *t) >= '0') && (c <= '9'); ++t) {
            long digit = c - '0';
            if (n > LONG_MAX / 10)
                return ERR(TIMES_10);
            n *= 10;
            if (digit > LONG_MAX - n)
                return ERR(TOO_BIG);
            n += digit;
        }
    }

    if (*t)
        return ERR(TRAILING_JUNK);
    if (s == t)
        return ERR(NO_DIGITS);

    return n;
}

#endif
