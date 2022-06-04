/*
 * Determine if a string starts with a sub-string.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libstring.h"

int strstarts(const char *string, const char *start) {
    int index = 0;

    /* Arguments cannot be NULL; will crash anyway */
    if(string == NULL) {
        fprintf(stderr, "%s", "strstarts: argument 1 cannot be NULL.\n");
        exit(EXIT_FAILURE);
    }

    if(start == NULL) {
        fprintf(stderr, "%s", "strstarts: argument 2 cannot be NULL.\n");
        exit(EXIT_FAILURE);
    }

    for(index = 0; start[index] != '\0'; index++) {
        if(string[index] == start[index])
            continue;

        return 0;
    }

    return 1;
}
