# =============================================================================
# dpp.dep.mk,v 1.2 2000/12/19 17:10:35 patrick Exp
#
# This include file <dpp.dep.mk> handles source code dependencies.  It
# generates makefiles corresponding to each source file (ending in .c or .cpp)
# that have the dependencies for that source file.  The generated file can
# then be included in the Makefile for the directory.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variables:
#
# C_COMPILE    - The compiler command for C files.
# CXX_COMPILE  - The compiler command for C++ files.
# DEP_GEN_FLAG - The flag passed to the compiler to generate dependencies.
# OBJDIR       - The directory to which the object file(s) will be written.
# OBJEXT       - Suffix for object file names (e.g., "o" or "obj").
# SRCS         - The list of source files for which dependencies are to be
#                generated.
#
# Example:
#         srcdir = /usr/src/proj1
#         OBJDIR = /usr/obj/proj1
#   DEP_GEN_FLAG = -M
#   DEPEND_FILES = file1.d file2.d file3.d
#
# With these settings, the source code comes from /usr/src/proj1 and the
# object files go into /usr/obj/proj1.
# =============================================================================

# Conditionally set $(OBJDIR) and $(DEPDIR) just to be safe.
OBJDIR		?= .
DEPDIR		?= .

DEPEND_FILES	:= $(addprefix $(DEPDIR)/, $(BASIC_OBJECTS:.$(OBJEXT)=.d))

ifdef DO_CLEANDEPEND
    CLEAN_FILES	+= $(DEPEND_FILES)
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
_CC_SED_EXP	= '\''s|\($*\)\.$(OBJEXT)[ :]*|$$(OBJDIR)/\1.$(OBJEXT) $@: |g'\''
_MKDEP_SED_EXP	= '\''s|.*\($*\)\.$(OBJEXT)[ :]*|$$(OBJDIR)/\1.$(OBJEXT) $@: |g'\''

# Determine, based on the C compiler, how to generate the dependencies.  If
# the compiler is cl (Windows only), use makedepend(1).  Otherwise, we can use
# the compiler itself to do the job.
ifeq ($(CC), cl)
    _C_DEPGEN	= $(SHELL) -ec 'makedepend -f- -o.$(OBJEXT)		\
                      $(DEPEND_FLAGS) -- $(DEPEND_EXTRAS) -- $< |	\
                      sed $(_MKDEP_SED_EXP) > $@ ; [ -s $@ ] || rm -f $@'
    _CXX_DEPGEN	= $(SHELL) -ec 'makedepend -f- -o.$(OBJEXT)		\
                      $(DEPEND_FLAGS) -D__cplusplus -- $(DEPEND_EXTRAS)	\
                      -- $< | sed $(_MKDEP_SED_EXP) > $@ ; [ -s $@ ] ||	\
                      rm -f $@'
else
    _C_DEPGEN	= $(SHELL) -ec '$(C_COMPILE) $(DEP_GEN_FLAG) $< |	\
                      sed $(_CC_SED_EXP) > $@ ; [ -s $@ ] || rm -f $@'
    _CXX_DEPGEN	= $(SHELL) -ec '$(CXX_COMPILE) $(DEP_GEN_FLAG) $< |	\
                      sed $(_CC_SED_EXP) > $@ ; [ -s $@ ] || rm -f $@'
endif

$(DEPDIR)/%.d: %.c
	@echo "Updating dependency file $@ ..."
	@$(_C_DEPGEN)

$(DEPDIR)/%.d: %.C
	@echo "Updating dependency file $@ ..."
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.CC
	@echo "Updating dependency file $@ ..."
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.cc
	@echo "Updating dependency file $@ ..."
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.c++
	@echo "Updating dependency file $@ ..."
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.cpp
	@echo "Updating dependency file $@ ..."
	@$(_CXX_DEPGEN)

$(DEPDIR)/%.d: %.cxx
	@echo "Updating dependency file $@ ..."
	@$(_CXX_DEPGEN)
