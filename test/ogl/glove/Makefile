# Generated automatically from Makefile.in by configure.
# -----------------------------------------------------------------------------
# Makefile.in for juggler/test/ogl/glove
# $Id$
# -----------------------------------------------------------------------------

srcdir		= .

COMPILE		= CC -DHAVE_CONFIG_H ${VJ_INCLUDES}  -ptv  -nostdinc -woff 3322 -w2 -g
VJ_INCLUDES	=  -I/usr/include/CC -I/usr/include -I${VJ_BASE_DIR}/include -I${srcdir}
VJ_LIBS		= -B dynamic -L${VJ_BASE_DIR}/lib32 -lJuggler -lJuggler_ogl -B dynamic -lm -lC
VPATH		= .

# This test program requires the following libraries to be linked:
#
#     -lGLU -lGL -lXext -lXmu -lX11 -lm -lC
#
# If the library uses POSIX threads, "-lpthread" must also be added to the
# list of libraries.
#
# The proper library path must be specified depending on the Application
# Binary Interface (ABI) used.  For o32 binaries, -L/usr/lib is the
# appropriate path.  For n32 binaries, use -L/usr/lib32.  For 64-bit binaries,
# use -L/usr/lib64.
EXTRA_LIBS	=   -L/usr/lib32 -limage -L/usr -lGLU -lGL -lXext -lXmu -lX11

OBJECTS		= main.o

# -----------------------------------------------------------------------------
# Targets.
# -----------------------------------------------------------------------------
default: all

all: glove

glove: ${OBJECTS}
	${COMPILE} -o $@ ${OBJECTS} ${VJ_LIBS} ${EXTRA_LIBS}

main.o: main.cpp application.h
	${COMPILE} -c -o $@ ${srcdir}/main.cpp

# -----------------------------------------------------------------------------
# Clean-up.
# -----------------------------------------------------------------------------
clean:
	rm -f Makedepend glove ${OBJECTS} core so_locations
	rm -rf ./ii_files
