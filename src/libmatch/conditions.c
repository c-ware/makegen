/*
 * Functions for evaluating conditions based off the position
 * of the cursor.
*/

#include <string.h>

#include "libmatch.h"

int libmatch_cond_before(struct LibmatchCursor *cursor, int ch,
                         const char *characters) {
    int index = cursor->cursor;

    while(index < cursor->length) {
        int cursor_char = cursor->buffer[index];

        if(cursor_char == ch)
            return 1;

        if(strchr(characters, cursor_char) != NULL)
            return 0;

        index++;
    }

    return 0;
}
