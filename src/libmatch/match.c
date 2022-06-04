/*
 * Functions used in matching text in a string.
*/

#include <string.h>
#include <stdarg.h>

#include "libmatch.h"

int libmatch_expect(struct LibmatchCursor *cursor, int count,
                    const char *characters) {
    int character = -1;
    int matched = 0;

    while((character = libmatch_cursor_getch(cursor)) != LIBMATCH_EOF) {
        if(strchr(characters, character) == NULL) {
            _libmatch_pushback(cursor);

            break;
        }

        matched++;

        if(matched == count)
            return 1;
    }

    return 0;
}

int libmatch_atleast(struct LibmatchCursor *cursor, int count,
                     const char *characters) {
    int character = -1;
    int matched = 0;

    while((character = libmatch_cursor_getch(cursor)) != LIBMATCH_EOF) {
        if(strchr(characters, character) == NULL) {
            _libmatch_pushback(cursor);

            break;
        }

        matched++;

    }

    if(matched >= count)
        return 1;

    return 0;
}

int libmatch_string_expect(struct LibmatchCursor *cursor, const char *string) {
    int matched = 0;
    int character = -1;
    int string_cursor = 0;

    while((character = libmatch_cursor_getch(cursor)) != LIBMATCH_EOF) {
        if(string[string_cursor] == character) {
            string_cursor++;

            if(string[string_cursor] == '\0')
                return 1;

            continue;
        }

        _libmatch_pushback(cursor);

        break;
    }

    return 0;
}

int libmatch_strings_expect(struct LibmatchCursor *cursor, ...) {
    va_list strings;
    const char *string = NULL;
    int index = 0;
    int cursor_position = cursor->cursor;
    int cursor_line = cursor->line;
    int cursor_character = cursor->character;

    va_start(strings, cursor);

    while((string = va_arg(strings, char *)) != NULL) {

        /* Reset cursor */
        if(libmatch_string_expect(cursor, string) == 0) {
            cursor->cursor = cursor_position;
            cursor->line = cursor_line;
            cursor->character = cursor_character;
            index++;

            continue;
        }

        return index;
    }

    return -1;
}

int libmatch_until(struct LibmatchCursor *cursor, const char *characters) {
    int matched = 0;
    int character = -1;

    while((character = libmatch_cursor_getch(cursor)) != LIBMATCH_EOF) {
        if(strchr(characters, character) != NULL) {
            _libmatch_pushback(cursor);

            break;
        }


        matched++;
    }

    return matched;
}

int libmatch_expect_next(struct LibmatchCursor *cursor,
                         const char *characters) {
    int next = libmatch_cursor_getch(cursor);

    if(next == EOF)
        return 0;

    if(strchr(characters, next) == NULL) {
        _libmatch_pushback(cursor);

        return 0;
    }

    return 1;
}

int libmatch_next_line(struct LibmatchCursor *cursor) {
    int skipped = 0;
    int character = -1;

    if(cursor->buffer[cursor->cursor] == '\n') {
        libmatch_cursor_getch(cursor);

        return 0;
    }

    while((character = libmatch_cursor_getch(cursor)) != LIBMATCH_EOF) {
        if(character == '\n')
            break;

        skipped++;
    }

    return skipped;
}
