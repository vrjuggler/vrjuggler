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
# File:          dzr.obj.mk,v
# Date modified: 2005/02/24 15:41:29
# Version:       1.14
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Object-generation code.  This is by far the most complicated of the mk files
# because it handles so many different situations based on the type of file
# being compiled.  Currently, the following source file types are supported:
# .c, .C, .CC, .cc, .cxx, .c++ and .cpp.
#
# dzr.obj.mk,v 1.14 2005/02/24 15:41:29 patrickh Exp
# -----------------------------------------------------------------------------
# The makefile that includes this file directly must define the following
# variables:
#
# C_COMPILE   - The command to compile C files with all the needed options.
# CXX_COMPILE - The command to compile C++ files with all the needed options.
# -----------------------------------------------------------------------------
# User makefiles can define the following variables:
#
# SRCS       - The list of sources to compile.
# OBJDIR     - The directory where the object files will be generated.  This
#              defaults to the current directory if it is not defined.
# OBJ_EXT    - The extension for object files (usually obj or o).  This
#              defaults to obj if the compiler being used is cl (the MS
#              Visual C++ compiler) or to o if any other compiler is being
#              used.
# EXTRA_PATH_FOR_SOURCES - Directories where source files (of any type) can be
#                          found besides the source directory set in
#                          $(srcdir).
# -----------------------------------------------------------------------------

OBJ_EXT?=		$(OS_OBJ_EXT)
OBJ_NAME_FLAG?=		-o $@
OBJ_BUILD_FLAG?=	-c

# If no object directory is being used, set it to the current directory.
OBJDIR?=	.

# Define the list of supported source file suffixes.
_suffix_list=	c C CC cc cpp c++ cxx f

# Loop over the suffixes, translate them to $(OBJ_EXT) and store the
# result in $(OBJS).
makeobjs=	$(filter %$(OBJ_EXT), $(foreach _suffix, $(_suffix_list), $(1:.$(_suffix)=$(OBJ_EXT))))

# Construct $(OBJS) from $(SRCS) using the makeobjs function.
BASIC_OBJS=	$(call makeobjs, $(SRCS))
OBJS=		$(addprefix $(OBJDIR)/, $(BASIC_OBJS))

C_COMPILE_LINE=		$(C_COMPILE) $(OBJ_NAME_FLAG) $(OBJ_BUILD_FLAG) $<
CXX_COMPILE_LINE=	$(CXX_COMPILE) $(OBJ_NAME_FLAG) $(OBJ_BUILD_FLAG) $<
F77_COMPILE_LINE=	$(F77_COMPILE) $(OBJ_NAME_FLAG) $(OBJ_BUILD_FLAG) $<

MAKE_THE_OBJDIR=	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR) ; fi

# Pattern rule for compiling $(OBJ_EXT) files from .c files.
$(OBJDIR)/%$(OBJ_EXT): %.c
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	-$(MAKE_THE_OBJDIR)
	$(C_COMPILE_LINE)

# Pattern rule for compiling $(OBJ_EXT) files from .C files.
$(OBJDIR)/%$(OBJ_EXT): %.C
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	-$(MAKE_THE_OBJDIR)
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling $(OBJ_EXT) files from .CC files.
$(OBJDIR)/%$(OBJ_EXT): %.CC
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	-$(MAKE_THE_OBJDIR)
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling $(OBJ_EXT) files from .cc files.
$(OBJDIR)/%$(OBJ_EXT): %.cc
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	-$(MAKE_THE_OBJDIR)
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling $(OBJ_EXT) files from .cpp files.
$(OBJDIR)/%$(OBJ_EXT): %.cpp
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	-$(MAKE_THE_OBJDIR)
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling $(OBJ_EXT) files from .c++ files.
$(OBJDIR)/%$(OBJ_EXT): %.c++
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	-$(MAKE_THE_OBJDIR)
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling $(OBJ_EXT) files from .cxx files.
$(OBJDIR)/%$(OBJ_EXT): %.cxx
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	-$(MAKE_THE_OBJDIR)
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling $(OBJ_EXT) files from .f files.
$(OBJDIR)/%$(OBJ_EXT): %.f
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	-$(MAKE_THE_OBJDIR)
	$(F77_COMPILE_LINE)

# Set up the vpath stuff for finding the source files.  The default is to
# check the source directory.  Users can extend this by setting a value for
# $(EXTRA_PATH_FOR_SOURCES) to some list of directories.
PATH_FOR_SOURCES=	$(srcdir)

vpath %.c $(PATH_FOR_SOURCES) $(EXTRA_PATH_FOR_SOURCES)
vpath %.C $(PATH_FOR_SOURCES) $(EXTRA_PATH_FOR_SOURCES)
vpath %.CC $(PATH_FOR_SOURCES) $(EXTRA_PATH_FOR_SOURCES)
vpath %.cc $(PATH_FOR_SOURCES) $(EXTRA_PATH_FOR_SOURCES)
vpath %.cpp $(PATH_FOR_SOURCES) $(EXTRA_PATH_FOR_SOURCES)
vpath %.c++ $(PATH_FOR_SOURCES) $(EXTRA_PATH_FOR_SOURCES)
vpath %.cxx $(PATH_FOR_SOURCES) $(EXTRA_PATH_FOR_SOURCES)
vpath %.f $(PATH_FOR_SOURCES) $(EXTRA_PATH_FOR_SOURCES)

define DZR_OBJ_echo_vars
@echo "Doozer OBJ settings"
@echo "OBJ_EXT: $(OBJ_EXT)"
@echo "OBJ_NAME_FLAG: $(OBJ_NAME_FLAG)"
@echo "OBJ_BUILD_FLAG: $(OBJ_BUILD_FLAG)"
@echo "OBJDIR: $(OBJDIR)"
@echo "BASIC_OBJS: $(BASIC_OBJS)"
@echo "OBJS: $(OBJS)"
@echo "C_COMPILE_LINE: $(C_COMPILE_LINE)"
@echo "PATH_FOR_SOURCES: $(PATH_FOR_SOURCES)"
@echo "EXTRA_PATH_FOR_SOURCES: $(EXTRA_PATH_FOR_SOURCES)"
endef
