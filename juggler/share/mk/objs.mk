# ***************** <VPR heading BEGIN do not edit this line> *****************
#
# VR Juggler Portable Runtime
#
# Original Authors:
#   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# ****************** <VPR heading END do not edit this line> ******************

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
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
# *************** <auto-copyright.pl END do not edit this line> ***************

# If no object directory is being used, set it to the current directory.
OBJDIR		?= .
DEPDIR		?= .

# Define the list of supported source file suffixes.
_suffix_list = c C CC cc cpp c++ cxx

# Loop over the suffixes, translate them to $(OBJEXT) and store the
# result in $(OBJS).
makeobjs = $(filter %.$(OBJEXT), $(foreach _suffix, $(_suffix_list), $(1:.$(_suffix)=.$(OBJEXT))))

# add a directory prefix to each source file
# OBJS := ${addprefix ${OBJDIR}/,$(SRCS)}
# Construct $(OBJS) from $(SRCS) using the makeobjs function.
OBJS = $(call makeobjs, $(SRCS))

# Rules for compiling
CREATE_OBJ_FILE_COMMAND = $(CXX_COMPILE) $(WARNINGS_FLAGS) $(OBJ_NAME_FLAG) -c $<
CREATE_OBJ_FILE_COMMAND_C = $(C_COMPILE) $(WARNINGS_FLAGS) $(OBJ_NAME_FLAG) -c $<

$(OBJDIR)/%.$(OBJEXT): %.c
	$(CREATE_OBJ_FILE_COMMAND_C)

$(OBJDIR)/%.$(OBJEXT): %.cxx
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.c++
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.cc
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.C
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.cpp
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.C
	$(CREATE_OBJ_FILE_COMMAND)

# Search path for source code and object files.
vpath
vpath %.c ${PATH_TO_SRCS}
vpath %.C ${PATH_TO_SRCS}
vpath %.cpp ${PATH_TO_SRCS}
vpath %.cxx ${PATH_TO_SRCS}
vpath %.c++ ${PATH_TO_SRCS}
vpath %.cc ${PATH_TO_SRCS}
vpath %.${OBJEXT} ${OBJDIR}
