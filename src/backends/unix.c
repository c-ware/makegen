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
 * Contains the logic for producing Makefiles for UNIX.
*/

#include "backends.h"

#include "../makegen.h"

void dump_objs_variable(FILE *location, struct FilesystemPaths files) {
    int index = 0;
    char object_path[PATH_LENGTH + 1];

    fprintf(location, "%s", "OBJS=");

    for(index = 0; index < carray_length(&files); index++) {
        struct FilesystemPath file = files.contents[index];

        /* Has to be a C file */
        if(file.path[strlen(file.path) - 2] != '.' || file.path[strlen(file.path) - 1] != 'c')
            continue;

        /* Make it a .o file */
        strncpy(object_path, file.path, PATH_LENGTH);
        object_path[strlen(object_path) - 1] = 'o';


        fprintf(location, "%s", object_path);

        if(index != carray_length(&files))
            fprintf(location, "%s", " ");
    }

    fprintf(location, "%c", '\n');
}

void unix_project_makefile(struct ArgparseParser parser, struct FilesystemPaths files) {
    int index = 0;
    FILE *location = stdout;
    struct Inclusions *file_inclusions = NULL;

    file_inclusions = carray_init(file_inclusions, INCLUSION);

    /* Dump some variables */
    fprintf(location, "%s", "CC=cc\n");

    dump_objs_variable(location, files);
    fprintf(location, "%s", "\n\n");

    /* Dump some rules */
    fprintf(location, "%s", "all: $(OBJS)\n\n");

    /* Dump targets for each file */
    for(index = 0; index < carray_length(&files); index++) {
        int file_index = 0;
        char source_file[PATH_LENGTH + 1] = "";
        char object_file[PATH_LENGTH + 1] = "";
        struct FilesystemPath file = files.contents[index];

        /* Has to be a C file */
        if(file.path[strlen(file.path) - 2] != '.' || file.path[strlen(file.path) - 1] != 'c')
            continue;

        /* Create paths */
        strncpy(source_file, file.path, PATH_LENGTH);
        strncpy(object_file, file.path, PATH_LENGTH);
        object_file[strlen(object_file) - 1] = 'o';

        /* FILE.o: FILE.c ... */
        fprintf(location, "%s: %s", object_file, source_file);

        /* Yank the files inclusions this file has out */
        makegen_extract_inclusions_buffer(file.path, file_inclusions);

        /* Dump included files */
        for(file_index = 0; file_index < carray_length(file_inclusions); file_index++) {
            char resolved_inclusion[PATH_LENGTH + 1] = "";
            char *header = file_inclusions->contents[file_index].path;

            makegen_resolve_path(file.path, header, resolved_inclusion, PATH_LENGTH);
            fprintf(location, " %s", resolved_inclusion);
        }

        fprintf(location, "%c", '\n');
        fprintf(location, "\t$(CC) -c %s -o %s $(CFLAGS)\n", source_file, object_file);
        fprintf(location, "%c", '\n');
    }


    carray_free(file_inclusions, INCLUSION);
}
