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
# File:          dzr.hosttype.mk,v
# Date modified: 2003/01/10 06:06:05
# Version:       1.15
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Simple mk file for determining the host platform name.
#
# dzr.hosttype.mk,v 1.15 2003/01/10 06:06:05 subatomic Exp
# -----------------------------------------------------------------------------
# The following variable is set here:
#
# HOSTTYPE - The name of the host platform (e.g., IRIX, Linux, etc.).
# COMPILERCONFIG - the name of the compiler config file to use (fullpath).
# COMPILER_TYPE - the name of the compiler to use (gcc, CC, cl.net, etc.)
# -----------------------------------------------------------------------------

DZR_HOSTTYPE:=	$(shell uname)

# Based on the value of $(DZR_HOSTTYPE), include platform-specific settings.
ifeq ($(findstring IRIX, $(DZR_HOSTTYPE)), IRIX)
   COMPILER_TYPE?=	mipspro
   HOSTTYPE=		IRIX
endif
ifeq ($(DZR_HOSTTYPE), Linux)
   COMPILER_TYPE?=	linux
   HOSTTYPE=		Linux
endif
ifeq ($(DZR_HOSTTYPE), FreeBSD)
   COMPILER_TYPE?=	freebsd
   HOSTTYPE=		FreeBSD
endif
ifeq ($(DZR_HOSTTYPE), Darwin)
   COMPILER_TYPE?=	darwin
   HOSTTYPE=		Darwin
endif
ifeq ($(DZR_HOSTTYPE), SunOS)
   COMPILER_TYPE?=	solaris
   HOSTTYPE=		Solaris
endif
ifeq ($(findstring WIN, $(DZR_HOSTTYPE)), WIN)
   COMPILER_TYPE?=	cl.net
   HOSTTYPE=		Win32
endif

HOSTTYPE?=	Unknown

# Define the platform-specific Java classpath/directory separator.
ifeq ($(HOSTTYPE), Win32)
   JCPS=	;
else
   JCPS=	:
endif

COMPILERCONFIG?=	$(DZR_BASE_DIR)/mk/dzr.$(COMPILER_TYPE).mk

# Include the compiler config file.
ifneq ($(COMPILERCONFIG),'')
   include $(COMPILERCONFIG)
endif
