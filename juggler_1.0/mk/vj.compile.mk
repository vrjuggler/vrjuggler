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
# This include file <vj.compile.mk> handles source code compiling.
# -----------------------------------------------------------------------------
# The Makefile including this file should define the following variables:
#
# OBJDIR          - The directory where the object file will be created.
# C_COMPILE       - The C compiler command line (without -c or -o <outfile>
#                   options).
# CXX_COMPILE     - The C++ compiler command line (without -c or -o <outfile>
#                   options).
# OBJ_FILE_SUFFIX - Suffix for object file names (e.g., "o" or "obj").
# OBJ_NAME_FLAG   - Flag (with any extra options) used for naming the object
#                   file being compiled here.
# -----------------------------------------------------------------------------

${OBJDIR}/%.${OBJ_FILE_SUFFIX}: %.c
	${C_COMPILE} -c ${OBJ_NAME_FLAG} $<

${OBJDIR}/%.${OBJ_FILE_SUFFIX}: %.cpp
	${CXX_COMPILE} -c ${OBJ_NAME_FLAG} $<

${OBJDIR}/%.${OBJ_FILE_SUFFIX}: %.C
	${CXX_COMPILE} -c ${OBJ_NAME_FLAG} $<
