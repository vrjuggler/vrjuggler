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
# File:          dzr.cl.net.mk,v
# Date modified: 2002/12/16 17:51:02
# Version:       1.3
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Win32-specfic settings for variables such as the C and C++ compilers and any
# platform-specfic options needed by various commands.
#
# dzr.cl.net.mk,v 1.3 2002/12/16 17:51:02 patrickh Exp
# -----------------------------------------------------------------------------
# This file sets values for the following variables:
#
# CC              - The C compiler (MS Visual C++).
# CXX             - The C++ compiler (MS Visual C++).
# AR              - The archiver (ar(1) on Win32).
# LD              - The linker ($(CXX) on Win32).
# UMASK           - The umask(1) program.
# UMASK_VAL       - A value for the umask on files that are created (object
#                   files, dependency files, etc.).
# OS_ABI          - Default Application Binary Interface for Win32.
# OS_ISA          - Default Instruction Set Architecture for Win32.
# OS_ABI_FLAGS    - ABI flags for Win32 object files, libraries and binaries.
# OS_CFLAGS       - Win32-specific C compiler options.
# OS_CXXFLAGS     - Win32-specific C++ compiler options.
# OS_DEBUG_FLAGS  - Win32-speicfic compiler debugging flags.
# OS_ARFLAGS      - Win32-specific archiver options.
# OS_LDFLAGS      - Win32-specific linker options.
# OS_INCLUDES     - Win32-specific include paths.
# OS_OPTIM_FLAGS  - Win32-specific compiler optimization flags.
# OS_SHLIB_FLAGS  - Win32-specific compiler flag to create a shared library.
# OS_LIBS_BEGIN   - Win32-specific options that need to go at the start of the
#                   linker's list of libraries and associated options.
# OS_LIBS_END     - Win32-specific options that need to go at the end of the
#                   linker's list of libraries and associated options.
# OS_STATIC_BEGIN - Beginning of libraries that are linked statically.
# OS_STATIC_END   - End of libraries that are linked statically.
# OS_SYS_LIBS     - Any extra Win32-specific system libraries that are needed.
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
# -----------------------------------------------------------------------------

# Use Visual C++ as the compiler and linker.
CC=		cl /nologo
CXX=		cl /nologo
AR=		lib /nologo
LD=		link /nologo
UMASK=		
UMASK_VAL=	

OBJ_NAME_FLAG=	/Fo$@
OBJ_BUILD_FLAG=	/c
USE_MAKEDEPEND=	1

# Basic compiler flags.
OS_ABI?=	WIN32
OS_ISA?=	i686
OS_ABI_FLAGS=	
OS_CFLAGS=	
OS_CXXFLAGS=	/GR /D_WIN32 /DWIN32
OS_DEBUG_FLAGS=	
OS_OPTIM_FLAGS=	/O2
OS_DEBUG_LINK_FLAGS=	/debug
OS_OPTIM_LINK_FLAGS=	/release
OS_ARFLAGS=	/nologo
OS_LDFLAGS=	/nologo
OS_INCLUDES=	

OS_LIB_FLAGS=	
OS_SHLIB_FLAGS=	/dll
OS_LD_NAME_FLAG=	/OUT:
OS_AR_NAME_FLAG=	/OUT:

# the flag to specify a library path.
OS_LIBPATH_FLAG= /libpath:

# the flag to pass linker args to the compiler
OS_COMPILER_LINKER_ARG= /link

# File extensions.
OS_LIB_EXT=	.lib
OS_SHLIB_EXT=	.dll
OS_LIB_PREFIX=	
OS_SHLIB_PREFIX=	
OS_EXE_EXT=	.exe
OS_OBJ_EXT=	.obj


# Wrapper options around the list of libraries needed at link time.  These
# are to be put at the start and end of $(LIBS) respectively when it is set.
OS_LIBS_BEGIN=	
OS_LIBS_END=	

# Wrapper options for doing static linking of libraries.
OS_STATIC_BEGIN=	
OS_STATIC_END=		

# Extra library options.
OS_LIBBITSUF=	
OS_DEPENDFLAGS=	-D_M_IX86 -D_WIN32 -DWIN32 -D__cplusplus
