# -----------------------------------------------------------------------------
# Makefile.in for juggler/samples/ogl/wand
# $Id$
#
# Generated for use on IRIX
# -----------------------------------------------------------------------------

.SUFFIXES: .cpp .o

srcdir		= .

EXTRA_FLAGS	= -n32 -mips3
COMPILE		= CC -DHAVE_CONFIG_H $(VJ_INCLUDES) -LANG:std  -woff 1183,3322 -w2 -g -gslim $(EXTRA_FLAGS)
LINK		= CC -LANG:std -woff 1183,3322 -w2  $(EXTRA_FLAGS)
VJ_INCLUDES	=  -I$(VJ_BASE_DIR)/include -I$(srcdir)
VJ_LIBS		= -B dynamic -L$(VJ_BASE_DIR)/lib32 -lJuggler -lJuggler_ogl 
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
EXTRA_LIBS	= -B dynamic   -lGLU -lGL -lX11 -lXext -lm -lC

OBJECTS		= main.o contextApp.o

# -----------------------------------------------------------------------------
# Targets.
# -----------------------------------------------------------------------------
default: all

all: contextApp

contextApp: $(OBJECTS)
	$(LINK) -o $@ $(OBJECTS) $(VJ_LIBS) $(EXTRA_LIBS)

main.o contextApp.o: contextApp.h

.cpp.o:
	$(COMPILE) -o $@ -c $<

# -----------------------------------------------------------------------------
# Clean-up.
# -----------------------------------------------------------------------------
clean:
	rm -f Makedepend contextApp contextApp.exe contextApp.ilk $(OBJECTS) *core	\
          so_locations *.?db
	rm -rf ./ii_files
