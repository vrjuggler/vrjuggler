# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

package SourceList;

require 5.004;

use strict qw(vars);

# -----------------------------------------------------------------------------
# Constructor.
# -----------------------------------------------------------------------------
sub new($;@)
{
   my $class = shift;

   # Make the anonymous array reference that will contain the list of files.
   # If a list of files was given as the optional arguments to the
   # constructor, assign that list to the new array reference.
   # XXX: I suspect that this could all be done in a single assignment to
   # \@_, but I am wary of using a reference to something that I didn't
   # create myself.
   my $files = [];
   @{$files} = @_ if @_;

   # The structure used for this class is a little complicated.  It uses
   # the following data members:
   #
   #     all_files - An array reference.
   #     suffixes  - A hash with suffixes as keys but with no associated
   #                 values.  A hash is used to prevent duplication of
   #                 suffixes.
   #     dirs      - A hash whose keys are the directories containing source
   #                 files.  The values associated with the keys are in turn
   #                 hashes whose keys are suffixes.  The values associated
   #                 with those keys are the actual files in an aray
   #                 reference.  Conceptually, it looks like this:
   #
   #                     dirs => {
   #                         dir1 => {
   #                             .c   => [ file1.c, file2.c ],
   #                             .cxx => [ file3.cxx, file4.cxx ],
   #                             :
   #                             :
   #                         },
   #                         dir2 => {
   #                             .c++ => [ file5.c++, file6.c++ ],
   #                             .cpp => [ file7.cpp, file8.cpp ],
   #                             :
   #                             :
   #                         },
   #                         :
   #                         :
   #                     }
   return bless {
      'all_files' => $files,    # Full list of all known source files
      'suffixes'  => {},        # List of suffixes used by those files
      'dirs'      => {}         # Complete categorization of source files
   }, $class;
}

# -----------------------------------------------------------------------------
# Destructor.
# -----------------------------------------------------------------------------
sub DESTROY($)
{
   my $this = shift;
   return 1;
}

# -----------------------------------------------------------------------------
# Read all the source files from the given directory.  This populates this
# object with the actual source list.
# -----------------------------------------------------------------------------
sub readSources($$)
{
   my $this   = shift;
   my $dir    = shift;
   my @nosrcs = @_;

   my $status;

   if ( opendir(SRCDIR, "$dir") )
   {
      my @all_files = readdir(SRCDIR);
      closedir(SRCDIR);

      ${$this->{'dirs'}}{"$dir"} = {};

      my $file;
      foreach $file ( @all_files )
      {
         # Only match C/C++ source files.
         if ( -f "$dir/$file" && $file =~ /(\.(c\+\+|cpp|cxx|cc|c))$/i )
         {
            my $ext = "$1";

            # Compare the current file name against the list of files to
            # be excluded.
            my $exclude = 0;
            foreach ( @nosrcs )
            {
               if ( "$dir/$file" eq "$_" )
               {
                  $exclude = 1;
                  last;
               }
            }

            # Skip this file if is to be excluded.
            next if $exclude;

            # Add this suffix to the main suffix hash.
            ${$this->{'suffixes'}}{"$ext"} = '';

            # Create the anonymous array for the files if this extension
            # has not already been registered.
            unless ( exists(${${$this->{'dirs'}}{"$dir"}}{"$ext"}) )
            {
               ${${$this->{'dirs'}}{"$dir"}}{"$ext"} = [];
            } 

            # Add this file to the list of files for the the current file
            # extension in this directory, and add it to the main list of
            # all source files.
            push(@{${$this->{'dirs'}{"$dir"}}{"$ext"}}, "$file");
            push(@{$this->{'all_files'}}, "$file");
         }
      }

      $status = 1;
   }
   else
   {
      warn "WARNING: Could not open directory $dir: $!\n";
      $status = 0;
   }

   return $status;
}

# -----------------------------------------------------------------------------
# Deterime if the given directory is known to this source file list.
#
# Returns:
#     0 - The directory is not in this source list.
#     1 - The directory is in this source list.
# -----------------------------------------------------------------------------
sub hasDirectory($$)
{
   my $this = shift;

   return exists(${$this->{'dirs'}}{"$_[0]"});
}

sub hasSuffix($$$)
{
   my $this = shift;
   my $dir  = shift;
   my $ext  = shift;

   return exists(${${$this->{'dirs'}}{"$dir"}}{"$ext"});
}

# -----------------------------------------------------------------------------
# Get all the files registered with this object.
# -----------------------------------------------------------------------------
sub getAllFiles($)
{
   my $this = shift;
   return @{$this->{'all_files'}};
}

# -----------------------------------------------------------------------------
# Get all the directories registered with this object.
# -----------------------------------------------------------------------------
sub getDirectories($)
{
   my $this = shift;
   return keys(%{$this->{'dirs'}});
}

# -----------------------------------------------------------------------------
# Get all the suffixes registered with this object or all the suffixes in use
# within a given directory.
# -----------------------------------------------------------------------------
sub getSuffixes($;$)
{
   my $this = shift;
   my $dir  = shift || '';

   my @suffixes = ();

   # If a specific directory is requested, return only its suffixes.
   if ( $dir )
   {
      @suffixes = keys(%{${$this->{'dirs'}}{"$dir"}});
   }
   # Otherwise, return all the known suffixes.
   else
   {
      @suffixes = keys(%{$this->{'suffixes'}});
   }

   return @suffixes;
}

# -----------------------------------------------------------------------------
# Get all the files in the given directory with the given suffix.
# -----------------------------------------------------------------------------
sub getFiles($$$)
{
   my $this   = shift;
   my $dir    = shift;
   my $suffix = shift;

   return @{${${$this->{'dirs'}}{"$dir"}}{"$suffix"}};
}

# -----------------------------------------------------------------------------
# Find a file in this source list.  Its containing directory is returned to
# the caller, or undef is returned if the file is not in this list.
# -----------------------------------------------------------------------------
sub findFile($$)
{
   my $this = shift;
   my $file = shift;

   my($dir, $suffix);
   foreach $dir ( $this->getDirectories() )
   {
      foreach $suffix ( $this->getSuffixes("$dir") )
      {
         if ( $file =~ /$suffix$/i )
         {
            foreach ( $this->getFiles("$dir", "$suffix") )
            {
               return $dir if "$file" eq "$_";
            }
         }
      }
   }

   return undef;
}

# -----------------------------------------------------------------------------
# Insert a file into this source list within the given directory.
# -----------------------------------------------------------------------------
sub insertFile($$$)
{
   my $this = shift;
   my $file = shift;
   my $dir  = shift;

   $file =~ /(\.(c\+\+|cpp|cxx|cc|c))$/i;
   my $ext = "$1";

   push(@{${${$this->{'dirs'}}{"$dir"}}{"$ext"}}, "$file");
}

1;
