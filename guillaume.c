#include "str2long.h"

long str2long_guillaume(const char *str)
{
	short positive = 1;	
	int i;	
	long res = 0, to_be_added, pow = 1;

	/* begin from last character; undefined if non-null terminated string */
	for (i = 0; str[i] != 0; i++);

	/* check for empty string that is considered as an error (empty string does not match ^-?[0-9]+\0$) */	
	if (i == 0)
	{
		error = 1;
		return -4;
	}

	/* check for negative number */	
	if (str[0] == '-')
	{
		positive = 0;
		if(str[1] == 0) {
			error = 1;
			return -7;
		}
	}
	
	for (i -= 1; i >= 0; i--)
	{
		/* ignore '-' if negative */		
		if (!positive && i == 0)
			break;		

		/* check if considered char fits in [0-9] */		
		if (str[i] < '0' || str[i] > '9')
		{
			error = 1;
			return -1;
		}			
		
		/* compute and store the value that has to be added (or substracted if negative) to the result */
		
		/* check for overflow: if pos * (considered number) > LONG_MAX ; this is useless on my system where LONG_MAX is 9223372036854775807 but this can be useful on different archs (e.g. LONG_MAX is 400 and input is 500)  */	
		if ((str[i] - '0') > 0 && pow > LONG_MAX / (str[i] - '0'))
		{
			error = 1;
			return -5;	
		}
		to_be_added = (str[i] - '0') * pow;

		if (positive)
		{
			/* check for overflow: if adding the value that has to be added result in LONG_MAX excess */
			if (LONG_MAX - res < to_be_added)
			{
				error = 1;
				return -2;
			}
			
			res += to_be_added;
		}
		
		else
		{
			/* check for overflow: same as above */				
			if (LONG_MIN - res > -to_be_added)
			{
				error = 1;
				return -3;
			}
		
			res -= to_be_added;
		}
		
		/* check for overflow: if pow that will be computed (pow *= 10) will exceed LONG_MAX ; this is useful when input looks like 900000000000000000000000000000 which is not handle by previous checks */		
		if (str[i-1] != '0' && ((positive && i > 0) || (!positive && i> 1)) && pow > LONG_MAX / 10)
		{		
			error = 1;
			return -6;
		}
		pow *= 10;
	}
	
	return res;
}
