# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# This include file <vj.lib.mk> handles compiling of libraries (both static
# and shared) from compiled object files.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variables:
#
# AR      - Path to ar(1) (or equivalent program).
# LD      - Path to ld(1) (or equivalent program).
# LDOPTS  - Extra, often platform-specific, options for the linker.
# LIBDIR  - The directory where the library will be made.
# LIBNAME - The name of the library to generate.
# OBJS    - The object files to be compiled into the named library.
# RANLIB  - Path to ranlib(1) if available.  ':' otherwise.
# -----------------------------------------------------------------------------

static-lib:
	${AR} ${LIBDIR}/${LIBNAME} ${OBJS}
	${RANLIB} ${LIBDIR}/${LIBNAME}

shared-lib:
	${LD} ${LDOPTS} -o ${LIBDIR}/${LIBNAME} ${OBJS}
