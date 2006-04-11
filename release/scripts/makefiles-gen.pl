#!/usr/local/bin/perl

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
# Generate Makefiles in the subdirectories of the installation directory
# (prefix) from Makefile.in's found in the source directory (startdir).  All
# command-line options shown here are required.
#
# Usage:
#     makefiles-gen.pl
#         --vars=<Path to a Perl file containing @..@ substitution values>
#         --srcdir=<Location of source code>
#         --prefix=<Base directory where Makefile will go>
#         --startdir=<Directory where search begins>
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

sub generateFile($$);

# Do this to include the path to the script in @INC.
BEGIN {
    $path = (fileparse("$0"))[1];
}

use lib("$path");
use InstallOps;

$InstallOps::pass_rec_func_cur_file_dir = 1;

$Win32 = 1 if $ENV{'OS'} =~ /Windows/;

# Turn off case insensitivity when parsing command-line options.
$Getopt::Long::ignorecase = 0;

# Initialize variables passed (by reference) to GetOptions();
%VARS = ();
@files = ();
($var_file, $startdir, $prefix) = ('', '', '');
($uname, $gname, $mode) = ('', '', '');

# Parse the command-line options and store the given values in %VARS which
# is indexed by the tag name to be replaced (e.g., 'CXX').
GetOptions("vars=s", \$var_file, "srcdir=s" => \$VARS{'srcdir'},
	   "prefix=s" => \$prefix, "startdir=s" => \$startdir,
	   'files=s' => \@files,
	   "uname=s" => \$uname, "gname=s" => \$gname, "mode=s" => \$mode);

if ( ! ($var_file && exists($VARS{'srcdir'}) && $prefix) )
{
   die <<USAGE_EOF;
Usage:
    $0
        --vars=<Path to a Perl file containing @..@ substitution values>
        --srcdir=<Location of source code>
        --prefix=<Base directory where Makefile will go>
      [ --startdir=<Directory where search begins>
        --uname=<Owner's user name> --gname=<Group name> --mode=<Mode bits> ]
USAGE_EOF
}

die "ERROR: Cannot specify file and directory recursion together!\n"
   if $#files != -1 && $startdir;

# Import the extra settings for %VARS found in $var_file if it was given
# on the command line.
if ( $var_file )
{
   my @path_info = fileparse("$var_file");
   push(@INC, "$path_info[1]");
   require "$path_info[0]";
}

umask(002);

# Defaults.
my($uid, $gid, $mode_bits) = ($<, (getpwuid($<))[3], "0644") unless $Win32;

if ( $uname )
{
   my(@user_info) = getpwnam("$uname") or die "getpwnam($uname): $!\n";
   $uid = $user_info[2];
}

if ( $gname && ! $Win32 )
{
   my(@group_info) = getgrnam("$gname") or die "getgrnam($gname): $!\n";
   $gid = $group_info[2];
}

$mode_bits = "$mode" if $mode;

if ( $startdir )
{
   chdir("$startdir") or die "ERROR: Cannot chdir to $startdir: $!\n";
   recurseDir(".", "$prefix");
}
else
{
   my $cwd = getcwd();
   foreach ( @files )
   {
      my @full_path = split(/\//);
      my $file      = pop(@full_path);
      my $new_dir   = join("/", @full_path);

      if ( chdir("$new_dir") )
      {
         generateFile("$file", "");
         chdir("$cwd");
      }
      else
      {
         warn "WARNING: Could not chdir to $new_dir: $!\n";
      }
   }
}

exit 0;

# -----------------------------------------------------------------------------
# If the given file is named Makefile.in, use it as a template to generate a
# makefile that will be installed.
#
# Syntax:
#     recurseAction($curfile, $curpath);
#
# Arguments:
#     $curfile - The name of the current file in the recursion process.
#     $curpath - The current directory stack.
# -----------------------------------------------------------------------------
sub recurseAction
{
   my $curfile = shift;
   my $curpath = shift;

   generateFile("$curfile", "$curpath") if "$curfile" eq "Makefile.in";
}

sub generateFile ($$)
{
   my $infile  = shift;
   my $curpath = shift;

   my $outfile = "$prefix/$curpath/$infile";
   $outfile    =~ s/\.in$//;

   print "Generating $outfile ...\n";

   my $workfile = "working.in-$$";

   # Make a working copy of the input file to be safe.
   copy("$infile", "$workfile") unless "$infile" eq "$workfile";

   # Replace the tags in $workfile with the values in %VARS.
   next if replaceTags("$workfile", %VARS) < 0;

   # Move $workfile to its final destination.
   copy("$workfile", "$outfile");

   if ( ! $Win32 )
   {
      chown($uid, $gid, "$outfile") or die "chown: $!\n";
   }

   chmod(oct($mode_bits), "$outfile") or die "chmod: $!\n";
   unlink("$workfile");
}
