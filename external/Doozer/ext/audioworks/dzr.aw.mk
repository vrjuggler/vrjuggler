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
# File:          dzr.aw.mk,v
# Date modified: 2002/02/23 17:51:45
# Version:       1.2
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Add AudioWorks paths and libraries to an existing application setup.  This
# makes no assumptions about the type of application but does require that
# dzr.app.mk is included before it so that the AudioWorks variables are
# defined properly.
#
# dzr.aw.mk,v 1.2 2002/02/23 17:51:45 patrickh Exp
# -----------------------------------------------------------------------------
#
# AW_INCLUDES
# AW_LDFLAGS
# AW_LIBS
# -----------------------------------------------------------------------------

include $(DZR_BASE_DIR)/mk/dzr.hosttype.mk

AW_INCLUDES?=	-I/usr/include/PSI
AW_LDFLAGS?=	-L/usr/lib$(OS_LIBBITSUF)/PSI
AW_LIBS?=	-law -lawhwi -lpsi -laudiofile
