#!/bin/sh

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

if test "x$JCCL_BASE_DIR" = "x" ; then
   echo "ERROR: \$JCCL_BASE_DIR environment variable required for VjControl"
   exit 1
fi

if test "x$TWEEK_BASE_DIR" = "x" ; then
   echo "WARNING: Defaulting \$TWEEK_BASE_DIR to $JCCL_BASE_DIR"
   TWEEK_BASE_DIR="$JCCL_BASE_DIR"
   export TWEEK_BASE_DIR
fi

if test "x$VJ_SHARE_DIR" = "x" ; then
   VJ_SHARE_DIR="$VJ_BASE_DIR/share/vrjuggler"
fi
if test "x$JCCL_SHARE_DIR" = "x" ; then
   JCCL_SHARE_DIR=$JCCL_BASE_DIR/#JCCL_SHARE_DIR#
fi

# Make sure java knows about the JCCL_BASE_DIR variable
EXTRA_JDK_ARGS="-DJCCL_BASE_DIR=$JCCL_BASE_DIR -DVJ_SHARE_DIR=$VJ_SHARE_DIR -DJCCL_SHARE_DIR=$JCCL_SHARE_DIR"

# Run tweek
. $TWEEK_BASE_DIR/bin/tweek-base.sh

exit $status
