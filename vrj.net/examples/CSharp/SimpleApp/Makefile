# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

default: all

CSC=		mcs /debug

LIBDIR=		../../../src/lib
EXE_NAME=	SimpleApp
EXE_DEPS=	/r:$(LIBDIR)/vprnet.dll		\
		/r:$(LIBDIR)/jcclnet.dll	\
		/r:$(LIBDIR)/gmtlnet.dll	\
		/r:$(LIBDIR)/gadgetnet.dll	\
		/r:$(LIBDIR)/vrjnet.dll		\
		/r:$(LIBDIR)/Tao.OpenGl.dll

CS_SRCS=	AssemblyInfo.cs SimpleApp.cs

EXE=		$(EXE_NAME).exe

all: $(EXE)

$(EXE): $(CS_SRCS)
	$(CSC) $(CS_SRCS) $(EXE_DEPS)

clobber:
	rm -f $(EXE)
