# ************** <auto-copyright.pl BEGIN do not edit this line> **************
# Doozer++
#
# Original Author:
#   Patrick Hartling
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
# File:          dpp.prog.mk,v
# Date modified: 2001/06/29 23:47:52
# Version:       1.3
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# =============================================================================
# dpp.prog.mk,v 1.3 2001/06/29 23:47:52 patrickh Exp
#
# This file <dpp.prog.mk> is intended to be used by makefiles that need to
# build programs.  It provides some handy targets and includes other .mk
# files that are typically necessary for this process.
# -----------------------------------------------------------------------------
# The targets defined here all invoke the 'all' target in the includning
# makefile.  They are as follows:
#
# static        - Build a statically linked binary with debugging symbols.
# dynamic       - Build a dynamically linked binary with debugging symbols.
# static_optim  - Build a statically linked optimized binary.
# dynamic_optim - Build a dynamically linked optimized binary.
#
# These targts pass values for the following variables:
#
# LDFLAGS_BEGIN - The linker flags needed prior to listing the libraries to
#                 link.
# LDFLAGS_END   - The linker flags needed after listing the libraries to
#                 link.  This is typically empty.
# LIBS          - The libraries to link.
# OPTIMIZER     - The optimizer flags for the compiler.  This will either be
#                 debugging options or optimization options.
# =============================================================================

static:
	@$(MAKE) LDFLAGS_BEGIN="$(LDFLAGS) $(LDFLAGS_STATIC)"		\
          LDFLAGS_END="$(LDFLAGS_DYNAMIC)" OPTIMIZER="$(DBG_FLAGS)"	\
          LIBS="$(LIBS) $(STATIC_LIBS)" all

dynamic:
	@$(MAKE) LDFLAGS_BEGIN="$(LDFLAGS)" OPTIMIZER="$(DBG_FLAGS)" all

static_optim:
	@$(MAKE) LDFLAGS_BEGIN="$(LDFLAGS) $(LDFLAGS_STATIC)"		\
          LDFLAGS_END="$(LDFLAGS_DYNAMIC)"  OPTIMIZER="$(OPT_FLAGS)"	\
          LIBS="$(LIBS) $(STATIC_LIBS)" all

dynamic_optim:
	@$(MAKE) LDFLAGS_BEGIN="$(LDFLAGS)" OPTIMIZER="$(OPT_FLAGS)" all

include $(MKPATH)/dpp.compile.mk
include $(MKPATH)/dpp.dep.mk
include $(MKPATH)/dpp.idl.mk
include $(MKPATH)/dpp.clean.mk
