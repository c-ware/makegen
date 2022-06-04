#ifndef CWARE_LIBERROR_H
#define CWARE_LIBERROR_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CWARE_LIBERROR_VERSION  "1.0.0"

#define liberror_is_null(function_name, argument)                 \
do {                                                              \
    if((argument) != NULL)                                        \
        break;                                                    \
                                                                  \
    fprintf(stderr, "%s", #function_name ": argument '" #argument \
            "' cannot be NULL\n");                                \
    abort();                                                      \
} while(0)

#define liberror_in_range(function_name, argument, start, end)        \
do {                                                                  \
    if((argument) >= (start) && (argument) <= end)                    \
        break;                                                        \
                                                                      \
    fprintf(stderr, "%s: argument '%s' (%i) out of range (%i, %i)\n", \
            #function_name, #argument, (start), (end), argument);     \
    abort();                                                          \
} while(0)

#define liberror_buffer_is_full(function_name, argument, needed_size, length) \
do {                                                                          \
    if((needed_size) <= (length))                                             \
        break;                                                                \
                                                                              \
    fprintf(stderr, "%s: argument '%s' (%s) produces truncated buffer at "    \
            "runtime. maximum length is %i\n", #function_name, #argument,     \
            argument, length);                                                \
    abort();                                                                  \
} while(0)

#define liberror_is_number(function_name, argument, format, value)    \
do {                                                                  \
    if((argument) != (value))                                         \
        break;                                                        \
                                                                      \
    fprintf(stderr, "%s: argument '%s' cannot be value " format "\n", \
            #function_name, #argument, value);                        \
    abort();                                                          \
} while(0);

#define liberror_failure(function_name, function)                           \
do {                                                                        \
    if((errno) == 0)                                                        \
        break;                                                              \
                                                                            \
    fprintf(stderr, "%s: function '%s' failed with error code %i (%s) "     \
            "(%s:%i)\n", #function_name, #function, errno, strerror(errno), \
            __FILE__, __LINE__);                                            \
    abort();                                                                \
} while(0)

#define liberror_is_negative(function_name, argument)         \
do {                                                          \
    if((argument) >= 0)                                       \
        break;                                                \
                                                              \
    fprintf(stderr, "%s: argument '%s' cannot be negative\n", \
            #function_name, #argument);                       \
    abort();                                                  \
} while(0)

#define liberror_is_positive(function_name, argument)         \
do {                                                          \
    if((argument) <= 0)                                       \
        break;                                                \
                                                              \
    fprintf(stderr, "%s: argument '%s' cannot be positive\n", \
            #function_name, #argument);                       \
    abort();                                                  \
} while(0)

#define liberror_unhandled(function_name) \
    fprintf(stderr, "%s: unexpected error condition (errno %i: %s) "       \
            "(%s:%i)\n", #function_name, errno, strerror(errno), __FILE__, \
            __LINE__)

#endif
