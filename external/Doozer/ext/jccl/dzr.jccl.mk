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
# File:          dzr.jccl.mk,v
# Date modified: 2002/06/27 22:19:36
# Version:       1.2
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# ----------------------------------------------------------------------------
# Basic variable settings for code that depends on the Juggler Configuration
# and Control Library (JCCL).  Only options relevant to JCCL go here.
#
# dzr.jccl.mk,v 1.2 2002/06/27 22:19:36 patrickh Exp
# ----------------------------------------------------------------------------

# The name of the command to run to get the info for JCCL.
JCCL_CONFIG=	jccl-config

ifdef JCCL_BASE_DIR
   JCCL_CONFIG:=	$(JCCL_BASE_DIR)/bin/$(JCCL_CONFIG)
endif

JCCL_CXX_FLAGS=		$(shell $(JCCL_CONFIG) --cxxflags)
JCCL_INCLUDES=		$(shell $(JCCL_CONFIG) --includes)
JCCL_LIBS=		$(shell $(JCCL_CONFIG) --libs)
JCCL_EXTRA_LIBS=	$(shell $(JCCL_CONFIG) --extra-libs)
JCCL_VERSION=		$(shell $(JCCL_CONFIG) --version)

JCCL_CXX_FLAGS_MIN=	$(shell $(JCCL_CONFIG) --cxxflags --min)
JCCL_INCLUDES_MIN=	$(shell $(JCCL_CONFIG) --includes --min)
JCCL_LIBS_MIN=		$(shell $(JCCL_CONFIG) --libs --min)
JCCL_EXTRA_LIBS_MIN=	$(shell $(JCCL_CONFIG) --extra-libs --min)

define JCCL_echo_vars
@echo "JCCL settings"
@echo "JCCL_CONFIG: $(JCCL_CONFIG)"
@echo "JCCL_CXX_FLAGS: $(JCCL_CXX_FLAGS)"
@echo "JCCL_INCLUDES: $(JCCL_INCLUDES)"
@echo "JCCL_LIBS: $(JCCL_LIBS)"
@echo "JCCL_EXTRA_LIBS: $(JCCL_EXTRA_LIBS)"
@echo "JCCL_CXX_FLAGS_MIN: $(JCCL_CXX_FLAGS_MIN)"
@echo "JCCL_INCLUDES_MIN: $(JCCL_INCLUDES_MIN)"
@echo "JCCL_LIBS_MIN: $(JCCL_LIBS_MIN)"
@echo "JCCL_EXTRA_LIBS_MIN: $(JCCL_EXTRA_LIBS_MIN)"
@echo "JCCL_SUBSYSTEM: $(JCCL_SUBSYSTEM)"
@echo "JCCL_VERSION: $(JCCL_VERSION)"
endef
