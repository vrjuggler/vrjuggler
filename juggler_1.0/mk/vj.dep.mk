# -----------------------------------------------------------------------------
# $Id$
#
# This include file <vj.dep.mk> handles Makefile dependencies.
# -----------------------------------------------------------------------------
# The targets defined by this file are:
#
# depend    - Generate the dependencies for the source files.
# newdepend - Forcibly regenerate the dependencies for the source files.
# -----------------------------------------------------------------------------
# The Makefile including this file should define the following variables:
#
# srcdir        - The directory where the source file(s) is/are located.
# DEPENDFLAGS   - Flags to pass to makedepend(1).
# DEPEND_EXTRAS - Extra flags that may not be recognized by makedepend(1)
#                 because they may be specific to a given compiler.  Any
#                 recognized options will be processed normally while any
#                 unrecognized options will be ignored.  Thus, this variable
#                 could be set to ${CLFAGS} to allow compiler options
#                 necessary for the source to compile to be passed to
#                 makedepend(1).
# DEPEND_SRCS   - The list of source files to be processed.
# OBJDIR        - The directory to which the object file(s) will be written.
# WORKDIR       - The working directory (not necessarily where the source
#                 files are found).
#
# Example:
#        srcdir = /usr/src/proj1
#        OBJDIR = /usr/obj/proj1
#   DEPENDFLAGS = ${INCLUDES} ${DEFS}
# DEPEND_EXTRAS = ${CFLAGS}
#   DEPEND_SRCS = file1.c file2.c file3.c
#       WORKDIR = /usr/build/proj1
#
# With these settings, the source code comes from /usr/src/proj1 and the
# object files go into /usr/obj/proj1.  The actual Makefile from which this
# is run is in /usr/build/proj1.
# -----------------------------------------------------------------------------
# Optionally, the including file can add to the following existing variables
# to provide additional local 'depend' and 'newdepend' functionality:
#
# DEPEND_DEPS    - Dependencies for the 'depend' target.
# NEWDEPEND_DEPS - Dependencies for the 'newdepend' target.
# -----------------------------------------------------------------------------

# Variables that should be used only by this file.
_DEPEND_FILE	= ${WORKDIR}/Makefile
_DEPEND_DONE	= .depend_done

DEPEND_DEPS	+= ${_DEPEND_DONE}
NEWDEPEND_DEPS	?=

# Add ${_DEPEND_DONE} to ${CLEAN_FLAGS} and conditionally set ${OBJDIR} just
# to be safe.
CLEAN_FILES	+= ${_DEPEND_DONE}
OBJDIR		?= .

depend: ${DEPEND_DEPS}

newdepend: ${NEWDEPEND_DEPS}
	-@rm -f ${_DEPEND_DONE}
	${MAKE} depend

# This target will allow the 'depend' target to be built only once.
${_DEPEND_DONE}:
	cd ${srcdir} &&							\
          makedepend -f${_DEPEND_FILE} -p'$${OBJDIR}/'			\
              ${DEPENDFLAGS} -- ${DEPEND_EXTRAS} -- ${DEPEND_SRCS}
	@touch ${WORKDIR}/${_DEPEND_DONE}
