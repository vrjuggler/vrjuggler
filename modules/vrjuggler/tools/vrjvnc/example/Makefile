# ********** <VRJ VNC auto-copyright.pl BEGIN do not edit this line> **********
#
# VRJ VNC is (C) Copyright 2003-2005 by Iowa State University
#
# Original Authors:
#   Patrick Hartling, Allen Bierbaum
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this application; if not, write to the Free
# Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
# MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *********** <VRJ VNC auto-copyright.pl END do not edit this line> ***********

# -----------------------------------------------------------------------------
# Makefile for vrjuggler/tools/vrjvnc/example.  This requires GNU make.
# -----------------------------------------------------------------------------

# XXX: This is kind of bad.  What if Doozer didn't come with VR Juggler?
DZR_BASE_DIR?=	$(VJ_BASE_DIR)/share/Doozer

include $(DZR_BASE_DIR)/mk/dzr.hosttype.mk
include $(DZR_BASE_DIR)/ext/vpr/dzr.vpr.mk

SYS_SETTING=	$(DZR_HOSTTYPE)-$(VPR_SUBSYSTEM)

APP_NAME=	vncapp-$(SYS_SETTING)$(OS_EXE_EXT)

all: $(APP_NAME)

# Basic options.
srcdir=		.
EXTRA_INCLUDES=	-I..

SRCS=		d3des.c			\
		vncauth.c		\
		VNCDesktop.cpp		\
		VNCInterface.cpp	\
		main.cpp		\
		vncApp.cpp

EXTRA_PATH_FOR_SOURCES=	$(srcdir)/..

OBJDIR=	$(SYS_SETTING).obj
DEPDIR=	$(SYS_SETTING).obj

include $(VJ_BASE_DIR)/share/vrjuggler/vrj.appdefs.mk
include $(DZR_BASE_DIR)/ext/vrjuggler/dzr.vrjuggler.glapp.mk

# -----------------------------------------------------------------------------
# Application build targets.
# -----------------------------------------------------------------------------
$(APP_NAME): $(OBJS)
	$(LINK) $(LINK_OUT)$@ $(OBJS) $(EXTRA_LIBS) $(LIBS)
