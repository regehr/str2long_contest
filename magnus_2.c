#include "str2long.h"

#define _64_bit
#if defined(_64_bit)
        #define RESULT_LONG_MAX LLONG_MAX
        #define TYPE_SUFFIX_CHAR 'i'
        typedef long long result_long;
#else
        #define RESULT_LONG_MAX LONG_MAX
        #define TYPE_SUFFIX_CHAR 'L'
        typedef long result_long;
#endif

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

// v2 - 'error' must be untouched if no parse error occurs, to allow client code
// to call str2long in batches and check 'error' once at the end.  Now return -999
// when parse fails so I can easily check my test cases if error is already 1.
//
// v3 - Typedef the result (long or long long) because 64 bit MSVC longs are still
// only 32 bits.  Fix rookie error (x + s[i] - '0' overflows, should of course be
// x - '0' + s[i]).
long str2long_magnus_2(const char *s)
{
        const char *const long_max_str = STRINGIFY(RESULT_LONG_MAX);
        int long_max_str_len = 0;
        int len = 0;
        int neg = 0;
        int starts_with_zero = 0;
        int maybe_overflow = 1;
        result_long result;

        //Assumes LONG_MAX is #defined in base 10 and ends in 'L' (or suffix starts with 'i')
        //long_max_str and long_max_str_len would be hard-coded in a proper implementation.
        // long_max_str = "2147483647", long_max_str_len = 10
        while (long_max_str[long_max_str_len] != TYPE_SUFFIX_CHAR)
                ++long_max_str_len;
 
        // Will the result by negative?
        if (s[0] == '-') {
                neg = 1;
                ++s;
        }

        // Skip leading zeroes, and remember if there was a zero at all (otherwise we will
        // think there were no digits).
        if (*s == '0') {
                starts_with_zero = 1;
                do
                        ++s;
                while (*s == '0');
        }

        while (s[len]) {
                if (s[len] < '0' || s[len] > '9' || len >= long_max_str_len) {
                        // Invalid character (bad input), or too many digits (overflow).
                        error = 1;
                        return -999;
                }
                // The maybe_overflow flag is only needed if the number of input digits is
                // exactly the number of digits in LONG_MAX.  Assuming this is the case,
                // the flag is set to 0 (won't overflow) as soon as a digit is found that
                // is less than the digit in the same position in LONG_MAX, or 2 (will
                // overflow) if the digit is greater.  While the flag remains 1 (might
                // overflow) we need to keep checking.
                if (maybe_overflow == 1)
                        if (s[len] < long_max_str[len])
                                maybe_overflow = 0;
                        else
                        if (s[len] > long_max_str[len]) {
                                // Edge case: we are at the final digit, the result will be negative,
                                // and the digit is one more than LONG_MAX's final digit.  This happens
                                // when the result is LONG_MIN.
                                // (Implementation-defined behaviour: LONG_MIN == -LONG_MAX - 1, and
                                // the final digit of LONG_MAX isn't 9.)
                                if (neg && len == long_max_str_len - 1 && !s[len + 1])
                                        if (s[len] == long_max_str[len] + 1)
                                                return -RESULT_LONG_MAX - 1;
                                maybe_overflow = 2;
                        }
                ++len;
        }
        if (len == 0) {
                // No input! (unless it was "0" (or "-0") and we stepped over it)
                if (!starts_with_zero)
                        error = 1;
                return starts_with_zero ? 0 : -999;
        }

        // len can't be > long_max_str_len.
        // If len is < long_max_str_len, it can be parsed safely.
        // If len == long_max_str_len, it can't be parsed if the overflow flag is 2.
        if (len == long_max_str_len)
                if (maybe_overflow == 2) {
                        error = 1;
                        return -999;
                }

        result = *s++ - '0';
        switch (len) {
                //I'm hoping the compiler creates a jump table and not a series of ifs.
                //Should be good for up to 64 bit ints, if more needed just add them.
                case 20: result = result * 10 + *s++ - '0';
                case 19: result = result * 10 + *s++ - '0';
                case 18: result = result * 10 + *s++ - '0';
                case 17: result = result * 10 + *s++ - '0';
                case 16: result = result * 10 + *s++ - '0';
                case 15: result = result * 10 + *s++ - '0';
                case 14: result = result * 10 + *s++ - '0';
                case 13: result = result * 10 + *s++ - '0';
                case 12: result = result * 10 + *s++ - '0';
                case 11: result = result * 10 + *s++ - '0';
                case 10: result = result * 10 + *s++ - '0';
                case  9: result = result * 10 + *s++ - '0';
                case  8: result = result * 10 + *s++ - '0';
                case  7: result = result * 10 + *s++ - '0';
                case  6: result = result * 10 + *s++ - '0';
                case  5: result = result * 10 + *s++ - '0';
                case  4: result = result * 10 + *s++ - '0';
                case  3: result = result * 10 + *s++ - '0';
                case  2: result = result * 10 - '0' + *s++; //this is the one that can overflow if not done in the right order!!
                case  1: return neg ? -result : result;
        }

        //we never get here
        {
                volatile int zero = 0;
                result = 1 / zero;
        }
        return -999;
}

