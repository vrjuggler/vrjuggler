#!/bin/sh
# Run this to generate all the initial makefiles, etc.

# dppgen.sh,v 1.4 2000/12/30 14:34:19 patrick Exp

DIE=0

: ${DPP_PATH=`pwd`/Doozer++}
: ${MACRO_PATH=`pwd`/macros}
: ${GGT_MACRO_PATH=`pwd`/macros.GGT}

if [ -n "$DPP_PATH" ]; then
   ACLOCAL_FLAGS="-I $DPP_PATH/config $ACLOCAL_FLAGS"
fi

if [ -n "$MACRO_PATH" ]; then
   ACLOCAL_FLAGS="-I $MACRO_PATH $ACLOCAL_FLAGS"
fi

if [ -n "$GGT_MACRO_PATH" ]; then
   ACLOCAL_FLAGS="-I $GGT_MACRO_PATH $ACLOCAL_FLAGS"
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

search_dirs="${srcdir-.}/modules ${srcdir-.}/external"
for coin in `find $search_dirs -name configure.in -print`
do 
  dr=`dirname $coin`
  if test -f $dr/NO-AUTO-GEN; then
    echo skipping $dr -- flagged as no auto-gen
  else
    echo processing $dr
    macrodirs=`sed -n -e 's,AM_ACLOCAL_INCLUDE(\(.*\)),\1,gp' < $coin`
    ( cd $dr
#      macrosdir=`find . -name macros -print`
#      for i in $macrodirs; do
#      done

      aclocalinclude="$ACLOCAL_FLAGS"
      echo "Running $ACLOCAL $aclocalinclude ..."
      $ACLOCAL $aclocalinclude || exit 1
      if grep "^AC_CONFIG_HEADER" configure.in >/dev/null
      then
        echo "Running $AUTOHEADER..."
        $AUTOHEADER || exit 1
      fi
      echo "Running $AUTOCONF ..."
      $AUTOCONF || exit 1
    )
  fi
done
