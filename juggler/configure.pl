#!/usr/bin/perl -w

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

use 5.005;

use strict 'vars';
use vars qw($base_dir $module $CONFIG_ARGS $PATH_ARGS $HOST_ARGS $FEATURE_ARGS
            $CUSTOM_ARGS $LAST_ARG_GROUP $DEFAULT_MODULE $Win32);
use vars qw(%MODULES);

use Cwd qw(chdir getcwd);
use File::Basename;
use File::Path;
use Getopt::Long;
use Pod::Usage;

# Subroutine prototypes.
sub parseConfigFile($);
sub configureModule($);
sub regenModuleInfo($);
sub generateMakefile(;$);
sub generateReconfig($@);
sub printHelp();
sub getConfigureHelp($$);
sub parseOutput($$);

$DEFAULT_MODULE = '';
%MODULES        = ();

my $help        = 0;
my $cfg         = "juggler.cfg";
$module         = '';
my $script_help = 0;
my $manual      = 0;
my $regen       = 0;

$CONFIG_ARGS    = 0;
$PATH_ARGS      = 1;
$HOST_ARGS      = 2;
$FEATURE_ARGS   = 3;
$CUSTOM_ARGS    = 4;
$LAST_ARG_GROUP = 5;

my @save_argv = @ARGV;

Getopt::Long::Configure('pass_through');
GetOptions('help|?' => \$help, 'cfg=s' => \$cfg, 'module=s' => \$module,
           'script-help' => \$script_help, 'manual' => \$manual,
           'regen' => \$regen)
   or pod2usage(2);

# Print the help output and exit if --script-help was on the command line or
# if no arguments were passed.
if ( $#save_argv == -1 || $script_help )
{
   pod2usage(1);
}

pod2usage(-exitstatus => 0, -verbose => 2) if $manual;

die "ERROR: No configuration given\n" unless $cfg;

$base_dir = (fileparse("$0"))[1];
$Win32 = 1 if $ENV{'OS'} && $ENV{'OS'} =~ /Windows/;

parseConfigFile("$cfg");

printHelp() && exit(0) if $help;

if ( $regen )
{
   if ( $module )
   {
      die "ERROR: No such module $module in $cfg!\n"
         unless defined($MODULES{"$module"});

      regenModuleInfo("$module");
      generateMakefile("$module");
   }
   elsif ( $DEFAULT_MODULE && defined($MODULES{"$DEFAULT_MODULE"}) )
   {
      regenModuleInfo("$DEFAULT_MODULE");
      generateMakefile("$DEFAULT_MODULE");
   }
   else
   {
      foreach ( keys(%MODULES) )
      {
         regenModuleInfo("$_");
      }

      generateMakefile();
   }
}
else
{
   if ( $module )
   {
      die "ERROR: No such module $module in $cfg!\n"
         unless defined($MODULES{"$module"});

      generateReconfig("$module", @save_argv);
      configureModule("$module");
      generateMakefile("$module");
   }
   elsif ( $DEFAULT_MODULE && defined($MODULES{"$DEFAULT_MODULE"}) )
   {
      generateReconfig("$DEFAULT_MODULE", @save_argv);
      configureModule("$DEFAULT_MODULE");
      generateMakefile("$DEFAULT_MODULE");
   }
   else
   {
      generateReconfig('', @save_argv);

      foreach ( keys(%MODULES) )
      {
         configureModule("$_");
      }

      generateMakefile();
   }
}

exit(0);

# =============================================================================
# Subroutines follow.
# =============================================================================

sub parseConfigFile ($)
{
   open(CFG, "$cfg") or die "ERROR: Could not read from $cfg: $!\n";

   my($cfg_file, $line);
   while ( $line = <CFG> )
   {
      $line =~ s/(#|\/\/).*$//;
      $cfg_file .= "$line";
   }

   close(CFG);

   while ( $cfg_file !~ /^\s*$/ )
   {
      if ( $cfg_file =~ /^\s*(\S+)\s*{(.+?)}\s*/s )
      {
         my $mod   = "$1";
         my $deps  = "$2";
         $cfg_file = $';

         while ( $deps !~ /^\s*$/ )
         {
            if ( $deps =~ /\s*(\S.+?):\s+(.+?);/ )
            {
               $deps = $';

               my @var_list = split(/\s*,\s*/, "$2");
               my %vars = ();
               my $dep_path = "$1";

               my $var;
               foreach $var ( @var_list )
               {
                  $var =~ /\s*(\w+)=(\S+)\s*/;
                  $vars{"$1"} = "$2";
               }

               push(@{$MODULES{"$mod"}},
                    {'path' => "$dep_path", 'env' => \%vars});
            }
            else
            {
               # XXX: Not quite right...
               $deps ='';
            }
         }
      }
      elsif ( $cfg_file =~ /^Default:\s+(\S+)\s*$/m )
      {
         $DEFAULT_MODULE = "$1";
         $cfg_file       = $';
      }

      $cfg_file =~ s/^\s*//;
   }
}

sub configureModule ($)
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

   die "ERROR: No module $module_name defined\n"
      unless defined($MODULES{"$module_name"});

   my $modref;
   foreach $modref ( @{$MODULES{"$module_name"}} )
   {
      my $mod_path = $$modref{'path'};

      mkpath("$mod_path", 1, 0755) unless -d "$mod_path";
      chdir("$mod_path")
         or warn "WARNING: Could not chdir to $mod_path\n";

      my $src_root;

      if ( $base_dir =~ /^\// )
      {
         $src_root = "$base_dir";
      }
      else
      {
         $src_root = "$cwd/$base_dir";
      }

      print "Running $src_root/$mod_path/configure @ARGV\n";
      system("$src_root/$mod_path/configure @ARGV 2>&1") == 0
         or die "Configuration of $module_name in $ENV{'PWD'} failed\n";

      foreach ( keys(%{$$modref{'env'}}) )
      {
         if ( /_CONFIG$/ )
         {
            $ENV{"$_"} = "$cwd/$mod_path/${$$modref{'env'}}{$_}";
         }
         elsif ( /_BASE_DIR$/ )
         {
            if ( "${$$modref{'env'}}{$_}" eq "instlinks" )
            {
               $ENV{"$_"} = "$safe_cwd/instlinks";
            }
            else
            {
               $ENV{"$_"} = "${$$modref{'env'}}{$_}";
            }
         }
         else
         {
            $ENV{"$_"} = "${$$modref{'env'}}{$_}";
         }
      }

      $ENV{'USE_BASE_DIR'} = 'yes';

      chdir("$cwd");
   }
}

sub regenModuleInfo ($)
{
   my $module_name = shift;

   my $cwd = getcwd();

   die "ERROR: No module $module_name defined\n"
      unless defined($MODULES{"$module_name"});

   my $modref;
   foreach $modref ( @{$MODULES{"$module_name"}} )
   {
      my $mod_path = $$modref{'path'};

      chdir("$mod_path")
         or die "WARNING: Could not chdir to $mod_path\n";
      system("./config.status 2>&1") == 0
         or die "Regeneration for $module_name in $ENV{'PWD'} failed\n";
      chdir("$cwd");
   }
}

sub generateMakefile (;$)
{
   my $gen_module = shift || '';

   open(INPUT, "$base_dir/Makefile.in")
      or die "ERROR: Could not read from $base_dir/Makefile.in: $!\n";

   my $input_file;
   while ( <INPUT> )
   {
      $input_file .= "$_";
   }

   close(INPUT);

   my $modules;
   my @module_array;

   if ( $gen_module )
   {
      foreach ( @{$MODULES{"$gen_module"}} )
      {
         $modules .= "${$_}{'path'} ";
      }
   }
   else
   {
      my $mod_name;
      foreach $mod_name ( keys(%MODULES) )
      {
         my $temp_mod;
         foreach $temp_mod ( @{$MODULES{"$mod_name"}} )
         {
            $modules .= "${$temp_mod}{'path'} ";
         }
      }
   }

   warn "WARNING: No modules defined!\n" unless $modules;

   my $cwd = getcwd();
   chdir("$base_dir");
   $input_file =~ s/\@JUGGLER_PROJECTS\@/$modules/g;

   if ( $Win32 )
   {
      # Get the Win32-friendly versions of these paths.  Then change the \'s
      # to /'s just to be safe.
      my $win_pwd = `cygpath -w $ENV{'PWD'}`;
      my $win_cwd = `cygpath -w $cwd`;
      chomp($win_pwd);
      chomp($win_cwd);

      $win_pwd =~ s/\\/\//g;
      $win_cwd =~ s/\\/\//g;

      $input_file =~ s/\@JUGGLERROOT_ABS\@/$win_pwd/g;
      $input_file =~ s/\@topdir\@/$win_cwd/g;
   }
   else
   {
      $input_file =~ s/\@JUGGLERROOT_ABS\@/$ENV{'PWD'}/g;
      $input_file =~ s/\@topdir\@/$cwd/g;
   }

   chdir("$cwd");

   print "Generating Makefile\n";
   open(OUTPUT, "> Makefile") or die "ERROR: Could not create Makefile: $!\n";
   print OUTPUT "$input_file";
   close(OUTPUT) or warn "WARNING: Failed to save Makefile: $!\n";
}

sub generateReconfig ($@)
{
   my $gen_module = shift;
   my @save_argv  = @_;

   my $modules;

   open(RECONFIG, "> reconfig");

   if ( $gen_module )
   {
      foreach ( @{$MODULES{"$gen_module"}} )
      {
         print RECONFIG "(cd ${$_}{'path'} && rm -f config.status " .
                        "config.cache config.log)\n"
      }
   }
   else
   {
      my $mod_name;
      foreach $mod_name ( keys(%MODULES) )
      {
         foreach ( @{$MODULES{"$mod_name"}} )
         {
            print RECONFIG "(cd ${$_}{'path'} && rm -f config.status " .
                           "config.cache config.log)\n"
         }
      }
   }

   print RECONFIG "$0 ", "@save_argv\n";
   close(RECONFIG);
   chmod(0755, "reconfig");
}

sub printHelp ()
{
   my @help_output = ();

   # Initialize the references that are contained within @help_output.
   my $i;
   for ( $i = 0; $i < $LAST_ARG_GROUP; $i++ )
   {
      $help_output[$i] = {};
   }

   if ( $module )
   {
      getConfigureHelp("$module", \@help_output);
   }
   elsif ( $DEFAULT_MODULE && defined($MODULES{"$DEFAULT_MODULE"}) )
   {
      getConfigureHelp("$DEFAULT_MODULE", \@help_output);
   }
   else
   {
      foreach ( keys(%MODULES) )
      {
         getConfigureHelp("$_", \@help_output);
      }
   }

   for ( $i = 0; $i < $LAST_ARG_GROUP; $i++ )
   {
      SWITCH:
      {
         if ( $i == $CONFIG_ARGS )
         {
            print "Configuration:\n";
            last SWITCH;
         }

         if ( $i == $PATH_ARGS )
         {
            print "Directory and file names:\n";
            last SWITCH;
         }

         if ( $i == $HOST_ARGS )
         {
            print "Host type:\n";
            last SWITCH;
         }

         if ( $i == $FEATURE_ARGS )
         {
            print "Features and packages:\n";
            last SWITCH;
         }

         if ( $i == $CUSTOM_ARGS )
         {
            print "--enable and --with options recognized:\n";
            last SWITCH;
         }
      }

      foreach ( sort(keys(%{$help_output[$i]})) )
      {
         print "  ${$help_output[$i]}{$_}\n";
      }
   }

   print "\n";

   print "Modules that may be built:\n";
   foreach ( keys(%MODULES) )
   {
      print "\t$_\n";
   }

   print "\nDefault module is $DEFAULT_MODULE\n" if $DEFAULT_MODULE;

   return 1;
}

sub getConfigureHelp ($$)
{
   my $mod_name    = shift;
   my $arg_arr_ref = shift;

   foreach ( @{$MODULES{"$mod_name"}} )
   {
      next unless -x "$base_dir/$$_{'path'}/configure";

      open(CFG_OUTPUT, "$base_dir/$$_{'path'}/configure --help |");

      my $cfg_output;
      while ( <CFG_OUTPUT> )
      {
         $cfg_output .= "$_";
      }

      close(CFG_OUTPUT);

      parseOutput("$cfg_output", $arg_arr_ref);
   }
}

sub parseOutput ($$)
{
   my $string      = shift;
   my $arg_arr_ref = shift;

   my $arg_group;

   while ( $string !~ /^\s*$/s )
   {
      if ( $string =~ /^Configuration:\s*/s )
      {
         $arg_group = $CONFIG_ARGS;
         $string = $';
      }
      elsif ( $string =~ /^Directory.*?:\s*/s )
      {
         $arg_group = $PATH_ARGS;
         $string = $';
      }
      elsif ( $string =~ /^Host.*?:\s*/s )
      {
         $arg_group = $HOST_ARGS;
         $string = $';
      }
      elsif ( $string =~ /^Features.*?:\s*/s )
      {
         $arg_group = $FEATURE_ARGS;
         $string = $';
      }
      elsif ( $string =~ /^--enable and --with.*?:\s*/s )
      {
         $arg_group = $CUSTOM_ARGS;
         $string = $';
      }
      elsif ( $string =~ /^\s*(--\w+,\s+--\w+)/s ||
              $string =~ /^\s*(--[\w-]+)\W/s )
      {
         my $param = "$1";
         ${$$arg_arr_ref[$arg_group]}{"$param"} = '';

         my $temp_string = "$string";

         if ( $temp_string =~ /($param.+?)\s+(--)/s ||
              $temp_string =~ /($param.+?)\s*$/s )
         {
            my $desc = "$1";
            my $remainder = "$2$'";

            if ( $desc =~ /^(\w.+?:)$/m )
            {
               $desc = $`;
               $remainder = "$1\n$remainder";
            }

            ${$$arg_arr_ref[$arg_group]}{"$param"} = "$desc";
            $string = "$remainder";
         }
      }
      elsif ( $string =~ /^(Usage|Options).*$/m )
      {
         $string = $';
      }

      $string =~ s/^\s*//s;
   }
}

__END__

=head1 NAME

configure.pl

=head1 SYNOPSYS

=head1 OPTIONS

=over 8

=item B<--script-help>

Print usage information of this script alone and exit.

=item B<--help>

Print usage information for all the known configure scripts.  The
knowledge of configure scripts comes from the configuration file.  The
output may be limited using the --module argument, described below.

=item B<--manual>

Print usage information of this script alone in UNIX manpage format and exit.

=item B<--cfg>=file

Name the configuration file to be used by this script.  If not specified,
it defaults to juggler.cfg which must exist in the current working
directory.

=item B<--module>=name

Limit the work done by this script to what is required by the named
module.  The given name must correspond to one listed in the aforementioned
configuration file.  This can be specified in conjunction with B<--help>
to limit the output to only what is appropriate for the named module.

=item B<--regen>

Just regenerate the files previously generated without running the
configure script(s) again.

=back

=head1 DESCRIPTION

