OBJECT_STYLE = N32
LIBBITSUF = 32

include $(ROOT)/usr/include/make/commondefs

include ../../../lib/Makefile.common

#-- look for sources in this directory
.PATH.h: ${C2_PATHS}
.INCLUDES: .h

OPTIMIZER = -g
LCXXDEFS =
LCXXINCS = $(.INCLUDES) -I.
LCXXOPTS = -woff 445 -DC2_SGI_IPC $(PF_LIBS)


PFROOT ?= $(ROOT)
PF_INC_DIR = $(PFROOT)/usr/include

DSOLINKS = \
        -L$(PFROOT)/usr/lib$(LIBBITSUF) \
        -L$(PFROOT)/usr/lib$(LIBBITSUF)/libpfdb \
        -L$(PFROOT)/lib$(LIBBITSUF)

DDSOLINKS = \
	-L$(PFROOT)/usr/lib$(LIBBITSUF)/Performer/Debug \
	-L$(PFROOT)/usr/lib$(LIBBITSUF)/Performer/Debug/libpfdb \
	-L$(PFROOT)/lib$(LIBBITSUF)

DBGLINKS = \
	-L$(PFROOT)/usr/lib$(LIBBITSUF)/Performer/DebugStatic \
	-L$(PFROOT)/usr/lib$(LIBBITSUF)/Performer/DebugStatic/libpfdb \
	-L$(PFROOT)/lib$(LIBBITSUF)
	

OPTLINKS = \
	-L$(PFROOT)/usr/lib$(LIBBITSUF)/Performer/Static \
	-L$(PFROOT)/usr/lib$(LIBBITSUF)/Performer/Static/libpfdb \
	-L$(PFROOT)/lib$(LIBBITSUF)
	

OGLLIB = -ignore_unresolved -lpf_ogl -lpfdu_ogl -lpfui -lpfutil_ogl

#if defined(PFSTATIC_CONVERTERS)
OGLLIB += -all $(PFSTATIC_CONVERTERS) -none
#endif

LIBOPENGL  = -lGLU -lGL -lXext

SYSTEM_OPENGL = \
        -lmpc \
	-limage \
	-lGLw \
	${LIBOPENGL} \
	-lfpe \
	-lXm \
	-lXt \
	-lXmu \
	-lX11 \
	-lm \
	-lmalloc \
	-lC

#if $(OBJECT_STYLE) == "64"
SYSTEM_OPENGL = \
	-limage \
	${LIBOPENGL} \
	-lXmu \
	-lX11 \
	-lm \
	-lC
#endif

#if $(OBJECT_STYLE) == "N32"
SYSTEM_OPENGL = \
	-limage \
	${LIBOPENGL} \
	-lXmu \
	-lX11 \
	-lm \
	-lC
#endif

PF_LIBS = $(DDSOLINKS) $(OGLLIB) $(SYSTEM_OPENGL)

C2_LIBS = -L. -L$(C2ROOT)/lib -lC2

MATRIX_OBJS = \
	    matrix.o

default: matrix

matrix: $(MATRIX_OBJS)
	${CXX} ${CXXFLAGS} -o matrix ${MATRIX_OBJS} ${SYSTEM_OPENGL} ${C2_LIBS} $(PF_LIBS)

clobber:
	rm $(CUBE_OBJS) cubes Makedepend
