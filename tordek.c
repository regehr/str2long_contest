#include "str2long.h"

long str2long_tordek(const char *input) {
    int negative = 0;
    long result = 0;

    error = 0;
    if (*input == '-') {
        negative = 1;
        input++;
    }

    do {
        int new_digit = (*input++) - '0';

        if (result < LONG_MIN/10 ||
            (result == LONG_MIN/10 &&
                new_digit > (LONG_MAX % 10 + negative))) {
            error = 1;
            return result;
        }

        result *= 10;
        result -= new_digit;
    } while (*input);

    if (negative) {
        return result;
    } else {
        return -result;
    }
}
