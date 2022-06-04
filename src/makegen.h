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

#ifndef CWARE_MAKEGEN_H
#define CWARE_MAKEGEN_H

/* Common libraries */
#include "carray.h"
#include "cstack.h"
#include "liberror.h"
#include "libpath/libpath.h"
#include "libmatch/libmatch.h"
#include "argparse/argparse.h"

/* Limits */
#define PATH_LENGTH             512 + 1
#define INCLUSION_PATH_LENGTH   256 + 1
#define FILENAME_LENGTH         128 + 1

/* Data structure properties */
#define INCLUSION_HEAP  1
#define INCLUSION_TYPE  struct Inclusion
#define INCLUSION_FREE(value)

#define PATH_ARRAY_HEAP  1
#define PATH_ARRAY_TYPE  struct FilesystemPath
#define PATH_ARRAY_FREE(value)

#define PATH_STACK_HEAP  1
#define PATH_STACK_TYPE  struct FilesystemPath
#define PATH_STACK_FREE(value)

/* Useful macros */
#define INIT_VARIABLE(variable) \
    memset(&(variable), 0, sizeof((variable)))

#define WHITESPACE_WITHOUT_LF   " \t\v\r"

/*
 * @docgen: structure
 * @brief: represents a file system path
 * @name: FilesystemPath
 *
 * @field path[PATH_LENGTH + 1]: the stored path
 * @type: char
*/
struct FilesystemPath {
    char path[PATH_LENGTH + 1];
};

/*
 * @docgen: structure
 * @brief: represents a stack of filesystem paths
 * @name: PathStack
 *
 * @field length: the length of the stack
 * @type: int
 *
 * @field capacity: the capacity of the stack
 * @type: int
 *
 * @field contents: the contents of the stack
 * @type: struct FilesystemPaths *
*/
struct PathStack {
    int length;
    int capacity;
    struct FilesystemPath *contents;
};

/*
 * @docgen: structure
 * @brief: an array of file paths
 * @name: FilesystemPaths
 *
 * @field length: the length of the array
 * @type: int
 *
 * @field capacity: the capacity of the array
 * @type: int
 *
 * @field contents: the contents of the array
 * @type: struct FilesystemPath *
*/
struct FilesystemPaths {
    int length;
    int capacity;
    struct FilesystemPath *contents;
};

/*
 * @docgen: structure
 * @brief: represents a header file that a source file includes.
 * @name: Inclusion
 *
 * @field path[INCLUSION_PATH_LENGTH + 1]: the path of the inclusion
 * @type: char
*/
struct Inclusion {
    char path[INCLUSION_PATH_LENGTH + 1];
};

/*
 * @docgen: structure
 * @brief: an array of inclusions
 * @name: Inclusions
 *
 * @field length: the length of the array
 * @type: int
 *
 * @field capacity: the capacity of the array
 * @type: int
 *
 * @field contents: the contents of the array
 * @type: struct Inclusion *
*/
struct Inclusions {
    int length;
    int capacity;
    struct Inclusion *contents;
};

/*
 * @docgen: function
 * @brief: extract inclusions from a source file
 * @name: makegen_extract_inclusions
 *
 * @description
 * @This function will parse a file, and extract all #include directives
 * @from it. However, this does NOT extract system headers, and instead will
 * @just extract local inclusions. This also does not apply recursively. It
 * @will only extract direct inclusions.
 * @description
 *
 * @error: path is NULL
 *
 * @param path: the file to extract inclusions from
 * @type: const char *
 *
 * @return: an array of inclusions in the file
 * @type: struct Inclusions *
*/
struct Inclusions *makegen_extract_inclusions(const char *path);

/*
 * @docgen: function
 * @brief: extract inclusions from a source file with an existing buffer
 * @name: makegen_extract_inclusions_buffer
 *
 * @description
 * @This function will parse a file, and extract all #include directives
 * @from it. However, this does NOT extract system headers, and instead will
 * @just extract local inclusions. This also does not apply recursively. It
 * @will only extract direct inclusions.
 * @
 * @Unlike its sister function, however, this does not make a new array to
 * @hold the inclusions. Rather, it uses an existing array that it resets.
 * @description
 *
 * @error: path is NULL
 *
 * @param path: the file to extract inclusions from
 * @type: const char *
 *
 * @param inclusions: the inclusions array to write to
 * @type: struct Inclusions *
*/
void makegen_extract_inclusions_buffer(const char *path, struct Inclusions *inclusions);

/*
 * @docgen: function
 * @brief: determine the path to a header relative to the makefile
 * @name: makegen_resolve_path
 *
 * @description
 * @Determine the path to a header that a source file includes, relative to the
 * @Makefile, based off of the location of the source file and writes the final
 * @path to a buffer.
 * @description
 *
 * @error: source_path is NULL
 * @error: header_path is NULL
 * @error: buffer_path is NULL
 * @error: length is negative
 *
 * @param source_path: the path to the source file
 * @type: const char *
 *
 * @param header_file: the path to the header file
 * @type: const char *
 *
 * @param buffer: the buffer to write the resolved path to
 * @type: char *
 *
 * @param length: the length of the buffer
 * @type: int
 *
 * @return: the number of bytes written
 * @type: int
*/
int makegen_resolve_path(const char *source_path, const char *header_path,
                         char *buffer, int length);




#endif
