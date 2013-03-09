#include "str2long.h"

static long MULT_MIN = LONG_MIN / 10;

long str2long_chucky_2 (const char * s) {
	char c = *s++;

	// string is empty
	if (!c) {
		error = 1;
		return error = 1;
	}

	// if it actually is negative, remember that and get the next char
	_Bool isNegative = 0;
	if (c == '-') {
		isNegative = 1;
		c = *s++;
	}

	// now handle the numbers part
	long number = 0; // we keep 'number' negative below
	do {
		// if non digit, error
		if (!(c >= '0' && c <= '9')) {
			error = 1;
			return 1;
		}

		int digit = -(c - '0'); // negative since we're trying to stay negative
		// since we have a new digit, need to shift number; make sure it's safe
		if (number < MULT_MIN) {
			error = 1;
			return 1;
		}
		number *= 10;

		// check if it's safe to add the new digit and do so
		if (LONG_MIN - number > digit) {
			error = 1;
			return 1;
		}
		number += digit;
	} while ((c = *s++));

	// flip the signs back around if needed
	if (!isNegative) {
		// as long as it's safe...
		if (LONG_MAX * -1 > number) {
			error = 1;
			return 1;
		}
		number *= -1;
	}

	return number; // success!
}
