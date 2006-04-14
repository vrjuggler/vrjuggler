#!/bin/sh
# Run this to generate all the initial makefiles, etc.

# dppgen.sh,v 1.4 2000/12/30 14:34:19 patrick Exp

DIE=0

: ${DPP_PATH=../../../Doozer++}
: ${MACRO_PATH=../../../macros}
: ${GGT_MACRO_PATH=../../../external/macros}

if [ -n "$DPP_PATH" ]; then
   ACLOCAL_FLAGS="-I $DPP_PATH/config -I $DPP_PATH/config/pkgs $ACLOCAL_FLAGS"
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
