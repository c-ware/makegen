/*
 * Functions related to directly reading from a string.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libmatch.h"

int libmatch_read_literal(struct LibmatchCursor *cursor, char *buffer,
                          int length) {
    int buffer_cursor = 0;
    int written = 0;
    int character = -1;
    int escaped = 0;

    if(libmatch_cursor_getch(cursor) != '"') {
        fprintf(stderr, "libmatch_read_literal: cursor not positioned on a "
                "double quote (cursor=%i, string='%s')\n", cursor->cursor - 1,
                cursor->buffer);
        abort();
    }

    while((character = libmatch_cursor_getch(cursor)) != EOF) {
        if(character == '"' && escaped == 0)
            break;

        if(character == '\\' && escaped == 0) {
            escaped = 1;

            continue;
        }

        escaped = 0;

        buffer[buffer_cursor] = character;
        written++;
        buffer_cursor++;

        if(written == length)
            break;
    }

    /* Cursor will be positioned after the LENGTH-th character, and so should
     * be positioned on a double quote in correct circumstances. */
    if(written == length && cursor->buffer[cursor->cursor] != '"') {
        fprintf(stderr, "libmatch_read_literal: no ending double quote found! "
                "was the buffer too small? if so, consider using "
                "libmatch_read_alloc_literal (cursor=%i, string='%s')\n",
                cursor->cursor, cursor->buffer);
        abort();
    }

    /* Cursor will be positioned after the double quote since the length is not
     * met, leaving the occurrence of a double quote as the only condition that
     * can stop the loop (outside of an EOF) */
    if(written < length && cursor->buffer[cursor->cursor - 1] != '"') {
        fprintf(stderr, "libmatch_read_literal: no ending double quote found! "
                "was the buffer too small? if so, consider using "
                "libmatch_read_alloc_literal (cursor=%i, string='%s')\n",
                cursor->cursor, cursor->buffer);
        abort();
    }

    buffer[written] = '\0';

    return written;
}

char *libmatch_read_alloc_literal(struct LibmatchCursor *cursor) {
    int written = 0;
    int character = -1;
    int escaped = 0;
    int buffer_cursor = 0;
    int capacity = LIBMATCH_INITIAL_BUFFER_SIZE;
    char *buffer = malloc(sizeof(char) * (LIBMATCH_INITIAL_BUFFER_SIZE + 1));

    if(libmatch_cursor_getch(cursor) != '"') {
        fprintf(stderr, "libmatch_read_alloc_literal: cursor not positioned on"
                " a double quote (cursor=%i, string='%s')\n", cursor->cursor -
                1, cursor->buffer);
        abort();
    }

    while((character = libmatch_cursor_getch(cursor)) != EOF) {
        if(character == '"' && escaped == 0)
            break;

        if(character == '\\' && escaped == 0) {
            escaped = 1;

            continue;
        }

        escaped = 0;

        buffer[buffer_cursor] = character;
        buffer_cursor++;
        written++;

        /* Resize buffer */
        if(written == capacity) {
            capacity += LIBMATCH_BUFFER_GROWTH;
            buffer = realloc(buffer, sizeof(char) * capacity);
        }
    }

    if(cursor->buffer[cursor->cursor - 1] != '"') {
        fprintf(stderr, "libmatch_read_alloc_literal: no ending double quote "
                "found! (cursor=%i, string='%s')\n",
                cursor->cursor, cursor->buffer);
        abort();
    }

    buffer[buffer_cursor] = '\0';

    return buffer;
}

int libmatch_read_n(struct LibmatchCursor *cursor, char *buffer, int count) {
    int written = 0;
    int character = -1;
    int buffer_cursor = 0;

    if(count == 0)
        return 0;

    while((character = libmatch_cursor_getch(cursor)) != EOF) {
        buffer[buffer_cursor] = character;
        buffer_cursor++;
        written++;

        if(written == count)
            break;
    }

    buffer[written] = '\0';

    return written;
}

int libmatch_read_until(struct LibmatchCursor *cursor, char *buffer,
                        int length, const char *characters) {
    int written = 0;
    int character = -1;
    int buffer_cursor = 0;

    while((character = libmatch_cursor_getch(cursor)) != EOF) {
        if(strchr(characters, character) != NULL) {
            _libmatch_pushback(cursor);

            break;
        }

        buffer[buffer_cursor] = character;
        buffer_cursor++;
        written++;

        if(written == length)
            break;
    }

    buffer[buffer_cursor] = '\0';

    return written;
}

char *libmatch_read_alloc_until(struct LibmatchCursor *cursor,
                                const char *characters) {
    int written = 0;
    int character = -1;
    int buffer_cursor = 0;
    int capacity = LIBMATCH_INITIAL_BUFFER_SIZE;
    char *buffer = malloc(sizeof(char) * (LIBMATCH_INITIAL_BUFFER_SIZE + 1));

    while((character = libmatch_cursor_getch(cursor)) != EOF) {
        if(strchr(characters, character) != NULL) {
            libmatch_cursor_ungetch(cursor);

            break;
        }

        buffer[buffer_cursor] = character;
        buffer_cursor++;
        written++;

        if(written == capacity) {
            capacity += LIBMATCH_BUFFER_GROWTH;
            buffer = realloc(buffer, sizeof(char) * capacity);
        }
    }

    buffer[buffer_cursor] = '\0';

    return buffer;
}
