# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

# Extract the Doxygen version information.
DOXYGEN_VERSION=	$(shell $(DOXYGEN) --version)
DOXYGEN_VERSION_NUMS=	$(subst ., , $(DOXYGEN_VERSION))
DOXYGEN_VERSION_MAJOR=	$(word 1, $(DOXYGEN_VERSION_NUMS))
DOXYGEN_VERSION_MINOR=	$(word 2, $(DOXYGEN_VERSION_NUMS))

RM=		rm -f

html: $(HTML_DOXYGEN_FILE)
	$(DOXYGEN) $(HTML_DOXYGEN_FILE)

man: $(MAN_DOXYGEN_FILE)
	$(DOXYGEN) $(MAN_DOXYGEN_FILE)

latex: $(LATEX_DOXYGEN_FILE)
	$(DOXYGEN) $(LATEX_DOXYGEN_FILE)

ps: $(LATEX_OUTPUT_DIR)
	$(MAKE) -C $(LATEX_OUTPUT_DIR) ps

# Doxygen 1.2 uses the target name 'pdf' to generate a PDF manual from LaTeX.
# Doxygen 1.3 and newer use the target name 'refman.pdf'.
pdf: $(LATEX_OUTPUT_DIR)
ifeq ($(DOXYGEN_VERSION_MINOR), 2)
	$(MAKE) -C $(LATEX_OUTPUT_DIR) pdf
else
	$(MAKE) -C $(LATEX_OUTPUT_DIR) refman.pdf
endif

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
