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
# File:          dzr.basicapp.mk,v
# Date modified: 2002/05/08 00:12:08
# Version:       1.5
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Basic configuration for compiling an application.  This is one of the
# application-generating mk files that can be included by a user makefile.
#
# dzr.basicapp.mk,v 1.5 2002/05/08 00:12:08 subatomic Exp
# -----------------------------------------------------------------------------

include $(DZR_BASE_DIR)/mk/dzr.app.mk
include $(DZR_BASE_DIR)/mk/dzr.obj.mk

LIBS=	$(LIBS_BEGIN) $(SYS_LIBS) $(LIBS_END)

include $(DZR_BASE_DIR)/mk/dzr.clean.mk
include $(DZR_BASE_DIR)/mk/dzr.dep.mk
