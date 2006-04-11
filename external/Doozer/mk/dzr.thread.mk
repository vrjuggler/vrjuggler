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
# File:          dzr.thread.mk,v
# Date modified: 2002/05/07 21:04:21
# Version:       1.3
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# 
#
# dzr.thread.mk,v 1.3 2002/05/07 21:04:21 subatomic Exp
# -----------------------------------------------------------------------------
# The following variables are set here:
#
# THREAD_LDFLAGS
# THREAD_LIBS
# -----------------------------------------------------------------------------

include $(DZR_BASE_DIR)/mk/dzr.hosttype.mk

ifeq ($(THREAD_USE_TYPE), NSPR)
   THREAD_LDFLAGS?=	

ifeq ($(HOSTTYPE), Win32)
   THREAD_LIBS?=	libnspr$(NSPR_VERISON).lib
else
   THREAD_LIBS?=	-lnspr$(NSPR_VERISON)
endif
endif

ifeq ($(THREAD_USE_TYPE), SPROC)
endif

ifeq ($(THREAD_USE_TYPE), POSIX)
ifeq ($(HOSTTYPE), Solaris)
   ifeq ($(CC), gcc)
      THREAD_LIBS?=	-pthreads
   endif
endif
ifeq ($(HOSTTYPE), FreeBSD)
   THREAD_LIBS?=	-pthread
endif
ifeq ($(HOSTTYPE), Linux)
   THREAD_LIBS?=	-pthread
endif
ifeq ($(HOSTTYPE), IRIX)
   THREAD_LIBS?=	-lpthread
endif
endif
