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
 * Contains the logic for producing a UNIX-style Makefile.
*/

#include "backends.h"

#include "../makegen.h"

void makefile_unix(struct ArgparseParser parser, struct FilesystemPaths files) {
    int index = 0;
    FILE *location = stdout;

    for(index = 0; index < carray_length(&files); index++) {
        struct Inclusions *file_inclusions = NULL;
        struct FilesystemPath file = files.contents[index];

        /* Has to be a C file */
        if(file.path[strlen(file.path) - 1] != 'c')
            continue;

        /* 'Change' this to an object file temporarily */
        file.path[strlen(file.path) - 1] = 'o';

        fprintf(location, "%s:", file.path);

        /* 'Change' this back to a C file */
        file.path[strlen(file.path) - 1] = 'c';

        fprintf(location, " %s\n", file.path);
    }
}
