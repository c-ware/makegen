/*
 * Determine if a path exists.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "libpath.h"

int libpath_exists(const char *path) {
    struct stat stat_buffer;

    __libpath_assert_nonnull("libpath_exists", path);

    if(stat(path, &stat_buffer) == -1)
        return 0;

    return 1;
}
