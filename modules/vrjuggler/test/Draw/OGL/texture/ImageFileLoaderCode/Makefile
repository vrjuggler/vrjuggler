#!gmake

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
# *************** <auto-copyright.pl END do not edit this line> ***************

CXX_COMPILE = CC -LANG:std
OBJS = TgaImporter.o SgiImporter.o FileHandler.o main.o ../Image.o
INCLUDE_PATHS = -I..
PATH_TO_SRCS = ..:.
PROGRAM_NAME = main

vpath
vpath %.cxx ${PATH_TO_SRCS}

all: $(OBJS)
	$(CXX_COMPILE) -o $(PROGRAM_NAME) $(OBJS) -lm
   
%.o: %.cpp
	$(CXX_COMPILE) $(INCLUDE_PATHS) $< -c -o $@

%.o: %.cxx
	$(CXX_COMPILE) $(INCLUDE_PATHS) $< -c -o $@



clean:
	rm -rf $(PROGRAM_NAME) $(OBJS) ii_files
