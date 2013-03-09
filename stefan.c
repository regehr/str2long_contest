#include "str2long.h"

#ifdef __builtin_expect
# define likely(x)       __builtin_expect((x),1)
# define unlikely(x)     __builtin_expect((x),0)
#else
# define likely(x)       (x)
# define unlikely(x)     (x)
#endif


/*
 * additional semantics: returns 0 for empty and "-" input, returns
 * partial result * if invalid characters are found, and returns
 * min/max values on overflow.
 */
long str2long_stefan (const char *s) {
	long result = 0;
	if ('-' == *s) {
		if (unlikely('\0' == *++s)) goto failed;
		do {
			unsigned char digit = ((unsigned char) *s) - (unsigned char) '0';
			if (unlikely(digit > 9)) goto failed;
			/*    (-(LONG_MIN + 10)) % 10 is the last digit of LONG_MIN */
			if (unlikely(result <= LONG_MIN / 10) && (result < LONG_MIN / 10 || digit > (-(LONG_MIN + 10)) % 10)) {
				result = LONG_MIN; /* optional */
				goto failed;
			}
			result = 10 * result - digit;
		} while (likely('\0' != *++s));
	} else {
		if (unlikely('\0' == *s)) goto failed;
		do {
			unsigned char digit = ((unsigned char) *s) - (unsigned char) '0';
			if (unlikely(digit > 9)) goto failed;
			if (unlikely(result >= LONG_MAX / 10) && (result > LONG_MAX / 10 || digit > LONG_MAX % 10)) {
				result = LONG_MAX; /* optional */
				goto failed;
			}
			result = 10 * result + digit;
		} while (likely('\0' != *++s));
	}
	return result;

failed:
	error = 1;
	return result;
}
