# Doozer++ is (C) Copyright 2000-2010 by Iowa State University
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
# This include file <dpp.compile.mk> handles source code compiling.
# -----------------------------------------------------------------------------
# The Makefile including this file should define the following variables:
#
# OBJDIR         - The directory where the object file will be created.
# C_COMPILE      - The C compiler command line (without -c or -o <outfile>
#                  options).
# CXX_COMPILE    - The C++ compiler command line (without -c or -o <outfile>
#                  options).
# OBJC_COMPILE   - The Objective-C compiler command line (without -c or -o
#                  <outfile> options).
# OBJCXX_COMPILE - The Objective-C++ compiler command line (without -c or -o
#                  <outfile> options).
# OBJEXT         - Suffix for object file names (e.g., "o" or "obj").
# OBJ_NAME_FLAG  - Flag (with any extra options) used for naming the object
#                  file being compiled here.
# OBJ_BUILD_FLAG - Flag (with any extra options) used to tell the compiler to 
#                  generate an object (precompiler only flag, usually -c)
# JAVAC          - The path to the Java compiler.
# JAVA_SRC_PATH  - The path to .java files in a directory other than
#                  $(srcdir).
# ORB_JAR        - The .jar file providing an ORB implementation.
# =============================================================================

OBJ_BUILD_FLAG?=	-c

# If we are building profiled objects, add the flags to the C and C++ compiler
# flags.
ifdef PROF_OBJ_BUILD
   CFLAGS+=		$(CC_PROF_FLAGS)
   CXXFLAGS+= 		$(CXX_PROF_FLAGS)
   OBJCFLAGS+=	 	$(OBJC_PROF_FLAGS)
   OBJCXXFLAGS+= 	$(OBJCXX_PROF_FLAGS)
endif

C_COMPILE_LINE=		$(C_COMPILE) $(OBJ_BUILD_FLAG) $(OBJ_NAME_FLAG) $<
CXX_COMPILE_LINE=	$(CXX_COMPILE) $(OBJ_BUILD_FLAG) $(OBJ_NAME_FLAG) $<
OBJC_COMPILE_LINE=	$(OBJC_COMPILE) $(OBJ_BUILD_FLAG) $(OBJ_NAME_FLAG) $<
OBJCXX_COMPILE_LINE=	$(OBJCXX_COMPILE) $(OBJ_BUILD_FLAG) $(OBJ_NAME_FLAG) $<

# Pattern rule for compiling .$(OBJEXT) files from .c files.
$(OBJDIR)/%.$(OBJEXT): %.c
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(C_COMPILE_LINE)

# Pattern rule for compiling .$(OBJEXT) files from .C files.
$(OBJDIR)/%.$(OBJEXT): %.C
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling .$(OBJEXT) files from .CC files.
$(OBJDIR)/%.$(OBJEXT): %.CC
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling .$(OBJEXT) files from .cc files.
$(OBJDIR)/%.$(OBJEXT): %.cc
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling .$(OBJEXT) files from .cpp files.
$(OBJDIR)/%.$(OBJEXT): %.cpp
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling .$(OBJEXT) files from .c++ files.
$(OBJDIR)/%.$(OBJEXT): %.c++
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling .$(OBJEXT) files from .cxx files.
$(OBJDIR)/%.$(OBJEXT): %.cxx
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(CXX_COMPILE_LINE)

# Pattern rule for compiling .$(OBJEXT) files from .m files.
$(OBJDIR)/%.$(OBJEXT): %.m
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(OBJC_COMPILE_LINE)

# Pattern rule for compiling .$(OBJEXT) files from .mm files.
$(OBJDIR)/%.$(OBJEXT): %.mm
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(OBJCXX_COMPILE_LINE)

# Pattern rule for compiling .$(OBJEXT) files from .M files.
$(OBJDIR)/%.$(OBJEXT): %.M
ifdef WARN_MSG
	$(warning $(WARN_MSG))
endif
	$(OBJCXX_COMPILE_LINE)

JAVA_SRC=	$(srcdir)

ifdef JAVA_SRC_PATH
   JAVA_SRC=	$(srcdir):$(JAVA_SRC_PATH)
endif

ifdef ORB_JAR
   ifdef CLASSPATH
      DPP_CLASSPATH=	$(ORB_JAR):$(CLASSPATH)
   else
      DPP_CLASSPATH=	$(ORB_JAR)
   endif
else
   ifdef CLASSPATH
      DPP_CLASSPATH=	$(CLASSPATH)
   else
      DPP_CLASSPATH=	.
   endif
endif

$(OBJDIR)/$(JAVA_PKG)/%.class: %.java
	$(JAVAC) $(JAVAC_FLAGS) $(EXTRA_JAVAC_FLAGS)			\
          -classpath "$(DPP_CLASSPATH)" -d "$(OBJDIR)"			\
          -sourcepath ".:$(JAVA_SRC)" $<
