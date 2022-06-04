/*
 * Remove a directory at a certain location.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef _MSDOS
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "libpath.h"

int libpath_rmdir(const char *path) {
    return rmdir(path);
}
