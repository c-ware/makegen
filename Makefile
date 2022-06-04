CC=cc
CFLAGS=
OBJS=src/main.o src/argparse/argparse.o src/argparse/argparse_internal.o 	   \
	 src/argparse/extractors.o src/libmatch/conditions.o src/libmatch/cursor.o \
	 src/libmatch/match.o src/libmatch/read.o src/header-extraction.o 		   \
	 src/libpath/cwd.o src/libpath/exists.o src/libpath/join_path.o			   \
	 src/libpath/mkdir.o src/libpath/rmdir.o src/libpath/is_directory.o		   \
	 src/libpath/dirname.o src/backends/unix.o src/resolve.o		   		   \
	 src/libstring/count.o src/libstring/replace.o src/libstring/reverse.o	   \
	 src/libstring/starts.o src/libstring/strip.o src/common.o

all: $(OBJS) makegen

clean:
	rm -f $(OBJS)
	rm -f makegen

makegen: $(OBJS)
	$(CC) $(OBJS) -o makegen

.c.o:
	$(CC) -c $< -o $@ $(CFLAGS)
