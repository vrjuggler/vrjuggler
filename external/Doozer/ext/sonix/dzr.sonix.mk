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
# File:          dzr.sonix.mk,v
# Date modified: 2002/09/12 15:40:47
# Version:       1.1
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# ----------------------------------------------------------------------------
# Basic variable settings for code that depends on Sonix.  Only options
# relevant to Sonix go here.
#
# dzr.sonix.mk,v 1.1 2002/09/12 15:40:47 patrickh Exp
# ----------------------------------------------------------------------------

# The name of the command to run to get the info for Sonix.
SNX_CONFIG=	sonix-config

ifdef SNX_BASE_DIR
   SNX_CONFIG:=	$(SNX_BASE_DIR)/bin/$(SNX_CONFIG)
endif

SNX_VERSION=			$(shell $(SNX_CONFIG) --version)

SNX_CXX_FLAGS=			$(shell $(SNX_CONFIG) --cxxflags)
SNX_INCLUDES=			$(shell $(SNX_CONFIG) --includes)
SNX_LIBS=			$(shell $(SNX_CONFIG) --libs)
SNX_EXTRA_LIBS=			$(shell $(SNX_CONFIG) --extra-libs)
SNX_EXTRA_LIBS_STATIC=		$(shell $(SNX_CONFIG) --extra-libs --static)

SNX_CXX_FLAGS_MIN=		$(shell $(SNX_CONFIG) --cxxflags --min)
SNX_INCLUDES_MIN=		$(shell $(SNX_CONFIG) --includes --min)
SNX_LIBS_MIN=			$(shell $(SNX_CONFIG) --libs --min)
SNX_EXTRA_LIBS_MIN=		$(shell $(SNX_CONFIG) --extra-libs --min)
SNX_EXTRA_LIBS_MIN_STATIC=	$(shell $(SNX_CONFIG) --extra-libs --min --static)

define SNX_echo_vars
@echo "Sonix settings"
@echo "SNX_CONFIG: $(SNX_CONFIG)"
@echo "SNX_CXX_FLAGS: $(SNX_CXX_FLAGS)"
@echo "SNX_INCLUDES: $(SNX_INCLUDES)"
@echo "SNX_LIBS: $(SNX_LIBS)"
@echo "SNX_EXTRA_LIBS: $(SNX_EXTRA_LIBS)"
@echo "SNX_CXX_FLAGS_MIN: $(SNX_CXX_FLAGS_MIN)"
@echo "SNX_INCLUDES_MIN: $(SNX_INCLUDES_MIN)"
@echo "SNX_LIBS_MIN: $(SNX_LIBS_MIN)"
@echo "SNX_EXTRA_LIBS_MIN: $(SNX_EXTRA_LIBS_MIN)"
@echo "SNX_SUBSYSTEM: $(SNX_SUBSYSTEM)"
@echo "SNX_VERSION: $(SNX_VERSION)"
endef
