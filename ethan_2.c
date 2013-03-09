#include "str2long.h"

long str2long_ethan_2(const char *str) {
	long m = 1;
	error = 1;

	if (*str == '-') {
		str++;
		m = -1;
	}
	if (*str == '\0')	// Empty string or just a -.
		return 0;

	// Strip leading zeros.
	while (*str == '0')
		str++;
	if (*str == '\0') {	// Just zeroes.
		error = 0;
		return 0;
	}

	// Check that everything is a digit and find the end.
	const char *p = str;
	while (*p != '\0') {
		if (!isdigit(*p))
			return 0;
		p++;
	}

	// Compute the value.
	long l = 0;
	for ( ; ; ) {
		p--;
		long d = *p-'0';

		if ((m < -1 && LONG_MIN/m < d) || (m > 0 && LONG_MAX/m < d))
			return 0;
		d *= m;

		if ((d < 0 && LONG_MIN-d > l) || (d > 0 && LONG_MAX-d < l))
			return 0;
		l += d;

		if (p == str)
			break;

		if ((m < 0 && LONG_MIN/10 > m) || (m > 0 && LONG_MAX/10 < m))
			return 0;
		m *= 10;
	}

	error = 0;
	return l;
}
