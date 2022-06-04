/*
 * Strip the last component of the path pointed to by PATH.
 * This is only indented to work with absolute paths, so do
 * not try to run this code on relative paths which contain
 * the .. or . special directories in them.
*/

#include <stdio.h>

#include "libpath.h"

void libpath_dirname(char *path) {
    printf("%s\n", path);
}
