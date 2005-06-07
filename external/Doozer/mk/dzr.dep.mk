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
# File:          dzr.dep.mk,v
# Date modified: 2002/08/02 21:47:44
# Version:       1.10
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# dzr.dep.mk,v 1.10 2002/08/02 21:47:44 subatomic Exp
#
# This include file <dzr.dep.mk> handles source code dependencies.  It
# generates makefiles (.d files actually) corresponding to each source file
# that have the dependencies for that source file.  The following sorce file
# types are supported: .c, .C, .CC, .cc, .cxx, .cpp and .c++.
# -----------------------------------------------------------------------------
# User makefiles can define the following variables:
#
# DEPDIR     - The directory where the dependency files will be generated.
#              This defaults to the current directory if it is not defined.
# -----------------------------------------------------------------------------
# The makefile that includes this file must define the following variables:
#
# CC          - The C compiler.
# C_COMPILE   - The compiler command for C files.
# CXX_COMPILE - The compiler command for C++ files.
# OBJDIR      - The directory to which the object file(s) will be written.
# OBJ_EXT  - Suffix for object file names (e.g., "o" or "obj").
# BASIC_OBJS  - The list of object files that will be compiled without the
#               $(OBJDIR) prefix.  We use this for the suffix change because
#               there is only one suffix for object files ($(OBJ_EXT)).
# EXTRA_DEPENDFLAGS - user settable thing to pass extra stuff to the makedepend
# -----------------------------------------------------------------------------

DEPDIR?=	.

# XXX: Not every compiler uses -M to generate depedencies.  This needs to be
# generalized.
DEP_GEN_FLAG=	-M
DEPEND_FILES=	$(addprefix $(DEPDIR)/, $(BASIC_OBJS:$(OBJ_EXT)=.d))

# These expressions reformat the output from the dependency text to be of the
# form:
#
#     $(OBJDIR)/file1.o $(DEPDIR)/file1.d : ...
#
# where file1 is the value in $* and file1.d is $@.  The first handles output
# from the C and C++ compilers which prints only the object file to be
# created ($*).  The second handles output from makedepend(1) which includes
# the path leading to the source file in the object file name.
_CC_SED_EXP=	'\''s|\($*\)\$(OBJ_EXT)[ :]*|$$(OBJDIR)/\1$(OBJ_EXT) $@: |g'\''
_MKDEP_SED_EXP=	'\''s|.*\($*\)\$(OBJ_EXT)[ :]*|$$(OBJDIR)/\1$(OBJ_EXT) $@: |g'\''

# Determine, based on the C compiler, how to generate the dependencies.  If
# the compiler is cl (Windows only), use makedepend(1).  Otherwise, we can use
# the compiler itself to do the job.
ifdef USE_MAKEDEPEND
   DEPENDFLAGS=		$(INCLUDES) $(EXTRA_DEPENDFLAGS) $(OS_DEPENDFLAGS)
   DEPEND_EXTRAS=	$(DEFS)
   _C_DEPGEN=	$(SHELL) -ec 'makedepend -f- -o$(OBJ_EXT)		\
		   $(DEPENDFLAGS) -- $(DEPEND_EXTRAS) -- $< |		\
		   sed $(_MKDEP_SED_EXP) > $@ ; [ -s $@ ] || rm -f $@'
   _CXX_DEPGEN=	$(SHELL) -ec 'makedepend -f- -o$(OBJ_EXT)		\
		   $(DEPENDFLAGS) -- $(DEPEND_EXTRAS) -- $< |		\
		   sed $(_MKDEP_SED_EXP) > $@ ; [ -s $@ ] || rm -f $@'
else
   _C_DEPGEN=	$(SHELL) -ec '$(C_COMPILE) $(DEP_GEN_FLAG) $< |		\
		   sed $(_CC_SED_EXP) > $@ ; [ -s $@ ] || rm -f $@'
   _CXX_DEPGEN=	$(SHELL) -ec '$(CXX_COMPILE) $(DEP_GEN_FLAG) $< |	\
		   sed $(_CC_SED_EXP) > $@ ; [ -s $@ ] || rm -f $@'
endif

MAKE_THE_DEPDIR=	-@if [ ! -d $(DEPDIR) ]; then mkdir $(DEPDIR) ; fi

$(DEPDIR)/%.d: %.c
	@echo "Updating dependency file $@ ..."
	-$(MAKE_THE_DEPDIR)
	@$(_C_DEPGEN)

$(DEPDIR)/%.d: %.C
	@echo "Updating dependency file $@ ..."
	-$(MAKE_THE_DEPDIR)
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.CC
	@echo "Updating dependency file $@ ..."
	-$(MAKE_THE_DEPDIR)
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.cc
	@echo "Updating dependency file $@ ..."
	-$(MAKE_THE_DEPDIR)
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.c++
	@echo "Updating dependency file $@ ..."
	-$(MAKE_THE_DEPDIR)
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.cpp
	@echo "Updating dependency file $@ ..."
	-$(MAKE_THE_DEPDIR)
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.cxx
	@echo "Updating dependency file $@ ..."
	-$(MAKE_THE_DEPDIR)
	@$(_CXX_DEPGEN)

# Include all the files in $(DEPEND_FILES) unless we are cleaning up the
# dependencies.  There are other cases when we do not want to include all
# these files, but I do not know of a good way to do the conditional include
# besides nested, gross conditional statements.
ifndef DO_CLEANDEPEND
ifneq ($(NO_DEPEND), YES)
   -include $(DEPEND_FILES)
endif
endif
