# =============================================================================
# dpp.compile.mk,v 1.1.1.1 2000/12/08 06:16:36 patrick Exp
#
# This include file <dpp.compile.mk> handles source code compiling.
# -----------------------------------------------------------------------------
# The Makefile including this file should define the following variables:
#
# OBJDIR        - The directory where the object file will be created.
# C_COMPILE     - The C compiler command line (without -c or -o <outfile>
#                 options).
# CXX_COMPILE   - The C++ compiler command line (without -c or -o <outfile>
#                 options).
# OBJEXT        - Suffix for object file names (e.g., "o" or "obj").
# OBJ_NAME_FLAG - Flag (with any extra options) used for naming the object
#                 file being compiled here.
# JAVAC         - The path to the Java compiler.
# JAVA_SRC_PATH - The path to .java files in a directory other than
#                 $(srcdir).
# ORB_JAR       - The .jar file providing an ORB implementation.
# =============================================================================

C_COMPILE_LINE		= $(C_COMPILE) $(OBJ_NAME_FLAG) -c $<
CXX_COMPILE_LINE	= $(CXX_COMPILE) $(OBJ_NAME_FLAG) -c $<

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

JAVA_SRC	= $(srcdir)

ifdef JAVA_SRC_PATH
    JAVA_SRC	= $(srcdir):$(JAVA_SRC_PATH)
endif

ifdef ORB_JAR
    ifdef CLASSPATH
        DPP_CLASSPATH	= $(ORB_JAR):$(CLASSPATH)
    else
        DPP_CLASSPATH	= $(ORB_JAR)
    endif
else
    ifdef CLASSPATH
        DPP_CLASSPATH	= $(CLASSPATH)
    else
        DPP_CLASSPATH	= .
    endif
endif

$(OBJDIR)/$(JAVA_PKG)/%.class: %.java
	$(JAVAC) -classpath $(DPP_CLASSPATH) -d $(OBJDIR)		\
          -sourcepath .:$(JAVA_SRC) $<
