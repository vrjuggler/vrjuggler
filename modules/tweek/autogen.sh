#!/bin/sh

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# Doozer++ is (C) Copyright 2000, 2001 by Iowa State University
#
# Original Author:
#   Patrick Hartling
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

# Run this to generate all the initial makefiles, etc.

# dppgen.sh,v 1.4 2000/12/30 14:34:19 patrick Exp

DIE=0

: ${DPP_PATH=../../Doozer++}
: ${MACRO_PATH=../../macros}

if [ -n "$MACRO_PATH" ]; then
   ACLOCAL_FLAGS="-I $MACRO_PATH $ACLOCAL_FLAGS"
fi

if [ -n "$DPP_PATH" ]; then
   ACLOCAL_FLAGS="-I $DPP_PATH/config -I $DPP_PATH/config/pkgs $ACLOCAL_FLAGS"
fi

: ${AUTOCONF=autoconf}
: ${AUTOHEADER=autoheader}
: ${ACLOCAL=aclocal}

($AUTOCONF --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`autoconf' installed to compile VR Juggler."
  echo "Download the appropriate package for your distribution,"
  echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
  DIE=1
}

($ACLOCAL --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: Missing \`aclocal'.  The version of \`automake'"
  echo "installed doesn't appear recent enough."
  echo "Get ftp://ftp.gnu.org/pub/gnu/automake-1.4.tar.gz"
  echo "(or a newer version if it is available)"
  DIE=1
}

if test "$DIE" -eq 1; then
  exit 1
fi

aclocalinclude="$ACLOCAL_FLAGS"
echo "Running $ACLOCAL $aclocalinclude ..."
$ACLOCAL $aclocalinclude
if grep "^AC_CONFIG_HEADER" configure.ac >/dev/null
then
  echo "Running $AUTOHEADER..."
  $AUTOHEADER
fi
echo "Running $AUTOCONF ..."
$AUTOCONF
