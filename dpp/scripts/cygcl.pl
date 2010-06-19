#!/usr/bin/perl -w

# cygcl -- wrap the Microsoft Visual Studio command-line tools to look like GCC
# Copyright (C) 2003, 2004
# Patrick Hartling

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

#
# $Id$
#

use strict 'vars';

use File::Basename;

my @msvc_args = ('/nologo');
my $msvc_tool = 'cl.exe';

my $gen_obj     = 0;
my $source_name = '';

my @work_argv = @ARGV;

while ( @work_argv )
{
   my $cur_arg = "$work_argv[0]";

#   print "Before: \$cur_arg = $cur_arg\n";

   # If the current argument is the beginning of a new parameter, we need to
   # deal with the case where it may be a path name that contains spaces.
   if ( $cur_arg =~ m|^[-/]| )
   {
      my $build_arg = 1;

      while ( $build_arg )
      {
         # If there is another argument following the current one and that
         # argument looks like a path name, we append it to $cur_arg.
         if ( $work_argv[1] && ! -f "$work_argv[1]" &&
              $work_argv[1] =~ /^\w+[\\\/]/ )
         {
#            print "Adding ' $work_argv[1]' to '$cur_arg'\n";
            $cur_arg .= " $work_argv[1]";
            shift(@work_argv);
         }
         # Otherwise, we probably have another new parameter in $work_argv[1],
         # so we leave it alone.
         else
         {
            $build_arg = 0;
         }
      }
   }

#   print "After: \$cur_arg = '$cur_arg'\n";

   SWITCH:
   {
      if ( $cur_arg =~ /^[-\/]lib$/i )
      {
         $msvc_tool = 'lib.exe';
         last SWITCH;
      }

      if ( $cur_arg =~ /^-link$/i )
      {
         $msvc_tool = 'link.exe';
         last SWITCH;
      }

      if ( $cur_arg =~ /^[-\/]dll$/i )
      {
         $msvc_tool = 'link.exe';
         push(@msvc_args, '/dll');
         last SWITCH;
      }

      # Run the preprocessor.
      if ( $cur_arg =~ /^-E$/ )
      {
         push(@msvc_args, '/E');
         last SWITCH;
      }

      # Preprocessor symbol definition.
      if ( $cur_arg =~ /^-D(.*)$/ )
      {
         my $sym = "$1";

         if ( "$sym" eq "" )
         {
            shift(@work_argv); # Drop the -D option
            push(@msvc_args, "/D'$work_argv[0]'");
         }
         else
         {
            push(@msvc_args, "/D'$sym'");
         }

         last SWITCH;
      }

      # Enable debugging symbols.
      if ( $cur_arg =~ /^-g$/ )
      {
         push(@msvc_args, '/Zi');
         last SWITCH;
      }

      if ( $cur_arg =~ /^-O\d?$/ )
      {
         push(@msvc_args, '/Ogityb2 /Gs');
         last SWITCH;
      }

      # Create an object file.
      if ( $cur_arg =~ /^[-\/]c$/ )
      {
         $gen_obj = 1;
         push(@msvc_args, '/c');
         last SWITCH;
      }

      if ( $cur_arg =~ /^-m(.+)$/ )
      {
         my $arch = "$1";

         if ( $arch =~ /386/ )
         {
            push(@msvc_args, '/G3');
         }
         elsif ( $arch =~ /486/ )
         {
            push(@msvc_args, '/G4');
         }
         elsif ( $arch =~ /pentium$/ )
         {
            push(@msvc_args, '/G5');
         }
         elsif ( $arch =~ /pentiumpro$/ )
         {
            push(@msvc_args, '/G6');
         }
         else
         {
            die "Unrecognized architecture option '$arch'\n";
         }

         last SWITCH;
      }

      if ( $cur_arg =~ /^-o(.*)$/ )
      {
         my $output_name = "$1";

         if ( "$output_name" eq "" )
         {
            shift(@work_argv);
            $output_name = "$work_argv[0]";
         }

         if ( $gen_obj )
         {
            push(@msvc_args, "/Fo$output_name");
         }
         else
         {
            push(@msvc_args, "/Fe$output_name");
         }

         last SWITCH;
      }

      # C source file.
      if ( $cur_arg =~ /\.c$/ )
      {
         $source_name     = basename($work_argv[0]);
         my $windows_path = `cygpath -w "$work_argv[0]"`;
         chomp($windows_path);
         push(@msvc_args, "/Tc'$windows_path'");
         last SWITCH;
      }

      # C++ source file.
      if ( $cur_arg =~ /\.(cpp|cxx|c\+\+|cc)$/i )
      {
         $source_name     = basename($work_argv[0]);
         my $windows_path = `cygpath -w "$work_argv[0]"`;
         chomp($windows_path);
         push(@msvc_args, "/Tp'$windows_path'");
         last SWITCH;
      }

      if ( $cur_arg =~ /^-I(.*)$/ )
      {
         my $dir = "$1";

         if ( "$dir" eq "" )
         {
            shift(@work_argv);
            $dir = "$work_argv[0]";
         }

         my $windows_path = `cygpath -w "$dir"`;
         chomp($windows_path);
         push(@msvc_args, "/I'$windows_path'");
         last SWITCH;
      }

      if ( $cur_arg =~ /^-L(.*)$/ )
      {
         my $dir = "$1";
         my $windows_path = `cygpath -w "$dir"`;
         chomp($windows_path);

         if ( exists($ENV{'LIB'}) )
         {
            $ENV{'LIB'} .= ";$windows_path";
         }
         else
         {
            $ENV{'LIB'} = "$windows_path";
         }

         last SWITCH;
      }

      if ( $cur_arg =~ /^-l(.*)$/ )
      {
         push(@msvc_args, "$1.lib");
         last SWITCH;
      }

      if ( $cur_arg =~ /^\// )
      {
         push(@msvc_args, "$cur_arg");
         last SWITCH;
      }

      # Deal with unrecognized options.
      die "Unknown option '$cur_arg'\n" if $cur_arg =~ /^-/;

      if ( -l $cur_arg )
      {
         my $windows_path = `cypath -w "$cur_arg"`;
         chomp($windows_path);
         push(@msvc_args, "$windows_path");
      }
      elsif ( -f $cur_arg )
      {
         my $windows_path = `cypath -w "$cur_arg"`;
         chomp($windows_path);
         push(@msvc_args, "$windows_path");
      }
      else
      {
         push(@msvc_args, "$cur_arg");
      }
   }

   shift(@work_argv);
}

my $output     = "$$.out";
my $err_output = "$$.err";

my $exit_status = 0;

print "$msvc_tool @msvc_args\n";
system("$msvc_tool @msvc_args >$output 2>$err_output") == 0
   or $exit_status = 1;

open(OUTPUT, "$output");

while ( <OUTPUT> )
{
   if ( $_ !~ /^$source_name\s?$/ )
   {
      print STDERR;
   }
}

close(OUTPUT);

open(OUTPUT_ERR, "$err_output");

while ( <OUTPUT_ERR> )
{
   if ( $_ !~ /^$source_name\s?$/ )
   {
      print STDERR;
   }
}

close(OUTPUT_ERR);

unlink("$output");
unlink("$err_output");

exit $exit_status;
