#include "str2long.h"

long str2long_matthew_3(const char* input)
{
    unsigned long total = 0;     /*  Accumulated total value  */
    const int neg = (*input == '-');  /*  Check for a leading minus sign  */
    int prev_error = error; /* Save the previous error value */
    
    error = 1;          /*  Be pessimistic: assume failure!  */
    if (neg) ++input;   /*  Skip the minus sign  */

    /*  Return if we were given an empty string or the string '-'  */
    if (*input == 0)    return -1;

    /*  Loop over characters while they're valid numbers  */
    while (*input >= '0' && *input <= '9') {
    
        /*  Get the next number from the input string  */
        long d = *(input++) - '0';
        
        /*  Check to see if we should abort due to overflow
        
            Negative numbers get an extra +1 (since they have a range
            that is one larger), with a check against d to prevent
            overflows -- this works with corner cases because we can
            only hit a maximum with a non-zero final digit (sneaky!).  */
        if (total > (LONG_MAX - d + (d ? neg : 0)) / 10)
            return -2;
        
        /* Otherwise, accumulate this value  */
        total = total*10 + d;
    }
    
    /*  If we've ended because of a non-numeric character
       (rather than the end of the string), we've failed.  */
    if (*input != 0)    return -3;
    
    /*  Once we reach this point, we can restore the error flag.  */
    error = prev_error;
    return (long)(neg ? (~total+1) : total);
}
