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
 * This header contains prototypes for the backends for producing different
 * types of Makefiles.
*/

#ifndef CWARE_MAKEGEN_BACKENDS_H
#define CWARE_MAKEGEN_BACKENDS_H

struct ArgparseParser;
struct FilesystemPaths;

/*
 * @docgen: function
 * @brief: create a unix makefile for a project.
 * @name: unix_project_makefile
 *
 * @description
 * @Create a new UNIX Makefile for a project. A 'project' is anything that will
 * @intend to result in a final binary, like a user application. Makefiles that
 * @are produced under this mode will have tests, and all object files compiled,
 * @along with an installation target for the final binary, and a clean target
 * @that will clean up things left by the build process.
 * @description
 *
 * @param parser: the argument parser to read arguments from
 * @type: struct ArgparseParser
 *
 * @param files: the files to use to make the Makefile
 * @type: struct FilesystemPaths *
*/
void unix_project_makefile(struct ArgparseParser parser, struct FilesystemPaths files);


/*
 * @docgen: function
 * @brief: create a unix makefile for a library
 * @name: unix_library_makefile
 *
 * @description
 * @Create a new UNIX Makefile for a library. I feel 'library' is more self
 * @explanatory than just 'project' but in a simple way, libraries do not result
 * @in a final binary. Rather, they are to be 'embedded' in a project which DOES
 * @result in a final binary. Libraries are still allowed to have tests, though.
 * @description
 *
 * @param parser: the argument parser to read arguments from
 * @type: struct ArgparseParser
 *
 * @param files: the files to use to make the Makefile
 * @type: struct FilesystemPaths *
*/
void unix_library_makefile(struct ArgparseParser parser, struct FilesystemPaths files);

#endif
