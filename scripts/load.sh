#!/bin/sh
# Load documentation and Makefiles. Requires docgen and makegen
# as dependencies. Although, this realistically should not be
# ran by end users, and should instead be ran by developers.

# This is for the now-deprecated single-executable docgen.
# docgen project ./src/main.c --section cware  \
#                             --format manpage \
#                             --title 'C-Ware Manuals' \
#                             --date "`date +'%B %d, %Y'`"

# This is for the new multi-stage docgen.
cat src/main.c | docgen-extractor-c | docgen-compiler-c | \
	docgen-backend-manpage --section cware --title \
	"C-Ware Manuals" --date "`date +'%B %d, %Y'`" \
	> doc/makegen.cware

makegen project unix --binary makegen --main src/main.c > Makefile
