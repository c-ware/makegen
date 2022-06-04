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
 * @brief: create a unix makefile
 * @name: unix_project_makefile
 *
 * @description
 * @Create a new UNIX Makefile in the directory the program was launched in,
 * @which should be in the root of the project.
 * @description
 *
 * @error: files is NULL
 *
 * @param parser: the argument parser to read arguments from
 * @type: struct ArgparseParser
 *
 * @param files: the files to use to make the Makefile
 * @type: struct FilesystemPaths *
*/
void unix_project_makefile(struct ArgparseParser parser, struct FilesystemPaths files);

#endif
