/*
 * Count the number of times a character appears in a string.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libstring.h"

int strcount(const char *string, const char *count) {
    int index = 0;
    int occurrences = 0;

    /* Arguments cannot be NULL; will crash anyway. */
    if(string == NULL) {
        fprintf(stderr, "%s", "strcount: argument 1 cannot be NULL.\n");
        exit(EXIT_FAILURE);
    }

    if(count == NULL) {
        fprintf(stderr, "%s", "strcount: argument 2 cannot be NULL.\n");
        exit(EXIT_FAILURE);
    }

    /* Count will never appear in the string */
    if(strlen(count) > strlen(string))
        return 0;

    for(index = 0; string[index] != '\0'; index++) {
        if(string[index] != count[0])
            continue;

        if(strstarts(string + index, count) == 0)
            continue;

        occurrences++;
    }

    return occurrences;
}
