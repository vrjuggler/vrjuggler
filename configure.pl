#!/usr/bin/env perl

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

use 5.005;

use strict 'vars';
use vars qw($base_dir $module $PRELUDE $FEATURE_ARGS $PACKAGE_ARGS $PROLOGUE
            $LAST_ARG_GROUP $OS $Win32 $CFG_LOAD_FUNC);
use vars qw(%MODULES);

use Cwd qw(chdir getcwd);
use File::Basename;
use File::Path;
use Getopt::Long;
use Pod::Usage;

BEGIN
{
   $base_dir = (fileparse("$0"))[1];
   $base_dir =~ s|/+$||;
}

use lib("$base_dir");
use JugglerConfigure;

# Subroutine prototypes.
sub mergeArgArrays($$);
sub loadDefaultArgs($);
sub generateTopLevelMakefile(;$);
sub generateModuleMakefiles($);
sub generatePreMakefile($);
sub generateReconfig($@);
sub listModules();
sub printHelp();
sub getConfigureHelp($$);
sub parseOutput($$);
sub getPlatform();

%MODULES = ();

my $all_help      = 0;
my $cfg           = "juggler.cfg";
my $user_cfg      = '';
$module           = '';
my $script_help   = 0;
my $manual        = 0;
my $mod_list      = 0;
my $args_file     = 'acdefaults.cfg';
my $args_mod_file = 'acdefaults.pl';
my $user_args     = '';
my $user_args_mod = '';
my $no_user_args  = 0;

$PRELUDE        = 0;
$FEATURE_ARGS   = 1;
$PACKAGE_ARGS   = 2;
$PROLOGUE       = 3;
$LAST_ARG_GROUP = 4;

$CFG_LOAD_FUNC = undef;
$OS            = '';

my @save_argv = @ARGV;

Getopt::Long::Configure('pass_through');
GetOptions('help|?' => \$script_help, 'cfg=s' => \$user_cfg,
           'module=s' => \$module, 'all-help' => \$all_help,
           'manual' => \$manual, 'modlist' => \$mod_list,
           'args=s' => \$user_args, 'argsmod=s' => \$user_args_mod,
           'noargs' => \$no_user_args, 'os=s' => \$OS)
   or pod2usage(2);

# Print the help output and exit if --help was on the command line.
pod2usage(1) if $script_help;
pod2usage(-exitstatus => 0, -verbose => 2) if $manual;

die "ERROR: No configuration given\n" unless $cfg || $user_cfg;

my $inst_prefix = '/usr/local';
my $arg;
foreach $arg ( @ARGV )
{
   if ( $arg =~ /--prefix=(.*)$/ )
   {
      $inst_prefix = "$1";
   }
}

$Win32 = 1 if $ENV{'OS'} && $ENV{'OS'} =~ /Windows/;

# On Windows, the command-line arguments can confuse the Cygwin shell.  For
# example, the character used to separate paths is ';', but the shell sees
# that as a statement separator.  We can deal with this by ensuring that the
# shell interprets command-line arguments as literal strings (i.e., by
# putting quotes around each argument).
if ( $Win32 ) 
{
   die "Absolute Cygwin paths confuse Visual C++.  Use a relative path.\n"
      if $0 =~ /^\//;

   for ( my $i = 0; $i <= $#save_argv; $i++ )
   {
      $save_argv[$i] = "\"$save_argv[$i]\"";
   }

   for ( my $i = 0; $i <= $#ARGV; $i++ )
   {
      $ARGV[$i] = "\"$ARGV[$i]\"";
   }
}

my $cfg_load = ("$user_cfg" eq "") ? "$base_dir/$cfg" : "$user_cfg";
%MODULES = JugglerConfigure::parseConfigFile("$cfg_load");

listModules() && exit(0) if $mod_list;
printHelp() && exit(0) if $all_help;

{
   my $cache_file_set = 0;

   foreach ( @ARGV )
   {
      if ( /-cache-f/ )
      {
         $cache_file_set = 1;
         last;
      }
   }

   # Unless the user passed --noargs, try to find default argument values.
   unless ( $no_user_args )
   {
      my $args_mod = ("$user_args_mod" eq "") ? "$base_dir/$args_mod_file"
                                              : "$user_args_mod";

      # Figure out what argument file to load, if any.  If the user specified
      # a file name on the command line, it will be in $user_args.  Otherwise,
      # we fall back on $base_dir/$args_file.
      my $args_load = ("$user_args" eq "") ? "$base_dir/$args_file"
                                           : "$user_args";

      if ( -r "$args_mod" )
      {
         require "$args_mod";

         if ( $CFG_LOAD_FUNC )
         {
            my @default_args = &$CFG_LOAD_FUNC();
            mergeArgArrays(\@ARGV, \@default_args);
         }
      }
      elsif ( -r "$args_load" )
      {
         loadDefaultArgs("$args_load");
      }
   }

   if ( ! $cache_file_set )
   {
      my $cwd = getcwd();
      push(@ARGV, "--cache-file=$cwd/config.cache");
   }

   # Configure the module named on the command line.
   if ( $module )
   {
      die "ERROR: No such module $module in $cfg!\n"
         unless defined($MODULES{"$module"});

      generateReconfig("$module", @save_argv);
      generateModuleMakefiles("$module");
      generateTopLevelMakefile("$module");
   }
   # If no module was named on the command line but we do have a default
   # module, configure it.
   elsif ( $JugglerConfigure::DEFAULT_MODULE &&
           defined($MODULES{"$JugglerConfigure::DEFAULT_MODULE"}) )
   {
      generateReconfig("$JugglerConfigure::DEFAULT_MODULE", @save_argv);
      generateModuleMakefiles("$JugglerConfigure::DEFAULT_MODULE");
      generateTopLevelMakefile("$JugglerConfigure::DEFAULT_MODULE");
   }
   # If neither of the above will do, just configure every module we know
   # about from the input file.
   else
   {
      generateReconfig('', @save_argv);

      foreach ( keys(%MODULES) )
      {
         generateModuleMakefiles("$_");
      }

      generateTopLevelMakefile();
   }

   my $gnu_make = 'gmake';

   # Do a quick check to try to find GNU make.  If we don't find it, then we
   # will just assume that GNU make is 'gmake' and that the user knows what
   # he or she is doing.
   foreach ( 'make', 'gmake' )
   {
      my $result = open(MAKE, "$_ -v 2>&1 |");
      next unless $result;
      my @output = <MAKE>;
      close(MAKE);

      if ( grep(/GNU Make/, @output) )
      {
         $gnu_make = $_;
         last;
      }
   }

   print <<EOF;
 
 GNU make is required to build the Juggler Suite.
 On your system, GNU make is the command $gnu_make.
 To build and install everything into $inst_prefix,
 run the following:
 
    $gnu_make build install
 
 To make a developer build and installation in
 ./instlinks, (debug only), simply run the following:

    $gnu_make

 To make an optimized build in ./instlinks, run the
 following:

    $gnu_make optim

EOF
}

exit(0);

# =============================================================================
# Subroutines follow.
# =============================================================================

sub mergeArgArrays ($$)
{
   my $dest_list   = shift;
   my $source_list = shift;

   foreach ( @$source_list )
   {
      # Strip leading and trailing whitespace.
      s/^\s+//;
      s/\s+$//;
      next if /^$/;   # Just to be safe...
      
      # Only add the argument if it is not already on the command line.
      m/^(--[^=]+)/;
      push(@$dest_list, "$_") unless grep(/$1/, @$dest_list);
   }
}

sub loadDefaultArgs ($)
{
   my $args_load = shift;

   if ( open(ARGS_FILE, "$args_load") )
   {
      print "Loading default arguments from $args_load ...\n";
      my $args_contents = '';

      while ( <ARGS_FILE> )
      {
         s/#.*$//;           # Strip comments
         next if /^\s*$/;    # Skip blank lines
         $args_contents .= "$_";
      }

      close(ARGS_FILE) or warn "WARNING: Could not close $args_load: $!\n";

      my $platform = getPlatform();

      while ( "$args_contents" ne '' )
      {
         my @args_list = ();

         # Read in the arguments for all platforms.
         if ( $args_contents =~ /^\s*all\s*{(.+?)}\s*/si )
         {
            @args_list     = split(m|$/|, "$1");
            $args_contents = $';
         }
         # Read in the arguments for the current platform.
         elsif ( $args_contents =~ /^\s*$platform\s*{(.+?)}\s*/sio )
         {
            @args_list     = split(m|$/|, "$1");
            $args_contents = $';
         }
         # Skip a platform that does not match $platform.
         elsif ( $args_contents =~ /^\s*(\S+)\b\s*{(.+?)}\s*/s )
         {
            print "Skipping $1\n";
            $args_contents = $';
         }

         mergeArgArrays(\@ARGV, \@args_list);
      }
   }
   else
   {
      warn "WARNING: Coult not read from $args_load: $!\n";
   }
}

sub generateTopLevelMakefile(;$)
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
      foreach ( $MODULES{"$gen_module"}->getDependencies() )
      {
         $modules .= $_->getPath() . " ";
      }
   }
   else
   {
      my $mod_name;
      foreach $mod_name ( keys(%MODULES) )
      {
         my $temp_mod;
         foreach $temp_mod ( $MODULES{"$mod_name"}->getDependencies() )
         {
            $modules .= $temp_mod->getPath() . " ";
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

sub generateModuleMakefiles($)
{
   my $module_name = shift;

   die "ERROR: No module $module_name defined\n"
      unless defined($MODULES{"$module_name"});

   my $cwd = getcwd();

   chdir("$base_dir");
   my $root_srcdir = getcwd();
   chdir("$cwd");

   my $depencency;
   foreach $depencency ( $MODULES{"$module_name"}->getDependencies() )
   {
      my $mod_path = $depencency->getPath();

      mkpath("$mod_path", 0, 0755) unless -d "$mod_path";

      # Do not try to proceed with $dependency unless we can chdir to
      # $mod_path.
      unless ( -e "$root_srcdir/$mod_path/Makefile.pre.in" )
      {
         warn "WARNING: Missing $root_srcdir/$mod_path/Makefile.pre.in!\n";
         next;
      }

      generatePreMakefile("$mod_path/Makefile.pre");
   }
}

sub generatePreMakefile($)
{
   my $output_file_name = shift;

   my $cwd = getcwd();

   chdir("$base_dir");
   my $root_srcdir = getcwd();
   my $input_file_name  = "$root_srcdir/$output_file_name.in";
   chdir("$cwd");

   open(INPUT, "$input_file_name")
      or die "ERROR: Could not read from $input_file_name: $!\n";

   my $input_file;
   while ( <INPUT> )
   {
      $input_file .= "$_";
   }

   close(INPUT);

   my $srcdir = (fileparse("$input_file_name"))[1];
   $srcdir =~ s|/+$||;

   my $configure_args = join(" ", @ARGV);

   # Use ksh to run configure if we are on Solaris.  Otherwise, use the default
   # shell.
   my $shell = ((getPlatform() =~ /solaris/i) ? 'ksh' : '$(SHELL)');

   $input_file =~ s/\@srcdir\@/$srcdir/g;
   $input_file =~ s/\@top_srcdir\@/$root_srcdir/g;
   $input_file =~ s/\@CONFIGURE_ARGS\@/$configure_args/g;
   $input_file =~ s/\@CFG_SHELL\@/$shell/g;

   print "Generating $output_file_name\n";
   open(OUTPUT, "> $output_file_name")
      or die "ERROR: Could not create $output_file_name: $!\n";
   print OUTPUT "$input_file";
   close(OUTPUT) or warn "WARNING: Failed to save $output_file_name: $!\n";
}

sub generateReconfig ($@)
{
   my $gen_module = shift;
   my @arg_list   = @_;

   my $modules;

   open(RECONFIG, "> reconfig");

   print RECONFIG "#!/bin/sh\n";
   print RECONFIG "if [ \"x\$1\" != \"x-q\" ]; then\n";

   if ( $gen_module )
   {
      foreach ( $MODULES{"$gen_module"}->getDependencies() )
      {
         print RECONFIG "   (cd " . $_->getPath() .
                        " && rm -f config.status config.log)\n";
      }
   }
   else
   {
      my $mod_name;
      foreach $mod_name ( keys(%MODULES) )
      {
         foreach ( $MODULES{"$mod_name"}->getDependencies() )
         {
            print RECONFIG "   (cd " . $_->getPath() .
                           " && rm -f config.status config.cache config.log)\n";
         }
      }
   }

   print RECONFIG "   rm -f config.cache\n";
   print RECONFIG "fi\n";

   # Print the command to run this script again.  The actual output will be
   # the exec shell command followed by the full path to the Perl
   # interpreter used to run this script; the same path to the script that
   # the user entered; and the full argument list given on the command line.
   print RECONFIG "exec $^X $0 ", "@arg_list \n";
   close(RECONFIG);
   chmod(0755, "reconfig");
}

sub listModules ()
{
   my $mod_name;
   foreach $mod_name ( keys(%MODULES) )
   {
      print "$mod_name";

#      if ( $#{$MODULES{"$mod_name"}} != -1 )
#      {
#         print " (Requires:";
#
#         my $dep_ref;
#         foreach $dep_ref ( @{$MODULES{"$mod_name"}} )
#         {
#            print " ${$dep_ref}{'path'}";
#         }
#
#         print ")";
#      }

      print "\n";
   }

   return 1;
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
   elsif ( $JugglerConfigure::DEFAULT_MODULE &&
           defined($MODULES{"$JugglerConfigure::DEFAULT_MODULE"}) )
   {
      getConfigureHelp("$JugglerConfigure::DEFAULT_MODULE", \@help_output);
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
         if ( $i == $PRELUDE )
         {
            last SWITCH;
         }

         if ( $i == $FEATURE_ARGS )
         {
            print "Optional Features:\n";
            last SWITCH;
         }

         if ( $i == $PACKAGE_ARGS )
         {
            print "\nOptional Packages:\n";
            last SWITCH;
         }

         if ( $i == $PROLOGUE )
         {
            print "\n";
            last SWITCH;
         }
      }

      foreach ( sort(keys(%{$help_output[$i]})) )
      {
         print "${$help_output[$i]}{$_}";
      }
   }

   print "\n";

   print "Modules that may be built:\n";
   foreach ( keys(%MODULES) )
   {
      print "\t$_\n";
   }

   print "\nDefault module is $JugglerConfigure::DEFAULT_MODULE\n"
      if $JugglerConfigure::DEFAULT_MODULE;

   return 1;
}

sub getConfigureHelp ($$)
{
   my $mod_name    = shift;
   my $arg_arr_ref = shift;

   my $configure_count = 0;

   foreach ( $MODULES{"$mod_name"}->getDependencies() )
   {
      next unless -x "$base_dir/$$_{'path'}/configure";

      $configure_count++;
      open(CFG_OUTPUT, "$base_dir/$$_{'path'}/configure --help |");

      my $cfg_output;
      while ( <CFG_OUTPUT> )
      {
         $cfg_output .= "$_";
      }

      close(CFG_OUTPUT);

      parseOutput("$cfg_output", $arg_arr_ref);
   }

   die "ERROR: No configure scripts found!  Please run autogen.sh\n" .
       "       (found in $base_dir) first.\n"
      if $configure_count == 0;
}

sub parseOutput ($$)
{
   my $string      = shift;
   my $arg_arr_ref = shift;

   while ( $string !~ /^\s*$/s )
   {
      # Match everything up to the list of optional features.  This forms the
      # prelude of the help output.
      if ( $string =~ /^(Usage:.*)(Optional Features:)/s )
      {
         $string = "$2$'";
         ${$$arg_arr_ref[$PRELUDE]}{'all'} = "$1";
      }
      # Handle the --enable and --disable list of options.
      elsif ( $string =~ /^(Optional Features:.*)(Optional Packages:)/s )
      {
         $string = "$2$'";

         my @param_list = split(/\n/, "$1");

         # Loop over all the lines of output in the "Optional Features" block.
         my $i;
         for ( $i = 0; $i <= $#param_list; $i++ )
         {
            if ( $param_list[$i] =~ /(--(enable|disable)\S+)/ )
            {
               my $param = "$1";

               # If $param does not exist in the hash of feature arguments,
               # we need to add it.
               if ( ! exists(${$$arg_arr_ref[$FEATURE_ARGS]}{"$param"}) )
               {
                  # Add the first line of information for $param.
                  my $param_info = "$param_list[$i]\n";

                  # If the following lines are a continuation of the info for
                  # $param, add them to $param_info as well.
                  while ( $i + 1 <= $#param_list &&
                          $param_list[$i + 1] !~ /--(enable|disable)/ )
                  {
                     $param_info .= "$param_list[$i + 1]\n";
                     $i++;
                  }

                  # Store the complete information block for $param.
                  ${$$arg_arr_ref[$FEATURE_ARGS]}{"$param"} = "$param_info";
               }
            }
         }
      }
      # Handle the --with and --without list of options.
      elsif ( $string =~ /^(Optional Packages:.*)(Some influential)/s )
      {
         $string = "$2$'";

         my @param_list = split(/\n/, "$1");

         # Loop over all the lines of output in the "Optional Packages" block.
         my $i;
         for ( $i = 0; $i <= $#param_list; $i++ )
         {
            if ( $param_list[$i] =~ /(--with\S+)/ )
            {
               my $param = "$1";

               # If $param does not exist in the hash of package arguments,
               # we need to add it.
               if ( ! exists(${$$arg_arr_ref[$PACKAGE_ARGS]}{"$param"}) )
               {
                  # Add the first line of information for $param.
                  my $param_info = "$param_list[$i]\n";

                  # If the following lines are a continuation of the info for
                  # $param, add them to $param_info as well.
                  while ( $i + 1 <= $#param_list &&
                          $param_list[$i + 1] !~ /--with/ )
                  {
                     $param_info .= "$param_list[$i + 1]\n";
                     $i++;
                  }

                  # Store the complete information block for $param.
                  ${$$arg_arr_ref[$PACKAGE_ARGS]}{"$param"} = "$param_info";
               }
            }
         }
      }
      # We'll keep everything after the line beginning with "Some influential"
      # to form the prologue of the help output.
      elsif ( $string =~ /^(Some influential.*)$/s )
      {
         $string = "$'";   # This should be the empty string.
         ${$$arg_arr_ref[$PROLOGUE]}{'all'} = "$1";
      }
      # Match anything else and strip it from the output.
      elsif ( $string =~ /^.*$/m )
      {
         $string = $';
      }

      $string =~ s/^\s*//s;
   }
}

sub getPlatform ()
{
   my $platform = "unknown";

   # Prefer the user-defined platform type over any auto-detected value.
   if ( "$OS" ne '' )
   {
      $platform = "$OS";
   }
   elsif ( defined($ENV{'OS'}) )
   {
      $platform = "$ENV{'OS'}";
   }
   elsif ( defined($ENV{'OSTYPE'}) )
   {
      $platform = "$ENV{'OSTYPE'}";
   }
   elsif ( defined($ENV{'OS_TYPE'}) )
   {
      $platform = "$ENV{'OS_TYPE'}";
   }
   elsif ( defined($ENV{'HOSTTYPE'}) )
   {
      $platform = "$ENV{'HOSTTYPE'}";
   }
   # As a last resort, fall back on the use of uname(1).
   else
   {
      chomp($platform = `uname -s`);
   }

   # XXX: This is a hack to deal with weird OS strings such as "linux-gnu".
   # We just make the platform be "linux" unless the user set the platform
   # type on the command line.
   $platform = 'linux' if ! $OS && $platform =~ /linux/i;

   return $platform;
}

sub getHostname ()
{
   my $hostname = '';

   if ( defined($ENV{'HOSTNAME'}) )
   {
      $hostname = "$ENV{'HOSTNAME'}";
   }
   else
   {
      chomp($hostname = `hostname`);
   }

   return $hostname;
}

__END__

=head1 NAME

configure.pl

=head1 SYNOPSIS

This script acts as the "glue" for a collection of Autoconf-based configure
scripts.  Based on a configuration file, it is capable of building a
dependency tree and running the configure scripts in the correct order such
that the dependencies are satisfied correctly.  Note that all modules must
be capable of having dependencies satisfied based entirely on the results
of running a dependent module's configure script.

=head1 DESCRIPTION

(Still need to write this...)

=head1 OPTIONS

=over 8

=item B<--help>

Print usage information of this script alone and exit.

=item B<--all-help>

Print usage information for all the known configure scripts.  The
knowledge of configure scripts comes from the configuration file.  The
output may be limited using the B<--module> argument, described below.
In order for this option to behave correctly, B<autogen.sh> must have been
run in the top-level Juggler source directory.

=item B<--manual>

Print usage information of this script alone in UNIX manpage format and exit.

=item B<--modlist>

Print a list of the available modules.

=item B<--cfg>=file

Name the configuration file to be used by this script.  If not specified,
it defaults to F<juggler.cfg>.  This file is discovered based on the
run-time path to this script, and thus the script and the default
configuration file must be in the same directory.  For example, if this
script is run as:

=over 4

../configure.pl

=back

then F<juggler.cfg> will be searched for as F<../juggler.cfg>.

=item B<--module>=name

Limit the work done by this script to what is required by the named
module.  The given name must correspond to one listed in the aforementioned
configuration file.  This can be specified in conjunction with B<--all-help>
to limit the output to only what is appropriate for the named module.

=item B<--args>=file

Name the file containing arguments to pass to the Autoconf-generated
configure script(s) that will be executed.  If not specified, it defaults
to F<acdefaults.cfg>, found using the same technique as is used for
F<juggler.cfg> (see B<--cfg> above).

=item B<--argsmod>=file

Name a F<.pl> file (what could be a Perl script) that will be "imported"
into this script.  This imported script allows users to write Perl code
that will extend the argument list passed to the Autoconf-generated
configure scripts.  If not specified, it defaults to F<acdefaults.pl>,
found using the same technique as is used for F<juggler.cfg> and
F<acdefaults.cfg> (see B<--cfg> above).

If both F<acdefaults.cfg> and F<acdefaults.pl> exist, F<acdefaults.pl> is
used, and F<acdefaults.cfg> is ignored.

=item B<--noargs>

Turns off the use of any default argument loading mechanism (see B<--args>
and B<--argsmod> above).

=item B<--os>=name

Define the host operating system.  This can be any string.  It will be used
to recognize which platform-specific block to load from the Autoconf default
arguments file.  Use of this argument forcibly overrides automatic detection
of the host operating system.  I<Use with caution!>

=back

=head1 FILE FORMATS

The following describe the formats of files that may be  loaded by this
script.

=head2 Configuration File

This is a magic file for which documentation cannot possibly be written.
Or maybe it could be written later.

=head2 Default Arguments File

name1
{
   arg1
   arg2
   ...
   argN
}

name2
{
   arg1
   arg2
   ...
   argN
}

=head2 Default Arguments Module

This must be a valid Perl package.  That means it must declare a package
name (any name is fine) and have the following as the last line:

1;

To plug into this script, the module must have a subroutine that takes
no arguments and returns an array.  The loading script is informed of
this subroutine through a global variable in its scope:
I<$main::CFG_LOAD_FUNC>.

Assign a reference to the module's subroutine as the first step in the
module.  For example, consider the case where we have a subroutine called
C<&loader>.  The loading module would be informed of this subroutine as
shown below:

$main::CFG_LOAD_FUNC = \&loader;

=head1 NOTES

The use of a default arguments module (see B<--argsmod> above) takes
precedence of a default arguments file (see B<--args>).  If both arguments
are specified (or both default files are found), the configuration file is
ignored entirely.  It is assumed that Perl code will be more capable of
filling in default arguments than a simple configuration file.

