# Generated automatically from Makefile.in by configure.
# -----------------------------------------------------------------------------
# Makefile.in for juggler/test/input
# $Id$
# -----------------------------------------------------------------------------

TERMTYPE = -D_OLD_TERMIOS
#TERMTYPE = 

srcdir		= .

COMPILE		= CC $(TERMTYPE) -DHAVE_CONFIG_H ${VJ_INCLUDES}  -ptv  -nostdinc -woff 3322 -w2 -g
VJ_INCLUDES	=  $(TERMTYPE) -I/usr/include/CC -I/usr/include -I${VJ_BASE_DIR}/include -I${srcdir}
VJ_LIBS		= $(TERMTYPE) -B static -L${VJ_BASE_DIR}/lib32 -lJuggler -lJuggler_ogl -B dynamic -lm -lC



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
default: aFlockTest

all: ${PROGS}

# ----------------
# aFlock test driver
# ----------------
aFlockTest: aFlockTest.o aFlock.o
	CC -mips3 -n32 -all -o $@ aFlockTest.o aFlock.o 

aFlockTest.o: aFlockTest.cpp
	CC $(TERMTYPE) -mips3 -n32 -all -I../.. namespaces=ON bool=ON -woff 3303 -c aFlockTest.cpp -o $@  

aFlock.o: ../../Input/vjPosition/aFlock.cpp
	CC $(TERMTYPE) -mips3 -n32 -all -I../.. namespaces=ON bool=ON -woff 3303 -c $@ ../../Input/vjPosition/aFlock.cpp

# ----------------
# vjFlock test driver
# ----------------
vjFlockTest: vjFlockTest.o vjFlock.o aFlock.o
	${COMPILE} $(TERMTYPE) -o $@ aFlock.o vjFlockTest.o vjFlock.o ${VJ_LIBS} ${EXTRA_LIBS}

vjFlockTest.o: vjFlockTest.cpp
	${COMPILE} $(TERMTYPE) -woff 3303 -c ${srcdir}/vjFlockTest.cpp -o $@ 

vjFlock.o: ../../Input/vjPosition/vjFlock.cpp
	${COMPILE} $(TERMTYPE) -woff 3303 -c ../../Input/vjPosition/vjFlock.cpp -o $@ 

aFlock.o: ../../Input/vjPosition/aFlock.cpp
	CC $(TERMTYPE) -mips3 -n32 -all namespaces=ON bool=ON -woff 3303 -c $@ ../../Input/vjPosition/aFlock.cpp


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
