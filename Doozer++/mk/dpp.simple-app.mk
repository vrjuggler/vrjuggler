# ************** <auto-copyright.pl BEGIN do not edit this line> **************
# Doozer++ is (C) Copyright 2000-2005 by Iowa State University
#
# Original Author:
#   Patrick Hartling
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
# File:          dpp.simple-app.mk,v
# Date modified: 2005/01/08 22:44:43
# Version:       1.8
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# =============================================================================
# dpp.simple-app.mk,v 1.8 2005/01/08 22:44:43 patrickh Exp
#
# This file <dpp.simple-app.mk> defines many targets for use in compiling a
# single application.
# -----------------------------------------------------------------------------
# The makefile including this file must define the following variables:
#
# topdir         - The top of the working tree.  If not specified, this
#                  defaults to the current directory.
# builddir       - The top of the build tree.  If not specified, this defaults
#                  to $(topdir).
# MKINSTALLDIRS  - Path to shell script for making directories.
#
# APP_NAME       - The name of the application executable.
# EXEEXT         - Extension given to executable files (.exe, for example).
#                  This may be empty on some platforms such as UNIX.
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
# PLATFORM       - The operating system abbreviation on which the application
#                  is being built.
#
# DBG_FLAGS      - Compiler debugging flags.
# OPT_FLAGS      - Compiler optimizing flags.
# PROF_OPT_FLAGS - Compiler optimizing flags to use when building profiled
#                  binaries.
#
# BUILDDIR       - Full destination path for the compiiled objects.
# DBG_BUILDDIR   - Full destination path for the compiled debugging objects.
# OPT_BUILDDIR   - Full destination path for the compiled optimizied objects.
# PROF_BUILDDIR  - Full destination path for the compiled profiled objects.
#
# EXEC_PERMS     - UNIX-style permissions for executable files.
# FILE_PERMS     - UNIX-style permissions for normal (non-executable) files.
# UMASK          - The permissions mask to use when installing files.
#
# Optionally, it can define the following variables for added functionality:
#
# APP_PROF_EXT        - An extension to $(APP_NAME) used to identify the
#                       binary as having profiling support.  This is
#                       appended to $(APP_NAME) before $(EXEEXT).
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
# PREAPP              - Extra steps to take at the beginning of the 'app'
#                       target.
# POSTAPP             - Extra steps to take after running the 'app' target.
#
# BEFOREINSTALL       - Extra steps to take before installing a full version
#                       of the application (e.g., debug).
# AFTERINSTALL        - Extra steps to take after installing a full version
#                       of the application (e.g., optim).
# PREINSTALL          - Extra steps to take before installing a specific
#                       version of the application (dbg, dbg-dso, etc.).
# POSTINSTALL         - Extra steps to take after installing a specific
#                       version of the application (opt, opt-dso, etc.).
#                      
# PREINSTALL_HEADERS  - Extra steps to take before installing the header
#                       files.
# POSTINSTALL_HEADERS - Extra steps to take after installing the header files.
#
# DBG_DIR             - Name of the directory where debugging objects are
#                       built.  If not specified, it defaults to "debug".
# OPT_DIR             - Name of the directory where optimized objects are
#                       are built.  If not specified, it defaults to "opt".
# PROF_DIR            - Name of the directory where profiled objects are
#                       built.  If not specified, it defaults to "profiled".
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
#                       $(builddir)/$(OBJDIR_NAME).
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
# EXTRA_OBJ_TARGETS   - Extra targets to build when making object files.
# -----------------------------------------------------------------------------
# Targets provided are:
#
# all              - Build everything.
# all-abi          - Build everything using all supported ABIs.
# debug            - Build the application with debugging symbols.
# optim            - Build the application with optimization enabled.
# profiled         - Build the application with profiling enabled.
# obj              - Build the object files using the default value for
#                    $(OBJDIR).
#
# install-all      - Install all versions of the application, all header files,
#                    the Data directory, the configuration GUI and the test
#                    code.
# install-all-abi  - Install every ABI possible on the target platform.
# install          - Install the full debugging version of the application.
# install-debug    - Same as 'install'.
# install-optim    - Install the full optimized version of the application.
# install-profiled - Install the full profiled version of the application.
# install-headers  - Install only the header files.
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
builddir?=		$(topdir)
OBJDIR_NAME?=		obj
BUILDDIR_BASE?=		$(builddir)/$(OBJDIR_NAME)
BUILDDIR?=		$(BUILDDIR_BASE)/$(PLATFORM)/$(ABI)/$(ISA)
DBG_BUILDDIR?=		$(BUILDDIR)/$(DBG_DIR)
OPT_BUILDDIR?=		$(BUILDDIR)/$(OPT_DIR)
PROF_BUILDDIR?= 	$(BUILDDIR)/$(PROF_DIR)

ifdef SRCS
include $(MKPATH)/dpp.obj-common.mk
include $(MKPATH)/dpp.compile.mk
include $(MKPATH)/dpp.dep.mk
include $(MKPATH)/dpp.idl.mk
include $(MKPATH)/dpp.clean.mk
endif

# =============================================================================
# The steps for building the 'debug', 'optim', and 'profiled' are as follows:
#
#    1) Build dependencies (if necessary).
#    2) Compile all object files that are out of date.
#    3) Compile the actual application from the object files.
#    4) Any user-defined steps.
# =============================================================================

# ---------------------------------------
# Build all versions of the application.
# ---------------------------------------
all:
	@echo "==============================================================="
	@echo "Building everything"
	@echo "==============================================================="
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) app $(EXTRA_OBJ_TARGETS)
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "==============================================================="
	@echo "$@ complete"
	@echo "==============================================================="

# -------------------------------
# Build all ABIs in $(ABI_LIST).
# -------------------------------
all-abi:
	@echo "==============================================================="
	@echo "Building everything using all ABIs"
	@echo "==============================================================="
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@for abi in $(ABI_LIST) ; do					\
              echo "------------------------------------------------" ;	\
              echo "Building application using $$abi" ;			\
              echo "------------------------------------------------" ;	\
              $(MAKE) dppABI=$$abi app ;				\
          done
ifdef EXTRA_OBJ_TARGETS
	@$(MAKE) $(EXTRA_OBJ_TARGETS)
endif
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "==============================================================="
	@echo "$@ complete"
	@echo "==============================================================="

# -----------------------------------------------
# Make the debugging version of the application.
# -----------------------------------------------
debug:
	@echo "========================================================"
	@echo "Making debugging version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) obj.dbg.build $(EXTRA_OBJ_TARGETS)
	@$(MAKE) app.dbg.build
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# -----------------------------------------------
# Make the optimized version of the application.
# -----------------------------------------------
optim:
	@echo "========================================================"
	@echo "Making static optimized version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) obj.opt.build $(EXTRA_OBJ_TARGETS)
	@$(MAKE) app.opt.build
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# ----------------------------------------------
# Make the profiled version of the application.
# ----------------------------------------------
profiled:
	@echo "========================================================"
	@echo "Making static profiled version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) obj.prof.build $(EXTRA_OBJ_TARGETS)
	@$(MAKE) app.prof.build
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# Build the optimized, debugging, (optionally) profiled object files, and the
# application.
app:
ifdef PREAPP
	@$(MAKE) $(PREAPP)
endif
	@$(MAKE) obj.opt.build
	@$(MAKE) app.opt.build
	@$(MAKE) obj.dbg.build
	@$(MAKE) app.dbg.build
ifdef BUILD_PROF_APP
	@$(MAKE) obj.prof.build
	@$(MAKE) app.prof.build
endif
ifdef POSTAPP
	@$(MAKE) $(POSTAPP)
endif

# --------------------------------------------------------------------
# Build only the object files using the default values for $(OBJDIR).
# --------------------------------------------------------------------
obj:
	@echo "------------------------------------------------"
	@echo "Making application object files"
	@echo "------------------------------------------------"
ifdef SUBDIR
	@$(MAKE) RECTARGET="$@" recursive
endif
	@echo "------------------------------------------------"
	@echo "Object files built"
	@echo "------------------------------------------------"

# Build the object files with the debugging flags enabled.
obj.dbg.build:
	@echo "------------------------------------------------"
	@echo "Building object files with debugging symbols"
	@echo "------------------------------------------------"
	@$(SHELL) $(MKINSTALLDIRS) $(DBG_BUILDDIR)
ifdef OBJECTS
	@$(MAKE) DBG_OBJ_BUILD=1 OPTIMIZER="$(DBG_FLAGS)"		\
          BASE_OBJDIR="$(DBG_BUILDDIR)" do-build
endif
ifdef SUBDIR
	@$(MAKE) RECTARGET="dbg" OPTIMIZER="$(DBG_FLAGS)"		\
          DBG_OBJ_BUILD=1 BASE_OBJDIR="$(DBG_BUILDDIR)" recursive
endif

# Build the object files with the optimization flags enabled.
obj.opt.build:
	@echo "------------------------------------------------"
	@echo "Building optimized object files"
	@echo "------------------------------------------------"
	@$(SHELL) $(MKINSTALLDIRS) $(OPT_BUILDDIR)
ifdef OBJECTS
	@$(MAKE) OPT_OBJ_BUILD=1 OPTIMIZER="$(OPT_FLAGS)"		\
          BASE_OBJDIR="$(OPT_BUILDDIR)" do-build
endif
ifdef SUBDIR
	@$(MAKE) RECTARGET="opt" OPTIMIZER="$(OPT_FLAGS)"		\
          OPT_OBJ_BUILD=1 BASE_OBJDIR="$(OPT_BUILDDIR)" recursive
endif

# Build the object files with the profiling and optimization flags enabled.
obj.prof.build:
	@echo "------------------------------------------------"
	@echo "Building profiled object files"
	@echo "------------------------------------------------"
	@$(SHELL) $(MKINSTALLDIRS) $(PROF_BUILDDIR)
ifdef OBJECTS
	@$(MAKE) PROF_OBJ_BUILD=1 OPTIMIZER="$(PROF_OPT_FLAGS)"		\
          BASE_OBJDIR="$(PROF_BUILDDIR)" do-build
endif
ifdef SUBDIR
	@$(MAKE) RECTARGET="prof" OPTIMIZER="$(PROF_OPT_FLAGS)"		\
          PROF_OBJ_BUILD=1 BASE_OBJDIR="$(PROF_BUILDDIR)" recursive
endif

# Build the application with the debugging flags enabled.
app.dbg.build:
	@echo "================================================"
	@echo "Building debugging application"
	@echo "================================================"
	@$(MAKE) OBJDIR="$(DBG_BUILDDIR)" DBG_APP_BUILD=1		\
          EXTRA_LDOPTS="$(LDOPTS_DBG)" $(APP_NAME)$(EXEEXT)

# Build the application with the optimization flags enabled.
app.opt.build:
	@echo "================================================"
	@echo "Building optmized application"
	@echo "================================================"
	@$(MAKE) OBJDIR="$(OPT_BUILDDIR)" OPT_APP_BUILD=1		\
          EXTRA_LDOPTS="$(LDOPTS_OPT)" $(APP_NAME)$(EXEEXT)

# Build the application with the profiling flags enabled.
app.prof.build:
	@echo "================================================"
	@echo "Building profiled application"
	@echo "================================================"
	@$(MAKE) OBJDIR="$(PROF_BUILDDIR)" PROF_APP_BUILD=1		\
          EXTRA_LDOPTS="$(LDOPTS_PROF)" $(APP_NAME)$(APP_PROF_EXT)$(EXEEXT)

# =============================================================================
# Installation targets.  The default (what is done by 'install') is to install
# the debugging version of the application.  The steps taken for a full
# installation are:
#
#     1) Create the full installation directory hierarchy.
#     2) Install the application.
#     3) Any user-defined installation steps.
# =============================================================================

# --------------------------------------------------------------------------
# Do a full installation all versions of the application and the associated
# files.
# --------------------------------------------------------------------------
install-all:
	@echo "==============================================================="
	@echo "Installing everything"
	@echo "==============================================================="
ifdef BEFOREINSTALL
	@$(MAKE) $(BEFOREINSTALL)
endif
	@$(MAKE) install-app
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# -----------------------------------------------------------------------------
# Do a full installation of all versions of the application built in each of
# the ABIs in $(ABI_LIST).
# -----------------------------------------------------------------------------
install-all-abi:
	@echo "==============================================================="
	@echo "Installing everything using all ABIs"
	@echo "==============================================================="
ifdef BEFOREINSTALL
	@$(MAKE) $(BEFOREINSTALL)
endif
	@for abi in $(ABI_LIST) ; do					\
              echo "------------------------------------------------" ;	\
              echo "Installing $$abi version of application" ;		\
              echo "------------------------------------------------" ;	\
              $(MAKE) dppABI=$$abi install-app ;			\
          done
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# Install all versions of the application (optimized, debugging, profiled).
install-app:
	@$(MAKE) install-optim
	@$(MAKE) install-debug
ifdef BUILD_PROF_APP
	@$(MAKE) install-profiled
endif

# Do everything needed after installing the application binaries.
do-post-install:
ifdef POSTINSTALL
	@$(MAKE) $(POSTINSTALL)
endif

# ------------------------------------------------------------
# Install the debugging version.
# ------------------------------------------------------------
install install-debug:
	@echo "==============================================================="
	@echo "Installing debugging version"
	@echo "==============================================================="
ifdef BEFOREINSTALL
	@$(MAKE) $(BEFOREINSTALL)
endif
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
	$(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)			\
          $(APP_NAME)$(EXEEXT) $(bindir)
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# ------------------------------------------------------------
# Install the optimized version.
# ------------------------------------------------------------
install-optim:
	@echo "==============================================================="
	@echo "Installing optimized version"
	@echo "==============================================================="
ifdef BEFOREINSTALL
	@$(MAKE) $(BEFOREINSTALL)
endif
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
	$(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)			\
          $(APP_NAME)$(EXEEXT) $(bindir)
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# ------------------------------------------------------------
# Install the profiled version.
# ------------------------------------------------------------
install-profiled:
	@echo "==============================================================="
	@echo "Installing profiled version"
	@echo "==============================================================="
ifdef BEFOREINSTALL
	@$(MAKE) $(BEFOREINSTALL)
endif
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
	$(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)			\
          $(APP_NAME)$(APP_PROF_EXT)$(EXEEXT) $(bindir)
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# Install the header files.
install-headers:
	@echo "------------------------------------------------"
	@echo "Installing header files"
	@echo "------------------------------------------------"
ifdef PREINSTALL_HEADERS
	@$(MAKE) $(PREINSTALL_HEADERS)
endif
	@$(MAKE) RECTARGET="install" recursive
ifdef POSTINSTALL_HEADERS
	@$(MAKE) $(POSTINSTALL_HEADERS)
endif

CLEAN_DIRS+=	$(DBG_BUILDDIR) $(OPT_BUILDDIR) $(PROF_BUILDDIR)
