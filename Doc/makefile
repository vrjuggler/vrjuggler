# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

include subdirs.mk

top_srcdir = ..

PERCEPS_DIR = .
PERCEPS= perl $(PERCEPS_DIR)/bin/perceps
FILTERS = ./filters
TEMPLATES_INT = ./templates/internal
TEMPLATES_PUB = ./templates/public

SRC_EX = ExampleClass.h ExampleClass.C
OUT_INT = ./internal_doc
OUT_PUB = ./public_doc
OUT_EX = ./ex_html
JAVA_CLASS = $(PERCEPS_DIR)/classes

# Fill these two in!!  Together, they form the installation prefix.
webroot=	$(HOME)/public_html/jugglerweb
instdir=	vrjuggler/1.0/programmer.reference

prefix_pub=	$(webroot)/$(instdir)
prefix_int=	$(webroot)/$(instdir).internal

docs: doc-internal doc-public

doc-internal:
	$(PERCEPS) -a -b -c -h -f -d $(OUT_INT) -F $(FILTERS)		\
          -t $(TEMPLATES_INT) $(SUBDIRS)
	cp $(JAVA_CLASS)/*.class $(OUT_INT)

doc-public:
	$(PERCEPS) -a -b -c -h -f -d $(OUT_PUB) -F $(FILTERS)		\
          -t $(TEMPLATES_PUB) $(SUBDIRS)

install-docs: install-doc-internal install-doc-public

install-doc-internal:
	if [ ! -d $(prefix_int) ]; then mkdir -p $(prefix_int) ; fi
	mv $(OUT_INT)/* $(prefix_int)/

install-doc-public:
	if [ ! -d $(prefix_pub) ]; then mkdir -p $(prefix_pub) ; fi
	mv $(OUT_PUB)/* $(prefix_pub)/

clean:
	@ if [ -f $(OUT_PUB)/* ]; then rm $(OUT_PUB)/*; fi
	@ if [ -f $(OUT_INT)/* ]; then rm $(OUT_INT)/*; fi
