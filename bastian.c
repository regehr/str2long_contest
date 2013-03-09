#include "str2long.h"

long str2long_bastian (const char *s)
{
	long result = 0;

	if( *s == '-' )
	{
		++s;
		if(*s == '\0')
			goto str2long_error;

		while(*s != '\0')
		{
			if(*s < '0' || *s > '9')
				goto str2long_error;

			if(result < LONG_MIN / 10)
				goto str2long_error;

			result *= 10;

			if(LONG_MIN + (*s - '0') > result)
				goto str2long_error;

			result -= *s - '0';

			++s;
		}
	}
	else
	{
		if(*s == '\0')
			goto str2long_error;

		while(*s != '\0')
		{
			if(*s < '0' || *s > '9')
				goto str2long_error;

			if(result > LONG_MAX / 10)
				goto str2long_error;

			result *= 10;

			if(LONG_MAX - (*s - '0') < result)
				goto str2long_error;

			result += *s - '0';

			++s;
		}
	}

	return result;

str2long_error:
	error = 1;
	return 0;
}
