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
# This include file <dpp.subdir.mk> handles recursive builds using a target
# called 'recursive'.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variable before
# including it:
#
# SUBDIR      - The subdirectories over which the recursion will be done.
#               Note that these directories will be built in parallel if the
#               -j flag is passed on the command line.
# -----------------------------------------------------------------------------
# The Makefile including this file should define the following variables
# before including it, or they should be passed as arguments to the recursive
# call to $(MAKE):
#
# BASE_OBJDIR - The directory to which the object file(s) will be written.
# DIRPRFX     - The prefix directory (if any) of the directories listed in
#               $(SUBDIR).  This is used for echoing what directory is being
#               entered.
# OPTIMIZER   - Optimizer flag(s) for the compiler.
# RECTARGET   - The name of the recursive target to build.
# =============================================================================

ifndef DEFAULT_SET
default: recursive
endif

# This is defined to be a phony target since everything listed in $(SUBDIR)
# is a directory that is supposed to exist in the current directory.
.PHONY: $(SUBDIR)

$(SUBDIR):
	@echo "===> $(DIRPRFX)$@"
	@$(MAKE) -C $@ BASE_OBJDIR="$(BASE_OBJDIR)"	\
            OPTIMIZER="$(OPTIMIZER)" $(RECTARGET)
	@echo "<=== $(DIRPRFX)$@"

# Using $(SUBDIR) as the dependencies to 'recursive' allows GNU make to build
# them all in parallel.
recursive: $(SUBDIR)
