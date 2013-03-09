#include "str2long.h"

long str2long_david (const char * str)
{
	int negative = 0;
  if (str[0] == '-')
	{
		negative = 1;
		str++;
	}

	unsigned long mag_limit = (unsigned long)LONG_MAX + negative;

	unsigned long mag = 0;
	while(*str)
	{
		char c = *str;
		if (c < '0' || c > '9')
		{
			error = 1;
			return 0;
		}

		unsigned char digit = c - '0';
		if (mag > (mag_limit - digit)/10)
		{
			error = 1;
			return 0;
		}

		mag = mag * 10 + digit;
		str++;
	}

	if(negative)
	{
		if (mag == mag_limit)
		{
			// Cannot do (long)mag because mag is too big.
			return LONG_MIN;
		}
		else
		{
			return -(long)mag;
		}
	}
	else
	{
		return mag;
	}
}

