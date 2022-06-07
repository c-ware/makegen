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
 * @docgen: project
 * @brief: generate makefiles for different dialects of make
 * @name: makegen
 *
 * @arguments
 * @makegen TARGET DIALECT [ --help | -h ] [ --src SRC | -s SRC]
 * @                       [ --tests TESTS | -t TESTS ]
 * @                       [ --main MAIN | -m MAIN]
 * @                       [ --binary BINARY | -b BINARY]
 * @                       [ --name NAME | -n NAME ]
 * @arguments
 *
 * @description
 * @A program designed to create Makefiles for many types of dialects of Make,
 * @and for many different types of purposes. The Makefiles outputted are intended
 * @to be used for the C programming language. As such, this program should be
 * @used to generate Makefiles that build C applications, and libraries.
 * @
 * @This program originated as a collection of M4 macros that would be evaluated
 * @from a template, and into a fully fledged Makefile. We originally did this
 * @because we wanted to make sure our Makefiles were as portable as possible,
 * @and common things that might be found in BSD Make (like variable modifiers,
 * @and the != variable assignment operator), and in GNU Make (wildcard), did
 * @not exist in POSIX make. So, we decided to write M4 macros that would be
 * @pre-cooked so that the end user of the library or application did not need
 * @to have M4 on their system to build the program.
 * @
 * @As primitive as this system sounds at first thought, it actually works quite
 * @well. Although, it had several drawbacks. The first one is that it can easily
 * @lead to inconsistency between the source and header files. With the way we
 * @originally did it, the only dependency of any object file was the source
 * @file itself, and not anything else. This was a problem because if you edited
 * @the header file, you had to either re-compile the file manually (which
 * @completely defeats the purpose of Make), or clean all object files and re-
 * @build the program. This also stretches the purpose of Make.
 * @
 * @The other problem that we came across was that, well, our Makefiles were not
 * @actually usable on NetBSD and OpenSolaris 10 from Oracle. This was because
 * @these dialects of Make do not allow implicit rules (.c.o) to build files that
 * @are in separate directory to the Makefile. This lead us to two main solutions
 * @to this problem. The first was recursive Makefiles, where each Makefile would
 * @only build dependencies in its directory, and call Make on any Makefiles in
 * @child directories of the Makefile. This at first was promising, but we quickly
 * @realized that this would very quickly become unmaintainable, and messy. This
 * @was mostly because we would need to produce Makefiles for two main dialects
 * @of Make-- POSIX Make, and nmake. The other solution, was to have a single
 * @Makefile for each dialect of Make that we wanted to target. This would allow
 * @us to create Makefiles for POSIX Make, without having to use recursive Make.
 * @And thus, makegen was born.
 * @
 * @Makegen allows the programmer to automatically generate Makefiles which build
 * @tests with all the object files of the program, build all source files into
 * @object files, build and install libraries with shared objects and headers,
 * @and more. All source files and tests have their direct inclusions extracted,
 * @so it is harder (but still not impossible) for inconsistencies to arise in
 * @the final product, as all directly included headers will be considered
 * @dependencies of the object or test file.
 * @
 * @Here is a table of all types of all targets for this program.
 * @table
 * @sep: ;
 * @Target;Description
 * @library;generate a makefile for a library (does not have a final binary)
 * @project;generate a makefile for a program (has a final binary)
 * @table
 * @
 * @Some targets will require certain options to give further information about
 * @how to generate the final Makefile. What follows is a table of what each
 * @target requires in terms of options. Only long-form options are given, so
 * @please scroll down for explanations of all the options that this program
 * @will expect.
 * @
 * @table
 * @sep: ;
 * @Target;Required;Optional
 * @project;--binary, --main;--src, --tests
 * @library;--name;--src, --tests
 * @table
 * @
 * @And the dialects of Make that this program can produce for the above
 * @targets are as follows.
 * @table
 * @sep: ;
 * @Dialect;Description
 * @unix;generate a POSIX Make which is intended to be ran under a *NIX
 * @table
 * @
 * @This program can accept the following options. Please see the synopsis for
 * @all the parameters they take. It should be noted that not all of these options
 * @make sense, or do anything on their own. However, they should make sense in
 * @the context of the target they are associated with (see the second table
 * @above for more information for what targets require what options)
 * @
 * @--help, -h
 * @    Display a smaller help menu
 * @
 * @--src, -s
 * @    The path to the source directory
 * @
 * @--tests, -t
 * @    The path to the tests directory
 * @
 * @--main, -m
 * @    The path to the file with the entry point of the program
 * @
 * @--binary, -b
 * @    The name of the final binary of the program
 * @
 * @--name, -n
 * @    The name of the library, and shared object
 * @description
 *
 * @reference: cware(cware)
*/

#include <dirent.h>
#include <stdlib.h>

#include "makegen.h"
#include "backends/backends.h"
#include "argparse/argparse_internal.h"

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
    struct ArgparseParser parser = argparse_init("makegen", argc, argv);

    /* Add some arguments */
    argparse_add_argument(&parser, "target");
    argparse_add_argument(&parser, "dialect");

    /* Add some options */
    argparse_add_option(&parser, "--help", "-h", 0);
    argparse_add_option(&parser, "--src", "-s", 1);
    argparse_add_option(&parser, "--tests", "-t", 1);
    argparse_add_option(&parser, "--main", "-m", 1);
    argparse_add_option(&parser, "--binary", "-b", 1);
    argparse_add_option(&parser, "--name", "-n", 1);

    /* Display a help message */
    if(argparse_option_exists(parser, "--help") != 0 ||
       argparse_option_exists(parser, "-h") != 0) {

        printf("%s", "Usage: makegen TARGET DIALECT [ --help | -h ]");
        printf("%s", " [ --src SRC | -s SRC]\n");
        printf("%s", "                              [ --tests TESTS | -t SRC]");
        printf("%s", " [ --main MAIN | -m MAIN]\n");
        printf("%s", "                              [ --binary BINARY | -b BINARY]");
        printf("%s", " [ --name NAME | -n NAME ]\n");
        printf("%c", '\n');
        printf("%s", "Generate different types of Makefiles\n");
        printf("%s", "\n");
        printf("%s", "Positional arguments:\n");
        printf("%s", "\ttarget\t\t\tthe type of Makefile to create\n");
        printf("%s", "\tdialect\t\t\tthe dialect of Makefile to create\n");
        printf("%s", "\n");
        printf("%s", "Options:\n");
        printf("%s", "\t--help, -h\t\tdisplay this message\n");
        printf("%s", "\t--src, -s\t\tthe directory containing source code\n");
        printf("%s", "\t--tests, -t\t\tthe directory containing test programs\n");
        printf("%s", "\t--main, -m\t\tthe file containing the entry point\n");
        printf("%s", "\t--binary, -b\t\tthe name of the binary\n");
        printf("%s", "\t--name, -n\t\tthe name of the library and shared object\n");

        exit(EXIT_FAILURE);
    }

    argparse_error(parser);

    /* Make sure an invalid target or dialect was not passed. */
    makegen_enumerate_target(argparse_get_argument(parser, "target"));
    makegen_enumerate_dialect(argparse_get_argument(parser, "dialect"));

    return parser;
}

int main(int argc, char **argv) {
    struct FilesystemPaths *paths = NULL;
    struct ArgparseParser parser = setup_arguments(argc, argv);
    const char *target = argparse_get_argument(parser, "target");
    const char *dialect = argparse_get_argument(parser, "dialect");

    paths = collect_source_files();

    /* Load the target maker */
    switch(makegen_enumerate_target(target)) {
        case MAKEGEN_TARGET_PROJECT:
            unix_project_makefile(parser, *paths);

            break;
        case MAKEGEN_TARGET_LIBRARY:
            unix_library_makefile(parser, *paths);

            break;
    }

    argparse_free(parser);
    carray_free(paths, PATH_ARRAY);

    return EXIT_SUCCESS;
}
