#ifndef CWARE_LIBSTRING_H
#define CWARE_LIBSTRING_H

/*
 * Reverses a string in place.
 *
 * @param string: the string to reverse
 * @return: the reversed string
*/
char *strrev(char *string);

/*
 * Removes characters from a string in place.
 *
 * @param string: the string to remove from
 * @param characters: the characters to strip
 * @return: the number of characters stripped
*/
int strstrip(char *string, const char *characters);

/*
 * Counts the number of times a sub string appears in a
 * string.
 *
 * @param string: the string to count in
 * @param count: the sub string to count
 * @return: the number of occurrences
*/
int strcount(const char *string, const char *count);

/*
 * Replaces a string in a string with another
 * string in place.
 *
 * @param string: the string to replace
 * @param length: the maximum length of the buffer
 * @param find: the string to replace
 * @param replace: the string to replace with
 * @return: number of replacements
*/
int strrepl(char *string, const char *find, const char *replace, size_t size);

/*
 * Checks if a string starts with a sub string.
 *
 * @param string: the string to check
 * @param start: the sub string to compare
 * @return: 1 if string starts with the sub string, 0 if it does not
*/
int strstarts(const char *string, const char *start);

/*
 * C89 compliant semi-implementation of the snprintf function from C99.
 *
 * @param buffer: the buffer to write to
 * @param length: the maximum length of the buffer
 * @param format: the format string
*/
size_t ls_snprintf(char *buffer, size_t length, const char *format, ...);

#endif
