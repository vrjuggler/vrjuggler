# Generated automatically from Makefile.in by configure.
# -----------------------------------------------------------------------------
# Makefile.in for juggler/test/ogl/combo
# $Id$
# -----------------------------------------------------------------------------
# IMPORTANT NOTE FOR IRIX USERS:
#     This application will not compile using the standard IRIX 6.x make(1).
#     Use either pmake(1) or gmake(1).
# -----------------------------------------------------------------------------

.SUFFIXES: .cpp

srcdir		= .

COMPILE		= CC -DHAVE_CONFIG_H ${VJ_INCLUDES}  -ptv  -woff 3322 -w2 -g
LINK		= CC  -ptv  -woff 3322 -w2 -g

GLUT_LIB	= -B static -L/home/users/allenb/Libs/glut/lib32 -lglut
GLUT_INC	= -I/home/users/allenb/Libs/glut/include

GLUI_LIB	= -B static -L/home/users/allenb/Libs/glui_v2/lib -lglui
GLUI_INC 	= -I/home/users/allenb/Libs/glui_v2

COMMON_INC      = -I${srcdir}/common
VJ_INCLUDES	= -I${VJ_BASE_DIR}/include -I${srcdir} ${COMMON_INC} ${GLUT_INC} ${GLUI_INC}
VJ_LIBS		= -B dynamic -L${VJ_BASE_DIR}/lib32 -lJuggler -lJuggler_ogl -B dynamic -lm -lC
VPATH		= .:./common

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
EXTRA_LIBS	=   -B dynamic -L/usr/lib32 -L/usr -lGLU -lGL -lXmu -lXext -lX11 -lm


TRACKER_CONFIG_APP_OBJECTS    = trackerConfig.o sgGlutApp.o sgGlutWin.o sgInput.o
# -----------------------------------------------------------------------------
# Targets.
# -----------------------------------------------------------------------------
default: all

all: trackerConfig

trackerConfig: ${TRACKER_CONFIG_APP_OBJECTS}
	${LINK} -o $@ ${TRACKER_CONFIG_APP_OBJECTS} ${VJ_LIBS} ${GLUI_LIB} ${GLUT_LIB} ${EXTRA_LIBS}


.cpp.o:
	${COMPILE} -c $<

# -----------------------------------------------------------------------------
# Clean-up.
# -----------------------------------------------------------------------------
clean:
	rm -f Makedepend testApp ${TEST_APP_OBJECTS} *core so_locations
	rm -rf ./ii_files
