#ifndef CWARE_LIBCMATCH_H
#define CWARE_LIBCMATCH_H

#include <stdio.h>

#define LIBMATCH_CURSOR_NULL 0, 0, NULL, 0, 0, 0
#define LIBMATCH_EOF         -1

#define LIBMATCH_INITIAL_BUFFER_SIZE    1024
#define LIBMATCH_BUFFER_GROWTH          512

/* Character classes */
#define LIBMATCH_LOWER      "abcdefghijklmnopqrstuvwxyz"
#define LIBMATCH_UPPER      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LIBMATCH_NUMERIC    "0123456789"
#define LIBMATCH_ALPHA      LIBMATCH_LOWER LIBMATCH_UPPER
#define LIBMATCH_ALPHANUM   LIBMATCH_LOWER LIBMATCH_ALPHA LIBMATCH_NUMERIC
#define LIBMATCH_WHITESPACE " \t\v\n\r"

#define LIBMATCH_UNPRINTABLE    \
    "\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xA\xB\xC\xD\xE\xF\x10\x11\x12\x13\x14" \
    "\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x20"
#define LIBMATCH_PRINTABLE      \
    "@`!Aa\"Bb#Cc$Dd%Ee&Ff'Gg(Hh)Ii*Jj+Kk,Ll-Mm.Nn/Oo0Pp1Qq2Rr3Ss4Tt5Uu6Vv7W" \
    "w8Xx9Yy:Zz;[{<\\|=]}>^~?"

#define _libmatch_pushback(cursor)          \
    if(cursor->pushback == 1) {             \
        libmatch_cursor_ungetch(cursor);    \
    }

/*
 * A 'cursor' used to tell where the matching process is in a
 * stream.
*/
struct LibmatchCursor {
    int length;
    int cursor;
    char *buffer;

    /* Coordinates-- useful for error reporting */
    int line;
    int character;

    /* Settings */
    int pushback;
};

/*
 * Initializes a new cursor with an existing buffer.
 *
 * @param buffer: the text buffer
 * @param length: the length of the buffer (not maximum length)
 * @return: a new cursor
*/
struct LibmatchCursor libmatch_cursor_init(char *buffer, int length);

/*
 * Initialize a new cursor by consuming a stream into a
 * heap-allocated buffer. The file cursor of the stream will
 * be at the end of the file after this is complete.
 *
 * @param stream: the stream to consume
 * @return: a new cursor
*/
struct LibmatchCursor libmatch_cursor_from_stream(FILE *stream);

/*
 * Returns the next character in the buffer, and advances the
 * cursor. If the end of the buffer is reached, LIBMATCH_EOF is
 * returned from this function, and the cursor is not advanced.
 *
 * @param cursor: the cursor to use
 * @return: the next character
*/
int libmatch_cursor_getch(struct LibmatchCursor *cursor);

/*
 * Enables pushback of the cursor on failure of a function.
 * 
 * @param cursor: the cursor to use
*/
void libmatch_cursor_enable_pushback(struct LibmatchCursor *cursor);

/*
 * Disables pushback of the cursor on failure of a function.
 * 
 * @param cursor: the cursor to use
*/
void libmatch_cursor_disable_pushback(struct LibmatchCursor *cursor);

/*
 * Pushes the cursor back by one character. If the cursor is
 * at the start of the buffer, then the cursor is not advanced
 * backwards.
 *
 * @param cursor: the cursor to use
*/
void libmatch_cursor_ungetch(struct LibmatchCursor *cursor);

/*
 * Pushes back the cursor n slots, until the cursor is at position
 * zero.
 *
 * @param cursor: the cursor to push back
 * @param distance: the number of characters to push back
 * @return: number of characters pushed back.
*/
int libmatch_cursor_unwind(struct LibmatchCursor *cursor, int distance);

/*
 * Releases a cursor from memory. Only use this if the cursor has a
 * buffer that is allocated on the heap!
 *
 * @param cursor: the cursor to release from memory
*/
void libmatch_cursor_free(struct LibmatchCursor *cursor);

/*
 * Assert that the exact next n characters are in a set of
 * characters. Once the correct number of characters are
 * matched, no more matching is done. It only advances the
 * cursor as many times as it needs to.
 *
 * @param cursor: the cursor to use
 * @param count: the number of times to match
 * @param characters: the legal characters
 * @return: 1 if the match was successful, 0 if it failed
*/
int libmatch_expect(struct LibmatchCursor *cursor, int count,
                    const char *characters);

/*
 * Assert that at least the next n characters are in a set
 * of characters. Rather than stop advancing the cursor as
 * soon as the correct number of matches are made, the
 * cursor will be advanced until the first non-matching
 * character.
 *
 * @param cursor: the cursor to use
 * @param count: the number of times to match
 * @param characters: the legal characters
 * @return: 1 if the match was successful, 0 if it failed
*/
int libmatch_atleast(struct LibmatchCursor *cursor, int count,
                     const char *characters);

/*
 * Assert that a string starts at the cursor. Advances the
 * cursor forward for each character in the string until the
 * NUL byte is reached, or a character that does not match its
 * corresponding character in the string is found. If such a
 * character is found, the first occurrence will be pushed
 * back into the cursor buffer and the match will fail.
 *
 * @param cursor: the cursor to use
 * @param string: the string to check
 * @return: 1 if the match was successful, 0 if it failed
*/
int libmatch_string_expect(struct LibmatchCursor *cursor, const char *string);

/*
 * Assert that one or more possible strings appear at the current
 * cursor location without advancing the cursor for each one.
 * Instead, only the string that matches will advance the cursor.
 * The arguments passed to the function must have the last one be
 * NULL to signal the end of the list.
 *
 * @param cursor: the cursor to use
 * @return: the index of the string in the argument list (0-indexed), or -1
*/
int libmatch_strings_expect(struct LibmatchCursor *cursor, ...);

/*
 * Advances the cursor until a character is found that does not
 * match any of the characters provided. The first non-matching
 * character that is found is pushed back into the buffer.
 *
 * @param cursor: the cursor to use
 * @param characters: the legal characters
 * @return: number of characters matched
*/
int libmatch_until(struct LibmatchCursor *cursor, const char *characters);

/*
 * Asserts that the next character in the cursor buffer is a
 * character in a set. If the character is matched, the cursor
 * is advanced. Otherwise, it is pushed back.
 *
 * @param cursor: the cursor to use
 * @param characters: the legal characters
 * @return: 1 if the match was successful, 0 if it failed
*/
int libmatch_expect_next(struct LibmatchCursor *cursor,
                         const char *characters);

/*
 * Skip the current line, and position the cursor to the start of
 * the next line.
 *
 * @param cursor: the cursor to use
 * @return: number of characters skipped, excluding the new line
*/
int libmatch_next_line(struct LibmatchCursor *cursor);

/*
 * Reads a string literal from the cursor position onwards. When
 * this function is invoked, The cursor must be positioned on the
 * starting quote of the string. This means that:
 *
 * cursor->buffer[cursor->cursor]
 *
 * Must evaluate to a double quote. If this is not the case, the
 * function will error. When parsing the string literal, the
 * sequence '\"' will be interpreted as a literal double quote in
 * the string. Otherwise, when a double quote is met, the string
 * will cease parsing.
 *
 * If the length is reached and the next immediate character is
 * not an unescaped double quote, an error will be raised. In a
 * situation where this behavior is not desirable, use a dynamic
 * buffer.
 *
 * The final string will be NUL-terminated.
 *
 * @param cursor: the cursor to use
 * @param buffer: the buffer to write the literal into
 * @param length: the maximum length of the buffer.
 * @return: length of the string
*/
int libmatch_read_literal(struct LibmatchCursor *cursor, char *buffer,
                          int length);

/*
 * Reads n characters from the cursor's buffer without the same
 * restrictons as libmatch_read_literal, like requiring an
 * opening double quote and ending with one. It just reads raw
 * characters. The buffer will be NUL terminated.
 *
 * @param cursor: the cursor to read from
 * @param buffer: the buffer to write to
 * @param count: number of characters to read
 * @return: number of characters read
*/
int libmatch_read_n(struct LibmatchCursor *cursor, char *buffer, int count);

/*
 * Same as libmatch_read_literal, except that it does builds
 * a new NUL-terminated string to store the literal in. The
 * final buffer must be released from memory.
 *
 * @param cursor: the cursor to use
 * @return: the new buffer
*/
char *libmatch_read_alloc_literal(struct LibmatchCursor *cursor);

/*
 * Reads from the cursor until a character in a set are met,
 * or the length of the buffer is met. The string is
 * NUL-terminated.
 *
 * @param cursor: the cursor to use
 * @param buffer: the buffer to read the text into
 * @param length: the maximum length of the buffer
 * @param characters: the characters to stop at
 * @return: the number of characters matched
*/
int libmatch_read_until(struct LibmatchCursor *cursor, char *buffer,
                        int length, const char *characters);

/*
 * Reads from the cursor until a character in a set are met.
 * As characters are read, a buffer is dynamically grown.
 * The final buffer is NUL-terminated, and must be released
 * from memory when no longer needed.
 *
 * @param cursor: the cursor to use
 * @param characters: the legal characters to stop at
 * @return: a new buffer
*/
char *libmatch_read_alloc_until(struct LibmatchCursor *cursor,
                                const char *characters);

/*
 * Determines whether or not the cursor is followed by another
 * character before a set of other characters. This function
 * does not advance the cursor.
 *
 * @param cursor: the cursor to use
 * @param ch: the character to check
 * @param characters: the characters that must be after
 * @return: 1 if the condition is true, 0 if its not
*/
int libmatch_cond_before(struct LibmatchCursor *cursor, int ch,
                         const char *characters);

#endif
