OBJS=src/common.o src/resolve.o src/header-extraction.o src/main.o src/libpath/join_path.o src/libpath/dirname.o src/libpath/is_directory.o src/libpath/mkdir.o src/libpath/cwd.o src/libpath/rmdir.o src/libpath/exists.o src/libstring/strip.o src/libstring/reverse.o src/libstring/replace.o src/libstring/starts.o src/libstring/count.o src/libmatch/match.o src/libmatch/conditions.o src/libmatch/read.o src/libmatch/cursor.o src/backends/unix.o src/argparse/argparse_internal.o src/argparse/argparse.o src/argparse/extractors.o 
TESTOBJS=src/common.o src/resolve.o src/header-extraction.o src/libpath/join_path.o src/libpath/dirname.o src/libpath/is_directory.o src/libpath/mkdir.o src/libpath/cwd.o src/libpath/rmdir.o src/libpath/exists.o src/libstring/strip.o src/libstring/reverse.o src/libstring/replace.o src/libstring/starts.o src/libstring/count.o src/libmatch/match.o src/libmatch/conditions.o src/libmatch/read.o src/libmatch/cursor.o src/backends/unix.o src/argparse/argparse_internal.o src/argparse/argparse.o src/argparse/extractors.o 
TESTS=
CC=cc
PREFIX=/usr/local
LDFLAGS=
LDLIBS=
CFLAGS=

all: $(OBJS) $(TESTS) makegen

clean:
	rm -rf $(OBJS)
	rm -rf $(TESTS)
	rm -rf vgcore.*
	rm -rf core*
	rm -rf makegen

install:
	mkdir -p $(PREFIX)
	install -m 755 makegen $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/makegen

src/common.o: src/common.c src/makegen.h
	$(CC) -c $(CFLAGS) src/common.c -o src/common.o

src/resolve.o: src/resolve.c src/makegen.h src/backends/backends.h
	$(CC) -c $(CFLAGS) src/resolve.c -o src/resolve.o

src/header-extraction.o: src/header-extraction.c src/makegen.h
	$(CC) -c $(CFLAGS) src/header-extraction.c -o src/header-extraction.o

src/main.o: src/main.c src/makegen.h src/backends/backends.h src/argparse/argparse_internal.h
	$(CC) -c $(CFLAGS) src/main.c -o src/main.o

src/libpath/join_path.o: src/libpath/join_path.c src/libpath/libpath.h
	$(CC) -c $(CFLAGS) src/libpath/join_path.c -o src/libpath/join_path.o

src/libpath/dirname.o: src/libpath/dirname.c src/libpath/libpath.h
	$(CC) -c $(CFLAGS) src/libpath/dirname.c -o src/libpath/dirname.o

src/libpath/is_directory.o: src/libpath/is_directory.c src/libpath/libpath.h
	$(CC) -c $(CFLAGS) src/libpath/is_directory.c -o src/libpath/is_directory.o

src/libpath/mkdir.o: src/libpath/mkdir.c src/libpath/libpath.h
	$(CC) -c $(CFLAGS) src/libpath/mkdir.c -o src/libpath/mkdir.o

src/libpath/cwd.o: src/libpath/cwd.c
	$(CC) -c $(CFLAGS) src/libpath/cwd.c -o src/libpath/cwd.o

src/libpath/rmdir.o: src/libpath/rmdir.c src/libpath/libpath.h
	$(CC) -c $(CFLAGS) src/libpath/rmdir.c -o src/libpath/rmdir.o

src/libpath/exists.o: src/libpath/exists.c src/libpath/libpath.h
	$(CC) -c $(CFLAGS) src/libpath/exists.c -o src/libpath/exists.o

src/libstring/strip.o: src/libstring/strip.c src/libstring/libstring.h
	$(CC) -c $(CFLAGS) src/libstring/strip.c -o src/libstring/strip.o

src/libstring/reverse.o: src/libstring/reverse.c src/libstring/libstring.h
	$(CC) -c $(CFLAGS) src/libstring/reverse.c -o src/libstring/reverse.o

src/libstring/replace.o: src/libstring/replace.c src/libstring/libstring.h
	$(CC) -c $(CFLAGS) src/libstring/replace.c -o src/libstring/replace.o

src/libstring/starts.o: src/libstring/starts.c src/libstring/libstring.h
	$(CC) -c $(CFLAGS) src/libstring/starts.c -o src/libstring/starts.o

src/libstring/count.o: src/libstring/count.c src/libstring/libstring.h
	$(CC) -c $(CFLAGS) src/libstring/count.c -o src/libstring/count.o

src/libmatch/match.o: src/libmatch/match.c src/libmatch/libmatch.h
	$(CC) -c $(CFLAGS) src/libmatch/match.c -o src/libmatch/match.o

src/libmatch/conditions.o: src/libmatch/conditions.c src/libmatch/libmatch.h
	$(CC) -c $(CFLAGS) src/libmatch/conditions.c -o src/libmatch/conditions.o

src/libmatch/read.o: src/libmatch/read.c src/libmatch/libmatch.h
	$(CC) -c $(CFLAGS) src/libmatch/read.c -o src/libmatch/read.o

src/libmatch/cursor.o: src/libmatch/cursor.c src/libmatch/libmatch.h
	$(CC) -c $(CFLAGS) src/libmatch/cursor.c -o src/libmatch/cursor.o

src/backends/unix.o: src/backends/unix.c src/backends/backends.h src/makegen.h
	$(CC) -c $(CFLAGS) src/backends/unix.c -o src/backends/unix.o

src/argparse/argparse_internal.o: src/argparse/argparse_internal.c src/argparse/argparse.h src/argparse/argparse_internal.h
	$(CC) -c $(CFLAGS) src/argparse/argparse_internal.c -o src/argparse/argparse_internal.o

src/argparse/argparse.o: src/argparse/argparse.c src/argparse/argparse.h src/argparse/argparse_internal.h
	$(CC) -c $(CFLAGS) src/argparse/argparse.c -o src/argparse/argparse.o

src/argparse/extractors.o: src/argparse/extractors.c src/argparse/argparse.h src/argparse/argparse_internal.h
	$(CC) -c $(CFLAGS) src/argparse/extractors.c -o src/argparse/extractors.o

makegen: $(OBJS)
	$(CC) $(OBJS) -o makegen $(LDFLAGS) $(LDLIBS)
