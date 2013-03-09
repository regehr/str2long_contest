#include "str2long.h"

static int char2dig(char c)
{
	if ((c >= '0') && (c <= '9'))
		return (c - '0');
	return -1;
}

#define NB_DIG (int)log10(LONG_MAX)

long str2long_renaud (const char *str)
{
	error = 1;
	long value = 1;
	int digit;
	int index = 0;
	int lead_zero = 0;
	if (str[0] == '-') {
		value = -1;
		str++;
	}
	//remove leading 0
	while (str[0] == '0')
	{
		str++;
		lead_zero = 1;
	}
	// special case for 0
	if (str[0] == '\0')
	{
		// error: case "-\0"
		if (lead_zero == 0)
			return 0L;
		error = 0;
		return 0L;
	}
	if ((digit = char2dig(str[index++])) == -1)
		return 0L;
	value *= digit;
	
	if (value > 0) {
		while ((digit = char2dig(str[index])) != -1) {
			/* check that value * 10 + cur_digit isn't too big */
			if ((index >= NB_DIG) && (value  > ((LONG_MAX - digit) / 10)))
				return 0;
			value = (value * 10) + digit;
			index++;
		}
	} else {
		while ((digit = char2dig(str[index])) != -1) {
			/* check that value * 10  - cur_digit isn't too small */
			if ((index >= NB_DIG) && (value < ((LONG_MIN + digit) / 10)))
				return 0;
			value = (value * 10) - digit;
			index++;
		}
	} 
	if (str[index] != '\0')
		return 0L;
	error = 0;
	return value;
	
}
