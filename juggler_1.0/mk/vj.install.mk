# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
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
# GROUP_NAME    - The name of the group that will own the files.  If this is
#                 not defined, no attempt is made to set an alternate group
#                 name.
# -----------------------------------------------------------------------------

ifndef FILE_PERMS
    FILE_PERMS	= 644
endif

ifdef GROUP_NAME
    GROUP_OPT	= -g ${GROUP_NAME}
endif

INSTALL_DEPS	?=

ifndef INSTALL_FILES
    INSTALL_FILES	= ${srcdir}/*.h
endif

ifndef INSTALL_PATH
    INSTALL_PATH	= ${includedir}
endif

install: ${INSTALL_DEPS}
	@for file in ${INSTALL_FILES} ; do				\
            echo "  $${file} -> ${INSTALL_PATH}" ;			\
            ${INSTALL} -m ${FILE_PERMS} ${GROUP_OPT} $${file}		\
              ${INSTALL_PATH} ;						\
        done
