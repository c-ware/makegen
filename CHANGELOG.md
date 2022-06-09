# CHANGELOG.md

DATE: June 0th, 2022
VERSION: 1.0.1

## Features
- Added header installation for libraries
- Added uninstall target to library and project targets
- Tests now have their inclusions as dependencies
- Added CFLAGS, LDLIBS, and LDFLAGS options

## Documentation
- Added makegen manual
- Added list of required options, and optional options (wow that's a
  a mouthful) that each target takes
- Added a full list of options and what each does

## Bug fixes
- Headers are no longer executable (woop-de-doo)
- Fixed memory leak from unreleased array
- Fixed segfault from missing NULL at the end of a variadic argument
- Adding LDFLAGS and LDLIBS to final binary for project Makefiles

## Misc
- Added functions that verify the existence of all options that
  a project, and library Makefile will need, respectively.
