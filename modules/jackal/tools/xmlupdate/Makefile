# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

# Paths to XSLT processing tools.
XSLTPROC=	xsltproc
XALAN=		xalan.sh

# Change this based on which XSLT processor you want to use.  For example, to
# use Xalan, change the value to 'update-xalan'.
XSLT_TARGET=	update-xsltproc

cfg-update-2.0-2.1:
	$(MAKE) XSL_FILE=cfg_2.0-2.1.xsl $(XSLT_TARGET)

desc-update-2.0-2.1:
	$(MAKE) XSL_FILE=desc_2.0-2.1.xsl $(XSLT_TARGET)

desc-update-2.1-2.2:
	$(MAKE) XSL_FILE=desc_2.1-2.2.xsl $(XSLT_TARGET)

desc-update-2.2-2.3:
	$(MAKE) XSL_FILE=desc_2.2-2.3.xsl $(XSLT_TARGET)

desc-update-2.3-3.0:
	$(MAKE) XSL_FILE=desc_2.3-3.0.xsl $(XSLT_TARGET)

jdef-update-3.0-3.1:
	$(MAKE) XSL_FILE=jdef_3.0-3.1.xsl $(XSLT_TARGET)

desc-update-2.0-2.2:
	$(MAKE) desc-update-2.0-2.1
	$(MAKE) desc-update-2.1-2.2

desc-update-2.0-3.1:
	$(MAKE) desc-update-2.0-2.1
	$(MAKE) desc-update-2.1-2.2
	$(MAKE) desc-update-2.2-2.3
	$(MAKE) desc-update-2.3-3.0
	$(MAKE) desc-update-3.0-3.1

# Generate $(OUTPUT_FILE) from $(INPUT_FILE) using xsltproc and $(XSL_FILE) for
# the stylesheet.
update-xsltproc:
	$(XSLTPROC) -o $(OUTPUT_FILE) $(XSL_FILE) $(INPUT_FILE)

# Generate $(OUTPUT_FILE) from $(INPUT_FILE) using Xalan and $(XSL_FILE) for
# the stylesheet.
update-xalan:
	$(XALAN) -in $(INPUT_FILE) -out $(OUTPUT_FILE) -xsl $(XSL_FILE)
