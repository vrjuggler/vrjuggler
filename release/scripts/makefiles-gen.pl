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
# Generate Makefiles in the subdirectories (SUBDIRS) of the installation
# directory (prefix) from Makefile.in's found in the source directory
# (startdir).  All command-line options shown here are required.  They are
# used to replace strings of the form "@...@" in the Makefile.in files found
# in the startdir directory tree.
#
# Usage:
#     makefiles-gen.pl
#         --vars=<Path to a Perl file containing @..@ substitution values>
#         --srcdir=<Location of source code>
#         --prefix=<Base directory where Makefile will go>
#         --startdir=<Directory where search begins>
#         --SUBDIRS=<Directories containing Makefile.in's>
#         --uname=<Owner's user name>
#         --gname=<Group name>
#         --mode=<Mode bits>
#
# NOTE:
#    In almost every situation, the --srcdir option should be passed the
#    value "." so that the source in the installation directory will be used.
# -----------------------------------------------------------------------------

require 5.004;

use Cwd;
use File::Basename;
use File::Copy;
use Getopt::Long;

# Do this to include the path to the script in @INC.
BEGIN {
    $path = (fileparse("$0"))[1];
}

use lib("$path");
use InstallOps;

$Win32 = 1 if $ENV{'OS'} =~ /Windows/;

# Turn off case insensitivity when parsing command-line options.
$Getopt::Long::ignorecase = 0;

if ( $#ARGV < 5 ) {
    die <<USAGE_EOF;
Usage:
    $0
        --vars=<Path to a Perl file containing @..@ substitution values>
        --srcdir=<Location of source code>
        --prefix=<Base directory where Makefile will go>
        --startdir=<Directory where search begins>
        --SUBDIRS=<Directories containing Makefile.in's>
      [ --uname=<Owner's user name> --gname=<Group name> --mode=<Mode bits> ]
USAGE_EOF
}

# Initialize variables passed (by reference) to GetOptions();
%VARS = ();
($var_file, $startdir, $prefix) = ('', '', '');
($uname, $gname, $mode) = ('', '', '');

# Parse the command-line options and store the given values in %VARS which
# is indexed by the tag name to be replaced (e.g., 'CXX').
GetOptions("vars=s", \$var_file, "srcdir=s" => \$VARS{'srcdir'},
	   "SUBDIRS=s" => \$VARS{'SUBDIRS'}, "prefix=s" => \$prefix,
	   "startdir=s" => \$startdir, "uname=s" => \$uname,
	   "gname=s" => \$gname, "mode=s" => \$mode);

# Import the extra settings for %VARS found in $var_file if it was given
# on the command line.
require "$var_file" if $var_file;

umask(002);
chdir("$startdir") or die "ERROR: Cannot chdir to $startdir: $!\n";

my ($basedir, $dir);
$basedir = cwd();

# Defaults.
my($uid, $gid, $mode_bits) = ($<, (getpwuid($<))[3], "0644") unless $Win32;

if ( $uname ) {
    $uid = (getpwnam("$uname"))[2] or die "getpwnam($uname): $!\n";
}

if ( $gname && ! $Win32 ) {
    $gid = (getgrnam("$gname"))[2] or die "getgrnam($gname): $!\n";
}

$mode_bits = "$mode" if $mode;

# Loop over the directories given with --SUBDIRS and generate the Makefiles
# from the corresponding Makefile.in's.
foreach $dir ( split(/\s/, "$VARS{'SUBDIRS'}") ) {
    my $outfile = "$prefix/$dir/Makefile";

    print "Generating $outfile ...\n";

    my $infile = "$basedir/$dir/Makefile.in";

    my $workfile;

    if ( $Win32 ) {
	$workfile = "C:/temp/Makefile.in";
    } else {
	$workfile = "/tmp/Makefile.in";
    }

    # Make a working copy of the input file to be safe.
    copy("$infile", "$workfile") unless "$infile" eq "$workfile";

    # Replace the tags in $workfile with the values in %VARS.
    next if replaceTags("$workfile", %VARS) < 0;

    # Move $workfile to its final destination.
    copy("$workfile", "$outfile");
    chown($uid, $gid, "$outfile") or die "chown: $!\n";
    chmod(oct($mode_bits), "$outfile") or die "chmod: $!\n";
    unlink("$workfile");
}

exit 0;
