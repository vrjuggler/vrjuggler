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

CXX=		c++
CSC=		mcs /debug
LD=		c++ -shared

CXXFLAGS=	$(shell vpr-config --cxxflags)
LIBDIR=		../../../src/lib
LDFLAGS=	$(shell vpr-config --libs) $(shell vpr-config --extra-libs)
DSO_NAME=	handle_obj
EXE_NAME=	tester
EXE_DEPS=	/r:$(LIBDIR)/vprnet.dll

CXX_SRCS=	handle_obj.cpp
CS_SRCS=	tester.cs

CXX_OBJS=	$(CXX_SRCS:.cpp=.o)

DSO=		lib$(DSO_NAME).so
EXE=		$(EXE_NAME).exe

%.o: %.cpp
	$(CXX) -Wall -W $(CXXFLAGS) $(INCUDES) -c $<

all: $(DSO) $(EXE)

$(DSO): $(CXX_OBJS)
	$(LD) -o $@ $(CXX_OBJS) $(LDFLAGS)

$(EXE): $(CS_SRCS)
	$(CSC) $(CS_SRCS) $(EXE_DEPS)

clean:
	rm -f $(CXX_OBJS)

clobber:
	rm -f $(DSO) $(EXE)
