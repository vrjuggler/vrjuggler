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

SUBDIRS		= $(top_srcdir)/Config $(top_srcdir)/Environment	\
                  $(top_srcdir)/Input $(top_srcdir)/Kernel		\
                  $(top_srcdir)/Math $(top_srcdir)/Performance		\
                  $(top_srcdir)/SharedMem $(top_srcdir)/Sound		\
                  $(top_srcdir)/Sync $(top_srcdir)/Threads		\
                  $(top_srcdir)/Utils
