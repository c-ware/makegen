OBJS=./src/header-extraction.o ./src/common.o ./src/resolve.o ./src/main.o ./src/libmatch/read.o ./src/libmatch/cursor.o ./src/libmatch/match.o ./src/libmatch/conditions.o ./src/libpath/exists.o ./src/libpath/cwd.o ./src/libpath/mkdir.o ./src/libpath/join_path.o ./src/libpath/rmdir.o ./src/libpath/dirname.o ./src/libpath/is_directory.o ./src/libstring/starts.o ./src/libstring/replace.o ./src/libstring/strip.o ./src/libstring/reverse.o ./src/libstring/count.o ./src/backends/unix.o ./src/argparse/extractors.o ./src/argparse/argparse_internal.o ./src/argparse/argparse.o 
TESTOBJS=./src/header-extraction.o ./src/common.o ./src/resolve.o ./src/libmatch/read.o ./src/libmatch/cursor.o ./src/libmatch/match.o ./src/libmatch/conditions.o ./src/libpath/exists.o ./src/libpath/cwd.o ./src/libpath/mkdir.o ./src/libpath/join_path.o ./src/libpath/rmdir.o ./src/libpath/dirname.o ./src/libpath/is_directory.o ./src/libstring/starts.o ./src/libstring/replace.o ./src/libstring/strip.o ./src/libstring/reverse.o ./src/libstring/count.o ./src/backends/unix.o ./src/argparse/extractors.o ./src/argparse/argparse_internal.o ./src/argparse/argparse.o 
TESTS=
CC=cc
CFLAGS=
PREFIX=/usr/local

all: $(OBJS) $(TESTS) makegen

clean:
	rm -rf $(OBJS)
	rm -rf $(TESTS)
	rm -rf vgcore.*
	rm -rf core*
	rm -rf makegen

install:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/bin
	install makegen $(PREFIX)/bin -m 755

./src/header-extraction.o: ./src/header-extraction.c ./src/makegen.h
	$(CC) -c ./src/header-extraction.c -o ./src/header-extraction.o $(CFLAGS)

./src/common.o: ./src/common.c ./src/makegen.h
	$(CC) -c ./src/common.c -o ./src/common.o $(CFLAGS)

./src/resolve.o: ./src/resolve.c ./src/makegen.h ./src/backends/backends.h
	$(CC) -c ./src/resolve.c -o ./src/resolve.o $(CFLAGS)

./src/main.o: ./src/main.c ./src/makegen.h ./src/backends/backends.h ./src/argparse/argparse_internal.h
	$(CC) -c ./src/main.c -o ./src/main.o $(CFLAGS)

./src/libmatch/read.o: ./src/libmatch/read.c ./src/libmatch/libmatch.h
	$(CC) -c ./src/libmatch/read.c -o ./src/libmatch/read.o $(CFLAGS)

./src/libmatch/cursor.o: ./src/libmatch/cursor.c ./src/libmatch/libmatch.h
	$(CC) -c ./src/libmatch/cursor.c -o ./src/libmatch/cursor.o $(CFLAGS)

./src/libmatch/match.o: ./src/libmatch/match.c ./src/libmatch/libmatch.h
	$(CC) -c ./src/libmatch/match.c -o ./src/libmatch/match.o $(CFLAGS)

./src/libmatch/conditions.o: ./src/libmatch/conditions.c ./src/libmatch/libmatch.h
	$(CC) -c ./src/libmatch/conditions.c -o ./src/libmatch/conditions.o $(CFLAGS)

./src/libpath/exists.o: ./src/libpath/exists.c ./src/libpath/libpath.h
	$(CC) -c ./src/libpath/exists.c -o ./src/libpath/exists.o $(CFLAGS)

./src/libpath/cwd.o: ./src/libpath/cwd.c
	$(CC) -c ./src/libpath/cwd.c -o ./src/libpath/cwd.o $(CFLAGS)

./src/libpath/mkdir.o: ./src/libpath/mkdir.c ./src/libpath/libpath.h
	$(CC) -c ./src/libpath/mkdir.c -o ./src/libpath/mkdir.o $(CFLAGS)

./src/libpath/join_path.o: ./src/libpath/join_path.c ./src/libpath/libpath.h
	$(CC) -c ./src/libpath/join_path.c -o ./src/libpath/join_path.o $(CFLAGS)

./src/libpath/rmdir.o: ./src/libpath/rmdir.c ./src/libpath/libpath.h
	$(CC) -c ./src/libpath/rmdir.c -o ./src/libpath/rmdir.o $(CFLAGS)

./src/libpath/dirname.o: ./src/libpath/dirname.c ./src/libpath/libpath.h
	$(CC) -c ./src/libpath/dirname.c -o ./src/libpath/dirname.o $(CFLAGS)

./src/libpath/is_directory.o: ./src/libpath/is_directory.c ./src/libpath/libpath.h
	$(CC) -c ./src/libpath/is_directory.c -o ./src/libpath/is_directory.o $(CFLAGS)

./src/libstring/starts.o: ./src/libstring/starts.c ./src/libstring/libstring.h
	$(CC) -c ./src/libstring/starts.c -o ./src/libstring/starts.o $(CFLAGS)

./src/libstring/replace.o: ./src/libstring/replace.c ./src/libstring/libstring.h
	$(CC) -c ./src/libstring/replace.c -o ./src/libstring/replace.o $(CFLAGS)

./src/libstring/strip.o: ./src/libstring/strip.c ./src/libstring/libstring.h
	$(CC) -c ./src/libstring/strip.c -o ./src/libstring/strip.o $(CFLAGS)

./src/libstring/reverse.o: ./src/libstring/reverse.c ./src/libstring/libstring.h
	$(CC) -c ./src/libstring/reverse.c -o ./src/libstring/reverse.o $(CFLAGS)

./src/libstring/count.o: ./src/libstring/count.c ./src/libstring/libstring.h
	$(CC) -c ./src/libstring/count.c -o ./src/libstring/count.o $(CFLAGS)

./src/backends/unix.o: ./src/backends/unix.c ./src/backends/backends.h ./src/makegen.h
	$(CC) -c ./src/backends/unix.c -o ./src/backends/unix.o $(CFLAGS)

./src/argparse/extractors.o: ./src/argparse/extractors.c ./src/argparse/argparse.h ./src/argparse/argparse_internal.h
	$(CC) -c ./src/argparse/extractors.c -o ./src/argparse/extractors.o $(CFLAGS)

./src/argparse/argparse_internal.o: ./src/argparse/argparse_internal.c ./src/argparse/argparse.h ./src/argparse/argparse_internal.h
	$(CC) -c ./src/argparse/argparse_internal.c -o ./src/argparse/argparse_internal.o $(CFLAGS)

./src/argparse/argparse.o: ./src/argparse/argparse.c ./src/argparse/argparse.h ./src/argparse/argparse_internal.h
	$(CC) -c ./src/argparse/argparse.c -o ./src/argparse/argparse.o $(CFLAGS)

makegen: $(OBJS)
	$(CC) $(OBJS) -o makegen
