# Generated automatically from Makefile.in by configure.
# -----------------------------------------------------------------------------
# Makefile.in for c2lib/test
# $Id$
# -----------------------------------------------------------------------------

SHELL		= /bin/sh

bindir		= ${exec_prefix}/bin
datadir		= ${prefix}/share
exec_prefix	= ${prefix}
includedir	= ${prefix}/include
libdir		= ${exec_prefix}/lib
libexecdir	= ${exec_prefix}/libexec
prefix		= /usr/local
sbindir		= ${exec_prefix}/sbin
srcdir		= /home/users/allenb/Source/juggler/test
sysconfdir	= ${prefix}/etc
topdir		= /home/users/allenb/Source/juggler
top_srcdir	= /home/users/allenb/Source/juggler
C2ROOT		= ${top_srcdir}
C2ROOT_ABS	= @C2ROOT_ABS@
C2_SRCDIR	= @C2_SRCDIR@/test
WORKDIR		= ${topdir}/test

CC		= cc
CXX		= CC
CPP		= cc -E
CXXCPP		= CC -E
CFLAGS		= -ptv  -mips3 -n32 -nostdinc -woff 1685,515,608,658,799,803,852,1048,1233,1499 -MDupdate Makedepend ${OPTIMIZER}
CPPFLAGS	= 
CXXFLAGS	= -ptv  -mips3 -n32 -nostdinc -woff 3322 -w2 -MDupdate Makedepend ${OPTIMIZER}
DEFS		= -DHAVE_CONFIG_H
DEPENDFLAGS	=   -I/usr/include/CC -I/usr/include -I${C2ROOT_ABS} -I${topdir}
DEPEND_EXTRAS	=  ${DEFS}
INCLUDES	=  -I/usr/include/CC -I/usr/include -I${C2ROOT} -I${topdir}
LDFLAGS		= 
LIBS		= 
OBJDIR		= .
VPATH		= /home/users/allenb/Source/juggler/test

SUBDIRS		= CfgTest input ogl pfNav



# -----------------------------------------------------------------------------
# Library targets.
# -----------------------------------------------------------------------------

default: all

all-recursive install-recursive depend-recursive newdepend-recursive:
	@set fnord ${MAKEFLAGS}; amf=$$2;				\
            list='${SUBDIRS}';						\
            for subdir in $$list; do					\
                target=`echo $@ | sed s/-recursive//`;			\
                echo "Making $$target in test/$$subdir";		\
                (cd $$subdir && ${MAKE} OPTIMIZER="${OPTIMIZER}"	\
                                    OBJDIR=${OBJDIR} $$target)		\
                    || case "$$amf" in					\
                         *=*) exit 1;;					\
                         *k*) fail=yes;;				\
                         *) exit 1;; esac;				\
            done && test -z "$$fail"

all: all-recursive

depend: depend-recursive

newdepend: newdepend-recursive

CfgTest input ogl pfNav:
	@set fnord ${MAKEFLAGS}; amf=$$2;				\
            list=$@;							\
            for subdir in $$list; do					\
                target=`echo $@ | sed s/-recursive//`;			\
                echo "Making $$target in test/$$subdir";		\
                (cd $$subdir && ${MAKE} OPTIMIZER="${OPTIMIZER}"	\
                                    OBJDIR=${OBJDIR} $$target)		\
                    || case "$$amf" in					\
                         *=*) exit 1;;					\
                         *k*) fail=yes;;				\
                         *) exit 1;; esac;				\
            done && test -z "$$fail"

# -----------------------------------------------------------------------------
# Clean-up.
# -----------------------------------------------------------------------------
clean-recursive:
	@set fnord ${MAKEFLAGS}; amf=$$2;		\
            rev=''; list='${SUBDIRS}';			\
            for subdir in $$list; do			\
                rev="$$subdir $$rev";			\
            done;					\
            for subdir in $$rev; do			\
                target=`echo $@ | sed s/-recursive//`;	\
                echo "Making $$target in $$subdir";	\
                (cd $$subdir && ${MAKE} $$target)	\
                    || case "$$amf" in			\
                         *=*) exit 1;;			\
                         *k*) fail=yes;;		\
                         *) exit 1;;			\
                       esac;				\
            done && test -z "$$fail"

clean: clean-recursive

clobber: clean
