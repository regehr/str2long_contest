#include "str2long.h"

/*
 * Error Codes to set error
 *
 */

#define NO_ERROR 0
#define ERROR_WITH_NULL_TERMINATED_STRING 1
#define WARNING_BEHAVIOR_IS_UNDEFINED 2


extern int error;

/*
 * Magic Numbers
 *
 */

#define BITS_PER_BYTES 8
#define TRUE 1
#define FALSE 0


long str2long_ken_4 (const char *c)
{
    
    long maxLong;             /* computed max long */
    long minLong;             /* computer min long */
    
    int  maxDigitSize;        /* compute the max possible string size we can convert */
    int  currentDigitCount;   /* current count of how many digits we have converted */
    int  isPositive;          /* number is positive */
    int  nonZeroSeen;         /* for tracking leading zeros */
    
    long max;                 /* used to compute max Digits */
    
    long total;               /* current long value of the string as conversion occurs */
    
    char currentDigit;  /* pointer to location in the string argument to convert */
    
    /* Do some initializing */
    
    error = NO_ERROR;        /* assume we have no error */
    total = 0;
    nonZeroSeen = FALSE;
    currentDigitCount = 0;
    isPositive = TRUE;       /* Assume positive - will test for negative asap */
    
    
    
    minLong = LONG_MIN;
    maxLong = LONG_MAX;
    max = LONG_MIN;
    
    maxDigitSize = 0;
    while (max / 10 != 0)
    {
        maxDigitSize++;
        max = max / 10;
    }
    maxDigitSize++;
    
    /* keep reading in digits until we get a valid integer to return otherwise error and compute error value */
    
    while (1)
    {
        if (c[currentDigitCount] == '\0')  /* End of String */
        {
            if (!isPositive && currentDigitCount == 1) /* we got a '-' sign but no numbers return this error */
            {
                error = WARNING_BEHAVIOR_IS_UNDEFINED;
                return 0;
            }
            if (currentDigitCount == 0) /* String was empty and so does not match regular Expression */
            {
                error = WARNING_BEHAVIOR_IS_UNDEFINED;
                return 0;
            }
            
            /* We hit a null termination and we have a valid string matching our regular expression
             and we haven't hit any limits. Return the value -- success! */
            
            return total;
        }
        
        /* Peel a new digit from the input string to convert */
        
        currentDigit = c[currentDigitCount];
        currentDigitCount++;
        
        /* Check to see if this digit will be more than our max digits */
        
        if (currentDigitCount > maxDigitSize) 
        {
            break;         }
        
        /* The regular expression can allow for a '-' in the first spot so check for this */
        
        if (currentDigitCount == 1 && currentDigit == '-') 
        {
            isPositive = FALSE;
            maxDigitSize++;      /* increase by 1 to account for the sign digit */
        }
        else if (currentDigit >= '0' && currentDigit <= '9') /* otherwise we only allow these digits */
        {
            /* At this point we know we are within the Max Digit Size. But we need to still check the limits. */
            
            if (isPositive) /* check for limit on a positive number */
            {
             
                /* check for overflow potential by first working backwards - divide by 10 see if we could have
                   an overflow. This will not check the potential 1s column digit we do this if we are tied.
                 */
                
                if ( maxLong / 10 <  total)
                {
                    break;
                }
                else if ( maxLong / 10 == total )
                {
                    /* okay we had a tie and so lets check the potential 1s column digit and if
                       it will overflow return an error
                     */
                    
                    if ( maxLong % 10 < (currentDigit - '0'))
                    {
                       break;
                    }
                }

            }
            else /* check for limit on a negative number */
            {
                /* 
                   First check if lowering the Min by a factor of 10 puts it below the total we know
                   we have an over run - break out
                 */
           
                if ( (minLong / 10) > total)
                {
                    break;
                }
                
                /* 
                 this is a special check on the 1s column -- the above does not check the 1s place digit
                 */
                
                else if ( minLong / 10 == total)
                {
                    if ( minLong % 10 > (-1 * (currentDigit - '0')) )
                    {
                        break;
                    }
                }
            }
            
            /* If we reach this point we know we still have a number not max digits and still in our
             limits so lets compute the running value and keep on converting digits */
            
            
            if (currentDigit == '0' && !nonZeroSeen)
            {
                /* okay we got a leading zero - increase maxDigits to allow for the leading zeros */
                
                maxDigitSize++;
                
            }
            else /* we got a 1 to 9 -- do the math and flag a nonZero as Seen */
            {
               nonZeroSeen = TRUE;
                
               if (isPositive)
               {
                   total = (total * 10) + (currentDigit - '0');
               }
               else
               {
                  total = (total * 10) - (currentDigit - '0');
               }
            }
            
        }
        else /* Invalid character does not match our regular expression*/
        {
            /* Regardless if rest of string is good and null terminated we hit a digit not matching
             the regular expression it is a undefined behavior. Return out properly */
            
            error = WARNING_BEHAVIOR_IS_UNDEFINED;
            return 0;
        }
    } /* while (1) */
    
    /* At this point we have an error. To compute proper error value we keep reading
     rest of the string. If we continue to match the regular expression until a null we get
     one error type. Otherwise it will be an undefined error. */
    
    while (1)
    {
        if (c[currentDigitCount] == '\0')
        {
            error = ERROR_WITH_NULL_TERMINATED_STRING;
            return 0;
        }
        if (c[currentDigitCount] < '0' || c[currentDigitCount] > '9')
        {
            /* Digit is not proper for regular expression */
            
            error = WARNING_BEHAVIOR_IS_UNDEFINED;
            return 0;
        }
        currentDigitCount++;
    }
    
}
