# ************** <auto-copyright.pl BEGIN do not edit this line> **************
# Doozer++
#
# Original Author:
#   Patrick Hartling
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
# File:          dpp.idl.mk,v
# Date modified: 2001/06/29 23:47:52
# Version:       1.3
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# =============================================================================
# dpp.idl.mk,v 1.3 2001/06/29 23:47:52 patrickh Exp
#
# This file <dpp.idl.mk> handles compiling a CORBA IDL file into source files
# for various languages using the appropriate IDL compiler.
# -----------------------------------------------------------------------------
# The makefile including this file must define the following variable:
#
# IDL            - The IDL compiler for generating C++ code.
# JIDL           - The IDL compiler for generating Java code.
# IDL_CPP_FILES  - The list of C++ files that the IDL compiler will generate.
#                  The order is very important.  For an IDL file called
#                  dsoIf.idl, the first generated file should be dsoIf.cpp.
#                  This is required so that make(1) will know what to do with
#                  what files.  Files corresponding to multiple IDL files can
#                  be listed in this variable as long as they follow this
#                  rule.
# IDL_JAVA_FILES - The list of Java files that the Java IDL compiler will
#                  generate.  The same rule for ordering of file names in
#                  $(IDL_CPP_FILES) applies here.
#
# Optionally, the following variables can be defined:
#
# IDL_INCLUDES         - Paths to extend the include path for the IDL
#                        compiler.  This uses exactly the same format as the
#                        usual $(INCLUDES) variable used with the C and C++
#                        compilers.
# IDL_OTHER_HEADER_DIR - A path to prepend to the included idl files of other
#                        idl files.
# IDL_THIS_HEADER_DIR  - A path to prepend to the included headers generated
#                        for the generated IDL file's header files.
# =============================================================================

ifneq ("$(IDL_THIS_HEADER_DIR)", "")
    IDL_OPTS	:= --this-header-dir $(IDL_THIS_HEADER_DIR)
endif

ifneq ("$(IDL_OTHER_HEADER_DIR)", "")
    IDL_OPTS	+= --other-header-dir $(IDL_OTHER_HEADER_DIR)
endif

# -----------------------------------------------------------------------------
# Generate C++ code (the files listed in $(IDL_CPP_FILES) from the
# corresponding IDL file.
# -----------------------------------------------------------------------------
idl: $(IDL_CPP_FILES)
ifdef _LOCAL_IDL
	@$(MAKE) _idl
endif

%.cpp %.h: %.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $<

# -----------------------------------------------------------------------------
# Generate Java code (the files listed in $(IDL_JAVA_FILES) from the
# corresponding IDL file.
# -----------------------------------------------------------------------------
jidl:
	@if [ ! -d $(JAVA_PKG) ] ; then mkdir -p $(JAVA_PKG) ; fi
	@$(MAKE) _run_jidl
ifdef _LOCAL_JIDL
	@$(MAKE) _jidl
endif

_run_jidl: $(IDL_JAVA_FILES)

$(JAVA_PKG)/%.java: %.idl
	$(JIDL) --package $(JAVA_PKG) $(IDL_INCLUDES) $<
