# -----------------------------------------------------------------------------
# $Id$
#
# This include file <vj.lib.mk> handles compiling of libraries (both static
# and shared) from compiled object files.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variables:
#
# AR      - Path to ar(1) (or equivalent program).
# LD      - Path to ld(1) (or equivalent program).
# LDOPTS  - Extra, often platform-specific, options for the linker.
# LIBDIR  - The directory where the library will be made.
# LIBNAME - The name of the library to generate.
# OBJS    - The object files to be compiled into the named library.
# -----------------------------------------------------------------------------

static-lib:
	${AR} ${LIBDIR}/${LIBNAME} ${OBJS}

shared-lib:
	${LD} ${LDOPTS} -o ${LIBDIR}/${LIBNAME} ${OBJS}
