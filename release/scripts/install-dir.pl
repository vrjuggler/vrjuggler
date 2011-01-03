#!/usr/bin/env perl

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Install an entire directory from the VR Juggler development tree without
# the CVS directories.  All files in the directory tree are copied to the
# destination directory.
#
# Usage:
#     install-dir.pl -i <source directory> -o <destination directory>
#                   [-u <user name> -g <group name> -m <mode>]
#
#     <source directory> - The source directory to be installed
#     <destination directory> - The location to install the source directory
# -----------------------------------------------------------------------------

require 5.004;

use Cwd;
use File::Basename;
use File::Copy;
use File::Path;
use Getopt::Std;

# Do this to include the path to the script in @INC.
BEGIN
{
   $path = (fileparse("$0"))[1];
}

use lib("$path");
use InstallOps;

# Our recurseAction() subroutine wants the current file as a parameter.
$InstallOps::pass_rec_func_cur_file = 1;

$Win32 = 1 if $ENV{'OS'} =~ /Windows/;

# Ensure that there are four command-line arguments.  If not, exit with
# error status.
if ( $#ARGV < 3 )
{
   warn "Usage: $0 -i <source directory> -o <destination directory>\n";
   exit 1;
}

# Get the -i and -o options and store their values in $opt_i and $opt_o
# respectively.
getopts('e:g:i:lm:o:v:');

my(@exts)    = split(/,/, "$opt_e") if $opt_e;
my $src_dir  = "$opt_i";
my $dest_dir = "$opt_o";

if ( $opt_v )
{
   my @path_info = fileparse("$opt_v");
   push(@INC, "$path_info[1]");
   require "$path_info[0]";
}

# Defaults.  getpwuid() is not implemented in the Win32 Perl port.
my($uid, $gid, $mode) = ($<, (getpwuid($<))[3], "0644") unless $Win32;

if ( $opt_u )
{
   $uname = "$opt_u" if $opt_u;
   my(@user_info) = getpwnam("$uname") or die "getpwnam($uname): $!\n";
   $uid = $user_info[2];
}

# getgrnam() is not implemented in the Win32 Perl port.
if ( $opt_g && ! $Win32 )
{
   $gname = "$opt_g" if $opt_g;
   my(@group_info) = getgrnam("$gname") or die "getgrnam($gname): $!\n";
   $gid = $group_info[2];
}

$mode = "$opt_m" if $opt_m;

my $make_symlink = ($opt_l ? 1 : 0);

umask(002);
mkpath("$dest_dir", 0, 0755);	# Make sure that $dest_dir exists

$start_dir = cwd() unless $dest_dir =~ /^\//;		# Save this for later

# Push the source directory onto the InstallOps module's internal directory
# stack.  This is used for recursion through the source directory.
push(@InstallOps::dirstack, "$src_dir");

# Recurse through $src_dir and create the destination directory tree.
# recurseAction() handles further work.
recurseDir("$src_dir", "$start_dir/$dest_dir");

exit 0;

# -----------------------------------------------------------------------------
# Copy the current file to the appropriate place in the destination directory
# tree.
#
# Syntax:
#     recurseAction($curfile);
#
# Arguments:
#     $curfile - The name of the current file in the recursion process.
# -----------------------------------------------------------------------------
sub recurseAction
{
   my $curfile = shift;

   # Install only the files with extensions listed in @exts if there are any
   # elements in @exts.
   if ( $#exts != -1 )
   {
      foreach ( @exts )
      {
         if ( $curfile =~ /$_$/ )
         {
            installFile("$curfile", $uid, $gid, "$mode", "$dest_dir",
                        $make_symlink);
            last;
         }
      }
   }
   else
   {
      # If the current file is a .in template file, process it before
      # installing.
      if ( $curfile =~ /^(.+?)\.in$/ )
      {
         my $filename = "$1";

         my $workfile;

         if ( $Win32 )
         {
            $workfile = "C:/temp/$curfile";
         }
         else
         {
            $workfile = "/tmp/$curfile";
         }

         # Make a working copy of the input file to be safe.
         my @stats = (stat("$curfile"))[8,9];
         copy("$curfile", "$workfile") unless "$curfile" eq "$workfile";

         # Replace the tags in $workfile with the values in %VARS.
         if ( replaceTags("$workfile", %VARS) < 0 )
         {
            copy("$curfile", "$filename");
         }
         # If replaceTags() succeeded, move the work file to the file name
         # to be installed.
         else
         {
            copy("$workfile", "$filename");
            unlink("$workfile")
               or warn "WARNING: Could not delete $workfile: $!";
         }

         # Apply the saved attributes of $curfile to $filename.
         utime(@stats, "$filename");

         installFile("$filename", $uid, $gid, "$mode", "$dest_dir",
                     $make_symlink);

         # Delete the generated file now that we are done with it.
         unlink("$filename") or warn "WARNING: Could not delete $filename: $!";
      }
      # Otherwise, install it as-is.
      else
      {
         installFile("$curfile", $uid, $gid, "$mode", "$dest_dir",
                     $make_symlink);
      }
   }
}
