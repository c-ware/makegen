#ifndef CWARE_LIBCWPATH_H
#define CWARE_LIBCWPATH_H

#define CWARE_LIBCWPATH_VERSION  "1.0.0"

/* Useful macros */
#define INIT_VARIABLE(variable) \
    memset(&(variable), 0, sizeof((variable)))

/* Error checking macros */
#define __libpath_assert_nonnull(name, argument)                             \
do {                                                                         \
    if((argument) == NULL) {                                                 \
        fprintf(stderr, name ": argument '" #argument "' cannot be NULL\n"); \
        exit(EXIT_FAILURE);                                                  \
    }                                                                        \
} while(0)

#define __libpath_assert_nonnegative(name, argument)                 \
do {                                                                 \
    if((argument) < 0) {                                             \
        fprintf(stderr, name ": argument '" #argument "' cannot be " \
                        "negative\n");                               \
        exit(EXIT_FAILURE);                                          \
    }                                                                \
} while(0)

/* 
 * The maximum path lengths of various operating systems
 * These values will contain at *least* the correct number of bytes
 * to contain a path. They may be rounded up to a power of two. The
 * actual path length should be LIBPATH_MAX_PATH + 1, where the + 1
 * is for the NUL byte.
*/
#ifdef _MSDOS
    #define LIBPATH_MAX_PATH    65
#endif

#ifdef _WIN32
    #define LIBPATH_MAX_PATH    260
#endif

#ifdef __linux__
    #define LIBPATH_MAX_PATH    4096
#endif

#if defined(ultrix) || defined(__FreeBSD__) || defined(__OpenBSD__) ||  defined(__sun)
    #define LIBPATH_MAX_PATH    1024
#endif

#ifdef _MSDOS
    #define LIBPATH_MAX_PATH    65
#endif

#ifdef __WATCOMC__
    #define LIBPATH_MAX_PATH    65
#endif

/*
 * Joins a variable number of path segments into a single buffer,
 * while respecting the maximum length of the buffer. Length should
 * be the actual length of the buffer - 1, for the NUL byte. Last
 * argument should be NULL.
 *
 * @param buffer: the buffer to write to
 * @param length: the length of the buffer
 * @return: number of bytes written
*/
size_t libpath_join_path(char *buffer, int length, ...);

/*
 * Creates a new directory at a a certain path with a given
 * permissions set. Second parameter is ignored on MS-DOS.
 *
 * @param path: the path to make the directory at
 * @param mode: the mode to make the directory with
 * @return: 0 if it was successful, and -1 if it was unsuccessful
*/
int libpath_mkdir(const char *path, int mode);

/*
 * Removes an empty directory at a certain path.
 *
 * @param path: the path to remove
 * @return: 0 if it was successful, and -1 if it was unsuccessful
*/
int libpath_rmdir(const char *path);

/*
 * Determines whether or not the path provided exists or not.
 *
 * @param path: the path to check
 * @return: 1 if the path exists, 0 if it does not exist
*/
int libpath_exists(const char *path);

/*
 * Retrieve the current working directory and write it to a buffer.
 *
 * @param buffer: the buffer to write to
 * @param length: the length of the buffer
 * @return: length of the buffer
*/
int libpath_cwd(char *buffer, int length);

/*
 * Determine if a path points to a directory.
 *
 * @param path: the path to check
 * @return: 1 if path points to a directory, 0 if it does not
*/
int libpath_is_directory(const char *path);

/*
 * Strip the last component of the path pointed to by PATH.
 * This is only indented to work with absolute paths, so do
 * not try to run this code on relative paths which contain
 * the .. or . special directories in them.
 *
 * @param path: the path to strip
*/
void libpath_dirname(char *path);

#endif
