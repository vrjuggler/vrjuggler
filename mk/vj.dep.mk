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

# -----------------------------------------------------------------------------
# This include file <vj.dep.mk> handles source code dependencies.  It
# generates makefiles corresponding to each source file (ending in .c or .cpp)
# that have the dependencies for that source file.  The generated file can
# then be included in the Makefile for the directory.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variables:
#
# MKEPS_C_COMPILE   - The compiler command for C files.
# MKEPS_CXX_COMPILE - The compiler command for C++ files.
# DEP_GEN_FLAG      - The flag passed to the compiler to generate dependencies.
# DEPEND_FILES      - The list of dependency makefiles to be generated.
# OBJDIR            - The directory to which the object file(s) will be
#                     written.
# OBJ_FILE_SUFFIX   - Suffix for object file names (e.g., "o" or "obj").
#
# Example:
#         srcdir = /usr/src/proj1
#         OBJDIR = /usr/obj/proj1
#   DEP_GEN_FLAG = -M
#   DEPEND_FILES = file1.d file2.d file3.d
#
# With these settings, the source code comes from /usr/src/proj1 and the
# object files go into /usr/obj/proj1.
# -----------------------------------------------------------------------------

# Conditionally set ${OBJDIR} just to be safe.
ifndef OBJDIR
    OBJDIR	= .
endif

ifdef DO_CLEANDEPEND
    CLEAN_FILES	+= ${DEPEND_FILES}
endif

# These expressions reformat the output from the dependency text to be of the
# form:
#
#     ${OBJDIR}/file1.o file1.d : ...
#
# where file1 is the value in $* and file1.d is $@.  The first handles output
# from the C and C++ compilers which prints only the object file to be
# created ($*).  The second handles output from makedepend(1) which includes
# the path leading to the source file in the object file name.
_CC_SED_EXP	= '\''s/\($*\)\.${OBJ_FILE_SUFFIX}[ :]*/$${OBJDIR}\/\1.${OBJ_FILE_SUFFIX} $@: /g'\''
_MKDEP_SED_EXP	= '\''s/.*\($*\)\.${OBJ_FILE_SUFFIX}[ :]*/$${OBJDIR}\/\1.${OBJ_FILE_SUFFIX} $@: /g'\''

%.d: %.c
	@echo "Updating dependency file $@ ..."
ifeq (${PLATFORM}, HP)
	@${SHELL} -ec 'makedepend -f- -o.${OBJ_FILE_SUFFIX}		\
                       ${DEPENDFLAGS} -- ${DEPEND_EXTRAS} -- $< |	\
                       sed ${_MKDEP_SED_EXP} > $@ ; [ -s $@ ] || rm -f $@'
else
  ifeq (${CC}, cl)
	@${SHELL} -ec 'makedepend -f- -o.${OBJ_FILE_SUFFIX}		\
                       ${DEPENDFLAGS} -- ${DEPEND_EXTRAS} -- $< |	\
                       sed ${_MKDEP_SED_EXP} > $@ ; [ -s $@ ] || rm -f $@'
  else
	@${SHELL} -ec '${MKDEPS_C_COMPILE} ${DEP_GEN_FLAG} $< |		\
                       sed ${_CC_SED_EXP} > $@ ; [ -s $@ ] || rm -f $@'
  endif
endif

%.d: %.cpp
	@echo "Updating dependency file $@ ..."
ifeq (${CC}, cl)
	@${SHELL} -ec 'makedepend -f- -o.${OBJ_FILE_SUFFIX}		\
                       ${DEPENDFLAGS} -- ${DEPEND_EXTRAS} -- $< |	\
                       sed ${_MKDEP_SED_EXP} > $@ ; [ -s $@ ] || rm -f $@'
else
	@${SHELL} -ec '${MKDEPS_CXX_COMPILE} ${DEP_GEN_FLAG} $< |		\
                       sed ${_CC_SED_EXP} > $@ ; [ -s $@ ] || rm -f $@'
endif
