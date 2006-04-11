# ************** <auto-copyright.pl BEGIN do not edit this line> **************
# Doozer
#
# Original Authors:
#   Patrick Hartling, Kevin Meinert
# -----------------------------------------------------------------------------
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
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          dzr.clean.mk,v
# Date modified: 2002/05/07 21:04:21
# Version:       1.6
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Basic clean-up file.  This defines a 'clean' target that pretty much every
# application developer would find useful.  It removes the objects files
# (listed in $(OBJS)) as well as any core files and user-specified junk files.
# A 'clobber' target is also defined that runs 'clean' and then removes the
# application binary (whose name is set in $(APP_NAME)).
#
# dzr.clean.mk,v 1.6 2002/05/07 21:04:21 subatomic Exp
# -----------------------------------------------------------------------------
# The following variable is available for users to set as they see fit:
#
# CLEAN_FILES   - The list of extra files that should be removed when the
#                'clean' target is run.
# CLEAN_DIRS    - The list of extra directories that can be removed when the
#                'clean' target is run.
# CLOBBER_FILES - The list of extra files that should be removed when the
#                'clobber' target is run.
# CLOBBER_DIRS  - The list of extra directories that should be removed when the
#                'clobber' target is run.
# -----------------------------------------------------------------------------

clean:
	-rm -f $(OBJS) *core core.* $(CLEAN_FILES)
	-rm -rf $(OBJDIR)/ii_files $(CLEAN_DIRS)

cleandepend:
	@$(MAKE) DO_CLEANDEPEND=1 _cleandepend

_cleandepend:
	-rm -f $(DEPDIR)/*.d

clobber:
	@$(MAKE) clean
	@$(MAKE) cleandepend
	-rm -f $(APP_NAME) $(SWITCHER_LIB_NAME) $(SWITCHER_MK_FILE) $(CLOBBER_FILES)
	-rm -rf $(CLOBBER_DIRS)
