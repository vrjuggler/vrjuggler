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
# File:          dzr.opensg.mk,v
# Date modified: 2002/03/19 21:39:12
# Version:       1.1
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# ----------------------------------------------------------------------------
# Basic variable settings for code that depends on the VR Juggler Portable
# Runtime (VPR).  Only options relevant to VPR go here.
#
# dzr.opensg.mk,v 1.1 2002/03/19 21:39:12 allenb Exp
# ----------------------------------------------------------------------------

# The name of the command to run to get the info for OpenSG.
OPENSG_CONFIG=	osg-config

#ifdef VPR_BASE_DIR
#   VPR_CONFIG:=	$(VPR_BASE_DIR)/bin/$(VPR_CONFIG)
#endif

OPENSG_CXX_FLAGS=	$(shell $(OPENSG_CONFIG) --cflags)
OPENSG_LIBS=	$(shell $(OPENSG_CONFIG) --libs) -lOSGBase -lOSGSystem
OPENSG_VERSION=	$(shell $(OPENSG_CONFIG) --version)

define OPENSG_echo_vars
@echo "OpenSG settings"
@echo "OPENSG_CONFIG: $(OPENSG_CONFIG)"
@echo "OPENSG_CXX_FLAGS: $(OPENSG_CXX_FLAGS)"
@echo "OPENSG_LIBS: $(OPENSG_LIBS)"
@echo "OPENSG_VERSION: $(OPENSG_VERSION)"
endef
