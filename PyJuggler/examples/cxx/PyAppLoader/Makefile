# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# PyJuggler is (C) Copyright 2002 by Patrick Hartling
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

srcdir=		.
prefix=		/usr/local

PYTHON_ROOT=	/usr/local
PYTHON_VERSION=	2.2

CXX=		c++
CXXFLAGS=	$(shell vrjuggler-config --cxxflags)			\
		-I$(PYTHON_ROOT)/include/python$(PYTHON_VERSION)	\
		$(DEBUG_FLAGS)
DEBUG_FLAGS=	-g -D_DEBUG
LDFLAGS=	-Wl,--export-dynamic
LIBS=		$(shell vrjuggler-config --libs)
EXTRA_LIBS=	$(shell vrjuggler-config --extra-libs)

BOOST_PYTHON_LIBDIR=	/home/patrick/src/Juggler/main/juggler/external/boost/libs/python/build/bin/libboost_python.so/gcc/release/inlining-on/runtime-link-dynamic/shared-linkable-true
BOOST_PYTHON_LIBS=	-L$(BOOST_PYTHON_LIBDIR) -lboost_python		\
			-L$(PYTHON_ROOT)/lib/python$(PYTHON_VERSION)/config \
			-lpython$(PYTHON_VERSION) -lutil

SRCS=		main.cpp
OBJS=		$(SRCS:.cpp=.o)
APP_NAME=	pyApp

vpath %.cpp $(srcdir)

all: $(APP_NAME)

$(APP_NAME): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(BOOST_PYTHON_LIBS) $(LIBS) $(EXTRA_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS)

clobber:
	$(MAKE) clean
	rm -f $(APP_NAME)
