#include "str2long.h"

long str2long_dokoto (const char * cnum)
{
   unsigned int result = 0, digit = 0, sign = 0, rest  = 0;
   size_t i = 0, x = 0;
   char ptr = '\0';    
    
	if ( (sign = (*cnum == '-')) ) cnum = cnum + 1;
	while ( (ptr = (char)*(cnum+(i++)) ) != '\0')
    {    
      if ( !('0' <= ptr && '9' >= ptr) ) return 1;        
      digit = (int)ptr - '0';        
		rest = result = (result == 0) ? digit : ((unsigned int)(result * 10) + digit);
		for (x = i; x > 0; --x)
		{
			if ( (char) ('0' + (rest % 10) ) != *( cnum+(x-1) ) ) return 1;		
			rest /= 10;			
		}		
    }    
	if (result > INT_MAX ) return 1;
   return (sign) ? (long)-1*result: (long)result;
}
