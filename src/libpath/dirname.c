/*
 * Strip the last component of the path pointed to by PATH.
 * This is only indented to work with absolute paths, so do
 * not try to run this code on relative paths which contain
 * the .. or . special directories in them.
 *
 * The algorithm is quite simple.
 *
 * Have s be the string to operate on.
 * Have i be used to store the offset from the start of the string.
 * Have n be used to store the index of the start of the last valid
 * directory separator.
 *
 * 1. Set i to 0
 * 1. Set n to -1
 * 2. Increment i until s[i] is equal to '\0', or s[i + 1] is equal to '\0'
 * 3. To continue, s[i] must be equal to '/'.
 * 4. To continue, s[i + 1] must be not be equal to '/'
 * 5. Set n to i.
 * 6. Break loop
 * 7. Set s[n] to '\0'
 *
 * It is important to note here that this algorithm naturally handles
 * situations where the length of s is 0 or 1. i starts at zero, and as a
 * requirement of how C strings work, must be NUL terminated. If the length
 * of s is 0, then s[i], which decays into s[0], will break the loop
 * immediately. The same will apply for when the length of s is 2.
*/

#include <stdio.h>
#include <string.h>

#include "libpath.h"

void libpath_dirname(char *path) {
    int index = 0;
    int separators = 0;
    int separator_index = -1;

    if(strcmp(path, "/") == 0)
        return;

    /* Find the last index in path that is a / followed by any
     * character that is not / */
    for(index = 0; path[index] != '\0' && path[index + 1] != '\0'; index++) {
        if(path[index] != '/')
            continue;

        if(path[index + 1] == '/')
            continue;

        separator_index = index;
        separators++;
        index++;
    }

    /* Handle paths of the format `/[^\.\/]+/?` */
    if(separators == 1) {
        path[1] = '\0';

        return;
    }

    path[separator_index] = '\0';
}
