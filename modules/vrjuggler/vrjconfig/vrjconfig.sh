#!/bin/sh

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

# If $JCCL_BASE_DIR is not set, fall back on the value of $VJ_BASE_DIR.  This
# is a reasonable default.
if test "x$JCCL_BASE_DIR" = "x" ; then
    echo "NOTE: Setting JCCL_BASE_DIR to $VJ_BASE_DIR"
    JCCL_BASE_DIR="$VJ_BASE_DIR"
    export JCCL_BASE_DIR
fi

# If $JCCL_DEFINITION_PATH is set, add that property definition argument.
if test "x$JCCL_DEFINITION_PATH" != "x" ; then
   jccl_def_arg="-DJCCL_DEFINITION_PATH=${JCCL_DEFINITION_PATH}"
   export JCCL_DEFINITION_PATH
fi

EXTRA_JDK_ARGS="-Dsun.java2d.opengl=True -DJCCL_BASE_DIR=${JCCL_BASE_DIR} -DVJ_BASE_DIR=${VJ_BASE_DIR} $jccl_def_arg"

APP_NAME='VRJConfig'

. $TWEEK_BASE_DIR/bin/tweek-base.sh \
   --beanpath="$JCCL_BASE_DIR/share/jccl/beans" \
   --beanpath="$VJ_BASE_DIR/share/vrjuggler/beans" \
   --defaultbean="Configuration Editor" "$@"

exit $status
