#!/bin/sh

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

# Shell script for running Tweek under JDK 1.x using the 'java' command.

# We have to have $VJ_BASE_DIR set.
if test "x$VJ_BASE_DIR" = "x" ; then
    echo "FATAL ERROR: VJ_BASE_DIR environment variable not set!"
    exit 1
fi

# If $TWEEK_BASE_DIR is not set, fall back on the value of $VJ_BASE_DIR.  This
# is a reasonable default.
if test "x$TWEEK_BASE_DIR" = "x" ; then
    echo "NOTE: Setting TWEEK_BASE_DIR to $VJ_BASE_DIR"
    TWEEK_BASE_DIR="$VJ_BASE_DIR"
    export TWEEK_BASE_DIR
fi

# If $JCCL_DEFINITION_PATH is not set, use the standard location for .jdef
# files used by VR Juggler.
if test "x$JCCL_DEFINITION_PATH" = "x" ; then
    echo "NOTE: Setting JCCL_DEFINITION_PATH to $VJ_BASE_DIR/share/vrjuggler/data/definitions"
    JCCL_DEFINITION_PATH="$VJ_BASE_DIR/share/vrjuggler/data/definitions"
    export JCCL_DEFINITION_PATH
fi

EXTRA_JDK_ARGS="-DVJ_BASE_DIR=${VJ_BASE_DIR} -DJCCL_DEFINITION_PATH=${JCCL_DEFINITION_PATH}"

. $TWEEK_BASE_DIR/bin/tweek-base.sh --defaultbean="Configuration Editor" "$@"

exit $status
