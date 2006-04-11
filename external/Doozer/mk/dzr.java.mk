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
# File:          dzr.java.mk,v
# Date modified: 2002/08/23 17:05:23
# Version:       1.1
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Java class-generation code.
#
# dzr.java.mk,v 1.1 2002/08/23 17:05:23 patrickh Exp
# -----------------------------------------------------------------------------
# The makefile that includes this file directly must define the following
# variables:
#
# JAVA_COMPILE - The command to compile Java files with all the needed options.
# -----------------------------------------------------------------------------
# User makefiles can define the following variables:
#
# JAVA_SRCS  - The list of sources to compile.
# CLASSDIR   - The directory where the object files will be generated.  This
#              defaults to the current directory if it is not defined.
# EXTRA_PATH_FOR_SOURCES - Directories where source files (of any type) can be
#                          found besides the source directory set in
#                          $(srcdir).
# -----------------------------------------------------------------------------

# If no object directory is being used, set it to the current directory.
JAVA_CLASSDIR?=	.

# Construct $(JAVA_CLASSES) from $(JAVA_SRCS).
BASIC_JAVA_CLASSES=	$(JAVA_SRCS:.java=.class)
JAVA_CLASSES=		$(addprefix $(JAVA_CLASSDIR)/, $(BASIC_JAVA_CLASSES))
JAVA_COMPILE_LINE=	$(JAVA_COMPILE) -d "$(JAVA_CLASSDIR)" $<

MAKE_THE_CLASSDIR=	@if [ ! -d $(JAVA_CLASSDIR) ]; then mkdir $(JAVA_CLASSDIR) ; fi

# Pattern rule for compiling $(OBJ_EXT) files from .c files.
$(JAVA_CLASSDIR)/%.class: %.java
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	-$(MAKE_THE_CLASSDIR)
	$(JAVA_COMPILE_LINE)

# Set up the vpath stuff for finding the source files.  The default is to
# check the source directory.  Users can extend this by setting a value for
# $(EXTRA_PATH_FOR_SOURCES) to some list of directories.
PATH_FOR_SOURCES=	$(srcdir)

vpath %.java $(PATH_FOR_SOURCES) $(EXTRA_PATH_FOR_SOURCES)
vpath %.class $(PATH_FOR_SOURCES) $(JAVA_CLASSDIR)
