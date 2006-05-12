#!/usr/local/bin/perl

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Cross-platform mtree based on the FreeBSD 3.0 version of mtree(8) which is
# based on the 4.3 BSD-Reno version of the utility.
#
#     Patrick Hartling <patrick@vrac.iastate.edu>
#     October/November 1998
# -----------------------------------------------------------------------------
# This file contains a modified version of the wrap() function included with
# Perl 5 in the Text::Wrap module.  It was written by David Muir Sharnoff
# <muir@idiom.com> with help from Tim Pierce and others including Jacqui
# Caren.
# -----------------------------------------------------------------------------

require 5.003;

use Cwd;
use File::Path;
use Getopt::Std;
use Text::Tabs qw(expand unexpand);
use Time::localtime;

# Require that all global variables be defined here.  Otherwise, they must be
# lexically scoped (my) variables.
use strict 'vars';
use vars qw(%opts $name_printed);

require "hostname.pl";

# Subroutine prototypes.
sub nonfatalError($;$);
sub fatalError($;$);
sub getCurPath();
sub printDirStack();
sub top(@);
sub getStats($);
sub printTree(;$);
sub checkPrint(%%);
sub parseSpecFile();
sub processFile($$$$$$$$$$$$$$$);
sub checkKeyVal($$);
sub printName($);
sub getType($);
sub checkType($$);
sub wrap($$$@);

my $Win32 = 1 if $ENV{'OS'} =~ /Windows/;

# Parse command-line options.
getopts('cdeif:np:Uu', \%opts);

# Initialize variables scoped to this package.
my(%KEYWORDS) = ();
my(%SET_DEFAULTS) = ();
my(@dirstack) = ();
my($space_count, $exit_status) = (0, 0);

# Passing -U enables -u.
#$opts{'u'} = 1 if $opts{'U'};

# If -c was given, print out the directory tree specification rooted at the
# current directory or at the directory given with -p if that argument was
# passed as well.
if ( $opts{'c'} )
{
   if ( $opts{'p'} )
   {
      chdir("$opts{'p'}") or die "$0: $opts{'p'}: $!\n";
   }

   print "#           user: ", (getpwuid($<))[0], "\n";
   print "#        machine: ", hostname(), "\n";
   print "#           tree: ", cwd(), "\n";
   print "#           date: ", ctime(), "\n";

   printTree(".");
}
# Otherwise, parse the specification file and compare it against the tree
# rooted at the current directory or at the directory given with the -p option
# if that argument was passed.
else
{
   parseSpecFile();
}

# $exit_status may have been modified if parseSpecFile() was run.
exit $exit_status;

# =========================================================================== #
# Subroutines follow                                                          #
# =========================================================================== #

# -----------------------------------------------------------------------------
# Print a non-fatal error message to STDERR.  This is similar to the
# functionality provided by warn().
#
# PRE: None.
# POST: An error is printed to STDERR that contains the script name, a path
#       if specified and an error message.
#
# ARGS: $msg  - The error message to print.
#       $path - The path to which the error corresponds.  Optional.
# -----------------------------------------------------------------------------
sub nonfatalError($;$)
{
   my $msg = shift;
   my $path = shift || "";

   print STDERR "$0: ";
   print STDERR "$path: " if $path;
   print STDERR "$msg\n";
}

# -----------------------------------------------------------------------------
# Print a fatal error message to STDERR and exit with status 1.  This is
# similar to the functionality provided by die().
#
# PRE: None.
# POST: An error is printed to STDERR that contains the script name, a path
#       if specified and an error message.  The script exits after the error
#       is printed.
#
# ARGS: $msg  - The error message to print.
#       $path - The path to which the error corresponds.  Optional.
# -----------------------------------------------------------------------------
sub fatalError($;$)
{
   my $msg = shift;
   my $path = shift || "";

   nonfatalError("$msg", "$path");
   exit 1;
}

# -----------------------------------------------------------------------------
# Return the current path as a scalar string.
#
# PRE: None.
# POST: A scalar string is generated from @dirstack.
#
# RETURNS: A scalar string containing the current path in @dirstack.
# -----------------------------------------------------------------------------
sub getCurPath()
{
   return join("/", @dirstack);
}

# -----------------------------------------------------------------------------
# Print the directory stack.  This will have a trailing "/" if @dirstack is
# not empty.
#
# PRE: None.
# POST: The current directory stack is printed to STDOUT.
# -----------------------------------------------------------------------------
sub printDirStack()
{
   foreach ( @dirstack )
   {
      print "$_/";
   }
}

# -----------------------------------------------------------------------------
# Return the top of the stack (array) passed.
#
# PRE: None.
# POST: The top element of the stack passed is returned.
#
# ARGS: One argument of type array.
#
# RETURNS: The top element of the stack passed if the stack is not empty.
#          undef is returned if the stack is empty.
# -----------------------------------------------------------------------------
sub top(@)
{
   return ( $#_ >= 0 ) ? $_[$#_] : undef;
}

# -----------------------------------------------------------------------------
# Generate a hash containing all the information about the given file (or
# directory) and return it to the caller.  The information returned will have
# keys 'name', 'mode', 'nlink', 'uid', 'gid', 'size' and 'time'.
#
# PRE: The file named by the single argument must exist.
# POST: The stat() info for the named file or directory is read and stored in
#       a hash.
#
# ARGS: $filename - A scalar argument naming a file or directory.
#
# RETURNS: A hash containing the stat() information for the named file or
#          directory.
# -----------------------------------------------------------------------------
sub getStats($)
{
   my $filename = shift;

   my(%info) = ();
   my(@fileinfo) = stat("$filename");

   # This is a (terrible?) hack to get the file permissions unmangled.
   $fileinfo[2] = sprintf("%o", $fileinfo[2]);
   $fileinfo[2] =~ s/^\d+(\d{4})$/\1/;

   $info{'name'}  = "$filename";
   $info{'mode'}  = "$fileinfo[2]";
   $info{'nlink'} = "$fileinfo[3]";
   $info{'uid'}   = "$fileinfo[4]";
   $info{'gid'}   = "$fileinfo[5]";
   $info{'size'}  = "$fileinfo[7]";
   $info{'time'}  = "$fileinfo[9]";
   $info{'type'}  = getType("$filename");

   return %info;
}

# -----------------------------------------------------------------------------
# Print the specification for the directory tree (and all its files) rooted
# at the named directory.
#
# PRE: None.
# POST: The mtree-readable tree specification is printed to STDOUT.
#
# ARGS: $cur_dir - The base directory of the tree to print.  If no name is
#                  passed, it defaults to the current directory.
# -----------------------------------------------------------------------------
sub printTree(;$)
{
   my $cur_dir = shift || ".";

   print "\n" unless $opts{'d'};

   chdir("$cur_dir") or fatalError("$!", getCurPath());
   push(@dirstack, "$cur_dir");

   opendir(DIR, ".") or fatalError("$!", getCurPath());
   my(@files) = readdir(DIR);
   closedir(DIR);

   my $file;
   my(@contents, $i) = ((), 0);

   # Get the stat() info for all the files/directories read.
   foreach $file ( @files )
   {
      # If -d was specified, we skip everything that is not a directory.
      # This saves a lot of time later.
      next if $opts{'d'} && ! -d "$file";

      my %file_info = getStats("$file");

      # Store a reference to the hash in @contents.  This keeps everything
      # ordered as it was read from the directory.
      $contents[$i++] = \%file_info;

      if ( "$file" eq "." )
      {
         # This is the starting directory if $cur_dir is '.'.
         if ( "$cur_dir" eq "." )
         {
            %SET_DEFAULTS = %file_info;
            delete($SET_DEFAULTS{'name'});
            delete($SET_DEFAULTS{'type'});
         }

         foreach ( keys(%SET_DEFAULTS) )
         {
            if ( "$SET_DEFAULTS{$_}" ne "$file_info{$_}" )
            {
               $SET_DEFAULTS{$_} = "$file_info{$_}";
            }
         }
      }
   }

   my(%cur_defaults) = ();
   my(%max_hash) = ();
   my(%counts) = ();
   my($key, $value);

   foreach ( @contents )
   {
      while ( ($key, $value) = each(%$_) )
      {
         next if "$key" eq "name" || "$key" eq "size" ||
                 "$key" eq "time" || "$key" eq "type";

         $counts{"$key"}{"$value"}++;
      }
   }

   foreach $key ( keys(%counts) )
   {
      foreach $value ( keys(%{$counts{"$key"}}) ) {
         if ( $counts{"$key"}{"$value"} > $max_hash{"$key"} )
         {
            $max_hash{"$key"} = $counts{"$key"}{"$value"};
            $cur_defaults{"$key"} = $value;
         }
      }
   }

   my(@dirs) = ();

   foreach $file ( @contents )
   {
      print "# ", getCurPath(), "\n" if "$$file{'name'}" eq "." && ! $opts{'n'};

      if ( "$$file{'type'}" eq 'dir' && "$$file{'name'}" !~ /^\.\.?$/ )
      {
         push(@dirs, $file);
      }
      else
      {
         my $name = "$$file{'name'}";
         next if "$name" eq "..";

         # If this is the '.' directory, then we may need to print out a
         # new batch of settings if anything is different between the
         # base default values (%SET_DEFAULTS) and the defaults for this
         # directory (%cur_defaults).
         if ( "$name" eq "." && checkPrint(%SET_DEFAULTS, %cur_defaults) )
         {
            %SET_DEFAULTS = %cur_defaults;

            # Only print the type as a directory if -d was given on the
            # command line.  In this case, only directories will be in
            # @contents, so this check is safe.
            if ( $opts{'d'} )
            {
               print "/set type=dir ";
            }
            else
            {
               print "/set type=file ";
            }

            foreach ( keys(%SET_DEFAULTS) )
            {
               print "$_=$SET_DEFAULTS{$_} ";
            }

            print "\n";
         }

         my $output = "";

         if ( "$$file{'type'}" eq 'dir' )
         {
            my $dirname = top(@dirstack);

            if ( length($dirname) >= 16 )
            {
               print " " x $space_count, "$dirname \\\n";
               $output = " " x (16 + $space_count);
            }
            else
            {
               $output .= " " x $space_count . "$dirname" .
                          " " x (16 - length($dirname));
            }
         }
         else
         {
            if ( length($name) >= 12 )
            {
               print " " x ($space_count + 4), "$name \\\n";
               $output = " " x (16 + $space_count);
            }
            else
            {
               $output .= " " x ($space_count + 4) . "$name" .
                          " " x (12 - length($name));
            }
         }

         $output .= "type=$$file{'type'} "
            unless "$$file{'type'}" eq 'file' || $opts{'d'};

         $output .= "uid=$$file{'uid'} "
            unless $cur_defaults{'uid'} eq $$file{'uid'};

         $output .= "gid=$$file{'gid'} "
            unless $cur_defaults{'gid'} eq $$file{'gid'};

         $output .= "mode=$$file{'mode'} "
            unless $cur_defaults{'mode'} eq $$file{'mode'};

         $output .= "nlink=$$file{'nlink'} "
            unless $cur_defaults{'nlink'} eq $$file{'nlink'};

         # These two are always printed.
         $output .= "size=$$file{'size'} time=$$file{'time'}";

         if ( length("$output") >= 70 )
         {
            $output =~ s/^(\s*)(\S.+)$/\2/;
            my(@lines) = wrap(70, "$1", " " x (16 + $space_count), "$output");

            my $i;
            for ( $i = 0; $i < $#lines - 1; $i++ )
            {
               print "$lines[$i] \\\n";
            }

            print "$lines[$#lines]\n";
         }
         else
         {
            print "$output\n";
         }
      }
   }

   foreach ( @dirs )
   {
      $space_count += 4 if $opts{'i'};
      printTree("$$_{'name'}");

      print " " x $space_count, "# ", getCurPath(), "\n" if ! $opts{'n'};
      print " " x $space_count, "..\n";
      print "\n" unless $opts{'d'};

      chdir("..");
      pop(@dirstack);
      $space_count -= 4 if $opts{'i'};
   }

   # This is always the LAST thing printed (i.e., after every tree has been
   # traversed).
   print "..\n" if top(@dirstack) eq ".";
}

# -----------------------------------------------------------------------------
# Determine if the contents of the base hash differ in any way from those of
# the local hash.
#
# PRE: None.
# POST: The two hashes are compared, and a value appropriate for testing in
#       conditionals is returned.
#
# ARGS: %base  - A basic set of keys and values (which in this case has
#                global scope).
#       %local - A more tightly scoped set of keys and values (which in this
#                case is used to override what is in %base).
#
# RETURNS: 0 - There are no differences between the two hashes.
#          1 - At least one difference between the two hashes.
# -----------------------------------------------------------------------------
sub checkPrint(%%)
{
   my (%base, %local) = @_;

   my $diff_count = 0;

   foreach ( keys(%base) )
   {
      return 1 if "$base{$_}" ne "$local{$_}";
   }

   foreach ( keys(%local) )
   {
      return 1 if "$base{$_}" ne "$local{$_}";
   }

   return 0;
}

# -----------------------------------------------------------------------------
# Read the specification file and compare it against the directory tree rooted
# at the current directory or at the tree rooted at the directory named by the
# -p argument if it was passed.
#
# PRE: None.
# POST: The specification file (read either from STDIN or from a file named
#       with the -f option) is read.  If -U and/or -u are given on the command
#       line, the directory tree is updated as necessary to match the
#       specification.  Otherwise, any differences between the specification
#       file and the tree are printed to STDOUT with no update.
# -----------------------------------------------------------------------------
sub parseSpecFile()
{
   # If -f was given on the command line, read the specification file from
   # the file named with that argument.
   if ( $opts{'f'} )
   {
      open(DISTFILE, "$opts{'f'}") or fatalError("Cannot open $opts{'f'}: $!");
   }
   # Otherwise, read it from STDIN.
   else
   {
      open(DISTFILE, "-");
   }

   # If -p was given on the command line, start the tree check from that
   # directory (assuming that it exists).
   if ( $opts{'p'} )
   {
      chdir("$opts{'p'}") or die "$0: $opts{'p'}: $!\n";
   }

   my $line;
   while ( $line = <DISTFILE> )
   {
      $line =~ s/#.*$//;                # Strip out comments

      next if $line =~ /^$/;            # Skip blank lines

      # Join lines ending in '\'.
      if ( $line =~ /^(.*)\\\s*$/ )
      {
         my $new_line = "$1";

         while ( $line = <DISTFILE> )
         {
            if ( $line =~ /^(.*)\\\s*$/ )
            {
               $new_line .= "$1";
            }
            else
            {
               $new_line .= "$line";
               last;
            }
         }

         $line = "$new_line";
      }

      # '/set ...' line.
      if ( $line =~ /^\/set\s+(\S.+)$/ )
      {
         foreach ( split(/\s+/, "$1") )
         {
            my($keyword, $value) = split(/=/, "$_");
            checkKeyVal("$keyword", "$value");
            $KEYWORDS{"$keyword"} = "$value";
         }
      }
      # '/unset ...' line.
      elsif ( $line =~ /^\/unset\s+(\S.+)$/ )
      {
         foreach ( split(/\s+/, "$1") )
         {
            delete($KEYWORDS{"$_"});
         }
      }
      # '..' directory change line.
      elsif ( $line =~ /^\s*\.\.\s*$/ )
      {
         pop(@dirstack);
         chdir("..");
      }
      # Named file or directory (possibly with options) line.
      elsif ( $line =~ /^\s*(\S+)\s*(\S.+)?$/ )
      {
         my $filename = "$1";

         my(%TEMP) = ();

         if ( $2 )
         {
            foreach ( split(/\s+/, "$2") )
            {
               my($keyword, $value) = split(/=/, "$_");
               checkKeyVal("$keyword", "$value");
               $TEMP{"$keyword"} = "$value";
            }
         }

         processFile("$filename", "$TEMP{'ignore'}", "$TEMP{'nochange'}",
                     "$TEMP{'cksum'}", "$TEMP{'gid'}", "$TEMP{'gname'}",
                     "$TEMP{'md5digest'}", "$TEMP{'mode'}", "$TEMP{'nlink'}",
                     "$TEMP{'uid'}", "$TEMP{'uname'}", "$TEMP{'size'}",
                     "$TEMP{'link'}", "$TEMP{'time'}", "$TEMP{'type'}");
      }
   }

   close(DISTFILE) or nonfatalError("Cannot close input file: $!");
}

# -----------------------------------------------------------------------------
# Process the named file by comparing its existence and various settings
# against what is expected to be found.
#
# PRE: The %KEYWORDS hash should have expected defaults set.
# POST: If -U and/or -u were given on the command line, the file is updated
#       to match its specification and the change is printed to STDOUT.
#       Otherwise, the information that would be changed is printed to STDOUT
#       with no update.
#
# ARGS: $name      - The name of the file to process.
#       $ignore    - Ignore any file heirarchy below this file.
#       $nochange  - Make sure this file exists but ignore all other
#                    attributes.
#       $cksum     - The checksum of the file using the default algorithm
#                    specified by the cksum(1) utility.
#       $gid       - The expected owner's group ID.
#       $gname     - The expected owner's group name.
#       $md5digest - The MD5 message digest of the file.
#       $mode      - The expected permission bits (in octal).
#       $nlink     - The expected number of hard links to the file.
#       $uid       - The expected owner's user ID.
#       $uname     - The expected owner's user name.
#       $size      - The expected size.
#       $link      - The file the link is expected to reference.
#       $time      - The expected modification time.
#       $type      - The expected file type.
# -----------------------------------------------------------------------------
sub processFile($$$$$$$$$$$$$$$)
{
   my $name      = shift;
   my $ignore    = $_[0] || $KEYWORDS{'ignore'};
   my $nochange  = $_[1] || $KEYWORDS{'nochange'};
   my $cksum     = $_[2] || $KEYWORDS{'cksum'};
   my $gid       = $_[3] || $KEYWORDS{'gid'};
   my $gname     = $_[4] || $KEYWORDS{'gname'};
   my $md5digest = $_[5] || $KEYWORDS{'md5digest'};
   my $mode      = $_[6] || $KEYWORDS{'mode'};
   my $nlink     = $_[7] || $KEYWORDS{'nlink'};
   my $uid       = $_[8] || $KEYWORDS{'uid'};
   my $uname     = $_[9] || $KEYWORDS{'uname'};
   my $size      = $_[10] || $KEYWORDS{'size'};
   my $link      = $_[11] || $KEYWORDS{'link'};
   my $time      = $_[12] || $KEYWORDS{'time'};
   my $type      = $_[13] || $KEYWORDS{'type'};

   if ( ! $Win32 )
   {
      $uname = getpwuid($<) if ! $uname;

      if ( $uname && ! $uid )
      {
         my(@user_info) = getpwnam("$uname") or die "getpwnam($uname): $!\n";
         $uid = $user_info[2];
      }

      if ( $gname && ! $gid )
      {
         my(@group_info) = getgrnam("$gname") or die "getgrnam($gname): $!\n";
         $gid = $group_info[2];
      }
   }

   SWITCH:
   {
      if ( "$type" eq 'dir' )
      {
         # XXX: This is kind of a hack to deal with Perl thinking that a
         # symlink pointing to a directory is actually a directory.
         if ( -l "$name" )
         {
            warn "WARNING: Symlink exists where directory expected (",
                 printDirStack(), ") -- removing link\n";
            unlink("$name");
         }

         if ( ! -d "$name" )
         {
            print "missing: ";
            printDirStack();
            print "$name";

            $exit_status = 2 if $opts{'u'};

            if ( $opts{'U'} || $opts{'u'} ) {
               mkdir("$name", 0755) or die "ERROR: Cannot mkdir $name: $!\n";
               chmod(oct($mode), "$name") if $mode;
               chown($uid, $gid, "$name");
               print " (created)\n";
            }
            else
            {
               print "\n";
               return;
            }
         }

         last SWITCH;
      }

      # If -d was given on the command line and execution reaches this
      # point, the type of $name is not a directory, so it should be
      # ignored.
      return if $opts{'d'};

      if ( "$type" eq 'file' )
      {
         if ( ! -f "$name" )
         {
            print "missing: ";
            printDirStack();
            print "$name ";
         }

         last SWITCH;
      }

      if ( "$type" eq 'link' )
      {
         if ( ! -l "$name" )
         {
            print "missing: ";
            printDirStack();
            print "$name ";
         }

         last SWITCH;
      }
   } # SWITCH

   # The file in question now exists.  If we are not ignoring other
   # attributes, check its other expected settings.
   if ( ! $nochange )
   {
      my(%STATS) = getStats("$name");
      local $name_printed = 0;

      if ( "$type" ne "" && ! checkType("$name", "$type") )
      {
         printName("$name");
         print "type ($type, ", getType("$name"), ")\n";
      }

      if ( "$mode" ne "" && $STATS{'mode'} != $mode )
      {
         printName("$name");
         print "permissions ($mode, $STATS{'mode'}";

         if ( $opts{'U'} || $opts{'u'} )
         {
            if ( ! chmod(oct($mode), "$name") )
            {
               warn "\nWARNING: Cannot set $name to mode $mode: $!\n";
            }
            else
            {
               print ", modified)\n";
               $STATS{'mode'} = $mode;
            }
         }
         else
         {
            print ")\n";
         }

         $exit_status = 2 if $opts{'u'};
      }

      if ( $uid && $uid != $STATS{'uid'} )
      {
         printName("$name");
         print "uid ($uid, $STATS{'uid'}";

         if ( $opts{'U'} || $opts{'u'} )
         {
            if ( ! chown($uid, $STATS{'gid'}, "$name") )
            {
               warn "\nWARNING: Cannot set $name owner to $uid: $!\n";
            }
            else
            {
               print ", modified)\n";
               $STATS{'uid'} = $uid;
            }
         }
         else
         {
            print ")\n";
         }

         $exit_status = 2 if $opts{'u'};
      }

      if ( $gid && $gid != $STATS{'gid'} )
      {
         printName("$name");
         print "gid ($gid, $STATS{'gid'}";

         if ( $opts{'U'} || $opts{'u'} )
         {
            if ( ! chown($STATS{'uid'}, $gid, "$name") )
            {
               warn "\nWARNING: Cannot set $name group to $gid: $!\n";
            }
            else
            {
               print ", modified)\n";
               $STATS{'gid'} = $gid;
            }
         }
         else
         {
            print ")\n";
         }

         $exit_status = 2 if $opts{'u'};
      }
   }

   # If this file is a directory and we are not ignoring the file heirarchy
   # below it, push it onto @dirstack and continue processing from within
   # the directory.
   if ( "$type" eq 'dir' && ! $ignore )
   {
      push(@dirstack, "$name");
      chdir("$name");
   }
}

# -----------------------------------------------------------------------------
# Check the validity of the given keyword and its value.  If it is invalid,
# the script will exit here with an error.
#
# PRE: None.
# POST: If the keyword and its value are valid, success status is returned.
#       Otherwise, the script exits with error status.
#
# ARGS: $keyword - One of the recognized keywords that can appear in an mtree
#                  specification file.
#       $value   - The value associated with $keyword.
#
# RETURNS: 1 - The keyword and value are valid.
# -----------------------------------------------------------------------------
sub checkKeyVal($$)
{
   my($keyword, $value) = @_;
   my $err = "";

   # Check group name.
   if ( "$keyword" eq "gname" && ! $Win32 ) {
      $err = "No such group '$value'" if getgrnam("$value") eq undef;
   }
   # Check user name.
   elsif ( "$keyword" eq "uname" && ! $Win32 ) {
      $err = "No such user '$value'" if getpwnam("$value") eq undef;
   }

   if ( $err )
   {
      die "$0: line $.: $err\n";
   }
   else
   {
      return 1;
   }
}

# -----------------------------------------------------------------------------
# Print the name of a file (or directory) and add space padding as necessary.
#
# PRE: None
# POST: The name of the given file is printed to STDOUT, and spaces are
#       added for padding as necessary.  If the file name is longer than
#       seven characters, a second line is used for the first line of the
#       file's specification.
#
# ARGS: $name - The name of the file to print.
# -----------------------------------------------------------------------------
sub printName($)
{
   my $name = shift;

   if ( ! $name_printed )
   {
      print "$name:", " " x (7 - length("$name"));
      print "\n        " if length("$name") >= 8;
      $name_printed = 1;
   }
   else
   {
      print " " x 8;
   }
}

# -----------------------------------------------------------------------------
# Get the type of the named file.
#
# PRE: The file exists in the current directory.
# POST: The type of the file is determined and returned.
#
# ARGS: $name - The name of the file to check.
#
# RETURNS: The type of the given file.
# -----------------------------------------------------------------------------
sub getType($)
{
   my $name = shift;

   my $type = "";

   # This is a directory (type => 'dir').
   if ( -d "$name" )
   {
      $type = 'dir';
   }
   # This is an ordinary file (type => 'file').
   elsif ( -f "$name" )
   {
      $type = 'file';
   }
   # This is a block device (type => 'block').
   elsif ( -b "$name" )
   {
      $type = 'block';
   }
   # This is a character device (type => 'char').
   elsif ( -c "$name" )
   {
      $type = 'char';
   }
   # This is a named pipe/FIFO (type => 'fifo').
   elsif ( -p "$name" )
   {
      $type = 'fifo';
   }
   # This is a socket (type => 'socket').
   elsif ( -S "$name" )
   {
      $type = 'socket';
   }
   # This is a symbolic link (type => 'link').  This is last because
   # $name might be a symbolic link to one of the above.
   elsif ( -l "$name" )
   {
      $type = 'link'
   }

   return "$type";
}

# -----------------------------------------------------------------------------
# Determine if the specified type matches the actual type of the named file.
#
# PRE: The file exists in the current directory.
# POST: The exptected type is compared against the actual type, and a value
#       suitable for use in a conditional statement is returned.
#
# ARGS: $name - The name of the file to check.
#       $type - The expected type of the file.
#
# RETURNS: 0 - The given type does not match the actual type of the file.
#          1 - The given type matches the actual type.
# -----------------------------------------------------------------------------
sub checkType($$)
{
   my $name = shift;
   my $type = shift;

   if ( "$type" eq getType("$name") )
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

# -----------------------------------------------------------------------------
# Wrap the text in the given array of lines to fit within the specified
# column width.
# -----------------------------------------------------------------------------
sub wrap($$$@)
{
   my($columns, $init_tab, $sub_tab, @text) = @_;

   my(@ret_val);
   my $text = expand(join(" ", @text));

   my $lead = "$init_tab";
   my $lead_len = $columns - length(expand("$lead")) - 1;

   $text =~ s/^\s+//;

   while ( length("$text") > $lead_len )
   {
      # Remove up to a line length of things that aren't new lines and tabs.
      if ( $text =~ s/^([^\n]{0,$lead_len})(\s|\Z(?!\n))// )
      {
         my($l, $r) = ($1, $2);
         $l =~ s/\s+$//;
         push(@ret_val, unexpand($lead . $l), "\n");
      }
      elsif ( $text =~ s/^([^\n]{$lead_len})// )
      {
         push(@ret_val, unexpand($lead . $1), "\n");
      }

      # Recompute the leader.
      $lead = "$sub_tab";
      $lead_len = $columns - length(expand("$lead")) - 1;
      $text =~ s/^\s+//;
   }

   push(@ret_val, "$lead" . "$text") if "$text" ne "";
   return @ret_val;
}
