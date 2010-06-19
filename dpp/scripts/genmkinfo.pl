#!/usr/bin/perl

# Doozer++ is (C) Copyright 2000-2010 by Iowa State University
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
#
# Original Author:
#   Patrick Hartling

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
