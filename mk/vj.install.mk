# -----------------------------------------------------------------------------
# $Id$
#
# This include file <vj.install.mk> handles install operations.  It adds an
# 'install' target.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variables:
#
# includedir - The installation directory for the header files.
# scriptdir  - The location of the scripts used in the build and release
#              process.
# srcdir     - The directory where the source file(s) is/are located.
# INSTALL    - The path to a BSD-compatible install program.
# -----------------------------------------------------------------------------
# Optionally, the including file can add to the following existing variables
# to provide additional local 'install':
#
# INSTALL_DEPS - Dependencies for the 'install' target.
# FILE_PERMS   - Permissions on normal (non-executable) files.
# GROUP_NAME   - The name of the group that will own the files.
# -----------------------------------------------------------------------------

ifndef FILE_PERMS
    FILE_PERMS	= 664
endif

ifndef GROUP_NAME
    GROUP_NAME	= c2dev
endif

INSTALL_DEPS	?=

install: ${INSTALL_DEPS}
	${scriptdir}/mkinstalldirs ${includedir}
	${INSTALL} -m ${FILE_PERMS} -g ${GROUP_NAME} ${srcdir}/*.h ${includedir}
