#!/bin/sh
# Run this to generate all the initial makefiles, etc.

# dppgen.sh,v 1.4 2000/12/30 14:34:19 patrick Exp

DIE=0

dpp_path=${DPP_PATH-../../Doozer++}

if [ -n "$dpp_path" ]; then
	ACLOCAL_FLAGS="-I $dpp_path/config -I ../../macros $ACLOCAL_FLAGS"
fi

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`autoconf' installed to compile VR Juggler."
  echo "Download the appropriate package for your distribution,"
  echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
  DIE=1
}

(aclocal --version) < /dev/null > /dev/null 2>&1 || {
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
echo "Running aclocal $aclocalinclude ..."
aclocal $aclocalinclude
if grep "^AC_CONFIG_HEADER" configure.in >/dev/null
then
  echo "Running autoheader..."
  autoheader
fi
echo "Running autoconf ..."
autoconf
