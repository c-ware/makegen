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
 * This file contains functions that are used for resolving a relative path
 * that contains `..` and `.` path components. The form expected is (in regex
 * form) (\.\.)+/.+
 *
 * We implement this to solve the problem of resolving header paths so that the
 * Makefile can include header changes in the re-compilation of a source file.
 * If a file `src/main.c` includes `../../main.h`, we cannot simply write
 *
 * src/main.o: src/main.c ../../main.h
 *    ...
 *
 * Into the Makefile, because the header relative to the Makefile:
 * A) Might not exist
 * B) Exists, but is the incorrect header
 *
 * If `src/main.c` were to include a file in the same directory as it, or in a
 * subdirectory inside of its directory, say, `main.h`, `./main.h`, or
 * `headers/main.h`, then it is as simple as stripping off the `./` (if it
 * exists) and tacking the inclusion path onto the directory path of `src/main.c`
 * to form `src/headers/main.h`
 *
 * However, this logic does not apply for inclusions that reference a parent
 * directory. For example, ../main.h`. In this scenario, we have to consume
 * repeated parent directories (../) until we come across the first non-parent
 * component. For each parent directory component we consume, we remove a
 * component of the path to the file. Once all have been consumed, append the
 * remaining path.
*/

#include "makegen.h"
#include "backends/backends.h"

int makegen_resolve_path(const char *source_path, const char *header_path,
                          char *buffer, int length) {
    int written = 0;
    int parent_references = 0;
    char source_path_dirname[PATH_LENGTH + 1] = "";
    char header_path_dirname[PATH_LENGTH + 1] = "";

    /* Strip the name of the source file off of the path */
    strncpy(source_path_dirname, source_path, PATH_LENGTH);
    libpath_dirname(source_path_dirname);

    /* If the inclusion path does not start with `../` then that means
     * the inclusion is happening in this directory, rather than a parent. */
    if(strstarts(header_path, "../") == 0) {
        written = libpath_join_path(buffer, length, source_path_dirname, "/", header_path, NULL);

        if(written >= length) {
            fprintf(stderr, "makegen_resolve_path: buffer is too small to fit resolved path (%s:%i)\n",
                    __FILE__, __LINE__);
            abort();
        }

        return written;
    }

    /* Inclusions involves referencing a parent directory. We strip
     * off the first 3n characters because the header path header
     * path *should* be essentially be chained references. If we
     * strip these off, we can tack this on the final path. */
    parent_references = strcount(header_path, "../");
    strncpy(header_path_dirname, header_path + (3 * parent_references), PATH_LENGTH);

    /* Keep ripping dirnames off until there are no more parent references */
    while(parent_references != 0) {
        libpath_dirname(source_path_dirname);

        parent_references--;
    }

    /* Write the final path */
    written = libpath_join_path(buffer, length, source_path_dirname, "/", header_path_dirname, NULL);

    if(written >= length) {
        fprintf(stderr, "makegen_resolve_path: buffer is too small to fit resolved path (%s:%i)\n",
                __FILE__, __LINE__);
        abort();
    }

    return written;
}
