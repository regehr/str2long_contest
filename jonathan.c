#include "str2long.h"

long str2long_jonathan(const char *const str)
{
static const long LONG_MIN_OVER_10 = LONG_MIN / 10;

/*
 * Implementation notes:
 *
 * Our basic strategy is
 * - check the input for validity concurrently with processing it
 * - avoid any chance of overflow by pre-checking any arithmetic
 *   operation which might overflow
 * - accumulate our result as a (signed) NEGATIVE running value so we can
 *   handle LONG_MIN cleanly and so we don't have to worry about unsigned-vs-int
 *   arithmetic conversions at the end
 * - our assumption on the character encoding lets us map a digit character
 *   c to a numeric digit via   c - '0'
 */

assert(str != NULL);		/* sanity check! */

#ifdef DEBUG
printf("str2long(str=\"%s\" @ %p)\n", str, (const void*)str);
#endif

bool start_char = true;		/* are we processing the starting input */
				/* character (= the only one which can */
				/* validly be a - sign)? */


bool minus_sign_seen = false;	/* have we seen a - sign? */
bool any_chars_seen  = false;	/* have we seen any non-'\0' characters yet? */
long mresult = 0;		/* accumulates result with a forced - sign */
const char* p = NULL;
	for (p = str ; *p != '\0' ; ++p)
	{
	/*
	 * loop invariant:
	 *	sign_seen and mresult
	 * accurately reflect all the characters we've processed
	 * in loop iterations before this one, i.e., all characters
	 * which are pointed to by pointers >= str and < p
	 *
	 * also,  mresult <= 0
	 */

	assert(mresult <= 0);		/* sanity check */

      #ifdef DEBUG
	printf("   processing p=%p --> '%c'\n", (const void*)p, *p);
	printf("      start_char=%s minus_sign_seen=%s mresult=%ld\n",
	       (start_char ? "true" : "false"),
	       (minus_sign_seen ? "true" : "false"),
	       mresult);
      #endif

	any_chars_seen = true;

	if (start_char)
		{
		start_char = false;

		/* the input character can legally be a - sign */
		if (*p == '-')
			{
		      #ifdef DEBUG
			printf("   ==> got minus sign\n");
		      #endif
			minus_sign_seen = true;
			continue;			/*** LOOP CONTROL ***/
			}
		}

	if (! ((*p >= '0') & (*p <= '9')))
		{
		/* invalid input character ==> error */
	      #ifdef DEBUG
		printf("   ==> *p is invalid ==> error return\n");
	      #endif
		error = 1;
		return 0;				/*** ERROR RETURN ***/
		}

	/*
	 * get to here ==> we have a digit ==> extract it
	 */
	const int digit = *p - '0';
	assert(digit >= 0);		/* sanity check */
	assert(digit <= 9);		/* sanity check */
      #ifdef DEBUG
	printf("   ==> got digit=%d\n", digit);
      #endif

	/*
	 * Now the tricky part: logically we want to do
	 *	result = 10*result + digit
	 * but we're actually using  mresult  which is negative,
	 * so we want to do
	 *	mresult = 10*mresult - digit
	 * However, we have to make sure that overflow can't occur.
	 */
	if (mresult < LONG_MIN_OVER_10)
		{
		/* the multiply would overflow ==> error return */
	      #ifdef DEBUG
		printf("   ==> multiply would overflow ==> error return\n");
	      #endif
		error = 1;
		return 0;				/*** ERROR RETURN ***/
		}
	mresult *= 10L;
      #ifdef DEBUG
	printf("   after multiply, mresult=%ld\n", mresult);
      #endif
	if (mresult < LONG_MIN+digit)
		{
		/* the subtract would overflow ==> error return */
	      #ifdef DEBUG
		printf("   ==> add would overflow ==> error return\n");
	      #endif
		error = 1;
		return 0;				/*** ERROR RETURN ***/
		}
	mresult -= digit;
      #ifdef DEBUG
	printf("   after subtract, mresult=%ld\n", mresult);
      #endif
	}

if (! any_chars_seen)
	{
	/* empty string ==> error return */
	error = 1;
	return 0;					/*** ERROR RETURN ***/
	}

/*
 * set the correct sign on the result
 */
if (minus_sign_seen)
	{
	/*
	 * The result should be negative;  mresult  is precisely
	 * the desired result.
	 */
      #ifdef DEBUG
	printf("   ==> returning negative result %ld\n", mresult);
      #endif
	return mresult;					/*** NORMAL RETURN ***/
	}
   else {
	/*
	 * The result should be positive; the desired result is  -mresult .
	 * But we need to make sure the negation doesn't overflow.
	 */
	if (mresult < -LONG_MAX)
		{
		/* the negation would overflow ==> error return */
	      #ifdef DEBUG
		printf("   ==> negation would overflow ==> error return\n");
	      #endif
		error = 1;
		return 0;				/*** ERROR RETURN ***/
		}
	const long result = -mresult;
      #ifdef DEBUG
	printf("   ==> returning positive result %ld\n", result);
      #endif
	return result;					/*** NORMAL RETURN ***/
	}
}
