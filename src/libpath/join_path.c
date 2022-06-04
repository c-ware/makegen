/*
 * Join a variable number of paths.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "libpath.h"

size_t libpath_join_path(char *buffer, int length, ...) {
    char *path = NULL;
    size_t written = 0;
    va_list paths = {0};
    int buffer_cursor = 0;

    __libpath_assert_nonnull("libpath_join_path", buffer);
    __libpath_assert_nonnegative("libpath_join_path", length);

    va_start(paths, length);

    /* Get each path component */
    while((path = va_arg(paths, char*)) != NULL) {
        int path_cursor = 0;

        if(buffer_cursor == length) {
            written = length + 1;

            break;

        }

        /* Write the path */
        for(path_cursor = 0; path[path_cursor] != '\0'; path_cursor++) {
            if(buffer_cursor == length)
                break;

            buffer[buffer_cursor] = path[path_cursor];
            buffer_cursor++;
            written++;
        }
    }

    va_end(paths);
    buffer[buffer_cursor] = '\0';

    return written;
}
