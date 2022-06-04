/*
 * Determine if a path is a directory.
*/

#include <string.h>
#include <sys/stat.h>

#include "libpath.h"

int libpath_is_directory(const char *path) {
    struct stat stat_buffer;

    INIT_VARIABLE(stat_buffer);

    if(stat(path, &stat_buffer) == -1)
        return 0;

    if((stat_buffer.st_mode & S_IFMT) != S_IFDIR)
        return 0;

    return 1;
}
