/*
 * Reverse a string in-place.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libstring.h"

char *strrev(char *string) {
    size_t index = 0;
    size_t length = 0;

    /* Arguments cannot be NULL */
    if(string == NULL) {
        fprintf(stderr, "%s", "strrev: argument 1 cannot be NULL.\n");
        exit(EXIT_FAILURE);
    }

    length = strlen(string);

    for(index = 0; index < length / 2; index++) {
        char left_temp = string[index];
        char right_temp = string[length - index - 1];

        string[index] = right_temp;
        string[length - index - 1] = left_temp;
    }

    return string;
}
