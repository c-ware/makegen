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

#include <dirent.h>
#include <stdlib.h>

#include "makegen.h"
#include "backends/backends.h"

void push_literal(struct PathStack *directories, const char *literal) {
    struct FilesystemPath new_path;

    INIT_VARIABLE(new_path);

    if(libpath_join_path(new_path.path, PATH_LENGTH, literal) == PATH_LENGTH) {
        fprintf(stderr, "push_literal: literal '%s' too long\n", literal);
        abort();
    }

    cstack_push(directories, new_path, PATH_STACK);
}

void exhaust_stack(struct FilesystemPaths *paths, struct PathStack *directories) {
    DIR *directory_stream = NULL;
    struct dirent *filesystem_node = NULL;

    /* Push initial directory */
    push_literal(directories, ".");

    while(cstack_length(directories) != 0) {
        struct FilesystemPath child_path = cstack_pop(directories, PATH_STACK);

        if((directory_stream = opendir(child_path.path)) == NULL) {
            fprintf(stderr, "exhaust_stack: child path '%s' could not be opened\n", child_path.path);
            exit(EXIT_FAILURE);
        }

        /* Iterate through the files and directories in this path */
        while((filesystem_node = readdir(directory_stream)) != NULL) {
            struct FilesystemPath child_node;

            INIT_VARIABLE(child_node);

            /* Ignore special directories */
            if(strcmp(filesystem_node->d_name, ".") == 0 || strcmp(filesystem_node->d_name, "..") == 0)
                continue;

            /* Make the final path */
            if(libpath_join_path(child_node.path, PATH_LENGTH, child_path.path,
                                 "/", filesystem_node->d_name, NULL) == PATH_LENGTH) {
                fprintf(stderr, "makegen: joined path between '%s' and '%s too long\n",
                        child_node.path, filesystem_node->d_name);
                exit(EXIT_FAILURE);
            }

            /* Add to the stack of directories, or into the array of files */
            if(libpath_is_directory(child_node.path) == 1) {
                cstack_push(directories, child_node, PATH_STACK);

                continue;
            }

            carray_append(paths, child_node, PATH_ARRAY);
        }

        closedir(directory_stream);
    }
}

static struct FilesystemPaths *collect_source_files(void) {
    struct FilesystemPaths *paths = NULL;
    struct PathStack *directories = NULL;

    paths = carray_init(paths, PATH_ARRAY);
    directories = cstack_init(directories, PATH_STACK);

    exhaust_stack(paths, directories);
    cstack_free(directories, PATH_STACK);

    return paths;
}

struct ArgparseParser setup_arguments(int argc, char **argv) {
    struct ArgparseParser parser = argparse_init("docgen", argc, argv);

    argparse_add_argument(&parser, "type");
    argparse_add_option(&parser, "--help", "-h", 0);

    /* Display a help message */
    if(argparse_option_exists(parser, "--help") != ARGPARSE_NOT_FOUND ||
       argparse_option_exists(parser, "-h") != ARGPARSE_NOT_FOUND) {

        printf("%s", "Usage: makegen TYPE [ --help | -h ]\n");
        printf("%s", "Generate different types of Makefiles\n");
        printf("%s", "\n");
        printf("%s", "Positional arguments:\n");
        printf("%s", "\ttype\t\t\tthe type of Makefile to make\n");
        printf("%s", "\n");
        printf("%s", "Options:\n");
        printf("\t--help, -h\t\tdisplay this message\n");

        exit(EXIT_FAILURE);
    }

    argparse_error(parser);

    return parser;
}

int main(int argc, char **argv) {
    int index = 0;
    struct FilesystemPaths *paths = NULL;
    struct ArgparseParser parser = setup_arguments(argc, argv);
    char buffer[256 + 1];

    paths = collect_source_files();
    /*
    makefile_unix(parser, *paths);
    */

    argparse_free(parser);
    carray_free(paths, PATH_ARRAY);

    return EXIT_SUCCESS;
}
