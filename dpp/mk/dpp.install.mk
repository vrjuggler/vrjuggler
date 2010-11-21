# Doozer++ is (C) Copyright 2000-2010 by Iowa State University
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
#
# Original Author:
#   Patrick Hartling

# VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.

# =============================================================================
# This include file <dpp.install.mk> handles install operations.  It adds an
# 'install' target.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variables:
#
# includedir - The installation directory for the header files.  This is
#              only needed if the default value for $(INSTALL_PATH) is to be
#              used.
# srcdir     - The directory where the source file(s) is/are located.  This is
#              only needed if the default value for $(INSTALL_FILES) is to be
#              used.
# INSTALL    - The path to a BSD-compatible install program.
#
# Optionally, the including file can add to the following existing variables
# to provide additional local 'install':
#
# INSTALL_DEPS   - Dependencies for the 'install' target.
# INSTALL_FILES  - The file(s) to be installed.
# INSTALL_PATH   - The path to which the file(s) will be installed.
# FILE_PERMS     - Permissions on normal (non-executable) files.
# GROUP_NAME     - The name of the group that will own the files.  If this is
#                  not defined, no attempt is made to set an alternate group
#                  name.
# SUBDIR         - The list of subdirectories where installation needs to be
#                  done.
# INSTALL_SUBDIR - The list of subdirectories where installation *alone* needs
#                  to be done.  This can be used in conjunction with $(SUBDIR)
#                  to list directories where no compiling will be done.
# -----------------------------------------------------------------------------
# The target defined here is as follows:
#
#     install - Recurse over any subdirectories in $(SUBDIR) running 'install'
#               in each and then install $(INSTALL_FILES) in the current
#               directory.
# =============================================================================

FILE_PERMS?=	644

ifdef GROUP_NAME
GROUP_OPT?=	-g $(GROUP_NAME)
endif

#INSTALL_DEPS?=	
INSTALL_FILES?=	$(wildcard $(srcdir)/*.h)
INSTALL_PATH?=	$(includedir)

install:
ifneq ($(INSTALL_DEPS), )
	$(MAKE) $(INSTALL_DEPS)
endif
ifneq ($(PRE_INSTALL), )
	$(MAKE) $(PRE_INSTALL)
endif
ifdef SUBDIR
	@for dir in $(SUBDIR) ; do					\
             $(MAKE) -C $$dir $@ ;					\
         done
endif
ifdef INSTALL_SUBDIR
	@for dir in $(INSTALL_SUBDIR) ; do				\
             $(MAKE) -C $$dir $@ ;					\
         done
endif
ifneq ($(INSTALL_FILES), )
ifeq ($(USE_DPP_BSD_INSTALL), yes)
	$(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT) $(EXTRA_INSTALL_ARGS)	\
          $(INSTALL_FILES) $(INSTALL_PATH)
else
	@for file in $(INSTALL_FILES) ; do				\
            echo "  $$file ==> $(INSTALL_PATH)" ;			\
            $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT)			\
              $(EXTRA_INSTALL_ARGS) $$file $(INSTALL_PATH) ;		\
         done
endif
endif
ifneq ($(POST_INSTALL), )
	$(MAKE) $(POST_INSTALL)
endif
