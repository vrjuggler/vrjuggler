#!/usr/bin/perl -w

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

use 5.004;

use strict 'vars';
use vars qw($base_dir $MODULE $root_dir $Win32);
use vars qw(@gen_files);
use vars qw(%MODULES);

use Cwd qw(chdir getcwd);
use File::Basename;
use Getopt::Long;

BEGIN
{
   $base_dir = (fileparse("$0"))[1];
}

use lib("$base_dir");
use JugglerConfigure;

sub configureModule($);
sub compileModule($);
sub buildTests($);
sub makeOutputFile($);

my $cfg = 'juggler.cfg';

my $user_cfg = '';
my $module   = '';

Getopt::Long::Configure('pass_through');
GetOptions('cfg=s' => \$user_cfg, 'module=s' => \$module);

# Give ourselves a convenient way to know which OS we're using.
$ENV{'OSTYPE'} = `uname -s` unless defined($ENV{'OSTYPE'});

$Win32 = 1 if $ENV{'OS'} && $ENV{'OS'} =~ /Windows/;

my $cfg_load = ("$user_cfg" eq "") ? "$base_dir/$cfg" : "$user_cfg";
%MODULES     = JugglerConfigure::parseConfigFile("$cfg_load");

# Assign $MODULE the value of the module that has been configured.  This will
# either be the value passed via --module or the default module in the loaded
# configuration file.
$MODULE = ("$module" eq "") ? "$JugglerConfigure::DEFAULT_MODULE" : "$module";

# Make a unique identifier for this instance of the script.  It begins with
# a process ID and has the host operating system type prepended if the right
# environment variable is defined.
my $unique  = $$;
$unique     = "$ENV{'OSTYPE'}-$unique";

my $build_dir = "build.$unique-test";
$root_dir     = getcwd();

# This array will keep track of all generated files containing the output of
# executing various commands.  At the end of the script, the files named in
# this array may be searched for keywords such as "error" or "fail".
@gen_files = ();

my $cfg_outfile   = makeOutputFile("configure.out");
my $build_outfile = makeOutputFile("buildworld.out");

mkdir("$build_dir") or die "ERROR: Failed to create $root_dir/$build_dir: $!\n";
chdir("$build_dir") or die "ERROR: Failed to chdir to $root_dir/$build_dir: $!\n";

if ( $unique =~ /irix/i )
{
   $ENV{'PATH'} = "/usr/freeware/bin:$ENV{'PATH'}";
}

configureModule("$MODULE") or die "Failed to configure $MODULE\n";
compileModule("$MODULE") or die "Failed to build $MODULE\n";
buildTests("$MODULE");

exit 0;

# -----------------------------------------------------------------------------
# Subroutines follow.
# -----------------------------------------------------------------------------

sub configureModule ($)
{
   my $module_name = shift;

   my $cfg_command = "perl ../configure.pl --module=$module_name @ARGV";
   print "Configuring ...\n  '$cfg_command'\n";
   return (system("$cfg_command >$cfg_outfile 2>&1") == 0);
}

sub compileModule ($)
{
   print "Building world ...\n";
   return (system("gmake buildworld >$build_outfile 2>&1") == 0);
}

sub buildTests ($)
{
   my $module_name = shift;

   my $cwd = getcwd();
   my $safe_cwd;

   if ( $Win32 )
   {
      $safe_cwd = `cygpath -w $cwd`;
      chomp($safe_cwd);
      $safe_cwd =~ s/\\/\//g;
   }
   else
   {
      $safe_cwd = "$cwd";
   }

   my $dependency;
   foreach $dependency ( $MODULES{"$module_name"}->getDependencies() )
   {
      my $test_dir = $dependency->getPath() . "/test/TestSuite";

      if ( -d "$test_dir" )
      {
         if ( chdir("$test_dir") )
         {
            my $full_test_dir = "$root_dir/$build_dir/$test_dir";

            # Set up any environment stuff necessary before compiling and
            # running the test suite.
            my %mod_env = $dependency->getEnvironment();
            foreach ( keys(%mod_env) )
            {
               if ( /_CONFIG$/ )
               {
                  next;
               }
               elsif ( /_BASE_DIR$/ )
               {
                  if ( "$mod_env{$_}" eq "instlinks" )
                  {
                     $ENV{"$_"} = "$safe_cwd/instlinks";
                  }
                  else
                  {
                     $ENV{"$_"} = "$mod_env{$_}";
                  }

                  # Extend the right path variable so that any shared libraries
                  # are found at run time.
                  if ( $Win32 )
                  {
                     # XXX: Is the Win32 path environment variable set up to
                     # act like a Cygwin path or a DOS path?  Probably Cygwin...
                     $ENV{'PATH'} .= ":$ENV{$_}/lib";
                  }
                  elsif ( $ENV{'OSTYPE'} =~ /irix/i )
                  {
                     if ( defined($ENV{'LD_LIBRARYN32_PATH'}) )
                     {
                        $ENV{'LD_LIBRARYN32_PATH'} .= ":$ENV{$_}/lib32";
                     }
                     else
                     {
                        $ENV{'LD_LIBRARYN32_PATH'} = "$ENV{$_}/lib32";
                     }
                  }
                  else
                  {
                     if ( defined($ENV{'LD_LIBRARY_PATH'}) )
                     {
                        $ENV{'LD_LIBRARY_PATH'} .= ":$ENV{$_}/lib";
                     }
                     else
                     {
                        $ENV{'LD_LIBRARY_PATH'} = "$ENV{$_}/lib";
                     }
                  }
               }
               else
               {
                  $ENV{"$_"} = "$mod_env{$_}";
               }
            }

            # Construct a name for this test based on the module test suite
            # path.
            my $test_name = "$test_dir";
            $test_name =~ s/\//_/g;

            print "Building test suite in $full_test_dir ...\n";

            my $test_outfile = makeOutputFile("$test_name.out");

            if ( system("gmake >>$build_outfile 2>&1") == 0 )
            {
               print "Running test suite ...\n";
               if ( system("./runner noninteractive >$test_outfile 2>&1") != 0 )
               {
                  warn "ERROR: Test suite failed!\n";
               }
            }
            else
            {
               warn "ERROR: Test suite build failed in $full_test_dir\n";
            }

            chdir("$cwd");
         }
         else
         {
            warn "WARNING: Could not chdir to $test_dir: $!\n";
         }
      }
   }
}

sub makeOutputFile ($)
{
   my $base_name = shift;
   my $full_name = "$root_dir/$base_name-$unique";
   push(@gen_files, "$full_name");

   return $full_name;
}
