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
# File:          dzr.dso.mk,v
# Date modified: 2002/07/10 22:45:54
# Version:       1.4
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# ----------------------------------------------------------------------------
# Basic variable settings for VR Juggler applications.  Only options
# relevant to VR Juggler go here.
#
# dzr.dso.mk,v 1.4 2002/07/10 22:45:54 allenb Exp
# ----------------------------------------------------------------------------

# The name of the command to run to get the info for VPR.
DSO_CONFIG=	dso-config

ifdef DSO_BASE_DIR
   DSO_CONFIG:=	$(DSO_BASE_DIR)/bin/$(DSO_CONFIG)
endif

DSO_CXX_FLAGS=	$(shell $(DSO_CONFIG) --cxxflags)
DSO_LIBS=	$(shell $(DSO_CONFIG) --libs)
DSO_EXTRA_LIBS=	$(shell $(DSO_CONFIG) --extra-libs)
DSO_VERSION=	$(shell $(DSO_CONFIG) --version)

DSO_CXX_FLAGS_MIN=	$(shell $(DSO_CONFIG) --cxxflags --min)
DSO_LIBS_MIN=	$(shell $(DSO_CONFIG) --libs --min)
DSO_EXTRA_LIBS_MIN=	$(shell $(DSO_CONFIG) --extra-libs --min)

DSO_EQIDL=	$(shell $(DSO_CONFIG) --eqidl)

define DSO_echo_vars
@echo "dso settings"
@echo "DSO_CONFIG: $(DSO_CONFIG)"
@echo "DSO_CXX_FLAGS: $(DSO_CXX_FLAGS)"
@echo "DSO_LIBS: $(DSO_LIBS)"
@echo "DSO_EXTRA_LIBS: $(DSO_EXTRA_LIBS)"
@echo "DSO_CXX_FLAGS_MIN: $(DSO_CXX_FLAGS_MIN)"
@echo "DSO_LIBS_MIN: $(DSO_LIBS_MIN)"
@echo "DSO_EXTRA_LIBS_MIN: $(DSO_EXTRA_LIBS_MIN)"
@echo "DSO_VERSION: $(DSO_VERSION)"
@echo "DSO_EQIDL: $(DSO_EQIDL)"
endef

