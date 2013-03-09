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

long str2long_ken_2 (const char *c)
{
    int  maxDigitSize;        /* compute the max possible string size we can convert */
    int  currentDigitCount;   /* current count of how many digits we have converted */
    int  isPositive;          /* number is positive */
    int  nonZeroSeen;         /* for tracking leading zeros */
    int  i;
    
    long max;                 /* used to compute max Digits */
    
    long total;               /* current long value of the string as conversion occurs */
    
    char currentDigit;  /* pointer to location in the string argument to convert */
    
    /* Do some initializing */
    
    error = NO_ERROR;        /* assume we have no error */
    total = 0;
    nonZeroSeen = FALSE;
    currentDigitCount = 0;
    isPositive = TRUE;       /* Assume positive - will test for negative asap */
    
    
    /* Compute Max digits, max positive long, Min long
     using <limits.h> LONG_MAX and LONG_MIN could be used here
     but this computes it without using <limits.h>
     */
    
    /* computes a value 2 to the power of # of bits in a long to get a LONG_MIN */
    
    max = 1;
    for (i= 0; i < (sizeof(long) * BITS_PER_BYTES) - 1; i++)
        max = max * 2;
    
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
        
        if (currentDigitCount > maxDigitSize) /* at this point integer will be too big */
        {
            break; /* Break out of Loop and continue to read string and return error */
        }
        
        /* The regular expression can allow for a '-' in the first spot so check for this*/
        
        if (currentDigitCount == 1 && currentDigit == '-') /* Our number will be negative flag it as such */
        {
            isPositive = FALSE;
            maxDigitSize++;      /* increase by 1 to account for the sign digit */
        }
        else if (currentDigit >= '0' && currentDigit <= '9') /* otherwise we only allow these digits */
        {
            /* At this point we know we are within the Max Digit Size. But we need to still check the limits. */
            
            if (isPositive) /* check for limit on a positive number */
            {
                /* if we compute the next digit and it flips to a negative value we know
                 the integer will be too big
                 */
                
                if ( (total * 10) + (currentDigit - '0') < 0 )
                {
                    break; /* handle error */
                }
            }
            else /* check for limit on a negative number */
            {
                /* if we compute the next digit and it flips to a positive value we know
                 the integer will be too small
                 */
                
                if ( (total * 10) - (currentDigit - '0') > 0)
                {
                    break; /* handle error */
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
