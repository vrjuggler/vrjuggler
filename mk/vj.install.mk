# -----------------------------------------------------------------------------
# $Id$
#
# This include file <vj.install.mk> handles install operations.  It adds an
# 'install' target.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variables:
#
# includedir - The installation directory for the header files.  This is
#              only needed if the default value for ${INSTALL_PATH} is to be
#              used.
# srcdir     - The directory where the source file(s) is/are located.  This is
#              only needed if the default value for ${INSTALL_FILES} is to be
#              used.
# INSTALL    - The path to a BSD-compatible install program.
# -----------------------------------------------------------------------------
# Optionally, the including file can add to the following existing variables
# to provide additional local 'install':
#
# INSTALL_DEPS  - Dependencies for the 'install' target.
# INSTALL_FILES - The file(s) to be installed.
# INSTALL_PATH  - The path to which the file(s) will be installed.
# FILE_PERMS    - Permissions on normal (non-executable) files.
# GROUP_NAME    - The name of the group that will own the files.
# -----------------------------------------------------------------------------

ifndef FILE_PERMS
    FILE_PERMS	= 664
endif

ifndef GROUP_NAME
    GROUP_NAME	= vrjuggler
endif

INSTALL_DEPS	?=

ifndef INSTALL_FILES
    INSTALL_FILES	= ${srcdir}/*.h
endif

ifndef INSTALL_PATH
    INSTALL_PATH	= ${includedir}
endif

install: ${INSTALL_DEPS}
	for file in ${INSTALL_FILES} ; do				\
            ${INSTALL} -m ${FILE_PERMS} -g ${GROUP_NAME} $${file}	\
              ${INSTALL_PATH} ;						\
        done
