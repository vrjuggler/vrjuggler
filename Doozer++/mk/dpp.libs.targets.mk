# ************** <auto-copyright.pl BEGIN do not edit this line> **************
# Doozer++ is (C) Copyright 2000-2005 by Iowa State University
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
# Date modified: 2006/04/14 19:06:12
# Version:       1.38
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# =============================================================================
# dpp.libs.targets.mk,v 1.38 2006/04/14 19:06:12 patrickh Exp
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
# PROFLIB_EXT    - File extension for profiled libraries.  This is inserted
#                  between the base library name and one of the above two
#                  extensions.
#
# DBG_DIR        - Name of the directory where debugging objects and libraries
#                  are built.
# OPT_DIR        - Name of the directory where optimized objects and libraries
#                  are built.
# PROF_DIR       - Name of the directory where profiled objects and libraries
#                  are built.
# DEFAULT_DIR    - Default diretory for installations.
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
# LIBDIR         - Full destination path for the compiled libraries.
# DBG_LIBDIR     - Full destination path for the compiled debugging libraries.
# OPT_LIBDIR     - Full destination path for the compiled optimized libraries.
# PROF_LIBDIR    - Full destination path for the compiled profiled libraries.
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
#                       version of the libraries (dbg, dbg-dso, etc.).
# POSTINSTALL         - Extra steps to take after installing a specific
#                       version of the libraries (opt, opt-dso, etc.).
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
# all              - Build everything.
# all-abi          - Build everything using all supported ABIs.
# debug            - Build the libraries (dynamic and static) with debugging
#                    symbols.
# dbg              - Build the debugging, static copy of the libraries.
# dbg-dso          - Build the debugging, dynamic shared object version of the
#                    libraries.
# optim            - Build the libraries (dynamic and static) with optimization
#                    enabled.
# opt              - Build the optimized, static version of the libraries.
# opt-dso          - Build the dynamic shared object version of the libraries.
# profiled         - Build the libraries (dynamic and static) with profiling
#                    enabled.
# prof             - Build the profiled, static version of the libraries.
# prof-dso         - Build the profiled, dynamic shared object version of the
#                    libraries.
# obj              - Build the object files using the default value for
#                    $(OBJDIR).
#
# install-all      - Install all versions of the libraries, all header files,
#                    the Data directory, the configuration GUI and the test
#                    code.
# install-all-abi  - Install every ABI possible on the target platform.
# install          - Install the full debugging version of the libraries (both
#                    static and dynamic libraries). 
# install-debug    - Same as 'install'.
# install-dbg      - Install the debugging, static version of the libraries.
# install-dbg-dso  - Install the debugging, dynamic shared object version of
#                    the libraries.
# install-optim    - Install the full optimized version of the libraries (both
#                    static and dynamic libraries).
# install-opt      - Install the optimized, static version of the libraries.
# install-opt-dso  - Install the optimized, dynamic shared object version of
#                    the libraries.
# install-profiled - Install the profiled version of the libraries (both
#                    static and dynamic libraries).
# install-prof     - Install the profiled, static version of the libraries.
# install-prof-dso - Install the profiled, dynamic shared object version of
#                    the libraries.
# install-headers  - Install only the header files.
# =============================================================================

INSTALL_LIBDIR_ABS?=	$(libdir)$(LIBBITSUF)
INSTALL_LIBDIR_REL?=	./

# =============================================================================
# The steps for building the 'dbg', 'dbg-dso', 'opt' and 'opt-dso' are as
# follows:
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
	@$(MAKE) BUILD_TYPE="all" $(BEFOREBUILD)
endif
	@$(MAKE) libs
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="all" $(AFTERBUILD)
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
	@$(MAKE) BUILD_TYPE="all" $(BEFOREBUILD)
endif
	@for abi in $(ABI_LIST) ; do					\
              echo "------------------------------------------------" ;	\
              echo "Building libraries using $$abi" ;			\
              echo "------------------------------------------------" ;	\
              $(MAKE) dppABI=$$abi libs ;				\
          done
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="all" $(AFTERBUILD)
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
	@$(MAKE) BUILD_TYPE="dbg" $(BEFOREBUILD)
endif
	@$(MAKE) BUILD_TYPE="dbg" obj.dbg.build
	@$(MAKE) BUILD_TYPE="dbg" lib.static.dbg.build
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="dbg" $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# ------------------------------------------------------------
# Make the debugging version of the libraries that uses DSOs.
# ------------------------------------------------------------
dbg-dso:
	@echo "========================================================"
	@echo "Making dynamic debugging version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) BUILD_TYPE="dbg" $(BEFOREBUILD)
endif
	@$(MAKE) BUILD_TYPE="dbg" obj.dbg.build
	@$(MAKE) BUILD_TYPE="dbg" lib.dynamic.dbg.build
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="dbg" $(AFTERBUILD)
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
	@$(MAKE) BUILD_TYPE="opt" $(BEFOREBUILD)
endif
	@$(MAKE) BUILD_TYPE="opt" obj.opt.build
	@$(MAKE) BUILD_TYPE="opt" lib.static.opt.build
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="opt" $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# ------------------------------------------------------------
# Make the optimized version of the libraries that uses DSOs.
# ------------------------------------------------------------
opt-dso:
	@echo "========================================================"
	@echo "Making dynamic optimized version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) BUILD_TYPE="opt" $(BEFOREBUILD)
endif
	@$(MAKE) BUILD_TYPE="opt" obj.opt.build
	@$(MAKE) BUILD_TYPE="opt" lib.dynamic.opt.build
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="opt" $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# --------------------------------------------
# Make the profiled version of the libraries.
# --------------------------------------------
prof:
	@echo "========================================================"
	@echo "Making static profiled version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) BUILD_TYPE="prof" $(BEFOREBUILD)
endif
	@$(MAKE) BUILD_TYPE="prof" obj.prof.build
	@$(MAKE) BUILD_TYPE="prof" lib.static.prof.build
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="prof" $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# -----------------------------------------------------------
# Make the profiled version of the libraries that uses DSOs.
# -----------------------------------------------------------
prof-dso:
	@echo "========================================================"
	@echo "Making dynamic profiled version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) BUILD_TYPE="prof" $(BEFOREBUILD)
endif
	@$(MAKE) BUILD_TYPE="prof" obj.prof.build
	@$(MAKE) BUILD_TYPE="prof" lib.dynamic.prof.build
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="prof" $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# Build the optimized, debugging, and (optionally) profiled object files; the
# static libraries, and the dynamic libraries.  Once this has been done, make
# the links in $(LIBDIR).
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
ifdef BUILD_PROF_LIBS
	@$(MAKE) obj.prof.build
	@$(MAKE) lib.static.prof.build
	@$(MAKE) lib.dynamic.prof.build
	@$(MAKE) _LIB_LINK_DIR="$(LIBDIR)" _HAVE_PROF_LIBS=1 lib.links.build
else
	@$(MAKE) _LIB_LINK_DIR="$(LIBDIR)" lib.links.build
endif
ifdef POSTLIB
	@$(MAKE) $(POSTLIB)
endif

# Make symlinks in $(_LIB_LINK_DIR) pointing to the library binaries in
# $(DEFAULT_DIR).
lib.links.build:
	@$(MAKE) LINK_DIR=$(_LIB_LINK_DIR) DEST_DIR=. lib.links.generic

lib.links.generic:
	@echo "------------------------------------------------"
	@echo "Creating symlinks in $(LINK_DIR)"
	@echo "------------------------------------------------"
	@for LIB in $(LIBS) ; do					\
           cd $(LINK_DIR) ;						\
           for lib in $(PROF_STATIC_LIBS) ; do				\
              rm -f $$lib &&						\
                $(LN_S) $(DEST_DIR)/$(PROF_DIR)/$$lib $$lib ;		\
           done ;							\
           for lib in $(STATIC_LIBS) ; do				\
              rm -f $$lib &&						\
                $(LN_S) $(DEST_DIR)/$(DEFAULT_DIR)/$$lib $$lib ;	\
           done ;							\
           for lib in $(PROF_DYNAMIC_LIBS) ; do				\
              rm -f $$lib &&						\
                $(LN_S) $(DEST_DIR)/$(PROF_DIR)/$$lib $$lib ;		\
              for link in $(PROF_DYNAMIC_LIB_LINKS) ; do		\
                 rm -f $$link &&					\
                   $(LN_S) $(DEST_DIR)/$(PROF_DIR)/$$lib $$link ;	\
              done ;							\
           done ;							\
           for lib in $(DYNAMIC_LIBS) ; do				\
              rm -f $$lib &&						\
                $(LN_S) $(DEST_DIR)/$(DEFAULT_DIR)/$$lib $$lib ;	\
              for link in $(DYNAMIC_LIB_LINKS) ; do			\
                 rm -f $$link &&					\
                   $(LN_S) $(DEST_DIR)/$(DEFAULT_DIR)/$$lib $$link ;	\
              done ;							\
           done ;							\
        done
ifeq ($(OS_TYPE), Win32)
	@for LIB in $(LIBS) ; do					\
           cd $(LINK_DIR) ;						\
           for lib in $(PROF_DYNAMIC_LIBS) ; do				\
              base_lib=`echo $$lib | sed -e 's/\.$(DYNAMICLIB_EXT)$$//'` ; \
              rm -f $$base_lib.lib &&					\
                 $(LN_S) $(DEST_DIR)/$(PROF_DIR)/$$base_lib.lib $$base_lib.lib ;	\
              for link in $(PROF_DYNAMIC_LIB_LINKS) ; do		\
                 base_link=`echo $$link | sed -e 's/\.$(DYNAMICLIB_EXT)$$//'` ; \
                 rm -f $$base_link.lib &&				\
                    $(LN_S) $(DEST_DIR)/$(PROF_DIR)/$$base_lib.lib $$base_link.lib ;	\
              done ;							\
           done ;							\
           for lib in $(DYNAMIC_LIBS) ; do				\
              base_lib=`echo $$lib | sed -e 's/\.$(DYNAMICLIB_EXT)$$//'` ; \
              rm -f $$base_lib.lib &&		 			\
                 $(LN_S) $(DEST_DIR)/$(DEFAULT_DIR)/$$base_lib.lib $$base_lib.lib ;	\
              for link in $(DYNAMIC_LIB_LINKS) ; do			\
                 base_link=`echo $$link | sed -e 's/\.$(DYNAMICLIB_EXT)$$//'` ; \
                 rm -f $$base_link.lib &&				\
                    $(LN_S) $(DEST_DIR)/$(DEFAULT_DIR)/$$base_lib.lib $$base_link.lib ; \
              done ;							\
           done ;							\
        done
endif

# ----------------------------------------------------------------------------
# Make the debugging version of the libraries building both static and
# dynamic versions of the library binaries.
# ----------------------------------------------------------------------------
debug:
	@echo "========================================================"
	@echo "Making debugging version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) BUILD_TYPE="dbg" $(BEFOREBUILD)
endif
	@$(MAKE) BUILD_TYPE="dbg" obj.dbg.build
	@$(MAKE) BUILD_TYPE="dbg" lib.static.dbg.build
	@$(MAKE) BUILD_TYPE="dbg" lib.dynamic.dbg.build
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="dbg" $(AFTERBUILD)
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
	@$(MAKE) BUILD_TYPE="opt" $(BEFOREBUILD)
endif
	@$(MAKE) BUILD_TYPE="opt" obj.opt.build
	@$(MAKE) BUILD_TYPE="opt" lib.static.opt.build
	@$(MAKE) BUILD_TYPE="opt" lib.dynamic.opt.build
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="opt" $(AFTERBUILD)
endif
	@echo "========================================================"
	@echo "$@ complete"
	@echo "========================================================"

# ----------------------------------------------------------------------------
# Make the profiled version of the libraries building both static and
# dynamic versions of the library binaries.
# ----------------------------------------------------------------------------
profiled:
	@echo "========================================================"
	@echo "Making profiled version"
	@echo "========================================================"
ifdef BEFOREBUILD
	@$(MAKE) BUILD_TYPE="prof" $(BEFOREBUILD)
endif
ifdef BUILD_PROF_LIBS
	@$(MAKE) BUILD_TYPE="prof" obj.prof.build
	@$(MAKE) BUILD_TYPE="prof" lib.static.prof.build
	@$(MAKE) BUILD_TYPE="prof" lib.dynamic.prof.build
endif
ifdef AFTERBUILD
	@$(MAKE) BUILD_TYPE="prof" $(AFTERBUILD)
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
	@for lib in "" $(LIBS) ; do					\
              $(SHELL) $(MKINSTALLDIRS) $(DBG_BUILDDIR)/$$lib ;		\
          done
	@$(MAKE) RECTARGET="dbg" OPTIMIZER="$(DBG_FLAGS)"		\
          DBG_OBJ_BUILD=1 BASE_OBJDIR="$(DBG_BUILDDIR)"			\
          BUILD_TYPE="dbg" recursive
ifdef EXTRA_OBJ_TARGETS
	@$(MAKE) OPTIMIZER="$(DBG_FLAGS)" DBG_OBJ_BUILD=1		\
          BASE_OBJDIR="$(DBG_BUILDDIR)" $(EXTRA_OBJ_TARGETS)
endif

# Build the object files with the optimization flags enabled.
obj.opt.build:
	@echo "------------------------------------------------"
	@echo "Building optimized object files"
	@echo "------------------------------------------------"
	@for lib in "" $(LIBS) ; do					\
              $(SHELL) $(MKINSTALLDIRS) $(OPT_BUILDDIR)/$$lib ;		\
          done
	@$(MAKE) RECTARGET="opt" OPTIMIZER="$(OPT_FLAGS)"		\
          OPT_OBJ_BUILD=1 BASE_OBJDIR="$(OPT_BUILDDIR)"			\
          BUILD_TYPE="opt" recursive
ifdef EXTRA_OBJ_TARGETS
	@$(MAKE) OPTIMIZER="$(OPT_FLAGS)" OPT_OBJ_BUILD=1		\
          BASE_OBJDIR="$(OPT_BUILDDIR)" $(EXTRA_OBJ_TARGETS)
endif

# Build the object files with the profiling and optimization flags enabled.
obj.prof.build:
	@echo "------------------------------------------------"
	@echo "Building profiled object files"
	@echo "------------------------------------------------"
	@for lib in "" $(LIBS) ; do					\
              $(SHELL) $(MKINSTALLDIRS) $(PROF_BUILDDIR)/$$lib ;	\
          done
	@$(MAKE) RECTARGET="prof" OPTIMIZER="$(PROF_OPT_FLAGS)"		\
          PROF_OBJ_BUILD=1 BASE_OBJDIR="$(PROF_BUILDDIR)"		\
          BUILD_TYPE="prof" recursive
ifdef EXTRA_OBJ_TARGETS
	@$(MAKE) OPTIMIZER="$(PROF_OPT_FLAGS)" PROF_OBJ_BUILD=1		\
          BASE_OBJDIR="$(PROF_BUILDDIR)" $(EXTRA_OBJ_TARGETS)
endif

# Build the static libraries with the debugging flags enabled.
lib.static.dbg.build:
	@echo "================================================"
	@echo "Building static debugging libraries"
	@echo "================================================"
	@for LIB in $(LIBS) ; do					\
           for lib in $(STATIC_LIBS) ; do				\
              $(MAKE) OBJDIR="$(DBG_BUILDDIR)/$$LIB"			\
                LIBDIR="$(DBG_LIBDIR)" EXTRA_LDOPTS="$(LDOPTS_DBG)"	\
                DBG_LIB_BUILD=1 $(DBG_LIBDIR)/$$lib || exit 1;		\
           done ;							\
        done

# Build the dynamic libraries with the debugging flags enabled.
lib.dynamic.dbg.build:
	@echo "================================================"
	@echo "Building dynamic debugging libraries"
	@echo "================================================"
	@for LIB in $(LIBS) ; do					\
           for lib in $(DYNAMIC_LIBS) ; do				\
              $(MAKE) OBJDIR="$(DBG_BUILDDIR)/$$LIB"			\
                LIBDIR="$(DBG_LIBDIR)" EXTRA_LDOPTS="$(LDOPTS_DBG)"	\
                DBG_LIB_BUILD=1 $(DBG_LIBDIR)/$$lib || exit 1;		\
           done ;							\
        done

# Build the static libraries with the optimization flags enabled.
lib.static.opt.build:
	@echo "================================================"
	@echo "Building static optmized libraries"
	@echo "================================================"
	@for LIB in $(LIBS) ; do					\
           for lib in $(STATIC_LIBS) ; do				\
              $(MAKE) OBJDIR="$(OPT_BUILDDIR)/$$LIB"			\
                LIBDIR="$(OPT_LIBDIR)" EXTRA_LDOPTS="$(LDOPTS_OPT)"	\
                OPT_LIB_BUILD=1 $(OPT_LIBDIR)/$$lib || exit 1;		\
           done ;							\
        done

# Build the dynamic libraries with the optimization flags enabled.
lib.dynamic.opt.build:
	@echo "================================================"
	@echo "Building dynamic optmized libraries"
	@echo "================================================"
	@for LIB in $(LIBS) ; do					\
           for lib in $(DYNAMIC_LIBS) ; do				\
              $(MAKE) OBJDIR="$(OPT_BUILDDIR)/$$LIB"			\
                LIBDIR="$(OPT_LIBDIR)" EXTRA_LDOPTS="$(LDOPTS_OPT)"	\
                OPT_LIB_BUILD=1 $(OPT_LIBDIR)/$$lib || exit 1;		\
           done ;							\
        done

# Build the static libraries with the profiling flags enabled.
lib.static.prof.build:
	@echo "================================================"
	@echo "Building static profiled libraries"
	@echo "================================================"
	@for LIB in $(LIBS) ; do					\
           for lib in $(PROF_STATIC_LIBS) ; do				\
              $(MAKE) OBJDIR="$(PROF_BUILDDIR)/$$LIB"			\
                LIBDIR="$(PROF_LIBDIR)" PROF_LIB_BUILD=1		\
                EXTRA_LDOPTS="$(LDOPTS_PROF)"				\
                $(PROF_LIBDIR)/$$lib || exit 1;				\
           done ;							\
        done

# Build the dynamic libraries with the profiling flags enabled.
lib.dynamic.prof.build:
	@echo "================================================"
	@echo "Building dynamic profiled libraries"
	@echo "================================================"
	@for LIB in $(LIBS) ; do					\
           for lib in $(PROF_DYNAMIC_LIBS) ; do				\
              $(MAKE) OBJDIR="$(PROF_BUILDDIR)/$$LIB"			\
                LIBDIR="$(PROF_LIBDIR)" PROF_LIB_BUILD=1		\
                EXTRA_LDOPTS="$(LDOPTS_PROF)"				\
                $(PROF_LIBDIR)/$$lib || exit 1;				\
           done ;							\
        done

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
# static debugging, dynamic debugging, static profiled, and dynamic profield).
# The final step of this is to make the syminks using the default value for
# $(DEFAULT_DIR).  None of the library installation targets are allowed to
# make their symlinks through the use of the $(NOLINK) variable.
install-libs:
	@$(MAKE) NOLINK=1 install-opt
	@$(MAKE) NOLINK=1 install-opt-dso
	@$(MAKE) NOLINK=1 install-dbg
	@$(MAKE) NOLINK=1 install-dbg-dso
ifdef BUILD_PROF_LIBS
	@$(MAKE) NOLINK=1 install-prof
	@$(MAKE) NOLINK=1 install-prof-dso
	@$(MAKE) _HAVE_PROF_LIBS=1 lib.links.install
else
	@$(MAKE) lib.links.install
endif

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
	@$(MAKE) install-dbg-dso
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
	@for LIB in $(LIBS) ; do					\
           for lib in $(STATIC_LIBS) ; do				\
              echo "$(DBG_LIBDIR)/$$lib ==> $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/" ; \
              $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
                $(EXTRA_INSTALL_ARGS)					\
                $(DBG_LIBDIR)/$$lib $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/ ;	\
           done ;							\
        done
	@$(MAKE) DEFAULT_DIR="$(DBG_DIR)" lib.links.install
endif

# -------------------------------------------------------------
# Install only the dynamic debugging version of the libraries.
# -------------------------------------------------------------
install-dbg-dso:
	@echo "------------------------------------------------"
	@echo "Installing dynamic debugging libraries"
	@echo "------------------------------------------------"
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
ifneq ("$(DYNAMIC_LIBS)", "")
	@for LIB in $(LIBS) ; do					\
           for lib in $(DYNAMIC_LIBS) ; do				\
              echo "$(DBG_LIBDIR)/$$lib ==> $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/" ; \
              $(INSTALL) -m $(EXEC_PERMS) $(GROUP_OPT_UNIX)		\
                $(EXTRA_INSTALL_ARGS) $(DBG_LIBDIR)/$$lib		\
                $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/$$lib ;		\
           done ;							\
        done
ifeq ($(OS_TYPE), Win32)
	@for LIB in $(LIBS) ; do					\
           for lib in $(DYNAMIC_LIBS) ; do				\
              base_lib=`echo $$lib | sed -e 's/\.$(DYNAMICLIB_EXT)$$//'` ; \
              echo "$(DBG_LIBDIR)/$$base_lib.lib ==> $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/" ; \
              $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
                $(EXTRA_INSTALL_ARGS) $(DBG_LIBDIR)/$$base_lib.lib	\
                $(INSTALL_LIBDIR_ABS)/$(DBG_DIR)/$$base_lib.lib ;	\
           done ;							\
        done
endif
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
	@$(MAKE) install-opt-dso
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
	@for LIB in $(LIBS) ; do					\
           for lib in $(STATIC_LIBS) ; do				\
              echo "$(OPT_LIBDIR)/$$lib ==> $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/" ; \
              $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
                $(EXTRA_INSTALL_ARGS) $(OPT_LIBDIR)/$$lib		\
                $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/ ;			\
           done ;							\
        done
	@$(MAKE) DEFAULT_DIR="$(OPT_DIR)" lib.links.install
endif

# -------------------------------------------------------------
# Install only the dynamic optimized version of the libraries.
# -------------------------------------------------------------
install-opt-dso:
	@echo "------------------------------------------------"
	@echo "Installing dynamic optimizied libraries"
	@echo "------------------------------------------------"
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
ifneq ("$(DYNAMIC_LIBS)", "")
	@for LIB in $(LIBS) ; do					\
           for lib in $(DYNAMIC_LIBS) ; do				\
              echo "$(OPT_LIBDIR)/$$lib ==> $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/" ; \
              $(INSTALL) -m $(EXEC_PERMS) $(GROUP_OPT_UNIX)		\
                $(EXTRA_INSTALL_ARGS) $(OPT_LIBDIR)/$$lib		\
                $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/$$lib ;		\
           done ;							\
        done
ifeq ($(OS_TYPE), Win32)
	@for LIB in $(LIBS) ; do					\
           for lib in $(DYNAMIC_LIBS) ; do				\
              base_lib=`echo $$lib | sed -e 's/\.$(DYNAMICLIB_EXT)$$//'` ; \
              echo "$(OPT_LIBDIR)/$$base_lib.lib ==> $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/" ; \
              $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
                $(EXTRA_INSTALL_ARGS) $(OPT_LIBDIR)/$$base_lib.lib	\
                $(INSTALL_LIBDIR_ABS)/$(OPT_DIR)/$$base_lib.lib ;	\
           done ;							\
        done
endif
	@$(MAKE) DEFAULT_DIR="$(OPT_DIR)" lib.links.install
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
	@$(MAKE) install-prof
	@$(MAKE) install-prof-dso
	@$(MAKE) do-post-install
ifdef AFTERINSTALL
	@$(MAKE) $(AFTERINSTALL)
endif

# ------------------------------------------------------------
# Install only the static profiled version of the libraries.
# ------------------------------------------------------------
install-prof:
	@echo "------------------------------------------------"
	@echo "Installing static profiled libraries"
	@echo "------------------------------------------------"
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
ifneq ("$(STATIC_LIBS)", "")
	@for LIB in $(LIBS) ; do					\
           for lib in $(PROF_STATIC_LIBS) ; do				\
              echo "$(PROF_LIBDIR)/$$lib ==> $(INSTALL_LIBDIR_ABS)/$(PROF_DIR)/" ; \
              $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
                $(EXTRA_INSTALL_ARGS) $(PROF_LIBDIR)/$$lib		\
                $(INSTALL_LIBDIR_ABS)/$(PROF_DIR)/ ;			\
           done ;							\
        done
	@$(MAKE) DEFAULT_DIR="$(PROF_DIR)" _HAVE_PROF_LIBS=1 lib.links.install
endif

# -------------------------------------------------------------
# Install only the dynamic optimized version of the libraries.
# -------------------------------------------------------------
install-prof-dso:
	@echo "------------------------------------------------"
	@echo "Installing dynamic profiled libraries"
	@echo "------------------------------------------------"
ifdef PREINSTALL
	@$(MAKE) $(PREINSTALL)
endif
ifneq ("$(PROF_DYNAMIC_LIBS)", "")
	@for LIB in $(LIBS) ; do					\
           for lib in $(PROF_DYNAMIC_LIBS) ; do				\
              echo "$(PROF_LIBDIR)/$$lib ==> $(INSTALL_LIBDIR_ABS)/$(PROF_DIR)/" ; \
              $(INSTALL) -m $(EXEC_PERMS) $(GROUP_OPT_UNIX)		\
                $(EXTRA_INSTALL_ARGS) $(PROF_LIBDIR)/$$lib		\
                $(INSTALL_LIBDIR_ABS)/$(PROF_DIR)/$$lib ;		\
           done ;							\
        done
ifeq ($(OS_TYPE), Win32)
	@for LIB in $(LIBS) ; do					\
           for lib in $(PROF_DYNAMIC_LIBS) ; do				\
              base_lib=`echo $$lib | sed -e 's/\.$(DYNAMICLIB_EXT)$$//'` ; \
              echo "$(PROF_LIBDIR)/$$base_lib.lib ==> $(INSTALL_LIBDIR_ABS)/$(PROF_DIR)/" ; \
              $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
                $(EXTRA_INSTALL_ARGS) $(PROF_LIBDIR)/$$base_lib.lib	\
                $(INSTALL_LIBDIR_ABS)/$(PROF_DIR)/$$base_lib.lib ;	\
           done ;							\
        done
endif
	@$(MAKE) DEFAULT_DIR="$(PROF_DIR)" _HAVE_PROF_LIBS=1 lib.links.install
endif

# Install symlinks to the installed library binaries in the destination
# directory.
lib.links.install:
ifndef NOLINK
ifdef MAKE_REL_SYMLINKS
	@$(MAKE) LINK_DIR=$(libdir)$(LIBBITSUF)				\
          DEST_DIR=$(INSTALL_LIBDIR_REL) lib.links.generic
endif # ifdef MAKE_REL_SYMLINKS
	@$(MAKE) LINK_DIR=$(INSTALL_LIBDIR_ABS) DEST_DIR=. lib.links.generic
endif # ifndef NOLINK

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
