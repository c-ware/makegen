/*
 * Make a new directory at a certain location.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef _MSDOS
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "libpath.h"

int libpath_mkdir(const char *path, int mode) {
#ifdef _MSDOS
    return mkdir(path);
#else
#ifdef __WATCOMC__
    return mkdir(path);
#else
    return mkdir(path, mode);
#endif
#endif
}
