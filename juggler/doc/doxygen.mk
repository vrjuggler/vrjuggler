# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

DOXYGEN?=		doxygen
HTML_OUTPUT_DIR?=	html
LATEX_OUTPUT_DIR?=	latex
MAN_OUTPUT_DIR?=	man
RTF_OUTPUT_DIR?=	rtf

RM=		rm -f

html: $(HTML_OUTPUT_DIR) $(HTML_DOXYGEN_FILE)
	$(DOXYGEN) $(HTML_DOXYGEN_FILE)

man: $(MAN_DOXYGEN_FILE)
	$(DOXYGEN) $(MAN_DOXYGEN_FILE)

latex: $(LATEX_DOXYGEN_FILE)
	$(DOXYGEN) $(LATEX_DOXYGEN_FILE)

ps: $(LATEX_OUTPUT_DIR)
	$(MAKE) -C $(LATEX_OUTPUT_DIR) ps

pdf: $(LATEX_OUTPUT_DIR)
	$(MAKE) -C $(LATEX_OUTPUT_DIR) pdf

rtf:
	$(DOXYGEN) $(RTF_DOXYGEN_FILE)

install-html:
	if [ ! -d $(prefix) ]; then mkdir -p $(prefix); fi
	cp -r $(HTML_OUTPUT_DIR)/* $(prefix)

install-ps:
	if [ ! -d $(prefix) ]; then mkdir -p $(prefix); fi
	cp $(LATEX_OUTPUT_DIR)/refman.ps $(prefix)

install-pdf:
	if [ ! -d $(prefix) ]; then mkdir -p $(prefix); fi
	cp $(LATEX_OUTPUT_DIR)/refman.pdf $(prefix)

clean:
ifneq ($(CLEAN_FILES), )
	$(RM) $(CLEAN_FILES)
endif
ifneq ($(CLEAN_DIRS), )
	$(RM) -r $(CLEAN_DIRS)
endif

clobber:
	@$(MAKE) clean
ifneq ($(CLOBBER_FILES), )
	$(RM) $(CLOBBER_FILES)
endif
	$(RM) -r $(HTML_OUTPUT_DIR) $(MAN_OUTPUT_DIR)                   \
          $(LATEX_OUTPUT_DIR) $(RTF_OUTPUT_DIR) $(CLOBBER_DIRS)
