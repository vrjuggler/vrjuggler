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

OBJS=	$(SRCS:.cpp=.o)
DLL=	$(LIBDIR)/lib$(MOD_NAME)_bridge.so

all: $(DLL)

%.o: %.cpp
	c++ -Wall -W $(CXXFLAGS) $(INCLUDES) -c $<

$(DLL): $(OBJS)
	c++ -shared -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS)

clobber:
	$(MAKE) clean
	rm -f $(DLL)
