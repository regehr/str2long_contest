#include "str2long.h"

#ifdef DEBUG_STR2LONG
#include <stdio.h>
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

// Up to 9 characters processed, nothing can overflow, so skip overflow checks 
#define SKIP_CHECK_LIMIT 9

/* 
- Don't call library functions; your code should perform the conversion itself
- It is fine to use constants from limits.h
- You should write conforming C code that works on x86 and x86-64. I'll probably only test entries using GCC and Clang but even so you are not allowed to use extensions such as GCC's 128-bit integers
- You may assume that implementation-defined behaviors are the standard ones for the x86 and x86-64 platforms on Linux machines
- Your code must not execute undefined behaviors (use a recent Clang to check for this)
*/
int error = 0;

// Returns length of valid number in chars if null-terminated, otherwise 0 on error
static int number_length(const char *p_number_str)
{
    int valid = 1;
    const char *cursor = p_number_str;
    int num_digits = 0;

    // NULL pointer never valid
    if (NULL == p_number_str) {
        DEBUG_PRINT("NULL == p_number_str");
        return 0;
    }
    
    // Skip first char if a negative sign. This is valid.
    if ('-' == *cursor) {
        cursor++;
    }
    
    while ((*cursor != '\0') && (*cursor >= '0') && (*cursor <= '9')) {
        cursor++;
        if (num_digits < INT_MAX) {
            num_digits++;
        }
    }
    
    // After reaching first non-digit, we must have a nul-terminator
    if ('\0' == *cursor) {
        // Valid number, return length, otherwise zero
        DEBUG_PRINT("Valid number?");
        return (num_digits != 0) ? num_digits : 0;
    } else {
        DEBUG_PRINT("No nul-terminator: '%s'", p_number_str);
        return 0;
    }
}

static int is_long_add_safe(long a, long b) {
    if ((0L == a) || (0L == b)) {
        return 1;
    }
    
    if ((b > 0L) && (a > (LONG_MAX - b))) {
        // Overflow of a + b
        return 0;
    } else if ((b < 0L) && (a < (LONG_MIN - b))) {
        // Underflow of a + b
        return 0;
    }
    
    return 1;
}

static int is_long_mul_safe(long a, long b) {
    long tmp;
    
    // Handle obvious cases first
    if ((0L == a) || (0L == b)) {
        // Prevent divide by zero below, since anything * 0 is valid
        return 1;
    } else if (-1L == b) {
        // Prevent (LONG_MIN / -1L): invariant is that b != -1L
        // a * -1 is always safe, except for a=INT_MIN
        return (LONG_MIN == a) ? 0 : 1;
    }
    
    if (a > (LONG_MAX / b)) {
        // Overflow of a * b
        return 0;
    } else if (a < (LONG_MIN / b)) {
        // Underflow of a * b
        return 0;
    }
    
    return 1;
}

/*
 * if input matches ^-?[0-9]+\0$ and the resulting integer is
 * representable as a long, return the integer; otherwise if
 * the input is a null-terminated string, set error to 1 and 
 * return any value; otherwise behavior is undefined
 */
long str2long_tennessee (const char *p_number_str)
{
    long converted = 0L;
    long digit_repr;
    long digit_radix;
    long cur_digit;
    const char *p_cursor;
    const char *p_zero_cursor;
    int is_negative = 0;
    int num_chars_done = 0;
    int seen_a_zero = 0;
    int num_digits = number_length(p_number_str);
    
    if (0 == num_digits) {
        // Invalid string: my undefined behavior is to set error to 2 and return zero
        error = 1;
        return 0L;
    } 
    
    // From now on, string is at least null-terminated and composed of digits: convert to long !
    
    // Step 1: Figure-out sign
    if ('-' == p_number_str[0]) {
        is_negative = 1;
    }
    
    // Step 2: initialize algorithm according to sign
    // Jump to last digit of number (CAVEAT: This could overflow if address is very large ???)
    p_cursor = (p_number_str + (is_negative ? 1 : 0) + num_digits) - 1;
    digit_radix = is_negative ? -1L : 1L;

    // Step 3: skip_leading_zeros
    p_zero_cursor = p_number_str + (is_negative ? 1 : 0);
    while ((num_digits > 0) && ('0' == *p_zero_cursor)) {
        p_zero_cursor++;
        num_digits--;
        seen_a_zero = 1;
    }

    if ((0 == num_digits) && seen_a_zero) {
        // Handle case of entire string of zeroes
        return 0L;
    }

    // Step 4: Scan through the digits from least-significant to most-significant. On the way, accumulate
    //         result of each significant digit converted, ensuring that all arithmetic is done only if
    //         it could be done without overflow. Slight optimization by skipping overflow checks (through
    //         short-circuit evaluation) if we have not processed enough digits to start seeing potential
    //         overflowing operations.
    for (num_chars_done = 0; num_chars_done < num_digits; num_chars_done++) {
        cur_digit = (long)(*p_cursor - '0');
        
        if ((num_chars_done >= SKIP_CHECK_LIMIT) && !is_long_mul_safe(digit_radix, cur_digit)) {
            // Would overflow on the current digit: error !
            error = 1;
            return converted;
        } else {
            digit_repr = digit_radix * cur_digit;
        }
        
        // Add current significant digit to result
        if ((num_chars_done >= SKIP_CHECK_LIMIT) && !is_long_add_safe(converted, digit_repr)) {
            // Would overflow on the current digit: error !
            error = 1;
            return converted;
        } else {
            converted = converted + digit_repr;
        }
        
        // Debug print to show the algorithm in action
        DEBUG_PRINT("%c|%ld|%ld|%ld|%d\n", *p_cursor, digit_radix, digit_repr, converted, num_digits);
        
        // Prepare next digit
        if ((num_chars_done >= SKIP_CHECK_LIMIT) && !is_long_mul_safe(digit_radix, 10L)) {
            // Would overflow trying to generate next position: stop
            num_chars_done++;
            break;
        } else {
            digit_radix *= 10L;
        }
        
        p_cursor--;
    }

    if (num_chars_done != num_digits) {
        // Would overflow on processing the next digit: error !
        error = 1;
        return converted;
    } else {
        // Number OK, return it !
        return converted;
    }
        
    // We are done, all cases covered by now
}
