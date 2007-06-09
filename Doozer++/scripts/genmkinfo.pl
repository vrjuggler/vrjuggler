#!/usr/bin/perl

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# Doozer++ is (C) Copyright 2000-2007 by Iowa State University
#
# Original Author:
#   Patrick Hartling
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

# genmkinfo.pl,v 1.5 2006/12/31 16:55:01 patrickh Exp

require 5.004;

use strict 'vars';

use Text::Wrap;

die "ERROR: No files given\n" unless $#ARGV > -1;

$Text::Wrap::columns = 75;

my @TARGETS = ();
my %INFO    = ();

my $makefile;
foreach $makefile ( @ARGV )
{
   if ( open(INPUT, "$makefile") )
   {
      my $contents;

      my $in_info_block = 0;
      my $cur_target    = '';

      while ( <INPUT> )
      {
         chomp;

         if ( /#\s+__DZR_INFO_BEGIN__/ )
         {
            $in_info_block = 1;
            $cur_target    = '';
            next;
         }
         elsif ( /#\s+__DZR_INFO_END__/ )
         {
            $in_info_block = 0;
            $cur_target    = '';
            next;
         }

         if ( $in_info_block )
         {
            if ( /#\s*T<(\S+)>\s+/ )
            {
               $cur_target          = "$1";
               $INFO{"$cur_target"} = $';
               $INFO{"$cur_target"} =~ s/\s+$//;  # Strip trailing whitespace
               push(@TARGETS, "$cur_target");
            }
            elsif ( "$cur_target" ne "" && /#\s*(\S.*)\s*$/ )
            {
               $INFO{"$cur_target"} .= " $1";
            }
         }
      }

      close(INPUT);
   }
   else
   {
      warn "WARNING: Could not read from $makefile: $!\n";
   }
}

foreach ( @TARGETS )
{
   print "$_:\n";
   print wrap("    ", "    ", $INFO{"$_"}), "\n";
}

exit(0);
