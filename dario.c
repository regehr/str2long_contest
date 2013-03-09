#include "str2long.h"

int ceillog10(long val){
	long v = val;
	int res = 1;
	while (v != 0){
		v /= 10;
		res++;
	}
	return res;
}

long str2long_dario (const char * str){
	int i = 0, starti = 0, len = 0;
	long result = 0, f = 1;
	int neg = str[0] == '-';
	if (neg){
		i = 1;
		--len;
		starti = 1;
		f = -1;
	}
	while (str[i] != '\0'){
		if (str[i] < '0' || str[i] > '9'){
			goto error;
		}
		if (str[i] == '0' && i==starti){
			++starti;
			--len;
		}
		++i;
	}
	len += i--;
	if (len >= ceillog10(LONG_MAX) || len < 1 &&  i + (neg ? 0 : 1) < 1){
		goto error;
	}
	while (i > starti){
		result += (str[i]-'0') * f;
		f *= 10;
		--i;
	}
	f *= (str[i]-'0');
	if (neg ? LONG_MIN - result > f: LONG_MAX - result < f){
		goto error;
	}
	result += f;
	return result;
 error:
	error = 1;
	return -1;
}

