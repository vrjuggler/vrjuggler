#!/bin/sh

# $Id$

mydir=`dirname $0`

if [ "x$mydir" = "x" ]; then
   omnipython_dir=`pwd`
else
   cwd=`pwd`
   cd $mydir
   omnipython_dir=`pwd`
   cd $cwd
fi

omnipython_cmd="$omnipython_dir/omnipython"
omniidl_cmd="$omnipython_cmd $omnipython_dir/omniidl.py"
exec $omniidl_cmd $*
