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
# File:          dpp.libs.targets.mk,v
# Date modified: 2001/06/29 23:47:52
# Version:       1.5
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# =============================================================================
# dpp.libs.targets.mk,v 1.5 2001/06/29 23:47:52 patrickh Exp
#
# This file <dpp.libs.targets.mk> defines many targets for use in compiling a
# software library (or a set of libraries).  It should not be included
# directly by a user makefile but instead should be included by any Doozer++
# .mk file that wants to build libraries.  Refer to dpp.libs.mk and
# dpp.libs-basic.mk for examples.  The including .mk file must set several
# variables used within this file.  They are described next.
# -----------------------------------------------------------------------------
# The makefile including this file must define the following variables:
#
# topdir         - The top of the build tree.
# MKINSTALLDIRS  - Path to shell script for making directories.
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
#
# DBG_DIR        - Name of the directory where debugging objects and libraries
#                  are built.
# OPT_DIR        - Name of the directory where optimized objects and libraries
#                  are built.
# DEFAULT_DIR    - Default diretory for installations.
#
# DBG_FLAGS      - Compiler debugging flags.  If not specified, it defaults to
#                  -D_DEBUG.
# OPT_FLAGS      - Compiler optimizing flags.  If not specified, it defaults
#                  to -D_OPT.
#
# BUILDDIR       - Full destination path for the compiiled objects.
# DBG_BUILDDIR   - Full destination path for the compiled debugging
#                  objects.
# OPT_BUILDDIR   - Full destination path for the compiled optimizied
#                  objects.
#
# LIBDIR         - Full destination path for the compiled libraries.
# DBG_LIBDIR     - Full destination path for the compiled debugging
#                  libraries.
# OPT_LIBDIR     - Full destination path for the compiled optimized
#                  libraries.
#
# EXEC_PERMS     - UNIX-style permissions for executable files.
# FILE_PERMS     - UNIX-style permissions for normal (non-executable) files.
# UMASK          - The permissions mask to use when installing files.
#
# Optionally, it can define the following variables for added functionality:
#
# GROUP_OPT_UNIX      - The argument given to UNIX-based commands for changing
#                       the group ownersihp on files and directories.  This
#                       must be of the form "-g <group name>".
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
#                       version of the libraries (dbg, dso, etc.).
# POSTINSTALL         - Extra steps to take after installing a specific
#                       version of the libraries (opt, ddso, etc.).
#                      
# PREINSTALL_HEADERS  - Extra steps to take before installing the header
#                       files.
# POSTINSTALL_HEADERS - Extra steps to take after installing the header files.
#
# EXTRA_OBJ_TARGETS   - Extra targets to build when making object files.
#
# INSTALL_LIBDIR_ABS  - Absolute path for the library installation directory.
#                       If not specified, it defaults to
#                       $(libdir)$(LIBBITSUF).
# INSTALL_LIBDIR_REL  - Relative path for the library installation directory.
#                       This is used for making relative symlinks within
#                       $(INSTALL_LIBDIR_ABS).  If not specified, it defaults
#                       to ./.
# MAKE_REL_SYMLINKS   - If defined, relative symlinks in $(INSTALL_LIBDIR_REL)
#                       will be made.  Otherwise, this installation step is
#                       skipped.
# -----------------------------------------------------------------------------
# Targets provided are:
#
# all             - Build everything.
# all-abi         - Build everything using all supported ABIs.
# debug           - Build the libraries (dynamic and static) with debugging
#                   symbols.
# dbg             - Build the debugging, static copy of the libraries.
# ddso            - Build the debugging, dynamic shared object version of the
#                   libraries.
# optim           - Build the libraries (dynamic and static) with optimization
#                   enabled.
# opt             - Build the optimized, static version of the libraries.
# dso             - Build the dynamic shared object version of the libraries.
# obj             - Build the object files using the default value for
#                   $(OBJDIR).
#
# install-all     - Install all versions of the libraries, all header files,
#                   the Data directory, the configuration GUI and the test
#                   code.
# install-all-abi - Install every ABI possible on the target platform.
# install         - Install the full debugging version of the libraries (both
#                   static and dynamic libraries). 
# install-debug   - Same as 'install'.
# install-dbg     - Install the debugging, static version of the libraries.
# install-ddso    - Install the debugging, dynamic shared object version of
#                   the libraries.
# install-optim   - Install the full optimized version of the libraries (both
#                   static and dynamic libraries).
# install-opt     - Install the optimized, static version of the libraries.
# install-dso     - Install the dynamic shared object version of the
#                   libraries.
# install-headers - Install only the header files.
# =============================================================================

INSTALL_LIBDIR_ABS ?= $(libdir)$(LIBBITSUF)
INSTALL_LIBDIR_REL ?= ./

# =============================================================================
# The steps for building the 'dbg', 'ddso', 'opt' and 'dso' are as follows:
#
#    1) Build dependencies (if necessary).
#    2) Compile all object files that are out of date.
#    3) Compile the actual libraries from the object files.
#    4) Any user-defined steps.
# =============================================================================

# ------------------------------------------
# Build all four versions of the libraries.
# ------------------------------------------
all:
	@echo "==============================================================="
	@echo "Building everything"
	@echo "==============================================================="
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) libs $(EXTRA_OBJ_TARGETS)
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
              echo "Building libraries using $$abi" ;			\
              echo "------------------------------------------------" ;	\
              $(MAKE) dppABI=$$abi libs ;				\
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

# ---------------------------------------------
# Make the debugging version of the libraries.
# ---------------------------------------------
dbg:
	@echo "========================================================"
	@echo "Making static debugging version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) obj.dbg.build $(EXTRA_OBJ_TARGETS)
	@$(MAKE) lib.static.dbg.build
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# ------------------------------------------------------------
# Make the debugging version of the libraries that uses DSOs.
# ------------------------------------------------------------
ddso:
	@echo "========================================================"
	@echo "Making dynamic debugging version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) obj.dbg.build $(EXTRA_OBJ_TARGETS)
	@$(MAKE) lib.dynamic.dbg.build
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# ---------------------------------------------
# Make the optimized version of the libraries.
# ---------------------------------------------
opt:
	@echo "========================================================"
	@echo "Making static optimized version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) obj.opt.build $(EXTRA_OBJ_TARGETS)
	@$(MAKE) lib.static.opt.build
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# ------------------------------------------------------------
# Make the optimized version of the libraries that uses DSOs.
# ------------------------------------------------------------
dso:
	@echo "========================================================"
	@echo "Making dynamic optimized version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) obj.opt.build $(EXTRA_OBJ_TARGETS)
	@$(MAKE) lib.dynamic.opt.build
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# Build the optimized and debugging object files, static libraries and
# dynamic libraries.  Once this has been done, make the links in $(LIBDIR).
libs:
ifdef PRELIB
	@$(MAKE) $(PRELIB)
endif
	@$(MAKE) obj.opt.build
	@$(MAKE) lib.static.opt.build
	@$(MAKE) lib.dynamic.opt.build
	@$(MAKE) obj.dbg.build
	@$(MAKE) lib.static.dbg.build
	@$(MAKE) lib.dynamic.dbg.build
	@$(MAKE) _LIB_LINK_DIR="$(LIBDIR)" lib.links.build
ifdef POSTLIB
	@$(MAKE) $(POSTLIB)
endif

# Make symlinks in $(_LIB_LINK_DIR) pointing to the library binaries in
# $(DEFAULT_DIR).
lib.links.build:
	@echo "------------------------------------------------"
	@echo "Creating symlinks in $(_LIB_LINK_DIR)"
	@echo "------------------------------------------------"
	@for lib in $(LIBS) ; do					\
            cd $(_LIB_LINK_DIR) && rm -f ./$$lib.$(STATICLIB_EXT) &&	\
              $(LN_S) $(DEFAULT_DIR)/$$lib.$(STATICLIB_EXT) ./ ;	\
            cd $(_LIB_LINK_DIR) && rm -f ./$$lib.$(DYNAMICLIB_EXT) &&	\
              $(LN_S) $(DEFAULT_DIR)/$$lib.$(DYNAMICLIB_EXT) ./ ;	\
          done

# ----------------------------------------------------------------------------
# Make the debugging version of the libraries building both static and
# dynamic versions of the library binaries.
# ----------------------------------------------------------------------------
debug:
	@echo "========================================================"
	@echo "Making debugging version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) obj.dbg.build $(EXTRA_OBJ_TARGETS)
	@$(MAKE) lib.static.dbg.build
	@$(MAKE) lib.dynamic.dbg.build
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# ----------------------------------------------------------------------------
# Make the optimized version of the libraries building both static and
# dynamic versions of the library binaries.
# ----------------------------------------------------------------------------
optim:
	@echo "========================================================"
	@echo "Making optimized version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) $(BEFOREBUILD)
endif
	@$(MAKE) obj.opt.build $(EXTRA_OBJ_TARGETS)
	@$(MAKE) lib.static.opt.build
	@$(MAKE) lib.dynamic.opt.build
ifdef AFTERBUILD
	@$(MAKE) $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# --------------------------------------------------------------------
# Build only the object files using the default values for $(OBJDIR).
# --------------------------------------------------------------------
obj:
	@echo "------------------------------------------------"
	@echo "Making library object files"
	@echo "------------------------------------------------"
	@$(MAKE) RECTARGET="$@" recursive
	@echo "------------------------------------------------"
	@echo "Object files built"
	@echo "------------------------------------------------"

# Build the object files with the debugging flags enabled.
obj.dbg.build:
	@echo "------------------------------------------------"
	@echo "Building object files with debugging symbols"
	@echo "------------------------------------------------"
	@$(SHELL) $(MKINSTALLDIRS) $(DBG_BUILDDIR)
	@for lib in $(LIBS) ; do					\
              $(SHELL) $(MKINSTALLDIRS) $(DBG_BUILDDIR)/$$lib ;		\
          done
	@$(MAKE) RECTARGET="dbg" OPTIMIZER="$(DBG_FLAGS)"		\
          BASE_OBJDIR="$(DBG_BUILDDIR)" recursive

# Build the object files with the optimization flags enabled.
obj.opt.build:
	@echo "------------------------------------------------"
	@echo "Building optimized object files"
	@echo "------------------------------------------------"
	@$(SHELL) $(MKINSTALLDIRS) $(OPT_BUILDDIR)
	@for lib in $(LIBS) ; do					\
              $(SHELL) $(MKINSTALLDIRS) $(OPT_BUILDDIR)/$$lib ;		\
          done
	@$(MAKE) RECTARGET="opt" OPTIMIZER="$(OPT_FLAGS)"		\
          BASE_OBJDIR="$(OPT_BUILDDIR)" recursive

# Build the static libraries with the debugging flags enabled.
lib.static.dbg.build:
	@echo "================================================"
	@echo "Building static debugging libraries"
	@echo "================================================"
ifneq ("$(STATIC_LIBS)", "")
	@for lib in $(STATIC_LIBS) ; do					\
            $(MAKE) OBJDIR="$(DBG_BUILDDIR)/$$lib" LIBDIR="$(DBG_LIBDIR)" \
              $(DBG_LIBDIR)/$$lib.$(STATICLIB_EXT) ;			\
          done
endif

# Build the dynamic libraries with the debugging flags enabled.
lib.dynamic.dbg.build:
	@echo "================================================"
	@echo "Building dynamic debugging libraries"
	@echo "================================================"
ifneq ("$(DYNAMIC_LIBS)", "")
	@for lib in $(DYNAMIC_LIBS) ; do				\
            $(MAKE) OBJDIR="$(DBG_BUILDDIR)/$$lib" LIBDIR="$(DBG_LIBDIR)" \
              $(DBG_LIBDIR)/$$lib.$(DYNAMICLIB_EXT) ;			\
          done
endif

# Build the static libraries with the optimization flags enabled.
lib.static.opt.build:
	@echo "================================================"
	@echo "Building static optmized libraries"
	@echo "================================================"
ifneq ("$(STATIC_LIBS)", "")
	@for lib in $(STATIC_LIBS) ; do					\
            $(MAKE) OBJDIR="$(OPT_BUILDDIR)/$$lib" LIBDIR="$(OPT_LIBDIR)" \
              $(OPT_LIBDIR)/$$lib.$(STATICLIB_EXT) ;			\
          done
endif

# Build the static libraries with the optimization flags enabled.
lib.dynamic.opt.build:
	@echo "================================================"
	@echo "Building dynamic optmized libraries"
	@echo "================================================"
ifneq ("$(DYNAMIC_LIBS)", "")
	@for lib in $(DYNAMIC_LIBS) ; do				\
            $(MAKE) OBJDIR="$(OPT_BUILDDIR)/$$lib" LIBDIR="$(OPT_LIBDIR)" \
              $(OPT_LIBDIR)/$$lib.$(DYNAMICLIB_EXT) ;			\
          done
endif

# =============================================================================
# Installation targets.  The default (what is done by 'install') is to install
# the static debugging version of the libraries.  The steps taken for a full
# installation are:
#
#     1) Create the full installation directory hierarchy.
#     2) Install the libraries.
#     3) Install the header files.
#     4) Any user-defined installation steps.
# =============================================================================

# ------------------------------------------------------------------------
# Do a full installation all versions of the libraries and the associated
# files.
# ------------------------------------------------------------------------
install-all:
	@echo "==============================================================="
	@echo "Installing everything"
	@echo "==============================================================="
ifdef BEFOREINSTALL
	@$(MAKE) $(BEFOREINSTALL)
endif
	@$(MAKE) install-libs
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# -----------------------------------------------------------------------------
# Do a full installation of all versions of the libraries built in each of the
# ABIs in $(ABI_LIST).
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
              echo "Installing $$abi version of libraries" ;		\
              echo "------------------------------------------------" ;	\
              $(MAKE) dppABI=$$abi install-libs ;			\
          done
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# Install all versions of the libraries (static optimized, dynamic optimized,
# static debugging and dynamic debugging).  The final step of this is to
# make the syminks using the default value for $(DEFAULT_DIR).  None of the
# library installation targets are allowed to make their symlinks through
# the use of the $(NOLINK) variable.
install-libs:
	@$(MAKE) NOLINK=1 install-opt
	@$(MAKE) NOLINK=1 install-dso
	@$(MAKE) NOLINK=1 install-dbg
	@$(MAKE) NOLINK=1 install-ddso
	@$(MAKE) lib.links.install

# Do everything needed after installing the library binaries.
do-post-install:
	@$(MAKE) install-headers
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
	@$(MAKE) install-dbg
	@$(MAKE) install-ddso
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# ------------------------------------------------------------
# Install only the static debugging version of the libraries.
# ------------------------------------------------------------
install-dbg:
	@echo "------------------------------------------------"
	@echo "Installing static debugging libraries"
	@echo "------------------------------------------------"
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
ifneq ("$(STATIC_LIBS)", "")
	@for lib in $(STATIC_LIBS) ; do					\
            echo "$(DBG_LIBDIR)/$$lib.$(STATICLIB_EXT) ==> $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/" ; \
            $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
              $(DBG_LIBDIR)/$$lib.$(STATICLIB_EXT)			\
              $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/ ;			\
          done
	@$(MAKE) DEFAULT_DIR="$(DBG_DIR)" lib.links.install
endif

# -------------------------------------------------------------
# Install only the dynamic debugging version of the libraries.
# -------------------------------------------------------------
install-ddso:
	@echo "------------------------------------------------"
	@echo "Installing dynamic debugging libraries"
	@echo "------------------------------------------------"
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
ifneq ("$(DYNAMIC_LIBS)", "")
	@for lib in $(DYNAMIC_LIBS) ; do				\
            echo "$(DBG_LIBDIR)/$$lib.$(DYNAMICLIB_EXT) ==> $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/" ; \
            $(INSTALL) -m $(EXEC_PERMS) $(GROUP_OPT_UNIX)		\
              $(DBG_LIBDIR)/$$lib.$(DYNAMICLIB_EXT)			\
              $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/ ;			\
          done
	@$(MAKE) DEFAULT_DIR="$(DBG_DIR)" lib.links.install
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
	@$(MAKE) install-opt
	@$(MAKE) install-dso
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# ------------------------------------------------------------
# Install only the static optimized version of the libraries.
# ------------------------------------------------------------
install-opt:
	@echo "------------------------------------------------"
	@echo "Installing static optimized libraries"
	@echo "------------------------------------------------"
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
ifneq ("$(STATIC_LIBS)", "")
	@for lib in $(STATIC_LIBS) ; do					\
            echo "$(OPT_LIBDIR)/$$lib.$(STATICLIB_EXT) ==> $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/" ; \
            $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
              $(OPT_LIBDIR)/$$lib.$(STATICLIB_EXT)			\
              $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/ ;			\
          done
	@$(MAKE) DEFAULT_DIR="$(OPT_DIR)" lib.links.install
endif

# -------------------------------------------------------------
# Install only the dynamic optimized version of the libraries.
# -------------------------------------------------------------
install-dso:
	@echo "------------------------------------------------"
	@echo "Installing dynamic optimizied libraries"
	@echo "------------------------------------------------"
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
ifneq ("$(DYNAMIC_LIBS)", "")
	@for lib in $(DYNAMIC_LIBS) ; do				\
            echo "$(OPT_LIBDIR)/$$lib.$(DYNAMICLIB_EXT) ==> $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/" ; \
            $(INSTALL) -m $(EXEC_PERMS) $(GROUP_OPT_UNIX)		\
              $(OPT_LIBDIR)/$$lib.$(DYNAMICLIB_EXT)			\
              $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/ ;			\
          done
	@$(MAKE) DEFAULT_DIR="$(OPT_DIR)" lib.links.install
endif

# Install symlinks to the installed library binaries in the destination
# directory.
lib.links.install:
ifndef NOLINK
ifdef MAKE_REL_SYMLINKS
	@echo "------------------------------------------------"
	@echo "Creating symlinks in $(libdir)$(LIBBITSUF)"
	@echo "------------------------------------------------"
	@for lib in $(LIBS) ; do					\
            for ext in $(DYNAMICLIB_EXT) $(STATICLIB_EXT) ; do		\
                echo "$$lib.$$ext -> $(INSTALL_LIBDIR_REL)/$(DEFAULT_DIR)/$$lib.$$ext" ; \
                cur_dir=`pwd` ;						\
                cd $(libdir)$(LIBBITSUF) && umask $(UMASK) &&		\
                  rm -f ./$$lib.$$ext &&				\
                  $(LN_S) $(INSTALL_LIBDIR_REL)/$(DEFAULT_DIR)/$$lib.$$ext ./ ; \
                cd "$$cur_dir" ;					\
            done ;							\
          done
ifdef DYNAMICLIB_VER
	@vals=`echo $(DYNAMICLIB_VER) | sed -e 's/\./ /g' -e 's/^ *//'` ; \
          if [ "x$$vals" != "x" ]; then					\
              for val in $$vals ; do					\
              done
          fi
endif
endif
	@echo "------------------------------------------------"
	@echo "Creating symlinks in $(INSTALL_LIBDIR_ABS)"
	@echo "------------------------------------------------"
	@for lib in $(LIBS) ; do					\
            for ext in $(DYNAMICLIB_EXT) $(STATICLIB_EXT) ; do		\
                echo "$$lib.$$ext -> $(DEFAULT_DIR)/$$lib.$$ext" ;	\
                cur_dir=`pwd` ;						\
                cd $(INSTALL_LIBDIR_ABS) && umask $(UMASK) &&		\
                  rm -f ./$$lib.$$ext &&				\
                  $(LN_S) $(DEFAULT_DIR)/$$lib.$$ext ./ ;		\
                cd "$$cur_dir" ;					\
            done ;							\
          done
ifdef DYNAMICLIB_VER
endif
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

CLEAN_DIRS	+= $(DBG_BUILDDIR) $(OPT_BUILDDIR)
