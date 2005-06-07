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
# File:          dzr.mipspro.mk,v
# Date modified: 2003/04/30 13:44:21
# Version:       1.2
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# IRIX-specfic settings for variables such as the C and C++ compilers and any
# platform-specfic options needed by various commands.
#
# dzr.mipspro.mk,v 1.2 2003/04/30 13:44:21 patrickh Exp
# -----------------------------------------------------------------------------
# This file sets values for the following variables:
#
# CC              - The C compiler (MIPSpro Compilers).
# CXX             - The C++ compiler (MIPSpro Compilers).
# AR              - The archiver ($(CXX) on IRIX).
# LD              - The linker ($(CXX) on IRIX).
# UMASK           - The umask(1) program.
# UMASK_VAL       - A value for the umask on files that are created (object
#                   files, dependency files, etc.).
# OS_ABI          - Default Application Binary Interface for IRIX.
# OS_ISA          - Default Instruction Set Architecture for IRIX.
# OS_ABI_FLAGS    - ABI flags for IRIX object files, libraries and binaries.
#                   This is constructed from $(OS_ABI) and $(OS_ISA).
# OS_CFLAGS       - IRIX-specific C compiler options.
# OS_CXXFLAGS     - IRIX-specific C++ compiler options.
# OS_DEBUG_FLAGS  - IRIX-speicfic compiler debugging flags.
# OS_ARFLAGS      - IRIX-specific archiver options.
# OS_LDFLAGS      - IRIX-specific linker options.
# OS_INCLUDES     - IRIX-specific include paths.
# OS_OPTIM_FLAGS  - IRIX-specific compiler optimization flags.
# OS_SHLIB_FLAGS  - IRIX-specific compiler flag to create a shared library.
# OS_LIBS_BEGIN   - IRIX-specific options that need to go at the start of the
#                   linker's list of libraries and associated options.
# OS_LIBS_END     - IRIX-specific options that need to go at the end of the
#                   linker's list of libraries and associated options.
# OS_STATIC_BEGIN - Beginning of libraries that are linked statically.
# OS_STATIC_END   - End of libraries that are linked statically.
# OS_SYS_LIBS     - Any extra IRIX-specific system libraries that are needed.
# OS_LIBBITSUF    - The suffix on a library directory specifying the bit width
#                   of the contained libraries.
# OS_LD_NAME_FLAG - flag to name your outputfile for your linker
# OS_AR_NAME_FLAG - flag to name your outputfile for your archiver
# OS_LIB_EXT      - file extension (.ext) that your platform's libraries have
# OS_SHLIB_EXT    - file extension that your platform's shared libraries have
# OS_LIB_PREFIX   - file prefix (lib) that your platform's libraries have
# OS_SHLIB_PREFIX - file prefix (lib) that your platform's shr libraries have
# OS_EXE_EXT      - file extension that your platform's executables have
# OS_OBJ_EXT      - file extension that your compiler's object files have
# OS_LD_LIBPATH_FLAG - the linker flag to specify a library path
# OS_CC_LIBPATH_FLAG - the compiler flag to specify a library path
# OS_COMPILER_LINKER_ARG  - the flag to pass linker args to the compiler   
#-----------------------------------------------------------------------------

# The MIPSpro C compiler, C++ compiler and linker.
CC=		cc
CXX=		CC
AR=		$(CXX)

ifdef LANGUAGE
   ifeq ($(LANGUAGE), C)
      LD=	$(CC)
   else
      LD=	$(CXX)
   endif
else
   LD=		$(CXX)
endif

UMASK=		umask
UMASK_VAL=	002

# Basic compiler flags.
OS_ABI?=		n32
OS_ISA?=		mips3
OS_ABI_FLAGS=		-$(OS_ABI) -$(OS_ISA)
OS_CFLAGS=		
OS_CXXFLAGS=		-LANG:std
OS_DEBUG_FLAGS=		-w2 -g -gslim
OS_OPTIM_FLAGS=		-O
OS_DEBUG_LINK_FLAGS=	-w2 -g -gslim
OS_OPTIM_LINK_FLAGS=	-O
OS_ARFLAGS=		-ruv
ifeq ($(AR),CC)
   OS_ARFLAGS=		$(OS_ABI_FLAGS) -LANG:std -J6 -ar -WR,-v -o 
endif
OS_LDFLAGS=		$(OS_ABI_FLAGS) -LANG:std -J6 -all
OS_INCLUDES=		
OS_LIB_FLAGS=
OS_SHLIB_FLAGS=		-KPIC -shared
OS_LD_NAME_FLAG=	-o 
OS_AR_NAME_FLAG=	

# File extensions.
OS_LIB_EXT=		.a
OS_SHLIB_EXT=		.so
OS_LIB_PREFIX=		lib
OS_SHLIB_PREFIX=	lib
OS_EXE_EXT=		
OS_OBJ_EXT=		.o

# Wrapper options around the list of libraries needed at link time.  These
# are to be put at the start and end of $(LIBS) respectively when it is set.
OS_LIBS_BEGIN=	-B dynamic
OS_LIBS_END=	

# Wrapper options for doing static linking of libraries.
OS_STATIC_BEGIN=	-B static
OS_STATIC_END=	-B dynamic

# Basic libraries needed by various types of applications.
OS_SYS_LIBS=	

# Extra library options.
OS_LIBBITSUF?=	32
OS_LINKALL_BEGIN=	-all
OS_LINKALL_END=	


# the flag to specify a library path.
OS_LIBPATH_FLAG= -L

# the flag to pass linker args to the compiler 
OS_COMPILER_LINKER_ARG=

