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
# File:          dzr.gadgeteer.mk,v
# Date modified: 2002/08/29 22:54:49
# Version:       1.3
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# ----------------------------------------------------------------------------
# Basic variable settings for code that depends on Gadgeteer.  Only options
# relevant to Gadgeteer go here.
#
# dzr.gadgeteer.mk,v 1.3 2002/08/29 22:54:49 patrickh Exp
# ----------------------------------------------------------------------------

# The name of the command to run to get the info for Gadgeteer.
GADGET_CONFIG=	gadgeteer-config

ifdef GADGET_BASE_DIR
   GADGET_CONFIG:=	$(GADGET_BASE_DIR)/bin/$(GADGET_CONFIG)
endif

GADGET_VERSION=			$(shell $(GADGET_CONFIG) --version)

GADGET_CXX_FLAGS=		$(shell $(GADGET_CONFIG) --cxxflags)
GADGET_INCLUDES=		$(shell $(GADGET_CONFIG) --includes)
GADGET_LIBS=			$(shell $(GADGET_CONFIG) --libs)
GADGET_EXTRA_LIBS=		$(shell $(GADGET_CONFIG) --extra-libs)
GADGET_EXTRA_LIBS_STATIC=	$(shell $(GADGET_CONFIG) --extra-libs --static)

GADGET_CXX_FLAGS_MIN=		$(shell $(GADGET_CONFIG) --cxxflags --min)
GADGET_INCLUDES_MIN=		$(shell $(GADGET_CONFIG) --includes --min)
GADGET_LIBS_MIN=		$(shell $(GADGET_CONFIG) --libs --min)
GADGET_EXTRA_LIBS_MIN=		$(shell $(GADGET_CONFIG) --extra-libs --min)
GADGET_EXTRA_LIBS_MIN_STATIC=	$(shell $(GADGET_CONFIG) --extra-libs --min --static)

define GADGET_echo_vars
@echo "Gadgeteer settings"
@echo "GADGET_CONFIG: $(GADGET_CONFIG)"
@echo "GADGET_CXX_FLAGS: $(GADGET_CXX_FLAGS)"
@echo "GADGET_INCLUDES: $(GADGET_INCLUDES)"
@echo "GADGET_LIBS: $(GADGET_LIBS)"
@echo "GADGET_EXTRA_LIBS: $(GADGET_EXTRA_LIBS)"
@echo "GADGET_CXX_FLAGS_MIN: $(GADGET_CXX_FLAGS_MIN)"
@echo "GADGET_INCLUDES_MIN: $(GADGET_INCLUDES_MIN)"
@echo "GADGET_LIBS_MIN: $(GADGET_LIBS_MIN)"
@echo "GADGET_EXTRA_LIBS_MIN: $(GADGET_EXTRA_LIBS_MIN)"
@echo "GADGET_SUBSYSTEM: $(GADGET_SUBSYSTEM)"
@echo "GADGET_VERSION: $(GADGET_VERSION)"
endef
