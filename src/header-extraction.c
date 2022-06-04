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
 * This file is stores functions used for extracting header files that are
 * included in a source file. We have this behavior because modification of a
 * header may or may not mean important parts of the header like constants,
 * function signatures, structure bodies, and more, have changed. This can lead
 * to bugs and inconsistencies in the final binary.
*/

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "makegen.h"

static int line_has_inclusion(struct LibmatchCursor cursor) {
    /* Does this line have a directive at the start? */
    if(libmatch_cursor_getch(&cursor) != '#')
        return 0;

    /* Local inclusions only */
    if(libmatch_cond_before(&cursor, '\"', "\n") == 0)
        return 0;

    /* Does this line have an alphabetical character before the end of the
     * line? */
    if(libmatch_cond_before(&cursor, '\n', LIBMATCH_ALPHA) == 1)
        return 0;

    libmatch_cursor_enable_pushback(&cursor);
      
    /* Some people are insane and so put a bunch of spaces and or
     * whitespace after the initial directive pound sign, so we have
     * to account for these people. */
    libmatch_until(&cursor, LIBMATCH_ALPHA);

    if(libmatch_string_expect(&cursor, "include") == 0)
        return 0;

    return 1;
}

static void extract_inclusions(FILE *file, struct Inclusions *inclusions) {
    struct LibmatchCursor cursor;
    struct Inclusion new_inclusion;

    INIT_VARIABLE(cursor);
    INIT_VARIABLE(new_inclusion);

    liberror_is_null(extract_inclusions, file);
    liberror_is_null(extract_inclusions, inclusions);

    cursor = libmatch_cursor_from_stream(file);

    /* Starts off at the first line */
    while(cursor.cursor < cursor.length) {
        if(line_has_inclusion(cursor) == 0) {
            libmatch_next_line(&cursor);

            continue;
        }

        libmatch_cursor_enable_pushback(&cursor);

        /* Get to the inclusion */
        libmatch_until(&cursor, "\"");
        
        libmatch_cursor_getch(&cursor);
        
        /* Read the inclusion path */
        if(libmatch_read_until(&cursor, new_inclusion.path, INCLUSION_PATH_LENGTH, "\"") == INCLUSION_PATH_LENGTH) {
            fprintf(stderr, "makegen: inclusion path on line %i too long\n", cursor.line + 1);
            exit(EXIT_FAILURE);
        }

        carray_append(inclusions, new_inclusion, INCLUSION);
    }

    libmatch_cursor_free(&cursor);
}

struct Inclusions *makegen_extract_inclusions(const char *path) {
    FILE *source_file = NULL;
    struct Inclusions *inclusions = NULL;

    liberror_is_null(makegen_extract_inclusions, path);
    inclusions = carray_init(inclusions, INCLUSION);

    if((source_file = fopen(path, "r")) == NULL) {
        fprintf(stderr, "makegen: could not open file '%s' (%s)\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    extract_inclusions(source_file, inclusions);
    fclose(source_file);

    return inclusions;
}

void makegen_extract_inclusions_buffer(const char *path, struct Inclusions *inclusions) {
    FILE *source_file = NULL;

    liberror_is_null(makegen_extract_inclusions, path);
    liberror_is_null(makegen_extract_inclusions, inclusions);

    /* Reset the array */
    inclusions->length = 0;

    if((source_file = fopen(path, "r")) == NULL) {
        fprintf(stderr, "makegen: could not open file '%s' (%s)\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    extract_inclusions(source_file, inclusions);
    fclose(source_file);
}
