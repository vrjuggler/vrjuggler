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
# File:          dzr.gl.mk,v
# Date modified: 2002/02/23 17:51:45
# Version:       1.2
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Basic configuration for compiling an OpenGL application.  This is one of the
# application-generating mk files that can be included by a user makefile.
#
# dzr.gl.mk,v 1.2 2002/02/23 17:51:45 patrickh Exp
# -----------------------------------------------------------------------------

include $(DZR_BASE_DIR)/mk/dzr.hosttype.mk

ifeq ($(GL_USE_MESA), YES)
   MESA=	Mesa
endif

ifeq ($(HOSTTYPE), IRIX)
GL_INCLUDES?=	
GL_LDFLAGS?=	
GL_LIBS= 	-lGLU -lGL
endif

ifeq ($(HOSTTYPE), Linux)
GL_INCLUDES?=	-I/usr/X11R6/include
GL_LDFLAGS?=	-L/usr/X11R6/lib
GL_LIBS= 	-l$(MESA)GLU -l$(MESA)GL
endif

ifeq ($(HOSTTYPE), FreeBSD)
GL_INCLUDES?=	-I/usr/X11R6/include
GL_LDFLAGS?=	-L/usr/X11R6/lib
GL_LIBS= 	-l$(MESA)GLU -l$(MESA)GL
endif

ifeq ($(HOSTTYPE), Solaris)
GL_INCLUDES?=	-I/usr/local/include
GL_LDFLAGS?=	-L/usr/local/lib
GL_LIBS= 	-l$(MESA)GLU -l$(MESA)GL
endif

ifeq ($(HOSTTYPE), Win32)
GL_INCLUDES?=	
GL_LDFLAGS?=	
GL_LIBS= 	opengl32.lib glu32.lib
endif
