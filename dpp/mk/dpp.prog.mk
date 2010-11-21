# Doozer++ is (C) Copyright 2000-2010 by Iowa State University
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
#
# Original Author:
#   Patrick Hartling

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
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.

# =============================================================================
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
