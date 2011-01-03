# Doozer++ is (C) Copyright 2000-2010 by Iowa State University
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
#
# Original Author:
#   Patrick Hartling

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
# This file <dpp.idl.mk> handles compiling a CORBA IDL file into source files
# for various languages using the appropriate IDL compiler.
# -----------------------------------------------------------------------------
# The makefile including this file must define the following variables:
#
# srcdir           - The directory containing the source files (IDL, C++, Java,
#                    etc.) to compile.
# C_IDL            - The IDL compiler for generating C code.
# CXX_IDL          - The IDL compiler for generating C++ code.
# OBJC_IDL         - The IDL compiler for generating Objective-C code.
# OBJCXX_IDL       - The IDL compiler for generating Objective-C++ code.
# JAVA_IDL         - The IDL compiler for generating Java code.
# IDL_C_FILES      - The list of C files that the IDL compiler will generate.
#                    The order is very important.  For an IDL file called
#                    myIf.idl, the first generated file should be myIf.cpp.
#                    This is required so that make(1) will know what to do with
#                    what files.  Files corresponding to multiple IDL files can
#                    be listed in this variable as long as they follow this
#                    rule.
# IDL_CXX_FILES    - The list of C++ files that the IDL compiler will generate.
#                    The same rule for ordering of file names in
#                    $(IDL_C_FILES) applies here.
# IDL_OBJC_FILES   - The list of Objective-C files that the IDL compiler will
#                    generate.  The same rule for ordering of file names in
#                    $(IDL_C_FILES) applies here.
# IDL_OBJCXX_FILES - The list of Objective-C++ files that the IDL compiler
#                    will generate.  The same rule for ordering of file names
#                    in $(IDL_C_FILES) applies here.
# IDL_JAVA_FILES   - The list of Java files that the Java IDL compiler will
#                    generate.  The same rule for ordering of file names in
#                    $(IDL_C_FILES) applies here.
# IDL_PYTHON_FILES - The list of Python files that the Python IDL compiler will
#                    generate.
#
# Optionally, the following variables can be defined:
#
# EXTRA_SRCS_PATH     - Directories besides $(srcdir) where source files may be
#                       found.
# C_IDL_OPTS          - General options to pass to the IDL-to-C compiler.
# CXX_IDL_OPTS        - General options to pass to the IDL-to-C++ compiler.
# OBJC_IDL_OPTS       - General options to pass to the IDL-to-Objective-C
#                       compiler.
# OBJCXX_IDL_OPTS     - General options to pass to the IDL-to-Objective-C++
#                       compiler.
# JAVA_IDL_OPTS       - General options to pass to the IDL-to-Java compiler.
# PYTHON_IDL_OPTS     - General options to pass to the IDL-to-Python compiler.
#
# C_IDL_INCLUDES      - Paths to extend the include path for the IDL-to-C
#                       compiler.
# CXX_IDL_INCLUDES    - Paths to extend the include path for the IDL-to-C++
#                       compiler.
# OBJC_IDL_INCLUDES   - Paths to extend the include path for the
#                       IDL-to-Objective-C compiler.
# OBJCXX_IDL_INCLUDES - Paths to extend the include path for the
#                       IDL-to-Objective-C++ compiler.
# JAVA_IDL_INCLUDES   - Paths to extend the include path for the IDL-to-Java
#                       compiler.
# PYTHON_IDL_INCLUDES - Paths to extend the include path for the IDL-to-Python
#                       compiler.
# =============================================================================

vpath %.idl $(srcdir) $(EXTRA_SRCS_PATH) $(EXTRA_JAVA_SRCS_PATH)

# -----------------------------------------------------------------------------
# Generate C code (the files listed in $(IDL_C_FILES) from the corresponding
# IDL file.
# -----------------------------------------------------------------------------
c_idl: $(IDL_C_FILES)
ifdef _LOCAL_C_IDL
	@$(MAKE) _c_idl
endif

ifdef IDL_C_FILES
%.c %.h: %.idl
	$(C_IDL) $(C_IDL_OPTS) $(C_IDL_INCLUDES) $<
endif

# -----------------------------------------------------------------------------
# Generate C++ code (the files listed in $(IDL_CXX_FILES) from the
# corresponding IDL file.
# -----------------------------------------------------------------------------
cxx_idl: $(IDL_CXX_FILES)
ifdef _LOCAL_CXX_IDL
	@$(MAKE) _cxx_idl
endif

ifdef IDL_CXX_FILES
%.cpp %.h: %.idl
	$(CXX_IDL) $(CXX_IDL_OPTS) $(CXX_IDL_INCLUDES) $<
endif

# -----------------------------------------------------------------------------
# Generate Objective-C code (the files listed in $(IDL_OBJC_FILES) from the
# corresponding IDL file.
# -----------------------------------------------------------------------------
objc_idl: $(IDL_OBJC_FILES)
ifdef _LOCAL_OBJC_IDL
	@$(MAKE) _objc_idl
endif

ifdef IDL_OBJC_FILES
%.m %.h: %.idl
	$(OBJC_IDL) $(OBJC_IDL_OPTS) $(OBJC_IDL_INCLUDES) $<
endif

# -----------------------------------------------------------------------------
# Generate Objective-C++ code (the files listed in $(IDL_OBJCXX_FILES) from the
# corresponding IDL file.
# -----------------------------------------------------------------------------
objcxx_idl: $(IDL_OBJCXX_FILES)
ifdef _LOCAL_OBJCXX_IDL
	@$(MAKE) _objcxx_idl
endif

ifdef IDL_OBJCXX_FILES
%.mm %.M %.h: %.idl
	$(OBJCXX_IDL) $(OBJCXX_IDL_OPTS) $(OBJCXX_IDL_INCLUDES) $<
endif

# -----------------------------------------------------------------------------
# Generate Java code (the files listed in $(IDL_JAVA_FILES) from the
# corresponding IDL file.
# -----------------------------------------------------------------------------
java_idl: $(IDL_JAVA_FILES)
ifdef _LOCAL_JAVA_IDL
	@$(MAKE) _java_idl
endif

ifdef IDL_JAVA_FILES
%.java: %.idl
	$(JAVA_IDL) $(JAVA_IDL_OPTS) $(JAVA_IDL_INCLUDES) $<
endif

# -----------------------------------------------------------------------------
# Generate Python code (the files listed in $(IDL_PYTHON_FILES) from the
# corresponding IDL file.
# -----------------------------------------------------------------------------
python_idl: $(IDL_PYTHON_FILES)
ifdef _LOCAL_PYTHON_IDL
	@$(MAKE) _python_idl
endif

ifdef IDL_PYTHON_FILES
%_idl.py: %.idl
	$(PYTHON_IDL) $(PYTHON_IDL_OPTS) $(PYTHON_IDL_INCLUDES) $<
endif
