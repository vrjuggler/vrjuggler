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
# File:          dzr.vpr.mk,v
# Date modified: 2002/06/27 22:19:36
# Version:       1.3
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# ----------------------------------------------------------------------------
# Basic variable settings for code that depends on the VR Juggler Portable
# Runtime (VPR).  Only options relevant to VPR go here.
#
# dzr.vpr.mk,v 1.3 2002/06/27 22:19:36 patrickh Exp
# ----------------------------------------------------------------------------

# The name of the command to run to get the info for VPR.
VPR_CONFIG=	vpr-config

ifdef VPR_BASE_DIR
   VPR_CONFIG:=	$(VPR_BASE_DIR)/bin/$(VPR_CONFIG)
endif

VPR_CXX_FLAGS=		$(shell $(VPR_CONFIG) --cxxflags)
VPR_LIBS=		$(shell $(VPR_CONFIG) --libs)
VPR_EXTRA_LIBS=		$(shell $(VPR_CONFIG) --extra-libs)
VPR_SUBSYSTEM=		$(shell $(VPR_CONFIG) --subsystem)
VPR_VERSION=		$(shell $(VPR_CONFIG) --version)

VPR_CXX_FLAGS_MIN=	$(shell $(VPR_CONFIG) --cxxflags --min)
VPR_LIBS_MIN=		$(shell $(VPR_CONFIG) --libs --min)
VPR_EXTRA_LIBS_MIN=	$(shell $(VPR_CONFIG) --extra-libs --min)

define VPR_echo_vars
@echo "vpr settings"
@echo "VPR_CONFIG: $(VPR_CONFIG)"
@echo "VPR_CXX_FLAGS: $(VPR_CXX_FLAGS)"
@echo "VPR_LIBS: $(VPR_LIBS)"
@echo "VPR_EXTRA_LIBS: $(VPR_EXTRA_LIBS)"
@echo "VPR_CXX_FLAGS_MIN: $(VPR_CXX_FLAGS_MIN)"
@echo "VPR_LIBS_MIN: $(VPR_LIBS_MIN)"
@echo "VPR_EXTRA_LIBS_MIN: $(VPR_EXTRA_LIBS_MIN)"
@echo "VPR_SUBSYSTEM: $(VPR_SUBSYSTEM)"
@echo "VPR_VERSION: $(VPR_VERSION)"
endef
