/*
 * An implementation of a dynamic array.
*/

#ifndef CWARE_LIBCARRAY_H
#define CWARE_LIBCARRAY_H

#define CARRAY_VERSION "2.0.0"

#ifndef CARRAY_INITIAL_SIZE
#define CARRAY_INITIAL_SIZE 5
#elif CARRAY_INITIAL_SIZE <= 0 
        "carray_init: default length must be greater than 0"
#endif

#ifndef CARRAY_RESIZE
#define CARRAY_RESIZE(size) \
    ((size) + 5)
#endif

#ifndef CARRAY_COUNTER_TYPE 
#define CARRAY_COUNTER_TYPE int
#endif

/* Error handlers */
#define __carray_assert_natural(macro_name, argument, value)          \
do {                                                                  \
    if((value) <= 0) {                                                \
        fprintf(stderr, "%s: %s cannot be less than or equal to zero" \
                        "(%s:%i)\n", macro_name, argument, __FILE__,  \
                        __LINE__);                                    \
        abort();                                                      \
    }                                                                 \
} while(0)

#define __carray_assert_nonnull(macro_name, argument, value)           \
do {                                                                   \
    if((value) == NULL) {                                              \
        fprintf(stderr, "%s: %s cannot be NULL (%s:%i)\n", macro_name, \
                        argument, __FILE__, __LINE__);                 \
        abort();                                                       \
    }                                                                  \
} while(0)

/* Operations */

#define carray_init(array, namespace)                          \
    (array);                                                   \
                                                               \
    (array) = malloc(sizeof(*(array)));                        \
    (array)->length = 0;                                       \
    (array)->capacity = CARRAY_INITIAL_SIZE;                   \
    (array)->contents = malloc(CARRAY_INITIAL_SIZE             \
                                * sizeof(namespace ## _TYPE)); \
    memset((array)->contents, 0, CARRAY_INITIAL_SIZE *         \
                                 sizeof(namespace ## _TYPE))

#define carray_init_stack(array, buffer, _capacity, namespace)            \
    (*(array));                                                           \
                                                                          \
    __carray_assert_nonnull("carray_init_stack", "array", array);         \
    __carray_assert_nonnull("carray_init_stack", "buffer", buffer);       \
    __carray_assert_natural("carray_init_stack", "_capacity", _capacity); \
                                                                          \
    (array)->length = 0;                                                  \
    (array)->contents = buffer;                                           \
    (array)->capacity = _capacity

#define carray_insert(array, index, value, namespace)                         \
    __carray_assert_nonnull("carray_insert", "array", array);                 \
    __carray_assert_nonnull("carray_insert", "array->contents",               \
                                             (array)->contents);              \
                                                                              \
    if((array)->length == (array)->capacity) {                                \
        if(namespace ## _HEAP == 1) {                                         \
            (array)->capacity = (CARRAY_COUNTER_TYPE)                         \
                                CARRAY_RESIZE((array)->capacity);             \
            (array)->contents = realloc((array)->contents,                    \
                                        sizeof(*(array)->contents)            \
                                        * (size_t) (array)->capacity);        \
        } else {                                                              \
            fprintf(stderr, "carray_insert: attempt to insert value '%s' "    \
                            "into full array (%s:%i)\n", #value, __FILE__,    \
                                                                 __LINE__);   \
            abort();                                                          \
        }                                                                     \
    }                                                                         \
                                                                              \
    if(index < 0 || index > (array)->length) {                                \
        fprintf(stderr, "carray_insert: attempt to insert at index %i, out "  \
                        "of bounds of array (%s:%i)\n", index, __FILE__,      \
                                                               __LINE__);     \
        abort();                                                              \
    }                                                                         \
                                                                              \
    memmove((array)->contents + index + 1,                                    \
            (array)->contents + index,                                        \
            sizeof(*(array)->contents) * (size_t) ((array)->length - index)); \
    (array)->contents[index] = value;                                         \
    (array)->length++

#define carray_pop(array, index, location)                                    \
    location;                                                                 \
                                                                              \
    __carray_assert_nonnull("carray_pop", "array", array);                    \
    __carray_assert_nonnull("carray_pop", "array->contents",                  \
                                            (array)->contents);               \
                                                                              \
    if(index < 0 || index >= (array)->length) {                               \
        fprintf(stderr, "carray_pop: attempt to pop index %i, out of bounds " \
                        "of array (%s:%i)\n", index, __FILE__, __LINE__);     \
        abort();                                                              \
    }                                                                         \
                                                                              \
    (array)->length--;                                                        \
    (location) = (array)->contents[(index)];                                  \
    memmove((array)->contents + index, (array)->contents + index + 1,         \
            sizeof(*((array)->contents))                                      \
            * (size_t) ((array)->length - index))

#define carray_remove(array, value, namespace)                                \
do {                                                                          \
    CARRAY_COUNTER_TYPE __CARRAY_ITER_INDEX = 0;                              \
                                                                              \
    __carray_assert_nonnull("carray_remove", "array", array);                 \
    __carray_assert_nonnull("carray_remove", "array->contents",               \
                                            (array)->contents);               \
                                                                              \
    for(__CARRAY_ITER_INDEX = 0; __CARRAY_ITER_INDEX < (array)->length;       \
                                                     __CARRAY_ITER_INDEX++) { \
                                                                              \
        if((namespace ## _COMPARE((array)->contents[__CARRAY_ITER_INDEX],     \
                                  value)) == 0)                               \
            continue;                                                         \
                                                                              \
        namespace ## _FREE((array)->contents[__CARRAY_ITER_INDEX]);           \
        (array)->length--;                                                    \
        memmove((array)->contents + __CARRAY_ITER_INDEX,                      \
                (array)->contents + __CARRAY_ITER_INDEX + 1,                  \
                sizeof(*(array)->contents) * (size_t) ((array)->length -      \
                                                       __CARRAY_ITER_INDEX)); \
                                                                              \
        __CARRAY_ITER_INDEX = -1;                                             \
        break;                                                                \
    }                                                                         \
                                                                              \
    if(__CARRAY_ITER_INDEX != -1) {                                           \
        fprintf(stderr, "carray_remove: attempt to remove value '%s' that is" \
                        " not in array. (%s:%i)\n", #value, __FILE__,         \
                        __LINE__);                                            \
        abort();                                                              \
    }                                                                         \
} while(0)

#define carray_find(array, value, location, namespace)                        \
    -1;                                                                       \
                                                                              \
    __carray_assert_nonnull("carray_find", "array", array);                   \
    __carray_assert_nonnull("carray_find", "array->contents",                 \
                                            (array)->contents);               \
                                                                              \
    do {                                                                      \
        CARRAY_COUNTER_TYPE __CARRAY_ITER_INDEX = 0;                          \
        location = -1;                                                        \
                                                                              \
        for(__CARRAY_ITER_INDEX = 0; __CARRAY_ITER_INDEX < (array)->length;   \
                                                     __CARRAY_ITER_INDEX++) { \
                                                                              \
            if((namespace ## _COMPARE((array)->contents[__CARRAY_ITER_INDEX], \
                                      value)) == 0)                           \
                continue;                                                     \
                                                                              \
            location = __CARRAY_ITER_INDEX;                                   \
                                                                              \
            break;                                                            \
        }                                                                     \
    } while(0)

#define carray_free(array, namespace)                                         \
do {                                                                          \
    CARRAY_COUNTER_TYPE __CARRAY_ITER_INDEX = 0;                              \
                                                                              \
    __carray_assert_nonnull("carray_free", "array", array);                   \
    __carray_assert_nonnull("carray_free", "array->contents",                 \
                                            (array)->contents);               \
                                                                              \
    for(__CARRAY_ITER_INDEX = 0; __CARRAY_ITER_INDEX < (array)->length;       \
                                                     __CARRAY_ITER_INDEX++) { \
        namespace ## _FREE((array)->contents[__CARRAY_ITER_INDEX]);           \
    }                                                                         \
                                                                              \
    if(namespace ## _HEAP == 1) {                                             \
        free((array)->contents);                                              \
        free((array));                                                        \
    }                                                                         \
} while(0)

#define carray_append(array, value, namespace)                                \
    __carray_assert_nonnull("carray_append", "array", array);                 \
    __carray_assert_nonnull("carray_append", "array->contents",               \
                                            (array)->contents);               \
    if((array)->length == (array)->capacity) {                                \
        if(namespace ## _HEAP == 1) {                                         \
            (array)->capacity = CARRAY_RESIZE((array)->capacity);             \
            (array)->contents = realloc((array)->contents,                    \
                                        sizeof(*(array)->contents)            \
                                        * (size_t) (array)->capacity);        \
        } else {                                                              \
            fprintf(stderr, "carray_append: array is full. maximum capacity " \
                            "of %i (%s:%i)\n", (array)->capacity,             \
                            __FILE__, __LINE__);                              \
            abort();                                                          \
        }                                                                     \
    }                                                                         \
                                                                              \
    (array)->contents[(array)->length] = value;                               \
    (array)->length++

#define carray_length(array) \
    ((array)->length)

#define carray_is_empty(array) \
    ((array)->length == 0)

#endif
