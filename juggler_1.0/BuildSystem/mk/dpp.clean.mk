# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# Doozer++ is (C) Copyright 2000, 2001 by Iowa State University
#
# Original Author:
#   Patrick Hartling
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
# *************** <auto-copyright.pl END do not edit this line> ***************

# =============================================================================
# dpp.clean.mk,v 1.4 2001/02/16 22:05:26 patrick Exp
#
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

CLEAN_FILES	= $(OBJECTS) $(CLASSES) *core so_locations vc60.*
CLEAN_DIRS	= ii_files

clean:
ifdef SUBDIR
	@$(MAKE) RECTARGET="$@" recursive
endif
	rm -f $(CLEAN_FILES)
	rm -rf $(CLEAN_DIRS)
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
ifdef SUBDIR
	@$(MAKE) DO_CLEANDEPEND=1 RECTARGET="$@" recursive
endif
	rm -f $(CLEAN_FILES) $(CLOBBER_FILES)
	rm -rf $(CLEAN_DIRS) $(CLOBBER_DIRS)
ifdef _LOCAL_CLOBBER
	@$(MAKE) _clobber
endif
