#include "str2long.h"

long str2long_ethan(const char *str) {
	long m = 1;
	error = 1;

	// Validate the string and find its end.
	if (*str == '-') {
		str++;
		m = -1;
	}
	const char *p = str;
	if (*p == '\0')
		return 0;
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
