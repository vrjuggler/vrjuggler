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
# File:          dzr.vrjuggler.pfapp.mk,v
# Date modified: 2003/10/03 12:45:32
# Version:       1.7
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Basic configuration for compiling a VR Juggler Performer application.  This
# is one of the application-generating mk files that can be included by a user
# makefile.
#
# dzr.vrjuggler.pfapp.mk,v 1.7 2003/10/03 12:45:32 patrickh Exp
# -----------------------------------------------------------------------------

ifndef APP_ABI
ifeq ($(OS_ABI), n32)
   APP_ABI=	N32
else
   ifeq ($(OS_ABI), 64)
      APP_ABI=	64
   endif
endif
endif

VRJ_LIBS+=		$(shell $(VRJ_CONFIG) vrj_pf --libs $(APP_ABI))
VRJ_LIBS_MIN+=		$(shell $(VRJ_CONFIG) vrj_pf --libs $(APP_ABI) --min)
VRJ_EXTRA_LIBS+=	$(shell $(VRJ_CONFIG) vrj_pf --extra-libs $(APP_ABI))
VRJ_EXTRA_LIBS_STATIC+=	$(shell $(VRJ_CONFIG) vrj_pf --extra-libs $(APP_ABI) --static)
VRJ_EXTRA_LIBS_MIN+=	$(shell $(VRJ_CONFIG) vrj_pf --extra-libs $(APP_ABI) --min)
VRJ_EXTRA_LIBS_MIN_STATIC+=	$(shell $(VRJ_CONFIG) vrj_pf --extra-libs $(APP_ABI) --min --static)

# VR Juggler 1.0 settings.
VJ_LIBS+=	-lJuggler_pf
