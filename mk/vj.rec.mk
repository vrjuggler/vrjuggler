# -----------------------------------------------------------------------------
# $Id$
#
# This include file <vj.rec.mk> handles recursive builds using a target called
# 'recursive'.
# -----------------------------------------------------------------------------
# The Makefile including this file should define the following variables:
#
# BASE_OBJDIR - The directory to which the object file(s) will be written.
# DIRPRFX     - The prefix directory (if any) of the directories listed in
#               ${SUBDIRS}.  This is used for echoing what directory is being
#               entered.
# OPTIMIZER   - Optimizer flag(s) for the compiler.
# RECTARGET   - The name of the recursive target to build.
# SUBDIRS     - The subdirectories over which the recursion will be done.
# -----------------------------------------------------------------------------

recursive:
	@amf=$$2; list='${SUBDIRS}';					\
            for subdir in $$list; do					\
                echo "===> ${DIRPRFX}$$subdir" ;			\
                (cd $$subdir && ${MAKE} OPTIMIZER="${OPTIMIZER}"	\
                                    BASE_OBJDIR="${BASE_OBJDIR}"	\
                                    ${RECTARGET})			\
                    || case "$$amf" in					\
                         *=*) exit 1;;					\
                         *k*) fail=yes;;				\
                         *) exit 1;;					\
                       esac;						\
                echo "<=== ${DIRPRFX}$$subdir" ;			\
            done && test -z "$$fail"
