#include "str2long.h"

long str2long_robert(const char *str)
{
        long result = 0;
        int i;

        /*error = 0;*/

        if (str[0] == '-') {
                for (i=1; str[i]; i++) {
                        int c = str[i];

                        if (c < '0' || '9' < c) {
                                error = 1;
                                return 0;
                        } else if (result < LONG_MIN/10) {
                                error = 1;
                                return 0;
                        } else if (result * 10 < (LONG_MIN + (c-'0'))) {
                                error = 1;
                                return 0;
                        } else
                                result = result * 10 - (c - '0');
                }
        } else {
                for (i=0; str[i]; i++) {
                        int c = str[i];

                        if (c < '0' || '9' < c) {
                                error = 1;
                                return 0;
                        } else if (result > LONG_MAX/10) {
                                error = 1;
                                return 0;
                        } else if (result * 10 > (LONG_MAX - (c-'0'))) {
                                error = 1;
                                return 0;
                        } else
                                result = result * 10 + (c - '0');
                }
        }
        return result;
}

