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
# File:          dzr.vrjuggler.mk,v
# Date modified: 2003/10/03 12:45:32
# Version:       1.13
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# ----------------------------------------------------------------------------
# Basic variable settings for VR Juggler applications.  Only options
# relevant to VR Juggler go here.
#
# dzr.vrjuggler.mk,v 1.13 2003/10/03 12:45:32 patrickh Exp
# ----------------------------------------------------------------------------

# Get basic VRJ information
#
# Parameters:
#   VRJ_LIBS_PARAM: (vrj | vrj_ogl | vrj_pf)
#        The library parameter to send to the vrjuggler-config script.
#        Default: vrj

ifndef APP_ABI
ifeq ($(OS_ABI), n32)
   APP_ABI=	N32
else
   ifeq ($(OS_ABI), 64)
      APP_ABI=	64
   endif
endif
endif

# The name of the command to run to get the info for VPR.
VRJ_CONFIG=		vrjuggler-config
VRJ_LIBS_PARAM?=	vrj

ifdef VJ_BASE_DIR
   VRJ_CONFIG:=		$(VJ_BASE_DIR)/bin/$(VRJ_CONFIG)
endif

# VR Juggler compile flags.
VRJ_CXX_FLAGS=			$(shell $(VRJ_CONFIG) --cxxflags $(APP_ABI))
VRJ_SUBSYSTEM=			$(shell $(VRJ_CONFIG) --subsystem)
VRJ_VERSION=			$(shell $(VRJ_CONFIG) --version)
VRJ_CXX_FLAGS_MIN=		$(shell $(VRJ_CONFIG) --cxxflags $(APP_ABI) --min)
VRJ_INCLUDES=			$(shell $(VRJ_CONFIG) --includes)

# Basic VR Juggler libs.
VRJ_LIBS=			$(shell $(VRJ_CONFIG) $(VRJ_LIBS_PARAM) --libs $(APP_ABI))
VRJ_EXTRA_LIBS=			$(shell $(VRJ_CONFIG) $(VRJ_LIBS_PARAM) --extra-libs $(APP_ABI))
VRJ_EXTRA_LIBS_STATIC=		$(shell $(VRJ_CONFIG) $(VRJ_LIBS_PARAM) --extra-libs $(APP_ABI) --static)

VRJ_LIBS_MIN=			$(shell $(VRJ_CONFIG) $(VRJ_LIBS_PARAM) --libs $(APP_ABI) --min)
VRJ_EXTRA_LIBS_MIN=		$(shell $(VRJ_CONFIG) $(VRJ_LIBS_PARAM) --extra-libs $(APP_ABI) --min)
VRJ_EXTRA_LIBS_MIN_STATIC=	$(shell $(VRJ_CONFIG) $(VRJ_LIBS_PARAM) --extra-libs $(APP_ABI) --min --static)

define VRJ_echo_vars
@echo "vrj settings"
@echo "VRJ_CONFIG: $(VRJ_CONFIG)"
@echo "VRJ_LIBS_PARAM: $(VRJ_LIBS_PARAM)"
@echo "VRJ_CXX_FLAGS: $(VRJ_CXX_FLAGS)"
@echo "VRJ_INCLUDES: $(VRJ_INCLUDES)"
@echo "VRJ_LIBS: $(VRJ_LIBS)"
@echo "VRJ_EXTRA_LIBS: $(VRJ_EXTRA_LIBS)"
@echo "VRJ_EXTRA_LIBS_STATIC: $(VRJ_EXTRA_LIBS_STATIC)"
@echo "VRJ_CXX_FLAGS_MIN: $(VRJ_CXX_FLAGS_MIN)"
@echo "VRJ_LIBS_MIN: $(VRJ_LIBS_MIN)"
@echo "VRJ_EXTRA_LIBS_MIN: $(VRJ_EXTRA_LIBS_MIN)"
@echo "VRJ_EXTRA_LIBS_MIN_STATIC: $(VRJ_EXTRA_LIBS_MIN_STATIC)"
@echo "VRJ_SUBSYSTEM: $(VRJ_SUBSYSTEM)"
@echo "VRJ_VERSION: $(VRJ_VERSION)"
endef

# VR Juggler 1.0 settings.
VJ_LIB_TYPE?=	debug

ifdef VJ_FULL_LIB_PATH
   VJ_LIB_PATH=	$(VJ_BASE_DIR)/lib$(OS_LIBBITSUF)/$(OS_ISA)/$(VJ_LIB_TYPE)
else
   VJ_LIB_PATH=	$(VJ_BASE_DIR)/lib$(OS_LIBBITSUF)
endif

# VR Juggler libraries.
VJ_INCLUDES?=	-I$(VJ_BASE_DIR)/include
VJ_LDFLAGS?=	-L$(VJ_LIB_PATH)
VJ_LIBS?=	-lJuggler -lJuggler_utils
