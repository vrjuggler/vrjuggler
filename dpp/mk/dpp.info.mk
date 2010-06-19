# Doozer++ is (C) Copyright 2000-2009 by Iowa State University
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
#
# Original Author:
#   Patrick Hartling

# =============================================================================
# This file <dpp.info.mk> is intended to be used by build systems that want to
# give users an easy way to find out about all the possible targets.  It
# defines two targets, 'help' and 'info', that print out the targets of the
# using makefile.  The information printed is denoted using special comment
# syntax.
# -----------------------------------------------------------------------------
# The makefile including this file must define the following variables:
#
# DPP_SCRIPTDIR - The directory containing Doozer++ scripts.  In particular,
#                 genmkinfo.pl is needed.
# SRC_MKFILES   - The file(s) that will be processed by genmkinfo.pl.
# PERL          - The full path to a Perl binary, version 5.004 or newer.
# -----------------------------------------------------------------------------
# The targets defined here are as follows:
#
# help - Print the help information as collected by genmkinfo.pl
# info - Same as 'help'
# =============================================================================

help info:
	@$(PERL) $(DPP_SCRIPTDIR)/genmkinfo.pl $(SRC_MKFILES)
