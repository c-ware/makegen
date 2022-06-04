/*
 * In-place string find and replace.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libstring.h"

size_t strrepl_final_size(const char *string, const char *find,
                                              const char *replace) {
    size_t length = strlen(string);
    size_t occurrences = (size_t) strcount(string, find);

    return (length - (strlen(find) * occurrences))
                   + (strlen(replace) * occurrences);
}

int strrepl_shift_back(char *string, const char *find, const char *replace,
                                                       size_t size) {
    size_t index = 0;
    size_t length = strlen(string);
    int occurrences = 0;

    for(index = 0; index < length; index++) {
        size_t cursor = 0;
        size_t offset = 0;

        if(strstarts(string + index, find) == 0)
            continue;

        occurrences++;

        /* Replace */
        for(cursor = 0; replace[cursor] != '\0'; cursor++) {
            string[index + cursor] = replace[cursor];
        }

        offset = index + strlen(replace);
        length -= (strlen(find) - strlen(replace));


        /* Shift */
        for(cursor = index + strlen(find); cursor < size; cursor++) {
            string[offset] = string[cursor];
            offset++;
        }
    }

    string[length] = '\0';

    return occurrences;
}

int strrepl_shift_forward(char *string, const char *find, const char *replace,
                                                          size_t size) {
    size_t index = 0;
    size_t length = strlen(string);
    int occurrences = 0;

    /* If the final size would exceed the bounds of the block, it should
     * not proceed. */
    if(strrepl_final_size(string, find, replace) > size)
        return -1;

    for(index = 0; index < length; index++) {
        size_t cursor = 0;
        size_t missed = 0;

        if(strstarts(string + index, find) == 0)
            continue;

        occurrences++;

        /* Shift everything over to make room for the replacement */
        for(cursor = length - 1; cursor != index - 1; cursor--) {
            string[cursor + (strlen(replace) - strlen(find))] = string[cursor];
        }

        /* Write the replace string into the string. If the shifting went out
         * of bounds, then it will only go up to the number of written bytes.
         * so data is not overwritten. */
        for(cursor = 0; replace[cursor] != '\0'; cursor++) {
            length++;
            string[index + cursor] = replace[cursor];
        }
    }

    return occurrences;
}

int strrepl(char *string, const char *find, const char *replace, size_t size) {
    if(string == NULL) {
        fprintf(stderr, "%s", "strrepl: first argument, 'string', cannot be "
                              "NULL\n");
        exit(EXIT_FAILURE);
    }

    if(find == NULL) {
        fprintf(stderr, "%s", "strrepl: second argument, 'find', cannot be"
                              "NULL\n");
        exit(EXIT_FAILURE);
    }

    if(replace == NULL) {
        fprintf(stderr, "%s", "strrepl: third argument, 'replace', cannot be"
                              "NULL\n");
        exit(EXIT_FAILURE);
    }

    if(strlen(string) > size) {
        fprintf(stderr, "strrepl: string is larger than the buffer. the length"
                " of the buffer is %li, and the string's length is %li.\n",
                size, strlen(string));
        exit(EXIT_FAILURE);
    }

    if(strlen(find) >= strlen(replace)) {
        return strrepl_shift_back(string, find, replace, size);
    }

    return strrepl_shift_forward(string, find, replace, size);
}
