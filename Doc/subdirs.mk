# -----------------------------------------------------------------------------
# $Id$
#
# This include file <subdirs.mk> lists the subdirectories over which PERCEPS
# recurses when it does its thing.  These directories are listed in the
# $(SUBDIRS) variable which can be used by the includer.
# -----------------------------------------------------------------------------
# The including makefile must define the following variable:
#
#     top_srcdir - The top directory where the directories containing source
#                  code to be parsed can be found.
# -----------------------------------------------------------------------------

GROUPS			= Config	\
                          Input		\
                          Kernel	\
                          Math		\
                          Primitives	\
                          Sound		\
                          Utils

CONFIG_GROUP_DIRS	= $(top_srcdir)/Config		\
                          $(top_srcdir)/Environment	\
                          $(top_srcdir)/Performance

INPUT_GROUP_DIRS	= $(top_srcdir)/Input

KERNEL_GROUP_DIRS	= $(top_srcdir)/Kernel

MATH_GROUP_DIRS		= $(top_srcdir)/Math

PRIMITIVES_GROUP_DIRS	= $(top_srcdir)/Sync		\
                          $(top_srcdir)/Threads

SOUND_GROUP_DIRS	= $(top_srcdir)/Sound

UTILS_GROUP_DIRS	= $(top_srcdir)/Utils

SUBDIRS			= $(CONFIG_GROUP_DIRS)		\
                          $(INPUT_GROUP_DIRS)		\
                          $(KERNEL_GROUP_DIRS)		\
                          $(MATH_GROUP_DIRS)		\
                          $(PRIMITIVES_GROUP_DIRS)	\
                          $(SOUND_GROUP_DIRS)		\
                          $(UTILS_GROUP_DIRS)
