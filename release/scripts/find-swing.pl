#!/usr/local/bin/perl

# -----------------------------------------------------------------------------
# $Id$
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
