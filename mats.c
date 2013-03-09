#include "str2long.h"

long str2long_mats( const char *buffer) {
	long parsed_value = 0;
	int e = ( NULL == buffer );
	int is_negative = 0;
	size_t buffer_index = 0;

	if ( !e && '-' == buffer[buffer_index] ) {
		is_negative = 1;
		++buffer_index;
	}

	if ( !e && 0 == buffer[buffer_index] ) {
		e = 1;
	}

	for (; !e && 0 != buffer[buffer_index]; ++buffer_index) {
		long digit = -1;
		switch ( buffer[buffer_index] ) {
			case '0': digit = 0; break;
			case '1': digit = 1; break;
			case '2': digit = 2; break;
			case '3': digit = 3; break;
			case '4': digit = 4; break;
			case '5': digit = 5; break;
			case '6': digit = 6; break;
			case '7': digit = 7; break;
			case '8': digit = 8; break;
			case '9': digit = 9; break;
			default: e = 1; break;
		}
		
		if ( !e && is_negative ) {
			if ( LONG_MIN / 10 <= parsed_value && LONG_MIN + digit <= 10 * parsed_value) {
				parsed_value = 10 * parsed_value - digit;
			} else {
				e = 1;
				parsed_value = LONG_MIN;
			}
		} else if ( !e && ! is_negative ) {
			if ( LONG_MAX / 10 >= parsed_value && LONG_MAX - digit >= 10 * parsed_value) {
				parsed_value = 10 * parsed_value + digit;
			} else {
				e = 1;
				parsed_value = LONG_MAX;
			}
		}
	}

	if ( e ) {
		error = 1;
	}
	return parsed_value;
}

