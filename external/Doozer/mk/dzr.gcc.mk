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
# File:          dzr.gcc.mk,v
# Date modified: 2005/02/24 15:41:29
# Version:       1.17
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# GCC-specfic settings for variables such as the C and C++ compilers and any
# compiler-specfic options needed by various commands.
#
# dzr.gcc.mk,v 1.17 2005/02/24 15:41:29 patrickh Exp
# -----------------------------------------------------------------------------
# This file sets values for the following variables:
#
# CC              - The C compiler (GCC).
# CXX             - The C++ compiler (GCC).
# AR              - The archiver.
# LD              - The linker.
# OS_CFLAGS       - GCC-specific C compiler options.
# OS_CXXFLAGS     - GCC-specific C++ compiler options.
# OS_DEBUG_FLAGS  - GCC-speicfic compiler debugging flags.
# OS_ARFLAGS      - GCC-specific archiver options.
# OS_LDFLAGS      - GCC-specific linker options.
# OS_INCLUDES     - GCC-specific include paths.
# OS_OPTIM_FLAGS  - GCC-specific compiler optimization flags.
# OS_SHLIB_FLAGS  - GCC-specific compiler flag to create a shared library.
# OS_STATIC_BEGIN - Beginning of libraries that are linked statically.
# OS_STATIC_END   - End of libraries that are linked statically.
# OS_LD_NAME_FLAG - flag to name your outputfile for your linker
# OS_AR_NAME_FLAG - flag to name your outputfile for your archiver
# OS_LIB_EXT      - file extension (.ext) that your platform's libraries have
# OS_SHLIB_EXT    - file extension that your platform's shared libraries have
# OS_LIB_PREFIX   - file prefix (lib) that your platform's libraries have
# OS_SHLIB_PREFIX - file prefix (lib) that your platform's shr libraries have
# OS_EXE_EXT      - file extension that your platform's executables have
# OS_OBJ_EXT      - file extension that your compiler's object files have
# OS_LIBPATH_FLAG - the linker flag to specify a library path
# OS_COMPILER_LINKER_ARG  - the flag to pass linker args to the compiler   
# -----------------------------------------------------------------------------

# Use GCC as the compiler and linker.
CC?=		gcc
CXX?=		g++
F77?=		g77
AR?=		ar

ifdef LANGUAGE
   ifeq ($(LANGUAGE), C)
      LD=	$(CC)
   else
      LD=	$(CXX)
   endif
else
   LD=		$(CXX)
endif

# Basic compiler flags.
OS_CFLAGS=		
OS_CXXFLAGS=		
OS_F77FLAGS=
OS_ARFLAGS=		-ruv
OS_LDFLAGS=		
OS_INCLUDES=		
OS_DEBUG_FLAGS=		-g
OS_OPTIM_FLAGS=		-O
OS_DEBUG_LINK_FLAGS=	-g
OS_OPTIM_LINK_FLAGS=	-O
OS_SHLIB_FLAGS=		-shared
OS_LD_NAME_FLAG=	-o 
OS_AR_NAME_FLAG=	

# File extensions.
OS_LIB_EXT=		.a
OS_SHLIB_EXT=		.so
OS_LIB_PREFIX=		lib
OS_SHLIB_PREFIX=	lib
OS_EXE_EXT=		
OS_OBJ_EXT=		.o

# Wrapper options for doing static linking of libraries.
OS_STATIC_BEGIN=	-Wl,-Bstatic
OS_STATIC_END=		-Wl,-Bdynamic
OS_LINKALL_BEGIN=	-Wl,--whole-archive
OS_LINKALL_END=		-Wl,--no-whole-archive

# the flag to specify a library path.
OS_LIBPATH_FLAG= -L

# the flag to pass linker args to the compiler 
OS_COMPILER_LINKER_ARG= -Wl,

