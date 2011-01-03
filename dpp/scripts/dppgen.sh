#!/bin/sh

# Doozer++ is (C) Copyright 2000-2010 by Iowa State University
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
#
# Original Author:
#   Patrick Hartling

# Run this to generate all the initial makefiles, etc.

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
