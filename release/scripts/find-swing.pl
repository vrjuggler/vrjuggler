#!/usr/local/bin/perl

#
# VRJuggler
#   Copyright (C) 1997,1998,1999,2000
#   Iowa State University Research Foundation, Inc.
#   All Rights Reserved
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

# -----------------------------------------------------------------------------
# This is a really simple script that searches the users $CLASSPATH
# environment variable looking for swingall.jar.  Once it finds it, it prints
# out the full path of the file.  If it is not found in $CLASSPATH, it falls
# back on a known location (Chris' Swing directory in his home directory).  If
# that file is not readable, the script bombs out with error status.
#
# Usage:
#     find-swing.pl
# -----------------------------------------------------------------------------

require 5.001;

use Env('CLASSPATH');

my $fallback = "/home/users/cjust/Swing/swingall.jar";
my(@dirs) = split(/:/, "$CLASSPATH");

my $dir = '';

foreach $dir ( @dirs ) {
    if ( $dir =~ /swingall\.jar$/ && -r "$dir" ) {
	print "$dir\n";
	exit 0;
    }
}

if ( -r "$fallback" ) {
    print "$fallback\n";
    exit 0;
}

print STDERR "$0: swingall.jar not found in $CLASSPATH or in $fallback\n";

exit 1;
