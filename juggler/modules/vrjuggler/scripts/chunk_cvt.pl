#!/usr/bin/perl

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

require 5.004;

use strict 'vars';
use vars qw($opt_i $opt_o);
use Getopt::Std;

getopt('i:o:');

if ( $opt_i && $opt_o )
{
   if ( open(INPUT, "$opt_i") )
   {
      my $chunk_file;
      while ( <INPUT> )
      {
         $chunk_file .= "$_";
      }

      close(INPUT);

      $chunk_file =~ s/^\s*//;
      $chunk_file =~ s/End//;

      open(OUTPUT, "> $opt_o")
         or die "ERROR: Could not create new output file $opt_o: $!\n";

      print OUTPUT "<ConfigChunkDB>\n";

      my (@chunk_types) = ();
      my (@embed_path)  = ();

      convertChunks("$chunk_file", \@chunk_types, \@embed_path);

      print OUTPUT "</ConfigChunkDB>\n";
   }
   else
   {
      die "ERROR: Could not read $opt_i: $!\n";
   }
}

exit 0;

sub convertChunks ($$$)
{
   my $chunks          = shift;
   my $chunk_types_ref = shift;
   my $embed_path_ref  = shift;

   while ( $chunks !~ /^\s*$/ )
   {
      $chunks = parseChunks("$chunks", $chunk_types_ref, $embed_path_ref);
      $chunks =~ s/^\s+//m;
   }
}

sub parseChunks ($$$)
{
   my $chunks          = shift;
   my $chunk_types_ref = shift;
   my $embed_path_ref  = shift;

   $chunks =~ s/\s*#.*$//m;

   SWITCH:
   {
      if ( $chunks =~ /^(\w+)\s*/s && $chunks !~ /^end/s )
      {
         my $type = "$1";
         $chunks = $';

         if ( $chunks =~ /^Name\s*"(.*?)"\s*$/m )
         {
            my $name = "$1";
            unshift(@$chunk_types_ref, "$type");
            push(@$embed_path_ref, "$name");

            $name = join("-&gt;", @$embed_path_ref) if $#$embed_path_ref > 0;

            print OUTPUT "<$$chunk_types_ref[0] name=\"$name\">\n";

            $chunks  = $';

            last SWITCH;
         }
         # No embedded chunk.
         elsif ( $chunks =~ /^{\s*(".*?")\s*}/s )
         {
            my $prop_value = "$1";
            $chunks = $';

            print OUTPUT "<$type>$prop_value</$type>\n";

            last SWITCH;
         }
         # Empty embedded chunk.
         elsif ( $chunks =~ /^{\s*}/s )
         {
            print OUTPUT "<$type></$type>\n";
            $chunks = $';

            last SWITCH;
         }
         # Embedded chunk.
         elsif ( $chunks =~ /^{\s*(.*?end)\s*}/s )
         {
            my $remainder = $';
            print OUTPUT "<$type>\n";
            push(@$embed_path_ref, "$type");
            convertChunks("$1", $chunk_types_ref, $embed_path_ref);
            pop(@$embed_path_ref);
            print OUTPUT "</$type>\n";

            $chunks = $remainder;

            last SWITCH;
         }
      }

      if ( $chunks =~ /^end/s )
      {
         $chunks = $';
         print OUTPUT "</$$chunk_types_ref[0]>\n";
         shift(@$chunk_types_ref);
         pop(@$embed_path_ref);

         last SWITCH;
      }
   }

   return $chunks;
}
