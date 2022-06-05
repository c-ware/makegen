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

/*
 * @docgen: function
 * @brief: build a path to one of the standard directories
 * @name: makegen_build_stddir_path
 *
 * @description
 * @Build a path to one of the standard directories that a dumper
 * @function uses. The purpose of this function is to make sure
 * @that final path written to the buffer will always prefixed with './'
 *
 * @This function will also do error checking to verify that the
 * @final buffer is not full. As for the name 'stddir', I really
 * @just could not figure out a better name to describe the
 * @directories that the program accepts as options other than as
 * @a 'standard' directory. Similar to the standard streams.
 * @description
 *
 * @param parser: the command line parser
 * @param longform: longform of the option
 * @param shortform: shortform of the option
 * @param default_path: the default path to the directory
 * @param buffer: the buffer to write the path to
 * @param length: the maximum length of the buffer
*/
void makegen_build_stddir_path(struct ArgparseParser parser, const char *longform,
                               const char *shortform, const char *default_path,
                               char *buffer, int length) {
    int written = 0;
    const char *standard_directory = NULL;

    standard_directory = makegen_get_option_with_default(parser, longform, shortform, default_path);

    /* Write with a prefix */
    if(strstarts(standard_directory, "./") == 0)
        written = libpath_join_path(buffer, length, "./", standard_directory, NULL);
    else
        written = libpath_join_path(buffer, length, standard_directory, NULL);

    /* Perform error checks */
    if(written < length)
        return;

    fprintf(stderr, "build_stddir_path: buffer could not hold path to standard directory '%s'\n",
            standard_directory);
    abort();
}

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

/*
 * Dump the targets that convert all source files in the `src`
 * directory into an object file in the same location. This will
 * also dump all inclusions in them.
 *
 * @param location: the file to dump the targets to
 * @param parser: the argument parser
 * @param files: the files in the project
*/
void dump_source_targets(FILE *location, struct ArgparseParser parser,
                         struct FilesystemPaths files) {
    int index = 0;
    char source_path[PATH_LENGTH + 1] = "";
    struct Inclusions *file_inclusions = NULL;

    file_inclusions = carray_init(file_inclusions, INCLUSION);
    build_stddir_path(parser, "--src", "-s", "./src", source_path, PATH_LENGTH);

    /* Dump targets for each file */
    for(index = 0; index < carray_length(&files); index++) {
        int file_index = 0;
        char source_file[PATH_LENGTH + 1] = "";
        char object_file[PATH_LENGTH + 1] = "";
        struct FilesystemPath file = files.contents[index];

        /* File has to be inside of the source directory */
        if(strstarts(file.path, source_path) == 0)
            continue;

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

void unix_project_makefile(struct ArgparseParser parser, struct FilesystemPaths files) {
    FILE *location = stdout;


    dump_objs_variable(location, files);

    /* Dump some variables */
    fprintf(location, "%s", "CC=cc\n");
    fprintf(location, "%s", "\n\n");
    fprintf(location, "%s", "all: $(OBJS)\n\n");

    dump_source_targets(location, parser, files);

}
