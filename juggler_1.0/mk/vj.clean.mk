# -----------------------------------------------------------------------------
# $Id$
#
# This include file <vj.clean.mk> handles clean-up operations.  It adds a
# 'clean' target and a 'cleandepend' target.
# -----------------------------------------------------------------------------
# The Makefiles that include this file can add to the ${CLEAN_FILES} variable
# to dictate which files should be removed when 'make clean' is done.
# Common files that are likely to always be removed should be listed here.
# A ${CLEAN_DIRS} variable is provided for recursively removing directories.
# It is used in the same manner as ${CLEAN_FILES}.
#
# To provide a "local" 'clean' target in the file including this code,
# define ${_LOCAL_CLEAN} before including this file and define a target
# called '_clean'.  It will be run after the standard 'clean' target is
# executed.
#
# Example:
#     CLEAN_FILES += file1.o file2.o
#
# Similarly, a "local" 'cleandepend' target can be used by defining
# #{_LOCAL_CLEANDEP} before including this file.  The name of the local
# target is '_cleandepend', and it will be run after the standard
# 'cleandepend' target is finished.
# -----------------------------------------------------------------------------

CLEAN_FILES	= core so_locations vc60.*
CLEAN_DIRS	= ii_files

clean:
	rm -f ${CLEAN_FILES}
	rm -rf ${CLEAN_DIRS}
ifdef _LOCAL_CLEAN
	${MAKE} _clean
endif

cleandepend:
	-rm -f *.d
ifdef _LOCAL_CLEANDEP
	@${MAKE} _cleandepend
endif
