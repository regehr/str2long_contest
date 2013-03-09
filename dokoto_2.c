#include "str2long.h"

long str2long_dokoto_2 (const char * cnum)
{	
	unsigned int result = 0, digit = 0, sign = 0, rest  = 0;
	size_t i = 0, x = 0;
	char ptr = '\0';    
	error = 0;
    if (!cnum) goto tag_end;
	if ( (sign = (*cnum == '-')) ) cnum = cnum + 1;
	while ( (ptr = (char)*(cnum+(i++)) ) != '\0')
    {    
		if ( !('0' <= ptr && '9' >= ptr) ) goto tag_end;        
		digit = (int)ptr - '0';        
		rest = result = (result == 0) ? digit : ((unsigned int)(result * 10) + digit);
		for (x = i; x > 0; --x)
		{
			if ( (char) ('0' + (rest % 10) ) != *( cnum+(x-1) ) ) goto tag_end;		
			rest /= 10;			
		}		
    }    
	if (i == 1) goto tag_end;
	if (result > INT_MAX ) goto tag_end;
	return (sign) ? (long)-1*(long)result: (long)result;
	
	tag_end:
		error = 1;	
		return result;
}
