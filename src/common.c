/*
 * Some common / useful functions that a backend should use.
*/

#include "makegen.h"

int makegen_enumerate_target(const char *string) {
    if(strcmp(string, "project") == 0)
        return MAKEGEN_TARGET_PROJECT;
    if(strcmp(string, "library") == 0)
        return MAKEGEN_TARGET_LIBRARY;

    fprintf(stderr, "makegen: unknown project type '%s'\n", string);
    fprintf(stderr, "%s", "Try 'makegen --help' for more information.\n");
    exit(EXIT_FAILURE);

    return MAKEGEN_UNKNOWN;
}

int makegen_enumerate_dialect(const char *string) {
    if(strcmp(string, "unix") == 0)
        return MAKEGEN_DIALECT_UNIX;

    fprintf(stderr, "makegen: unknown dialect type '%s'\n", string);
    fprintf(stderr, "%s", "Try 'makegen --help' for more information.\n");
    exit(EXIT_FAILURE);

    return MAKEGEN_UNKNOWN;
}
