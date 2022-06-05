/*
 * Copyright 2022 C-Ware
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *    
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

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

const char *makegen_get_option_with_default(struct ArgparseParser parser,
                                            const char *longform,
                                            const char *shortform,
                                            const char *default_option) {

    if(argparse_option_exists(parser, longform) == 1)
        return argparse_get_option_parameter(parser, longform, 0);

    if(argparse_option_exists(parser, shortform) == 1)
        return argparse_get_option_parameter(parser, shortform, 0);

    return default_option;
}
