#!/usr/local/bin/perl

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Install a source (i.e., software source) directory to a specified
# destination directory.  Only pre-defined file types (determined by file
# extension) are copied.  No CVS subdirectories are installed.
#
# Usage:
#     install-src.pl -i <source directory> -o <destination directory>
#                   [-u <user name> -g <group name> -m <mode>]
#
#     <source directory> - The source directory to be installed
#     <destination directory> - The location to install the source directory
# -----------------------------------------------------------------------------

require 5.004;

use Cwd;
use File::Basename;
use File::Path;
use Getopt::Std;

# Do this to include the path to the script in @INC.
BEGIN {
    $path = (fileparse("$0"))[1];
}

use lib("$path");
use InstallOps;

# Our recurseAction() subroutine wants the current file as a parameter.
$InstallOps::pass_rec_func_cur_file = 1;

$Win32 = 1 if $ENV{'OS'} =~ /Windows/;

# Ensure that there are four command-line arguments.  If not, exit with
# error status.
if ( $#ARGV < 3 || $#ARGV > 9 ) {
    warn "Usage: $0 -i <source directory> -o <destination directory>\n";
    exit 1;
}

# Get the -i and -o options and store their values in $opt_i and $opt_o
# respectively.
getopt('g:i:m:o:u:');

my $dest_dir = "$opt_o";

# Defaults for ownership and permissions (except on Win32).
my($uid, $gid, $mode) = ($<, (getpwuid($<))[3], "0644") unless $Win32;

if ( $opt_u ) {
    $uname = "$opt_u" if $opt_u;
    my(@user_info) = getpwnam("$uname") or die "getpwnam($uname): $!\n";
    $uid = $user_info[2];
}

if ( $opt_g && ! $Win32 ) {
    $gname = "$opt_g" if $opt_g;
    my(@group_info) = getgrnam("$gname") or die "getgrnam($gname): $!\n";
    $gid = $group_info[2];
}

$mode = "$opt_m" if $opt_m;

# If the mode does not have at least one bit set for execution, define
# $script_mode to set the execute bit for all owners.
if ( $mode !~ /[157]/ ) {
    if ( $mode =~ /^(\d)(\d)(\d)(\d)$/ ) {
        $script_mode = sprintf("%d%d%d%d", $1, $2 + 1, $3 + 1, $4 + 1);
    }
    elsif ( $mode =~ /^(\d)(\d)(\d)$/ ) {
        $script_mode = sprintf("0%d%d%d", $1 + 1, $2 + 1, $3 + 1);
    }
}
else {
    $script_mode = "$mode";
}

umask(002);
mkpath("$dest_dir", 0, 0755);	# Make sure that $dest_dir exists

$start_dir = cwd() unless $dest_dir =~ /^\//;		# Save this for later

# Push the source directory onto the InstallOps module's internal directory
# stack.  This is used for recursion through the source directory.
push(@InstallOps::dirstack, "$opt_i");

# Recurse through $src_dir and create the destination directory tree.
# recurseAction() handles further work.
recurseDir("$opt_i", "$start_dir/$dest_dir");

exit 0;

# -----------------------------------------------------------------------------
# Copy files of a selected type (based on extension) to the appropriate place
# in the destination directory tree.
#
# Syntax:
#     recurseAction($curfile);
#
# Arguments:
#     $curfile - The name of the current file in the recursion process.
# -----------------------------------------------------------------------------
sub recurseAction {
    my $curfile = shift;

    # List of installable file extensions.  These are checked with a
    # case-insensitive regular expression.
    my (@exts) = qw(.txt .c .h .cxx .cpp .pl .desc .dsc .mk .htm .html .gif
                    .jpg .dsw .dsp .java .jar);

    my $installed = 0;

    my $ext = '';
    foreach $ext ( @exts ) {
        if ( $curfile =~ /$ext$/i ) {
            installFile("$curfile", $uid, $gid, "$mode", "$dest_dir");
            $installed = 1;
            last;
        }
    }

    unless ( $installed ) {
        # This is equivalent to a C switch block.
        SWITCH: {
            # Match README.
            if ( "$curfile" eq "README" ) {
                installFile("$curfile", $uid, $gid, "$mode", "$dest_dir");
                last SWITCH;
            }

            # Match Makefile.
            if ( "$curfile" eq "Makefile" ) {
                installFile("$curfile", $uid, $gid, "$mode", "$dest_dir");
                last SWITCH;
            }
        }
    }
}
