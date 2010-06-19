# Doozer++ is (C) Copyright 2000-2009 by Iowa State University
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
#
# Original Author:
#   Patrick Hartling

# =============================================================================
# This file <dpp.libs.mk> defines many targets for use in compiling a software
# library (or a set of libraries).  An including makefile can take advantage
# of this file by extending its logic and using the provided targets
# (documented below) within its own more complex, project-specific targets.
# -----------------------------------------------------------------------------
# The makefile including this file must define the following variables:
#
# topdir         - The top of the build tree.
# MKINSTALLDIRS  - Path to shell script for making directories.
# MKPATH         - The path to the Doozer++ .mk files.
#
# ABI            - Application Binary Interface type.  This may be used for
#                  other binary types when supported by the operating system
#                  (e.g., "ELF", "N32" or "64").
# ABI_LIST       - List of all possible ABI/ISA combinations that can be built
#                  on the target system.
# ISA            - Instruction Set Architecture.  This may be used for other
#                  binary types when supported by the operating system (e.g.,
#                  "mips3" or "i386").
# LIBBITSUF      - Suffix for the library path used by the linker on IRIX
#                  only.  For n32 binaries, it will be 32.  For 64-bit
#                  binaries, it will be 64.
# PLATFORM       - The operating system abbreviation on which the library is
#                  being built.
#
# DYNAMICLIB_EXT - File extension for dynamic libraries.
# STATICLIB_EXT  - File extension for shared libraries.
# PROFLIB_EXT    - File extension for profiled libraries.  This is inserted
#                  between the base library name and one of the above two
#                  extensions.
#
# Optionally, it can define the following variables for added functionality:
#
# EXEC_PERMS          - UNIX-style permissions for executable files.
# FILE_PERMS          - UNIX-style permissions for normal (non-executable)
#                       files.
# UMASK               - The permissions mask to use when installing files.  If
#                       not specified, it defaults to 002.
# GROUP_NAME          - The name of the group used for ownership of installed
#                       files.
# GROUP_OPT_UNIX      - The argument given to UNIX-based commands for changing
#                       the group ownersihp on files and directories.  This
#                       must be of the form "-g <group name>".  If
#                       $(GROUP_NAME) is given and a value for this variable
#                       is not specified, it defaults to -g $(GROUP_NAME).
#                       Otherwise, no value is defined.
#
# BEFOREBUILD         - Extra steps to take before any object files are built.
# AFTERBUILD          - Extra steps to take after all object files are built.
# PRELIB              - Extra steps to take at the beginning of the 'libs'
#                       target.
# POSTLIB             - Extra steps to take after running the 'libs' target.
#
# BEFOREINSTALL       - Extra steps to take before installing a full version
#                       of the libraries (e.g., debug).
# AFTERINSTALL        - Extra steps to take after installing a full version
#                       of the libraries (e.g., optim).
# PREINSTALL          - Extra steps to take before installing a specific
#                       version of the libraries (dbg, dbg-dso, etc.).
# POSTINSTALL         - Extra steps to take after installing a specific
#                       version of the libraries (opt, opt-dso, etc.).
#                      
# PREINSTALL_HEADERS  - Extra steps to take before installing the header
#                       files.
# POSTINSTALL_HEADERS - Extra steps to take after installing the header files.
#
# DBG_DIR             - Name of the directory where debugging objects and
#                       libraries are built.  If not specified, it defaults
#                       to "debug".
# OPT_DIR             - Name of the directory where optimized objects and
#                       libraries are built.  If not specified, it defaults
#                       to "opt".
# PROF_DIR            - Name of the directory where profiled objects and
#                       libraries are built.  If not specified, it defaults
#                       to "profiled".
# DEFAULT_DIR         - Default diretory for installations.  This should be
#                       either $(DBG_DIR) or $(OPT_DIR).  If not specified,
#                       it defaults to $(DBG_DIR).
#
# DBG_FLAGS           - Compiler debugging flags.  If not specified, it
#                       defaults to -D_DEBUG.
# OPT_FLAGS           - Compiler optimizing flags.  If not specified, it
#                       defaults to "-D_OPT -DNDEBUG".
# PROF_OPT_FLAGS      - Compiler optimizing flags to use when building profiled
#                       binaries.  If not specified, it defaults to
#                       $(OPT_FLAGS).
# PROF_FLAGS          - Compiler profiling flags.
#
# OBJDIR_NAME         - Name of the directory where objects will be compiled.
#                       If not specified, it defaults to obj.
# BUILDDIR_BASE       - Full destination path for the compiled objects.  If
#                       not specified, it defaults to
#                       $(topdir)/$(OBJDIR_NAME).
# BUILDDIR            - Full destination path for the compiiled objects for a
#                       given ISA.  If not specified, it defaults to
#                       $(BUILDDIR_BASE)/$(PLATFORM)/$(ABI)/$(ISA).
# DBG_BUILDDIR        - Full destination path for the compiled debugging
#                       objects.  If not specified, it defaults to
#                       $(BUILDDIR)/$(DBG_DIR).
# OPT_BUILDDIR        - Full destination path for the compiled optimizied
#                       objects.  If not specified, it defaults to
#                       $(BUILDDIR)/$(OPT_DIR).
# PROF_BUILDDIR       - Full destination path for the compiled profiled
#                       objects.  If not specified, it defaults to
#                       $(BUILDDIR)/$(PROF_DIR).
#
# LIBDIR_NAME         - Name of the directory where library binaries will be
#                       compiled.  If not specified, it defaults to
#                       lib$(LIBBITSUF).
# LIBDIR_BASE         - Full destination path for the compiled libraries.  If
#                       not specified, it defaults to
#                       $(topdir)/$(LIBDIR_NAME).
# LIBDIR              - Full destination path for the compiled libraries for a
#                       given ISA.  If not specified, it defaults to 
#                       $(LIBDIR_BASE)/$(ISA).
# DBG_LIBDIR          - Full destination path for the compiled debugging
#                       libraries.  If not specified, it defaults to
#                       $(LIBDIR)/$(DBG_DIR).
# OPT_LIBDIR          - Full destination path for the compiled optimized
#                       libraries.  If not specified, it defaults to
#                       $(LIBDIR)/$(OPT_DIR).
# PROF_LIBDIR         - Full destination path for the compiled profiled
#                       libraries.  If not specified, it defaults to
#                       $(LIBDIR)/$(PROF_DIR).
#
# EXTRA_OBJ_TARGETS   - Extra targets to build when making object files.
# -----------------------------------------------------------------------------
# Refer to the comments at the top of dpp.libs.targets.mk for the list of
# provided targets.
# =============================================================================

MKINSTALLDIRS?=		mkinstalldirs
DBG_FLAGS?=		-D_DEBUG
OPT_FLAGS?=		-D_OPT -DNDEBUG
PROF_OPT_FLAGS?=	$(OPT_FLAGS)

# =============================================================================
# Build and installation directory naming and permission information.
# =============================================================================
DBG_DIR?=	debug
OPT_DIR?=	opt
PROF_DIR?=	profiled
DEFAULT_DIR?=	$(DBG_DIR)

EXEC_PERMS?=	755
FILE_PERMS?=	644
UMASK?=		002

# If we have a name for the install group, define group-setting options for
# UNIX commands (chown(1), chgrp(1)).
ifdef GROUP_NAME
   GROUP_OPT_UNIX?=	-g $(GROUP_NAME)
endif

# ------------------------------------------------
# Object file build and installation directories.
# ------------------------------------------------
topdir?=		.
OBJDIR_NAME?=		obj
BUILDDIR_BASE?= 	$(topdir)/$(OBJDIR_NAME)
BUILDDIR?= 		$(BUILDDIR_BASE)/$(PLATFORM)/$(ABI)/$(ISA)
DBG_BUILDDIR?= 		$(BUILDDIR)/$(DBG_DIR)
OPT_BUILDDIR?=		$(BUILDDIR)/$(OPT_DIR)
PROF_BUILDDIR?= 	$(BUILDDIR)/$(PROF_DIR)

# --------------------------------------------
# Library build and installation directories.
# --------------------------------------------

# $(LIBDIR_NAME) is the name of the library directory including an ABI suffix
# if applicable.
LIBDIR_NAME?=	lib$(LIBBITSUF)

# $(LIBDIR_BASE) is the base directory where the libraries will be built.
LIBDIR_BASE?=	$(topdir)/$(LIBDIR_NAME)

# $(LIBDIR) is the first-level subdirectory of $(LIBDIR_BASE) that is
# named by the instruction set being used ($(ISA)).
LIBDIR?=	$(LIBDIR_BASE)/$(ISA)

# $(DBG_LIBDIR) and $(OPT_LIBDIR) are the directories where the libraries
# are actually built.  The first is for a debugging copy of the libraries and
# the second is for an optimized version of the libraries.
DBG_LIBDIR?=	$(LIBDIR)/$(DBG_DIR)
OPT_LIBDIR?=	$(LIBDIR)/$(OPT_DIR)
PROF_LIBDIR?=	$(LIBDIR)/$(PROF_DIR)

# $(INSTALL_LIBDIR_ABS) is the full path to the base directory (minus any
# debugging or optimized subdirectories) where the libraries will be
# installed.  $(INSTALL_LIBDIR_REL) is only the $(ISA) subdirectory of the
# full path.
INSTALL_LIBDIR_ABS=	$(libdir)$(LIBBITSUF)/$(ISA)
INSTALL_LIBDIR_REL=	./$(ISA)

MAKE_REL_SYMLINKS=	1

include $(MKPATH)/dpp.libs.targets.mk
