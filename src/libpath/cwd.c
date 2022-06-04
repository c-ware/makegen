/*
 * Retrieve the current working directory.
*/

#include <string.h>
#include <unistd.h>

int libpath_cwd(char *buffer, int length) {
    getcwd(buffer, length);

    return strlen(buffer);
}
