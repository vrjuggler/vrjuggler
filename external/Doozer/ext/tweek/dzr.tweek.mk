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
# File:          dzr.tweek.mk,v
# Date modified: 2003/11/27 05:35:26
# Version:       1.6
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# ----------------------------------------------------------------------------
# Basic variable settings for code that depends on Tweek.  Only options
# relevant to Tweek go here.
#
# dzr.tweek.mk,v 1.6 2003/11/27 05:35:26 patrickh Exp
# ----------------------------------------------------------------------------

ifndef APP_ABI
ifeq ($(OS_ABI), n32)
   APP_ABI=	N32
else
   ifeq ($(OS_ABI), 64)
      APP_ABI=	64
   endif
endif
endif

# The name of the command to run to get the info for Tweek.
ifdef TWEEK_BASE_DIR
   TWEEK_CONFIG=	$(TWEEK_BASE_DIR)/bin/tweek-config
else
   TWEEK_CONFIG=	tweek-config
endif

TWEEK_PREFIX=		$(shell $(TWEEK_CONFIG) --prefix)
TWEEK_VERSION=		$(shell $(TWEEK_CONFIG) --version)
TWEEK_HAVE_CXX=		$(shell $(TWEEK_CONFIG) --has-cxx)

ifeq ($(TWEEK_HAVE_CXX), Y)
TWEEK_CXX_FLAGS=	$(shell $(TWEEK_CONFIG) --cxxflags $(APP_ABI))
TWEEK_INCLUDES=		$(shell $(TWEEK_CONFIG) --includes)
TWEEK_LIBS=		$(shell $(TWEEK_CONFIG) --libs $(APP_ABI))
TWEEK_EXTRA_LIBS=	$(shell $(TWEEK_CONFIG) --extra-libs $(APP_ABI))

TWEEK_CXX_FLAGS_MIN=	$(shell $(TWEEK_CONFIG) --cxxflags $(APP_ABI) --min)
TWEEK_INCLUDES_MIN=	$(shell $(TWEEK_CONFIG) --includes $(APP_ABI) --min)
TWEEK_LIBS_MIN=		$(shell $(TWEEK_CONFIG) --libs $(APP_ABI) --min)
TWEEK_EXTRA_LIBS_MIN=	$(shell $(TWEEK_CONFIG) --extra-libs $(APP_ABI) --min)

TWEEK_IDL_CXX=		$(shell $(TWEEK_CONFIG) --idl cxx)
TWEEK_IDLFLAGS_CXX=	$(shell $(TWEEK_CONFIG) --idlflags cxx)
TWEEK_IDL_INCFLAG_CXX=	$(shell $(TWEEK_CONFIG) --idlincflag cxx)
TWEEK_IDL_GENDIR_CXX=	$(shell $(TWEEK_CONFIG) --idlgendir cxx)
endif

TWEEK_HAVE_JAVA=	$(shell $(TWEEK_CONFIG) --has-java)

ifeq ($(TWEEK_HAVE_JAVA), Y)
TWEEK_JARS=		$(shell $(TWEEK_CONFIG) --jars)
TWEEK_EXT_JARS=		$(shell $(TWEEK_CONFIG) --ext-jars)

TWEEK_IDL_JAVA=		$(shell $(TWEEK_CONFIG) --idl java)
TWEEK_IDLFLAGS_JAVA=	$(shell $(TWEEK_CONFIG) --idlflags java)
TWEEK_IDL_INCFLAG_JAVA=	$(shell $(TWEEK_CONFIG) --idlincflag java)
TWEEK_IDL_GENDIR_JAVA=	$(shell $(TWEEK_CONFIG) --idlgendir java)
endif

TWEEK_HAVE_PYTHON=	$(shell $(TWEEK_CONFIG) --has-python)

ifeq ($(TWEEK_HAVE_PYTHON), Y)
TWEEK_IDL_PYTHON=		$(shell $(TWEEK_CONFIG) --idl python)
TWEEK_IDLFLAGS_PYTHON=		$(shell $(TWEEK_CONFIG) --idlflags python)
TWEEK_IDL_INCFLAG_PYTHON=	$(shell $(TWEEK_CONFIG) --idlincflag python)
TWEEK_IDL_GENDIR_PYTHON=	$(shell $(TWEEK_CONFIG) --idlgendir python)
endif

define TWEEK_echo_vars
@echo "TWEEK settings"
@echo "TWEEK_CONFIG: $(TWEEK_CONFIG)"
@echo "TWEEK_CXX_FLAGS: $(TWEEK_CXX_FLAGS)"
@echo "TWEEK_INCLUDES: $(TWEEK_INCLUDES)"
@echo "TWEEK_LIBS: $(TWEEK_LIBS)"
@echo "TWEEK_EXTRA_LIBS: $(TWEEK_EXTRA_LIBS)"
@echo "TWEEK_CXX_FLAGS_MIN: $(TWEEK_CXX_FLAGS_MIN)"
@echo "TWEEK_INCLUDES_MIN: $(TWEEK_INCLUDES_MIN)"
@echo "TWEEK_LIBS_MIN: $(TWEEK_LIBS_MIN)"
@echo "TWEEK_EXTRA_LIBS_MIN: $(TWEEK_EXTRA_LIBS_MIN)"
@echo "TWEEK_VERSION: $(TWEEK_VERSION)"
endef
