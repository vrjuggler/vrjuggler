# Generated automatically from Makefile.in by configure.
# -----------------------------------------------------------------------------
# Makefile.in for juggler/test/input
# $Id$
# -----------------------------------------------------------------------------

srcdir		= .

COMPILE		= CC -DHAVE_CONFIG_H ${VJ_INCLUDES}  -ptv  -nostdinc -woff 3322 -w2 -g
VJ_INCLUDES	=  -I/usr/include/CC -I/usr/include -I${VJ_BASE_DIR}/include -I${srcdir}
VJ_LIBS		= -B static -L${VJ_BASE_DIR}/lib32 -lJuggler -lJuggler_ogl -B dynamic -lm -lC

# These test programs require the following libraries to be linked:
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

OBJECTS		= main.o main-ibox.o main-inputgroup.o main-logiclass.o
PROGS		= go go-ibox go-inputgroup go-logiclass

# -----------------------------------------------------------------------------
# Targets.
# -----------------------------------------------------------------------------
default: all

all: ${PROGS}

# ----------------
# aFlock test driver
# ----------------
aFlockTest: aFlockTest.o aFlock.o
	${COMPILE} -o $@ aFlockTest.o aFlock.o 

aFlockTest.o: aFlockTest.cpp
	${COMPILE} -woff 3303 -c -o $@ ${srcdir}/aFlockTest.cpp

aFlock.o: ../../Input/vjPosition/aFlock.cpp
	${COMPILE} -woff 3303 -c -o $@ ../../Input/vjPosition/aFlock.cpp

# ----------------
# vjFlock test driver
# ----------------
vjFlockTest: vjFlockTest.o
	${COMPILE} -o $@ vjFlockTest.o ${VJ_LIBS} ${EXTRA_LIBS}

vjFlockTest.o: vjFlockTest.cpp
	${COMPILE} -woff 3303 -c -o $@ ${srcdir}/vjFlockTest.cpp

# ----------------
# new
# ----------------
new: new.o
	${COMPILE} -o $@ new.o ${VJ_LIBS} ${EXTRA_LIBS}

new.o: new.cpp
	${COMPILE} -c -o $@ ${srcdir}/new.cpp

# ----------------
# go
# ----------------
go: main.o
	${COMPILE} -o $@ main.o ${VJ_LIBS} ${EXTRA_LIBS}

main.o: main.cpp
	${COMPILE} -c -o $@ ${srcdir}/main.cpp

# ----------------
# go-ibox
# ----------------
go-ibox: main-ibox.o
	${COMPILE} -o $@ main-ibox.o ${VJ_LIBS} ${EXTRA_LIBS}

main-ibox.o: main-ibox.cpp
	${COMPILE} -c -o $@ ${srcdir}/main-ibox.cpp

# ----------------
# go-inputgroup
# ----------------
go-inputgroup: main-inputgroup.o
	${COMPILE} -o $@ main-inputgroup.o ${VJ_LIBS} ${EXTRA_LIBS}

main-inputgroup.o: main-inputgroup.cpp
	${COMPILE} -c -o $@ ${srcdir}/main-inputgroup.cpp

# ----------------
# go-logiclass
# ----------------
go-logiclass: main-logiclass.o
	${COMPILE} -o $@ main-logiclass.o ${VJ_LIBS} ${EXTRA_LIBS}

main-logiclass.o: main-logiclass.cpp
	${COMPILE} -c -o $@ ${srcdir}/main-logiclass.cpp

# -----------------------------------------------------------------------------
# Clean-up.
# -----------------------------------------------------------------------------
clean:
	rm -f Makedepend ${PROGS} ${OBJECTS} core so_locations
	rm -rf ./ii_files
