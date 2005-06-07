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
# File:          dzr.freebsd.mk,v
# Date modified: 2002/09/22 23:09:42
# Version:       1.7
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# FreeBSD-specfic settings for variables such as the C and C++ compilers and
# any platform-specfic options needed by various commands.
#
# dzr.freebsd.mk,v 1.7 2002/09/22 23:09:42 patrickh Exp
# -----------------------------------------------------------------------------
# This file sets values for the following variables:
#
# UMASK           - The umask(1) program.
# UMASK_VAL       - A value for the umask on files that are created (object
#                   files, dependency files, etc.).
# OS_ABI          - Default Application Binary Interface for FreeBSD.
# OS_ISA          - Default Instruction Set Architecture for FreeBSD.
# OS_ABI_FLAGS    - ABI flags for FreeBSD object files, libraries and binaries.
# OS_LIBS_BEGIN   - FreeBSD-specific options that need to go at the start of
#                   the linker's list of libraries and associated options.
# OS_LIBS_END     - FreeBSD-specific options that need to go at the end of the
#                   linker's list of libraries and associated options.
# OS_SYS_LIBS     - Any extra FreeBSD-specific system libraries that are
#                   needed.
# OS_LIBBITSUF    - The suffix on a library directory specifying the bit width
#                   of the contained libraries.
#
# This file may extend values for the following variables:
#
# OS_CFLAGS       - FreeBSD-specific C compiler options.
# OS_CXXFLAGS     - FreeBSD-specific C++ compiler options.
# OS_DEBUG_FLAGS  - FreeBSD-speicfic compiler debugging flags.
# OS_ARFLAGS      - FreeBSD-specific archiver options.
# OS_LDFLAGS      - FreeBSD-specific linker options.
# OS_INCLUDES     - FreeBSD-specific include paths.
# OS_OPTIM_FLAGS  - FreeBSD-specific compiler optimization flags.
# -----------------------------------------------------------------------------

include $(DZR_BASE_DIR)/mk/dzr.gcc.mk

UMASK=		umask
UMASK_VAL=	002

OS_ABI?=	elf
OS_ISA?=	i386
#OS_ABI_FLAGS=	-$(OS_ABI)

# Extensions to the basic compiler flags.
OS_CFLAGS+=		
OS_CXXFLAGS+=		
OS_DEBUG_FLAGS+=	
OS_ARFLAGS+=		
OS_LDFLAGS+=		
OS_INCLUDES+=		
OS_OPTIM_FLAGS+=	

# Wrapper options around the list of libraries needed at link time.  These
# are to be put at the start and end of $(LIBS) respectively when it is set.
OS_LIBS_BEGIN=	
OS_LIBS_END=	

# Basic libraries needed by various types of applications.
OS_SYS_LIBS=	

# Extra library options.
OS_LIBBITSUF=	
