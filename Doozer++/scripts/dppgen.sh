#!/bin/sh

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# Doozer++ is (C) Copyright 2000-2008 by Iowa State University
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

# dppgen.sh,v 1.9 2008/01/01 15:29:22 patrickh Exp

DIE=0

# Fill in the "<...>" string with the default path to your Doozer++ directory
# and uncomment the following line for easier use.
# : ${DPP_PATH=<set your default DPP_PATH here>}

if [ -n "$DPP_PATH" ]; then
   ACLOCAL_FLAGS="-I $DPP_PATH/config -I $DPP_PATH/config/pkgs $ACLOCAL_FLAGS"
fi

: ${ACLOCAL=aclocal}
: ${AUTOCONF=autoconf}
: ${AUTOHEADER=autoheader}

($AUTOCONF --version) < /dev/null > /dev/null 2>&1 || {
   echo
   echo "**Error**: You must have \`autoconf' installed to compile."
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

for coin in `find ${srcdir-.} -name configure.in -print`
do 
   dr=`dirname $coin`
   if test -f $dr/NO-AUTO-GEN; then
      echo skipping $dr -- flagged as no auto-gen
   else
      echo processing $dr
      macrodirs=`sed -n -e 's,AM_ACLOCAL_INCLUDE(\(.*\)),\1,gp' < $coin`
      ( cd $dr
        macrosdir=`find . -name macros -print`
#        for i in $macrodirs; do
#        done

        aclocalinclude="$ACLOCAL_FLAGS"
        echo "Running $ACLOCAL $aclocalinclude ..."
        $ACLOCAL $aclocalinclude
        if grep "^AC_CONFIG_HEADER" configure.in >/dev/null
        then
           echo "Running $AUTOHEADER..."
           $AUTOHEADER
        fi
        echo "Running $AUTOCONF ..."
        $AUTOCONF
      )
   fi
done
