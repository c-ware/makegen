/*
 * Cursor-related functions.
*/

#include <stdlib.h>

#include "libmatch.h"

struct LibmatchCursor libmatch_cursor_init(char *buffer, int length) {
    struct LibmatchCursor new_cursor = {LIBMATCH_CURSOR_NULL};

    new_cursor.cursor = 0;
    new_cursor.length = length;
    new_cursor.buffer = buffer;

    return new_cursor;
}

struct LibmatchCursor libmatch_cursor_from_stream(FILE *stream) {
    struct LibmatchCursor new_cursor = {LIBMATCH_CURSOR_NULL};
    char *new_buffer = malloc(sizeof(char) * LIBMATCH_INITIAL_BUFFER_SIZE);

    int length = 0;
    int capacity = LIBMATCH_INITIAL_BUFFER_SIZE;
    char character = -1;

    /* Keep reading the linitial size until an EOF is found. */
    while((character = fgetc(stream)) != EOF) {
        if(length == capacity) {
            capacity += LIBMATCH_BUFFER_GROWTH;
            new_buffer = realloc(new_buffer, sizeof(char) * capacity);
        }


        new_buffer[length] = character;
        length++;
    }

    new_cursor.buffer = new_buffer;
    new_cursor.length = length;

    return new_cursor;
}

int libmatch_cursor_getch(struct LibmatchCursor *cursor) {
    int character = -1;

    /* Return EOF */
    if(cursor->cursor == cursor->length)
        return LIBMATCH_EOF;

    character = cursor->buffer[cursor->cursor];
    cursor->cursor++;

    /* Handle coordinates */
    if(character == '\n') {
        cursor->line++;
        cursor->character = 0;
    }

    cursor->character++;

    return character;
}

void libmatch_cursor_ungetch(struct LibmatchCursor *cursor) {
    if(cursor->cursor == 0)
        return;

    cursor->cursor--;

    /* Decrement line and character */
    if(cursor->buffer[cursor->cursor] == '\n') {
        int index = cursor->cursor - 1;
        int length = 0;

        cursor->line--;

        /* Determine the length of this line */
        while(index >= 0) {
            if(cursor->buffer[index] == '\n') {
                cursor->character = length;

                break;
            }
            
            index--; 
            length++;
        }

        return;
    }

    cursor->character--;
}

int libmatch_cursor_unwind(struct LibmatchCursor *cursor, int distance) {
    int unwound = 0;

    while((cursor->cursor > 0) && (unwound < distance)) {
        libmatch_cursor_ungetch(cursor);
        unwound++;
    }

    return unwound;
}

void libmatch_cursor_enable_pushback(struct LibmatchCursor *cursor) {
    cursor->pushback = 1;
}

void libmatch_cursor_disable_pushback(struct LibmatchCursor *cursor) {
    cursor->pushback = 0;
}

void libmatch_cursor_free(struct LibmatchCursor *cursor) {
    free(cursor->buffer);
}
