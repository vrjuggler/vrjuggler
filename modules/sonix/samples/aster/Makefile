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
# *************** <auto-copyright.pl END do not edit this line> ***************

# =============================================================================
# Basic template makefile for applications.
# $Id$
#
# *** IMPORTANT NOTES -- READ THESE FIRST ***
#     1) This file _requires_ the use of GNU make (gmake).
# =============================================================================

# Nothing but comments should come before this line.
default: all

# -----------------------------------------------------------------------------
# Application-specific variable settings.  It is safe to modify these.
# -----------------------------------------------------------------------------

# The application name that is built.  A value must be set for this!
APP_NAME=	aster$(EXE_EXT)

# The directory where the source files for this application are located.
srcdir=  	.

# Set a langauge for compiling.  This is optional and will default to C++ if
# nothing is specified.  Currently supported langauges are C and C++.
#LANGUAGE=	

# Compiler flags needed for this application.  They are used as follows:
#
#    DEBUG_APP         - If set to TRUE, this application will be compiled with
#                        debugging symbols and no optimized symbols
#    OPTIM_APP         - If set to TRUE, this application will be compiled with
#                        optimized symbols and no debugging symbols
#    EXTRA_CFLAGS      - Extra application-specific C compiler flags
#    EXTRA_CXXFLAGS    - Extra application-specific C++ compiler flags
#    EXTRA_DEBUG_FLAGS - Extra application-specific compiler debugging flags
#    EXTRA_INCLUDES    - Extra include paths (-I... options) for the compilers
#    EXTRA_OPTIM_FLAGS - Extra application-specific compiler optimization flags
#    EXTRA_UI_LIBS     - Extra user interface libraries needed for linking.
#
DEBUG_APP=		TRUE
OPTIM_APP=		FALSE
EXTRA_CFLAGS=		
EXTRA_CXXFLAGS=		
EXTRA_DEBUG_FLAGS=	
EXTRA_INCLUDES=		-I. -I$(SNX_BASE_DIR)/include
EXTRA_OPTIM_FLAGS=	
EXTRA_UI_FLAGS=		

# The list of all source files needed for this application.  Based on this,
# a list of object files is put in $(OBJS) automatically.
SRCS=			aster.cpp cubeGeometry.cpp

# This is for extra libraries needed specfically for your application.  The
# general libraries needed for applications are in $(LIBS).
EXTRA_LIBS=		

# This is for extra system libraries (-lpthread for example) needed
# specifically for this application.  These are added to the linker command
# after all the basic libraries and all the system libraries.
EXTRA_SYS_LIBS=		

# Extend this as necessary to find source files that are not in the current
# directory.  Set EXTRA_PATH_FOR_SOURCES to all the directories that you
# have sources in. (current dir is already included by default)
EXTRA_PATH_FOR_SOURCES=	

# Additional files and directories besides the standard ones that need to be
# removed by the 'clean' target.
CLEAN_FILES=		
CLEAN_DIRS=		

# The following include line MUST COME BEFORE the targets for compiling the
# application.
include $(VJ_BASE_DIR)/share/vrjuggler/vrj.appdefs.mk

# IRIX
ifeq ($(HOSTTYPE), IRIX)
   EXTRA_LIBS += -L/usr/X11R6/lib -lGLU -lGL -lglut -lX11 -lXmu -lm -lpthread -L$(SNX_BASE_DIR)/lib $(LINKALL_BEGIN) -lsonix $(LINKALL_END)
endif

#LiNUX
ifeq ($(HOSTTYPE), Linux)
   EXTRA_LIBS += -lGLU -lGL -lglut -lX11 -lXmu -lm -lpthread -L$(SNX_BASE_DIR)/lib $(LINKALL_BEGIN) -lsonix $(LINKALL_END) -lopenal
endif

# Win32
ifeq ($(HOSTTYPE), Win32)
   EXTRA_LIBS += 
   EXTRA_CXXFLAGS = -D__cplusplus -D_WIN32 -DWIN32 $(INCLUDE_PATHS)
endif

# -----------------------------------------------------------------------------
# Targets.
# -----------------------------------------------------------------------------
all: $(APP_NAME)

# Target for the application to be built.
$(APP_NAME): $(OBJS)
	$(LINK) $(LINK_OUT)$@ $(OBJS) $(EXTRA_LIBS) $(LIBS)
