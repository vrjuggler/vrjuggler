# -----------------------------------------------------------------------------
# $Id$
#
# This include file <vj.rec.mk> handles recursive builds using a target called
# 'recursive'.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variable before
# including it:
#
# SUBDIRS     - The subdirectories over which the recursion will be done.
#               Note that these directories will be built in parallel if the
#               -j flag is passed on the command line.
# -----------------------------------------------------------------------------
# The Makefile including this file should define the following variables
# before including it, or they should be passed as arguments to the recursive
# call to ${MAKE}:
#
# BASE_OBJDIR - The directory to which the object file(s) will be written.
# DIRPRFX     - The prefix directory (if any) of the directories listed in
#               ${SUBDIRS}.  This is used for echoing what directory is being
#               entered.
# OPTIMIZER   - Optimizer flag(s) for the compiler.
# RECTARGET   - The name of the recursive target to build.
# -----------------------------------------------------------------------------

# This is defined to be a phony target since everything listed in ${SUBDIRS}
# is a directory that is supposed to exist in the current directory.
.PHONY: ${SUBDIRS}

${SUBDIRS}:
	@echo "===> ${DIRPRFX}$@"
	@${MAKE} -C $@ BASE_OBJDIR="${BASE_OBJDIR}"			\
            OPTIMIZER="${OPTIMIZER}" ${RECTARGET}
	@echo "<=== ${DIRPRFX}$@"

# Using ${SUBDIRS} as the dependencies to 'recursive' allows GNU make to
# build them all in parallel.
recursive: ${SUBDIRS}
