/*
 * Strip characters from a string
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libstring.h"

int strstrip(char *string, const char *characters) {
    int stripped = 0;
    size_t index = 0;
    size_t cursor = 0;

    if(string == NULL) {
        fprintf(stderr, "%s", "strstrip: argument 1 cannot be NULL.\n");
        exit(EXIT_FAILURE);
    }

    if(characters == NULL) {
        fprintf(stderr, "%s", "strstrip: argument 1 cannot be NULL.\n");
        exit(EXIT_FAILURE);
    }

    for(index = 0; string[index] != '\0'; index++) {
        if(strchr(characters, string[index]) != NULL) {
            stripped++;

            continue;
        }

        string[cursor] = string[index];
        cursor++;
    }

    string[cursor] = '\0';

    return stripped;
}
