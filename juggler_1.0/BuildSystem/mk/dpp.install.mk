# =============================================================================
# dpp.install.mk,v 1.4 2000/12/30 04:56:27 patrick Exp
#
# This include file <dpp.install.mk> handles install operations.  It adds an
# 'install' target.
# -----------------------------------------------------------------------------
# The Makefile including this file must define the following variables:
#
# includedir - The installation directory for the header files.  This is
#              only needed if the default value for ${INSTALL_PATH} is to be
#              used.
# srcdir     - The directory where the source file(s) is/are located.  This is
#              only needed if the default value for ${INSTALL_FILES} is to be
#              used.
# INSTALL    - The path to a BSD-compatible install program.
#
# Optionally, the including file can add to the following existing variables
# to provide additional local 'install':
#
# INSTALL_DEPS  - Dependencies for the 'install' target.
# INSTALL_FILES - The file(s) to be installed.
# INSTALL_PATH  - The path to which the file(s) will be installed.
# FILE_PERMS    - Permissions on normal (non-executable) files.
# GROUP_NAME    - The name of the group that will own the files.  If this is
#                 not defined, no attempt is made to set an alternate group
#                 name.
# SUBDIR        - The list of subdirectories where installation needs to be
#                 done.
# -----------------------------------------------------------------------------
# The target defined here is as follows:
#
#     install - Recurse over any subdirectories in $(SUBDIR) running 'install'
#               in each and then install $(INSTALL_FILES) in the current
#               directory.
# =============================================================================

FILE_PERMS	?= 644
GROUP_OPT	?= -g $(GROUP_NAME)
#INSTALL_DEPS	?=
INSTALL_FILES	?= $(srcdir)/*.h
INSTALL_PATH	?= $(includedir)

install: $(INSTALL_DEPS)
ifdef SUBDIR
	@for dir in $(SUBDIR) ; do					\
             $(MAKE) -C $$dir $@ ;					\
         done
endif
ifdef INSTALL_FILES
	@for file in $(INSTALL_FILES) ; do				\
            echo "  $$file ==> $(INSTALL_PATH)" ;			\
            $(INSTALL) -m $(FILE_PERMS) $(GROUP_OPT) $$file		\
              $(INSTALL_PATH) ;						\
         done
endif
