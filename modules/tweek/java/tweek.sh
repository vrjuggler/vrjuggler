#!/bin/sh

# **************** <Tweek heading BEGIN do not edit this line> ****************
# Tweek
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
# **************** <Tweek heading END do not edit this line> ******************

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
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
# *************** <auto-copyright.pl END do not edit this line> ***************

# Shell script for running Tweek under jdk 1.2 using the 'java' command.

if test "x$TWEEK_BASE_DIR" = "x" ; then
    echo "FATAL ERROR: TWEEK_BASE_DIR environment variable not set!"
    exit 1
fi

# If the environment variable JDK_HOME is not set, default it to /usr/java.
if test "x${JDK_HOME}" = "x" ; then
    echo "WARNING: Setting JDK_HOME environment variable to /usr/java"
    JDK_HOME='/usr/java' ; export JDK_HOME
fi

# Make sure that $JDK_HOME/bin/java exists and is executable.  If it is not,
# exit with an error message and status 1.
if test ! -x "$JDK_HOME/bin/java" ; then
    echo "ERROR: Could not find java executable $JDK_HOME/bin/java"
    status=1
# Otherwise, start up Tweek and exit with status 0.
else
    CLASSPATH="${TWEEK_BASE_DIR}/bin/jdom.jar:${TWEEK_BASE_DIR}/bin/xerces.jar:${TWEEK_BASE_DIR}/bin/Tweek.jar:${TWEEK_BASE_DIR}/bin/TweekBeans.jar:${TWEEK_BASE_DIR}/bin/TweekComm.jar"
    export CLASSPATH

    $JDK_HOME/bin/java -Djava.security.policy="$TWEEK_BASE_DIR/bin/java.security.policy.txt" -DTWEEK_BASE_DIR="${TWEEK_BASE_DIR}" org.vrjuggler.tweek.Tweek $*
    status=0
fi

exit $status
