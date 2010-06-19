# Doozer++ is (C) Copyright 2000-2009 by Iowa State University
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
#
# Original Author:
#   Patrick Hartling

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

# =============================================================================
# This include file <dpp.dep.mk> handles source code dependencies.  It
# generates makefiles corresponding to each source file (ending in .c or .cpp)
# that have the dependencies for that source file.  The generated file can
# then be included in the Makefile for the directory.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variables:
#
# BASIC_OBJECTS  - The list of objects files (with extension .$(OBJEXT)) to be
#                  compiled.  The dependency file names are generated based on
#                  the contents of this variable.
# C_COMPILE      - The compiler command for C files.
# CXX_COMPILE    - The compiler command for C++ files.
# OBJC_COMPILE   - The compiler command for Objective-C files.
# OBJCXX_COMPILE - The compiler command for Objective-C++ files.
# DEPDIR         - The directory to which the depenecy file(s) will be written.
#                  If not given, it defaults to the current directory.
# DEP_GEN_FLAG   - The flag passed to the compiler to generate dependencies.
# MKDEP_C        - The dependency generator command for C files.  If not given,
#                  this defaults to the value of $(C_COMPILE).  Note that this
#                  should not include the flag that actually generates the
#                  dependency list.
# MKDEP_CXX      - The dependency generator command for C++ files.  If not
#                  given, this defaults to the value of $(CXX_COMPILE).  Note
#                  that this should not include the flag that actually
#                  generates the dependency list.
# MKDEP_OBJC     - The dependency generator command for Objective-C files.  If
#                  not given, this defaults to the value of $(CXX_COMPILE).
#                  Note that this should not include the flag that actually
#                  generates the dependency list.
# MKDEP_OBJCXX   - The dependency generator command for Objective-C++ files.
#                  If not given, this defaults to the value of
#                  $(OBJCXX_COMPILE).  Note that this should not include the
#                  flag that actually generates the dependency list.
# OBJDIR         - The directory to which the object file(s) will be written.
#                  If not given, it defaults to the current directory.
# OBJEXT         - Suffix for object file names (e.g., "o" or "obj").
# SRCS           - The list of source files for which dependencies are to be
#                  generated.
#
# Example:
#         srcdir = /usr/src/proj1
#         DEPDIR = /usr/src/proj1
#         OBJDIR = /usr/obj/proj1
#         OBJEXT = o
#  BASIC_OBJECTS = file1.$(OBJEXT) file2.$(OBJEXT) file3.$(OBJEXT)
#   DEP_GEN_FLAG = -M
#
# With these settings, the source code comes from /usr/src/proj1 and the
# object files go into /usr/obj/proj1.
# =============================================================================

# Conditionally set $(OBJDIR) and $(DEPDIR) just to be safe.
OBJDIR?=	.
DEPDIR?=	.
MKDEP_C?=	$(C_COMPILE)
MKDEP_CXX?=	$(CXX_COMPILE)
MKDEP_OBJC?=	$(OBJC_COMPILE)
MKDEP_OBJCXX?=	$(OBJCXX_COMPILE)

DEPEND_FILES:=	$(addprefix $(DEPDIR)/, $(BASIC_OBJECTS:.$(OBJEXT)=.d))

ifdef DO_CLEANDEPEND
   CLEAN_FILES+=	$(DEPEND_FILES)
endif

# These expressions reformat the output from the dependency text to be of the
# form:
#
#     $(OBJDIR)/file1.o $(DEPDIR)/file1.d : ...
#
# where file1 is the value in $* and file1.d is $@.  The first handles output
# from the C and C++ compilers which prints only the object file to be
# created ($*).  The second handles output from makedepend(1) which includes
# the path leading to the source file in the object file name.
_CC_SED_EXP=	'\''s|\($*\)\.$(OBJEXT)[ :]*|$$(OBJDIR)/\1.$(OBJEXT) $@: |g'\''
_MKDEP_SED_EXP=	'\''s|.*\($*\)\.$(OBJEXT)[ :]*|$$(OBJDIR)/\1.$(OBJEXT) $@: |g'\''

_DEPGEN_MKDEP=	$(SHELL) -ec '$(MAKEDEPEND) -f- -o.$(OBJEXT)		\
		   $(DEPEND_FLAGS) -- $(DEPEND_EXTRAS) -- $< |		\
		   sed $(_MKDEP_SED_EXP) > $@'

# If we have to use makedepend(1), then go that route.  Otherwise, use the
# compiler's built-in dependency generation features.
ifeq ($(USE_MAKEDEPEND), Y)
   C_DEPGEN=		$(_DEPGEN_MKDEP)
   CXX_DEPGEN=		$(_DEPGEN_MKDEP)
   OBJC_DEPGEN=		$(_DEPGEN_MKDEP)
   OBJCXX_DEPGEN=	$(_DEPGEN_MKDEP)
else
   C_DEPGEN=		$(SHELL) -ec '$(MKDEP_C) $(DEP_GEN_FLAG) $< |	\
			   sed $(_CC_SED_EXP) > $@' 2>/dev/null
   CXX_DEPGEN=		$(SHELL) -ec '$(MKDEP_CXX) $(DEP_GEN_FLAG) $< |	\
			   sed $(_CC_SED_EXP) > $@' 2>/dev/null
   OBJC_DEPGEN=		$(SHELL) -ec '$(MKDEP_OBJC) $(DEP_GEN_FLAG) $< | \
			   sed $(_CC_SED_EXP) > $@' 2>/dev/null
   OBJCXX_DEPGEN=	$(SHELL) -ec '$(MKDEP_OBJCXX) $(DEP_GEN_FLAG) $< | \
			   sed $(_CC_SED_EXP) > $@' 2>/dev/null
endif

$(DEPDIR)/%.d: %.c
	@echo "Updating dependency file $@ ..."
	@$(C_DEPGEN)

$(DEPDIR)/%.d: %.C
	@echo "Updating dependency file $@ ..."
	@$(CXX_DEPGEN)

$(DEPDIR)/%.d: %.CC
	@echo "Updating dependency file $@ ..."
	@$(CXX_DEPGEN)

$(DEPDIR)/%.d: %.cc
	@echo "Updating dependency file $@ ..."
	@$(CXX_DEPGEN)

$(DEPDIR)/%.d: %.c++
	@echo "Updating dependency file $@ ..."
	@$(CXX_DEPGEN)

$(DEPDIR)/%.d: %.cpp
	@echo "Updating dependency file $@ ..."
	@$(CXX_DEPGEN)

$(DEPDIR)/%.d: %.cxx
	@echo "Updating dependency file $@ ..."
	@$(CXX_DEPGEN)

$(DEPDIR)/%.d: %.m
	@echo "Updating dependency file $@ ..."
	@$(OBJC_DEPGEN)

$(DEPDIR)/%.d: %.mm
	@echo "Updating dependency file $@ ..."
	@$(OBJCXX_DEPGEN)

$(DEPDIR)/%.d: %.M
	@echo "Updating dependency file $@ ..."
	@$(OBJCXX_DEPGEN)
