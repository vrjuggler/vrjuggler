# =============================================================================
# dpp.libs.mk,v 1.2 2000/12/30 22:31:35 patrick Exp
#
# This file <dpp.libs.mk> defines many targets for use in compiling a software
# library (or a set of libraries).  An including makefile can take advantage
# of this file by extending its logic and using the provided targets
# (documented below) within its own more complex, project-specific targets.
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
# Optionally, it can define the following variables for added functionality:
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
# DBG_DIR             - Name of the directory where debugging objects and
#                       libraries are built.
# OPT_DIR             - Name of the directory where optimized objects and
#                       libraries are built.
# DEFAULT_DIR         - Default diretory for installations.  This should be
#                       either $(DBG_DIR) or $(OPT_DIR).  If not specified,
#                       it defaults to $(DBG_DIR).
#
# DBG_FLAGS           - Compiler debugging flags.  If not specified, it
#                       defaults to -D_DEBUG.
# OPT_FLAGS           - Compiler optimizing flags.  If not specified, it
#                       defaults to -D_OPT.
#
# GROUP_NAME          - Name of the group owner for installed files.
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
#
# EXTRA_OBJ_TARGETS   - Extra targets to build when making object files.
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

MKINSTALLDIRS	?= mkinstalldirs
DBG_FLAGS	?= -D_DEBUG
OPT_FLAGS	?= -D_OPT

# =============================================================================
# Build and installation directory naming and permission information.
# =============================================================================
DBG_DIR		?= debug
OPT_DIR		?= opt
DEFAULT_DIR	?= $(DBG_DIR)

UMASK		?= 002

# If we have a name for the install group, define group-setting options for
# UNIX commands (chown(1), chgrp(1)).
ifdef GROUP_NAME
    GROUP_OPT_UNIX	?= -g $(GROUP_NAME)
endif

# ------------------------------------------------
# Object file build and installation directories.
# ------------------------------------------------
topdir		?= .
OBJDIR_NAME	?= obj
BUILDDIR_BASE	?= $(topdir)/$(OBJDIR_NAME)
BUILDDIR	?= $(BUILDDIR_BASE)/$(PLATFORM)/$(ABI)/$(ISA)
DBG_BUILDDIR	?= $(BUILDDIR)/$(DBG_DIR)
OPT_BUILDDIR	?= $(BUILDDIR)/$(OPT_DIR)

# --------------------------------------------
# Library build and installation directories.
# --------------------------------------------

# $(LIBDIR_NAME) is the name of the library directory including an ABI suffix
# if applicable.
LIBDIR_NAME	?= lib$(LIBBITSUF)

# $(LIBDIR_BASE) is the base directory where the libraries will be built.
LIBDIR_BASE	?= $(topdir)/$(LIBDIR_NAME)

# $(LIBDIR) is the first-level subdirectory of $(LIBDIR_BASE) that is
# named by the instruction set being used ($(ISA)).
LIBDIR		?= $(LIBDIR_BASE)/$(ISA)

# $(DBG_LIBDIR) and $(OPT_LIBDIR) are the directories where the libraries
# are actually built.  The first is for a debugging copy of the libraries and
# the second is for an optimized version of the libraries.
DBG_LIBDIR	?= $(LIBDIR)/$(DBG_DIR)
OPT_LIBDIR	?= $(LIBDIR)/$(OPT_DIR)

# $(_install_libdir_abs) is the full path to the base directory (minus any
# debugging or optimized subdirectories) where the libraries will be
# installed.  $(_install_libdir_rel) is only the $(ISA) subdirectory of the
# full path.
_install_libdir_abs = $(libdir)$(LIBBITSUF)/$(ISA)
_install_libdir_rel = ./$(ISA)

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
	@$(MAKE) _build_dbg_obj $(EXTRA_OBJ_TARGETS)
	@$(MAKE) _build_dbg_lib_static
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
	@$(MAKE) _build_dbg_obj $(EXTRA_OBJ_TARGETS)
	@$(MAKE) _build_dbg_lib_dynamic
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
	@$(MAKE) _build_opt_obj $(EXTRA_OBJ_TARGETS)
	@$(MAKE) _build_opt_lib_static
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
	@$(MAKE) _build_opt_obj $(EXTRA_OBJ_TARGETS)
	@$(MAKE) _build_opt_lib_dynamic
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
	@$(MAKE) _build_opt_obj
	@$(MAKE) _build_opt_lib_static
	@$(MAKE) _build_opt_lib_dynamic
	@$(MAKE) _build_dbg_obj
	@$(MAKE) _build_dbg_lib_static
	@$(MAKE) _build_dbg_lib_dynamic
	@$(MAKE) _LIB_LINK_DIR="$(LIBDIR)" _lib_links
ifdef POSTLIB
	@$(MAKE) $(POSTLIB)
endif

# Make symlinks in $(_LIB_LINK_DIR) pointing to the library binaries in
# $(DEFAULT_DIR).
_lib_links:
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
	@$(MAKE) _build_dbg_obj $(EXTRA_OBJ_TARGETS)
	@$(MAKE) _build_dbg_lib_static
	@$(MAKE) _build_dbg_lib_dynamic
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
	@$(MAKE) _build_opt_obj $(EXTRA_OBJ_TARGETS)
	@$(MAKE) _build_opt_lib_static
	@$(MAKE) _build_opt_lib_dynamic
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
_build_dbg_obj:
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
_build_opt_obj:
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
_build_dbg_lib_static:
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
_build_dbg_lib_dynamic:
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
_build_opt_lib_static:
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
_build_opt_lib_dynamic:
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
	@$(MAKE) _install_lib_links

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
            echo "$(DBG_LIBDIR)/$$lib.$(STATICLIB_EXT) ==> $(_install_libdir_abs)/$(DBG_DIR)/" ; \
            $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
              $(DBG_LIBDIR)/$$lib.$(STATICLIB_EXT)			\
              $(_install_libdir_abs)/$(DBG_DIR)/ ;			\
          done
	@$(MAKE) DEFAULT_DIR="$(DBG_DIR)" _install_lib_links
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
            echo "$(DBG_LIBDIR)/$$lib.$(DYNAMICLIB_EXT) ==> $(_install_libdir_abs)/$(DBG_DIR)/" ; \
            $(INSTALL) -m $(EXEC_PERMS) $(GROUP_OPT_UNIX)		\
              $(DBG_LIBDIR)/$$lib.$(DYNAMICLIB_EXT)			\
              $(_install_libdir_abs)/$(DBG_DIR)/ ;			\
          done
	@$(MAKE) DEFAULT_DIR="$(DBG_DIR)" _install_lib_links
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
            echo "$(OPT_LIBDIR)/$$lib.$(STATICLIB_EXT) ==> $(_install_libdir_abs)/$(OPT_DIR)/" ; \
            $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT_UNIX)		\
              $(OPT_LIBDIR)/$$lib.$(STATICLIB_EXT)			\
              $(_install_libdir_abs)/$(OPT_DIR)/ ;			\
          done
	@$(MAKE) DEFAULT_DIR="$(OPT_DIR)" _install_lib_links
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
            echo "$(OPT_LIBDIR)/$$lib.$(DYNAMICLIB_EXT) ==> $(_install_libdir_abs)/$(OPT_DIR)/" ; \
            $(INSTALL) -m $(EXEC_PERMS) $(GROUP_OPT_UNIX)		\
              $(OPT_LIBDIR)/$$lib.$(DYNAMICLIB_EXT)			\
              $(_install_libdir_abs)/$(OPT_DIR)/ ;			\
          done
	@$(MAKE) DEFAULT_DIR="$(OPT_DIR)" _install_lib_links
endif

# Install symlinks to the installed library binaries in the destination
# directory.
_install_lib_links:
ifndef NOLINK
	@echo "------------------------------------------------"
	@echo "Creating symlinks in $(libdir)$(LIBBITSUF)"
	@echo "------------------------------------------------"
	@for lib in $(LIBS) ; do					\
            for ext in $(DYNAMICLIB_EXT) $(STATICLIB_EXT) ; do		\
                echo "$$lib.$$ext -> $(_install_libdir_rel)/$(DEFAULT_DIR)/$$lib.$$ext" ; \
                cd $(libdir)$(LIBBITSUF) && umask $(UMASK) &&		\
                  rm -f ./$$lib.$$ext &&				\
                  $(LN_S) $(_install_libdir_rel)/$(DEFAULT_DIR)/$$lib.$$ext ./ ; \
            done ;							\
          done
	@echo "------------------------------------------------"
	@echo "Creating symlinks in $(_install_libdir_abs)"
	@echo "------------------------------------------------"
	@for lib in $(LIBS) ; do					\
            for ext in $(DYNAMICLIB_EXT) $(STATICLIB_EXT) ; do		\
                echo "$$lib.$$ext -> $(DEFAULT_DIR)/$$lib.$$ext" ;	\
                cd $(_install_libdir_abs) && umask $(UMASK) &&		\
                  rm -f ./$$lib.$$ext &&				\
                  $(LN_S) $(DEFAULT_DIR)/$$lib.$$ext ./ ;		\
            done ;							\
          done
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
