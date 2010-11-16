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
# This include file <dpp.clean.mk> handles clean-up operations.  It adds a
# 'clean' target and a 'cleandepend' target.
# -----------------------------------------------------------------------------
# The Makefiles that include this file can add to the $(CLEAN_FILES) variable
# to dictate which files should be removed when 'make clean' is done.
# Common files that are likely to always be removed should be listed here.
# A $(CLEAN_DIRS) variable is provided for recursively removing directories.
# It is used in the same manner as $(CLEAN_FILES).
#
# To provide a "local" 'clean' target in the file including this code,
# define $(_LOCAL_CLEAN) before including this file and define a target
# called '_clean'.  It will be run after the standard 'clean' target is
# executed.
#
# Example:
#     CLEAN_FILES += file1.o file2.o
#
# Similarly, a "local" 'cleandepend' target can be used by defining
# $(_LOCAL_CLEANDEP) before including this file.  The name of the local
# target is '_cleandepend', and it will be run after the standard
# 'cleandepend' target is finished.
# =============================================================================

CLEAN_FILES=	$(OBJECTS) $(CLASSES) *core core.[1-9]* so_locations vc60.*
CLEAN_DIRS=	ii_files SunWS_cache

clean:
ifdef SUBDIR
	@$(MAKE) RECTARGET="$@" recursive
endif
ifneq ($(CLEAN_FILES), )
	rm -f $(CLEAN_FILES)
endif
ifneq ($(CLEAN_DIRS), )
	rm -rf $(CLEAN_DIRS)
endif
ifdef _LOCAL_CLEAN
	@$(MAKE) _clean
endif

cleandepend:
ifdef SUBDIR
	@$(MAKE) DO_CLEANDEPEND=1 RECTARGET="$@" recursive
endif
	-rm -f *.d
ifdef _LOCAL_CLEANDEP
	@$(MAKE) _cleandepend
endif

clobber:
	@$(MAKE) DO_CLEANDEPEND=1 clean
ifdef SUBDIR
	@$(MAKE) DO_CLEANDEPEND=1 RECTARGET="$@" recursive
endif
ifneq ($(CLOBBER_FILES), )
	rm -f $(CLOBBER_FILES)
endif
ifneq ($(CLOBBER_DIRS), )
	rm -rf $(CLOBBER_DIRS)
endif
ifdef _LOCAL_CLOBBER
	@$(MAKE) _clobber
endif
