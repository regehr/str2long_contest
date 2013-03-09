#include "str2long.h"

#define ERROR_IF(c) if (c) { error = 1; break; }

long str2long_thomas(const char *s)
{
	long v = 0;
	int digit, negative = 0;

	if ((negative = (*s == '-')))
		++s;

	do
	{
		ERROR_IF(*s < '0' || *s > '9');
		digit = *s++ - '0';

		if (negative)
		{
			ERROR_IF(v < (LONG_MIN + digit) / 10);
			v = v * 10 - digit;
		}
		else
		{
			ERROR_IF(v > (LONG_MAX - digit) / 10);
			v = v * 10 + digit;
		}
	}
	while (*s);

	return v;
}

#undef ERROR_IF
