#!/bin/sh

set -e

ACLOCAL_FLAGS="-I config"

aclocal $ACLOCAL_FLAGS  || \
    aclocal $ACLOCAL_FLAGS -I /usr/local/share/aclocal
libtoolize --force 
autoheader
automake --add-missing
autoconf
