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
# File:          dzr.plexus.mk,v
# Date modified: 2002/07/10 22:45:54
# Version:       1.2
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# ----------------------------------------------------------------------------
# Basic variable settings for Plexus applications.  Only options relevant
# to Plexus go here.
#
# dzr.plexus.mk,v 1.2 2002/07/10 22:45:54 allenb Exp
# ----------------------------------------------------------------------------

# The name of the command to run to get the info for VPR.
PLEXUS_CONFIG=	plexus-config

ifdef PLX_BASE_DIR
   PLEXUS_CONFIG:=	$(PLX_BASE_DIR)/bin/$(PLEXUS_CONFIG)
endif

PLX_CXX_FLAGS=	$(shell $(PLEXUS_CONFIG) --cxxflags)
PLX_LIBS=	$(shell $(PLEXUS_CONFIG) --libs)
PLX_EXTRA_LIBS=	$(shell $(PLEXUS_CONFIG) --extra-libs)
PLX_VERSION=	$(shell $(PLEXUS_CONFIG) --version)

PLX_CXX_FLAGS_MIN=	$(shell $(PLEXUS_CONFIG) --cxxflags --min)
PLX_LIBS_MIN=	$(shell $(PLEXUS_CONFIG) --libs --min)
PLX_EXTRA_LIBS_MIN=	$(shell $(PLEXUS_CONFIG) --extra-libs --min)

define PLX_echo_vars
@echo "Plexus settings"
@echo "PLEXUS_CONFIG: $(PLEXUS_CONFIG)"
@echo "PLX_CXX_FLAGS: $(PLX_CXX_FLAGS)"
@echo "PLX_LIBS: $(PLX_LIBS)"
@echo "PLX_EXTRA_LIBS: $(PLX_EXTRA_LIBS)"
@echo "PLX_CXX_FLAGS_MIN: $(PLX_CXX_FLAGS_MIN)"
@echo "PLX_LIBS_MIN: $(PLX_LIBS_MIN)"
@echo "PLX_EXTRA_LIBS_MIN: $(PLX_EXTRA_LIBS_MIN)"
@echo "PLX_VERSION: $(PLX_VERSION)"
endef
