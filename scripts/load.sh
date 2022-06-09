#!/bin/sh
# Load documentation and Makefiles. Requires docgen and makegen
# as dependencies. Although, this realistically should not be
# ran by end users, and should instead be ran by developers.

docgen project ./src/main.c --section cware  \
                            --format manpage \
                            --title 'C-Ware Manuals' \
                            --date "`date +'%B %d, %Y'`"

makegen project unix --binary makegen --main src/main.c > Makefile
