#!/usr/bin/perl -w

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# Doozer++ is (C) Copyright 2000, 2001 by Iowa State University
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

# $Id$

use 5.005;

use Cwd qw(chdir getcwd);
use File::Basename;
use File::Path;
use Getopt::Long;
use Pod::Usage;

use strict 'vars';
use vars qw($indent $log_file $full_path $debug_level);
use vars qw($CRITICAL_LVL $WARNING_LVL $CONFIG_LVL $STATE_LVL $VERB_LVL
            $HVERB_LVL $HEX_LVL);

# Subroutine prototypes.
sub printVersion();
sub parse($$);
sub parseModule($$$;$);
sub handleInclude($$$);
sub expandWildcardLine($$);
sub expandWildcards($$$$$);
sub doOverride($$);
sub overrideValue($$$);
sub checkoutModules($);
sub updateModule($$$$);
sub checkoutModule($$$$$$$);
sub runCvsCommand($);
sub modifyCvsEntries($$);
sub expandEnvVars($);
sub printDebug($@);
sub nextSpinnerFrame($);

# *********************************************************************
# Here is the version for this script!

my $VERSION = '0.1.5';
# *********************************************************************

my $cfg_file      = '';
my $help          = 0;
my $print_version = 0;
my $verbose       = 0;
my $entry_mod     = 0;
my $force_install = 0;
my $manual        = 0;

my (@limit_modules) = ();
my (@overrides)     = ();
my (%cmd_overrides) = ();

$CRITICAL_LVL = 0;
$WARNING_LVL  = 1;
$CONFIG_LVL   = 2;
$STATE_LVL    = 3;
$VERB_LVL     = 4;
$HVERB_LVL    = 5;
$HEX_LVL      = 6;

$debug_level = $CRITICAL_LVL;
GetOptions('cfg=s' => \$cfg_file, 'help' => \$help, 'override=s' => \@overrides,
           'debug=i' => \$debug_level, 'set=s' => \%cmd_overrides,
           'version' => \$print_version, 'verbose' => \$verbose,
           'entry-mod' => \$entry_mod, 'force-install' => \$force_install,
           'target=s' => \@limit_modules, 'manual' => \$manual)
   or pod2usage(2);

# Print the help output and exit if --help was on the command line.
pod2usage(1) if $help;
pod2usage(-exitstatus => 0, -verbose => 2) if $manual;

# Print the version number and exit if --version was on the command line.
printVersion() && exit(0) if $print_version;

# If we are doing verbose output and the user did not change the debug level,
# then we push the debug level up to the max.
$debug_level = $HVERB_LVL if $verbose && $debug_level <= $CRITICAL_LVL;

if ( ! $cfg_file )
{
   if ( -r "Gatherrc" )
   {
      $cfg_file = 'Gatherrc';
   }
   elsif ( -r ".gatherrc" )
   {
      $cfg_file = '.gatherrc';
   }
   else
   {
      $cfg_file = "$ENV{'HOME'}/.gatherrc";
   }
}

$log_file = "gather.log";
open(LOG_FILE, "> $log_file")
   or warn "WARNING: Could not create log file $log_file: $!\n";

my (%orig_modules) = ();
parse("$cfg_file", \%orig_modules) or die "ERROR: Failed to parse $cfg_file\n";
my %override_modules = %orig_modules;

if ( $#overrides == -1 )
{
   if ( -r ".gatherrc-override" )
   {
      push(@overrides, '.gatherrc-override');
   }
   elsif ( -r "$ENV{'HOME'}/.gatherrc-override" )
   {
      push(@overrides, "$ENV{'HOME'}/.gatherrc-override");
   }
}

my $override = '';
foreach $override ( @overrides )
{
   if ( open(OVERRIDE, "$override") )
   {
      my $line;
      while ( $line = <OVERRIDE> )
      {
         chomp($line);

         # Strip comments.
         $line =~ s/#.*$//;

         # Skip blank lines.
         next if $line =~ /^\s*$/;

         # The current line has at least one wildcard.
         if ( $line =~ /\*/ )
         {
            if ( $line !~ /\*\./ )
            {
               warn "ERROR: Invalid wildcard use at $override:$.\n";
            }
            else
            {
               my(@override_lines) = expandWildcardLine("$line",
                                                        \%orig_modules);

               foreach ( @override_lines )
               {
                  doOverride("$_", \%override_modules);
               }
            }
         }
         # The current line has no wildcards.
         else
         {
            doOverride("$line", \%override_modules);
         }
      }

      close(OVERRIDE);
   }
   else
   {
      warn "WARNING: Could not open override file $override: $!\n";
   }
}

my $key;
foreach $key ( keys(%cmd_overrides) )
{
   # This is needed because expandWildcardLine() and doOverride() expect to
   # see something of the form "<key> = <value>".
   my $line = "$key = $cmd_overrides{$key}";

   # The current line has at least one wildcard.
   if ( $key =~ /\*/ )
   {
      if ( $key !~ /\*\./ )
      {
         warn "ERROR: Invalid wildcard use at $override:$.\n";
      }
      else
      {
         my(@override_lines) = expandWildcardLine("$line", \%orig_modules);

         foreach ( @override_lines )
         {
            doOverride("$_", \%override_modules);
         }
      }
   }
   # The current line has no wildcards.
   else
   {
      doOverride("$line", \%override_modules);
   }
}

my (%targeted_modules) = ();

if ( $#limit_modules == -1 )
{
   %targeted_modules = %override_modules;
}
else
{
   foreach ( @limit_modules )
   {
      if ( ! defined($override_modules{"$_"}) )
      {
         warn "WARNING: Trying to target unknown module '$_'\n";
         next;
      }

      $targeted_modules{"$_"} = $override_modules{"$_"};
   }
}

checkoutModules(\%targeted_modules);
exit 0;

# -----------------------------------------------------------------------------
# Subroutines follow.
# -----------------------------------------------------------------------------

sub printVersion ()
{
   print "$VERSION\n";

   return 1;
}

sub parse ($$)
{
   my $file       = shift;
   my $module_ref = shift;

   my $status = 1;

   open(INPUT, "$file") or die "ERROR: Could not open $file: $!\n";

   my $cfg_data = '';
   while ( <INPUT> )
   {
      $cfg_data .= $_;
   }
   close(INPUT);

   $cfg_data =~ s|/\*.*?\*/||gs; # Strip out C-style comments
   $cfg_data =~ s/#.*$//gm;      # Strip out shell-style comments
   $cfg_data =~ s|//.*$||gm;     # Strip out C++-style comments 

   my $mod_count = 0;
   while ( $cfg_data =~ /\s*(\w.*?\w?)\s*{\s*(.*?)\s*}\s*;\s*/s )
   {
      my $module_name = "$1";
      my $module_body = "$2";
      $cfg_data       = $';

      $indent = 0;
      print "Loading $module_name from $file ...\n";
      my $parse_stat = parseModule("$module_body", "$module_name", $module_ref);
      return 0 if $parse_stat == -1;
      $mod_count++
   }

   warn "WARNING: Nothing happened which probably means a parse error\n"
      unless $mod_count > 0;

   return $status;
}

sub parseModule ($$$;$)
{
   my $module_body = shift;
   my $module_name = shift;
   my $module_ref  = shift;
   my $in_module   = shift || 0;

   print " " x $indent, "Parsing $module_name\n";

   my $status = 1;

   # Ensure that all of these are initialized just for safety's sake.
   $$module_ref{"$module_name"}{'CVSROOT'} = '';
   $$module_ref{"$module_name"}{'Module'}  = [];
   $$module_ref{"$module_name"}{'Date'}    = '';
   $$module_ref{"$module_name"}{'Tag'}     = '';
   $$module_ref{"$module_name"}{'Path'}    = '.';

   # Initialize this module's dependency hash with an anonymous hash
   # reference.
   $$module_ref{"$module_name"}{'deps'} = {};

   while ( "$module_body" ne "" && $status == 1 )
   {
      printDebug $HEX_LVL, ">" x 78, "\nNew module body:\n$module_body\n",
                           "<" x 78, "\n";

      SWITCH:
      {
         # Matched an include.
         if ( $module_body =~ /^(s?include\s+(.+?);)/s )
         {
            my $inc_string = "$1";
            my $inc_file   = "$2";

            $indent += 4;
            $status = handleInclude("$inc_string", "$inc_file", \$module_body);
            $indent -= 4;

            last SWITCH;
         }

         if ( $module_body =~ /^CVSROOT:\s+(\S+?)\s*;/s )
         {
            $$module_ref{"$module_name"}{'CVSROOT'} = "$1";
            $module_body = $';
            last SWITCH;
         }

         if ( $module_body =~ /^Module:\s+(.+?)\s*;/s )
         {
            my $temp = "$1";
            $module_body = $';

            my($cvs_module_name, $install_name) = ('', '');

            if ( $temp =~ /^(.+?)\s*\[(.*)\]\s*$/ )
            {
               $cvs_module_name = "$1";
               $install_name    = "$2";
            }
            else
            {
               $cvs_module_name = "$temp";
            }

            push(@{$$module_ref{"$module_name"}{'Module'}},
                 {$cvs_module_name => $install_name});
            last SWITCH;
         }

         # Matched a tag/branch setting for this module.
         if ( $module_body =~ /^Tag:\s+(\S+?)\s*;/s )
         {
            $$module_ref{"$module_name"}{'Tag'}  = "$1";
            $module_body = $';
            last SWITCH;
         }

         # Matched a date setting for this module.
         if ( $module_body =~ /^Date:\s+(.+?)\s*;/s )
         {
            $$module_ref{"$module_name"}{'Date'}  = "$1";
            $module_body = $';
            last SWITCH;
         }

         # Matched a path setting for this module.
         if ( $module_body =~ /^Path:\s+(.+?)\s*;/s )
         {
            my $path = "$1";
            expandEnvVars(\$path);
            $$module_ref{"$module_name"}{'Path'}  = "$path";
            $module_body   = $';
            last SWITCH;
         }

         # Matched the beginning of a sub-module.
         if ( $module_body =~ /^(\w.*?\w?)\s*{\s*(.*)/s )
         {
            $indent += 4;
            ($module_body = parseModule("$2", "$1",
                                        $$module_ref{"$module_name"}{'deps'},
                                        1))
               or return 0;
            $indent -= 4;
            print " " x $indent, "Returning to $module_name\n";
            last SWITCH;
         }

         # We have reached the end of the module.
         if ( $module_body =~ /^}/ && $in_module )
         {
            print " " x $indent, "Finished parsing $module_name\n";
            return $';
         }

         warn "Parse error in the following:\n$module_body\n";
         $status = -1;
      }

      $module_body =~ s/^\s+(\S*)/$1/s;
   }

   return $status;
}

sub handleInclude ($$$)
{
   my $inc_string = shift;
   my $inc_file   = shift;
   my $text_ref   = shift;

   my $sinclude = 1 if $inc_string =~ /^sinclude/;

   my $status = 1;

   expandEnvVars(\$inc_file);

   if ( open(INC_FILE, "$inc_file") )
   {
      my $loaded_body = '';
      while ( <INC_FILE> )
      {
         $loaded_body .= $_;
      }
      close(INC_FILE);

      $$text_ref =~ s/\Q$inc_string\E/$loaded_body/s;
   }
   else
   {
      warn "WARNING: Failed to load $inc_file: $!\n" unless $sinclude;
      $$text_ref =~ s/\Q$inc_string\E//s;
      $status = 0;
   }

   return $status;
}

sub expandWildcardLine ($$)
{
   my $line       = shift;
   my $module_ref = shift;

   my (@expanded_lines) = ();

   my ($path, $value) = split(/\s*=\s*/, "$line");
   my (@path_arr)     = split(/\./, "$path");

   local $full_path = "$path";

   if ( $#path_arr < 1 )
   {
      warn "ERROR: Invalid hierarchy in $line ($override:$.)\n";
   }
   else
   {
      my (@modules) = ();

      if ( "$path_arr[0]" eq "*" )
      {
         @modules = keys(%$module_ref);
      }
      else
      {
         $modules[0] = $path_arr[0];
      }

      my $module;
      foreach $module ( @modules )
      {
         my (@work_arr) = @path_arr;
         $work_arr[0] = "$module";
         my $work_path = "$full_path";
         $work_path =~ s/^\*/$module/;

         expandWildcards(\@work_arr, $module_ref, $value, \@expanded_lines,
                         $work_path);
      }
   }

   return @expanded_lines;
}

sub expandWildcards ($$$$$)
{
   my $path_ref       = shift;
   my $module_ref     = shift;
   my $override_value = shift;
   my $lines_ref      = shift;
   my $work_path      = shift;

   my $current_value = shift(@$path_ref);
   printDebug $VERB_LVL, "current_value: $current_value\n";

   if ( "$current_value" eq "*" )
   {
      die "This should never happen!\n";
   }
   else
   {
      # A wildcard in this position (<Module>.*) must always indicate a
      # project dependency.
      if ( $$path_ref[0] && $$path_ref[0] eq "*" )
      {
         my (@temp_arr) = ();
         my ($dep_name, $new_path);

         shift(@$path_ref);
         my $save_path = $work_path;
         printDebug $VERB_LVL, "Unexpanded Path: $work_path\n";

         foreach $dep_name ( keys(%{$$module_ref{"$current_value"}{'deps'}}) )
         {
            printDebug $VERB_LVL, "Dep: $dep_name\n";
            @temp_arr = @$path_ref;
            unshift(@temp_arr, "$dep_name");
            printDebug $HVERB_LVL, "path_arr: @$path_ref\n";
            printDebug $HVERB_LVL, "temp_arr: @temp_arr\n";

            $work_path =~ s/\*/$dep_name/;
            printDebug $VERB_LVL, "New Path: $work_path\n";

            expandWildcards(\@temp_arr, $$module_ref{"$current_value"}{'deps'},
                            $override_value, $lines_ref, $work_path);
            $work_path = $save_path;
         }

         # If the above expansion gave us <Module>.<Setting>, we now have
         # only <Setting> which is meaningless.
         # XXX: It seems like this should not be necessary, but it may be the
         # result of using a different array reference in the recursive calls.
         shift(@$path_ref) if $#$path_ref == 0;
      }
      elsif ( defined($$module_ref{"$current_value"}) )
      {
         printDebug $STATE_LVL, "Matched $current_value\n";
         printDebug $VERB_LVL, "Checking for $$path_ref[0] in $current_value\n";

         # We have reached the end of the recursion.
         if ( defined($$module_ref{"$current_value"}{$$path_ref[0]}) )
         {
            printDebug $STATE_LVL,
                       "Adding line '$work_path = $override_value'\n";
            push(@$lines_ref, "$work_path = $override_value");
         }
         elsif ( defined($$module_ref{"$current_value"}{'deps'}{$$path_ref[0]}) )
         {
            printDebug $STATE_LVL, "Matched dependency $$path_ref[0]\n";
            expandWildcards($path_ref, $$module_ref{"$current_value"}{'deps'},
                            $override_value, $lines_ref, $work_path);
         }
         else
         {
            warn "WARNING: $work_path not found!\n";
         }
      }
      else
      {
         warn "WARNING: $work_path not found!\n";
      }
   }
}

sub doOverride ($$)
{
   my $line       = shift;
   my $module_ref = shift;

   my ($path, $value) = split(/\s*=\s*/, "$line");
   my (@path_arr)     = split(/\./, "$path");

   if ( $#path_arr < 1 )
   {
      warn "ERROR: Invalid hierarchy in $line ($override:$.)\n";
   }
   else
   {
      local $full_path = "$path";
      overrideValue(\@path_arr, $module_ref, "$value");
   }
}

sub overrideValue ($$$)
{
   my $path_ref       = shift;
   my $module_ref     = shift;
   my $override_value = shift;

   my $current_value = shift(@$path_ref);

   if ( defined($$module_ref{"$current_value"}) )
   {
#      print "Matched $current_value\n";
#      print "Checking for ", $$path_ref[0], " in $current_value\n";

      # We have reached the end of the recursion.
      if ( defined($$module_ref{"$current_value"}{$$path_ref[0]}) )
      {
         if ( $$path_ref[0] eq "Module" )
         {
            my $module_list = "$override_value";
            $module_list =~ s/;/ /g;

            my(@modules)       = split(/;/, "$module_list");
            my(@mod_overrides) = ();

            my $mod;
            foreach $mod ( @modules )
            {
               my($module_name, $install_name) = ('', '');

               if ( $mod =~ /^\s*(.+?)\s*\[(.*)\]\s*$/ )
               {
                  $module_name  = "$1";
                  $install_name = "$2";
               }
               else
               {
                  $module_name = "$mod";
               }

               push(@mod_overrides, {$module_name => $install_name});
            }

            print "Overriding $full_path with $module_list\n";
            $$module_ref{"$current_value"}{'Module'} = \@mod_overrides;
         }
         elsif ( $$path_ref[0] eq "Path" )
         {
            my $new_path = "$override_value";
            expandEnvVars(\$new_path);

            print "Overriding $full_path with $new_path\n";
            $$module_ref{"$current_value"}{'Path'} = "$new_path";
         }
         else
         {
            print "Overriding $full_path with $override_value\n";
            $$module_ref{"$current_value"}{$$path_ref[0]} = "$override_value";
         }
      }
      elsif ( defined($$module_ref{"$current_value"}{'deps'}{$$path_ref[0]}) )
      {
#         print "Matched dependency $$path_ref[0]\n";
         overrideValue($path_ref, $$module_ref{"$current_value"}{'deps'},
                       $override_value);
      }
      else
      {
         warn "WARNING: $full_path not found!\n";
      }
   }
   else
   {
      warn "WARNING: $full_path not found!\n";
   }
}

sub checkoutModules ($)
{
   my $mod_ref = shift;

   my $mod_name;
   foreach $mod_name ( keys(%$mod_ref) )
   {
      my $module = '';
      foreach $module ( @{$$mod_ref{"$mod_name"}{'Module'}} )
      {
         my($cvs_module_name, $install_name) = each(%$module);

         if ( defined($$mod_ref{"$mod_name"}{'CVSROOT'}) && $cvs_module_name )
         {
            # If a module is already checked out, update it instead of doing
            # a new checkout.
            if ( ($install_name &&
                    -d "$$mod_ref{$mod_name}{'Path'}/$install_name") ||
                 -d "$$mod_ref{$mod_name}{'Path'}/$cvs_module_name" )
            {
               updateModule("$mod_name", $$mod_ref{"$mod_name"}{'Path'},
                            "$cvs_module_name", "$install_name");
            }
            else
            {
               printDebug $VERB_LVL, "$mod_name --> ",
                          "$$mod_ref{$mod_name}{'CVSROOT'} ",
                          "$$mod_ref{$mod_name}{'Tag'} ",
                          "$$mod_ref{$mod_name}{'Date'} ",
                          "$cvs_module_name (to $$mod_ref{$mod_name}{'Path'}";
               printDebug $VERB_LVL, "/$install_name" if $install_name;
               printDebug $VERB_LVL, ")\n";

               checkoutModule($mod_name, $$mod_ref{"$mod_name"}{'CVSROOT'},
                              $$mod_ref{"$mod_name"}{'Tag'},
                              $$mod_ref{"$mod_name"}{'Date'},
                              "$cvs_module_name",
                              $$mod_ref{"$mod_name"}{'Path'},
                              "$install_name");
            }
         }
      }

      checkoutModules($$mod_ref{"$mod_name"}{'deps'});
   }
}

sub updateModule ($$$$)
{
   my $name         = shift;
   my $path         = shift;
   my $cvs_module   = shift;
   my $install_name = shift;

   my $module = ("$install_name" ne "") ? "$install_name" : "$cvs_module";

   print "Updating $name ($module) in $path ...\n";

   my $cur_dir = getcwd();
   chdir("$path") if "$path" ne "" && "$path" ne ".";

   my $status;

   if ( chdir("$module") )
   {
      $status = runCvsCommand('cvs update');
   }
   else
   {
      warn "WARNING: Update of $module in $path failed: $!\n";
      $status =  0;
   }

   chdir("$cur_dir");
   print "Done\n";

   return $status;
}

sub checkoutModule ($$$$$$$)
{
   my $name         = shift;
   my $cvsroot      = shift;
   my $tag          = shift;
   my $date         = shift;
   my $cvs_module   = shift;
   my $path         = shift;
   my $install_name = shift;

   if ( "$cvsroot" ne "" && "$cvs_module" ne "" )
   {
      print LOG_FILE "Checking out $cvs_module from $cvsroot ...\n";
      my $cmd_line = "cvs -d $cvsroot checkout ";

      $cmd_line .= "-r $tag " if $tag && "$tag" ne "HEAD";
      $cmd_line .= "-D \"$date\" " if $date;

      $cmd_line .= "$cvs_module";
      print "Checking out source for $name: $cvs_module -- please wait ...\n";
      print LOG_FILE "$cmd_line\n";
      my $cwd = getcwd();

      # The name for the temporary checkout directory.
      my $temp_checkout_dir = "$cwd/temp.$$";

      # Create the temporary directory for the initial module checkout.
      # If it already exists, just use it.
      if ( -d "$temp_checkout_dir" || mkdir("$temp_checkout_dir", 0700) )
      {
         chdir("$temp_checkout_dir")
            or warn "WARNING: Could not chdir to $temp_checkout_dir: $!\n";
      }
      # If the temporary directory creation failed, we'll just fall back on
      # the current working directory.
      else
      {
         $temp_checkout_dir = "$cwd";
      }

      runCvsCommand("$cmd_line");

      chdir("$cwd");
      mkdir("$path", 0755) unless -d "$path";
      chdir("$path") if $path;

      # This is the full path to the directory where the checked-out module
      # will be placed after being downloaded.
      my $inst_dir = getcwd();

      # We have an alternate installation name that the actual path to the
      # checked-out module.
      if ( $install_name )
      {
         print "    Moving $cvs_module to $install_name ...\n";
         
         my $install_dir = '';

         # The installation name contains at least two directories.
         if ( $install_name =~ /\// )
         {
            $install_dir = (fileparse("$install_name"))[1];
         }
         # The installation does not contain a directory, so the goal is
         # simply to rename a checked out directory.
         else
         {
            $install_dir = '.';
         }

         # Create the path for the module's target location unless it already
         # exists.
         mkpath($install_dir, 0, 0755) unless -d "$install_dir";

         my $orig_module_name = '';

         # Save the current working directory.  It is the directory where
         # the module will be installed.
         my $target_dir = getcwd();

         # Now we must go into the checkout directory to do the renaming.
         chdir("$temp_checkout_dir")
            or die "ERROR: Could not chdir to $temp_checkout_dir: $!\n";

         # If the checked out module contained a path, we have to deal with
         # that.
         if ( $cvs_module =~ /\// )
         {
            my ($module_dir, $ext);
            ($orig_module_name, $module_dir, $ext) = fileparse("$cvs_module");

            # Go to the subdirectory containing the actual module.
            chdir("$module_dir")
               or die "ERROR: Could not chdir to $temp_checkout_dir/$module_dir: $!\n";
         }
         # Otherwise, we can just use the module name as it was originally
         # given.
         else
         {
            $orig_module_name = "$cvs_module";
         }

         printDebug $VERB_LVL, "$orig_module_name --> $target_dir/$install_name\n";

         if ( -d "$target_dir/$install_name" && ! $force_install )
         {
            warn "    WARNING: Module renaming failed--target directory already exists!\n";
         }
         else
         {
            rmtree("$target_dir/$install_name") if $force_install;
            printDebug $STATE_LVL, "Moving $orig_module_name to " .
                                   "$target_dir/$install_name\n";
            rename("$orig_module_name", "$target_dir/$install_name")
               or warn "    WARNING: Module renaming failed: $!\n";

            # If the user requested to do so, modify $dest_dir/CVS/Entries to
            # include $dir.
            modifyCvsEntries("$target_dir", "$install_name") if $entry_mod;
         }
      }
      # We are going to use the default module name, but we have to get it
      # from the temporary directory to the place the user wants it.
      else
      {
         my @module_path = split(/\//, "$cvs_module");

         my $skipped_path = '';
         my $dest_dir = "$inst_dir";

         # We have to iterate over each directory in @module_path and find
         # the first path element that does not exist in $dest_dir.  The
         # reason is that a previous module may have set up part of this
         # module's path already, and we cannot overwrite what already exists.
         my $dir;
         foreach $dir ( @module_path )
         {
            # We found a directory that does not already exist, so we can go
            # ahead with the move.
            if ( ! -d "$dest_dir/$dir" )
            {
               my $src_dir = "$temp_checkout_dir/$skipped_path";
               printDebug $STATE_LVL, "Renaming $src_dir/$dir to $dest_dir/$dir\n";
               rename("$src_dir/$dir", "$dest_dir/$dir");

               # If the user requested to do so, modify $dest_dir/CVS/Entries
               # to include $dir.
               modifyCvsEntries("$dest_dir", "$dir") if $entry_mod;

               last;
            }
            # $dir already exists in $dest_dir, so append it to $dest_dir and
            # move on.
            else
            {
               $skipped_path .= "$dir/";
               $dest_dir     .= "/$dir";
               printDebug $STATE_LVL, "Descending to $skipped_path ...\n";
            }
         }
      }

      printDebug $STATE_LVL, "Removing tree $temp_checkout_dir\n";
      rmtree("$temp_checkout_dir")
         or warn "WARNING: Could not clean up temporary checkout directory: $!\n";

      print "Done\n";
      chdir("$cwd");
   }
   else
   {
      warn " " x $indent, "Nothing to check out for $name!\n";
   }
}

sub runCvsCommand ($)
{
   my $cmd_line = shift;

   # Open a pipe to read from the output of $cmd_line.
   open(CVS_CMD, "$cmd_line 2>&1 |") or die "Can't fork: $!\n";
   $| = 1;

   # Power users will appreciate seeing the output from CVS as it happens.
   if ( $verbose )
   {
      print "$_" while <CVS_CMD>;
   }
   # For simpler folk, we will just write out to a log file.  To keep them
   # placated, however, there will be a little spinner that runs while CVS is
   # doing its job.
   else
   {
      my $next_char = '|';

      print "Working ... $next_char";

      while ( <CVS_CMD> )
      {
         print LOG_FILE "$_";
         $next_char = nextSpinnerFrame("$next_char");
      }
   }

   $| = 0;

   # Close up our pipe now that we are done with it.
   close(CVS_CMD);

   if ( $? && ! $verbose )
   {
      warn "\nWARNING: An error may have occurred when running CVS.  " .
           "Check $log_file\n";
   }

   return 1;
}

sub modifyCvsEntries ($$)
{
   my $base_dir = shift;
   my $new_dir  = shift;

   my $entries_file = "$base_dir/CVS/Entries";

   printDebug $STATE_LVL, "Adding entry to $entries_file";

   my @entries = ();

   if ( -r "$entries_file" ) 
   {
      if ( open(ENTRIES, "$entries_file") )
      {
         @entries = <ENTRIES>;
         close(ENTRIES);

         my(@ent_list) = grep(/$new_dir/, @entries);

         if ( $#ent_list == -1 )
         {
            push(@entries, "D/$new_dir////");
         }
         else
         {
            printDebug $WARNING_LVL, "$new_dir already exists in $entries_file\n";
         }
      }
      else
      {
         warn "WARNING: Could not append $new_dir to $entries_file";
         return;
      }
   }
   else
   {
      push(@entries, "D/$new_dir////");
   }

   if ( open(ENTRIES, "> $entries_file") )
   {
      foreach ( @entries )
      {
         chomp;
         print ENTRIES "$_\n";
      }

      close(ENTRIES);
   }
   else
   {
      warn "WARNING: Could not create $entries_file: $!\n";
   }
}

sub expandEnvVars ($)
{
   my $text_ref = shift;

   while ( $$text_ref =~ /\${(.+?)}/ )
   {
      my $var_name = "$1";
      $$text_ref =~ s/\${$var_name}/$ENV{$var_name}/g;
   }
}

sub printDebug ($@)
{
   my $level = shift;
   print STDERR @_ if $debug_level >= $level;
}

sub nextSpinnerFrame ($)
{
   my $last_frame = shift;

   my $next_frame = '';

   # Move on to the next character of the animation sequence.
   if ( $last_frame eq "|" )
   {
      $next_frame = '/';
   }
   elsif ( $last_frame eq "/" )
   {
      $next_frame = '-';
   }
   elsif ( $last_frame eq "-" )
   {
      $next_frame = '\\';
   }
   elsif ( $last_frame eq "\\" )
   {
      $next_frame = '|';
   }

   # Print one status character per line of CVS output.
   print "\b$next_frame";

   return "$next_frame";
}

__END__

=head1 NAME

cvs-gather.pl

=head1 SYNOPSYS

This script gathers together CVS modules from one or more repositories
into a single development tree.

=head1 OPTIONS

=over 8

=item B<--help>

Print usage information.

=item B<--cfg=<filename>>

Specify the name of the module configuration to load. If not given,
the current directory is searched for a Gatherrc file and then a .gatherrc
file. If one is not found, the user's home directory is searched for the
same file.

=item B<--debug=<level>>

Set the debug output level (0-5).

=item B<--entry-mod>

Modify CVS/Entries to include a newly downloaded module.  This
is I<not> the default behavior.

=item B<--force-install>

When re-downloading a module, force removal of the existing
directory.  Without this option, a warning is printed, and the
existing directory is not removed.

=item B<--target <module1>> B<--target <module2>> ... B<--target <moduleN>>

=item B<--target <module1,module2,...,moduleN>>

Limit the gathered modules to those listed.  The list may be defined by
multiple B<--target> arguments or by specifying a single B<--target>
argument with a comma-separated list of module names.  The named module
may only be a "top-level" module, and all dependencies of the named module
will be gathered.  In other words, the named module cannot exist solely as
a dependency of an enclosing module.

=item B<--manual>

Print the documentation for this script in manpage format.  This has
exactly the same effect as invoking perldoc(1) on this script.

=item B<--override=<filename>>

Specify the name of an an override file used to override values set
by the loaded module configuration file. There may be zero or more of
these. If not specified, the current directory is searched for the
file .gatherrc-override. If not found, the user's home directory is
searched for the same file.

=item B<--set> <key1>=<value1> --set <key2>=<value2> ... --set <keyN>=<valueN>

Override the setting for "key" with "value". This supercedes any
settings in the module configuration file and any loaded override
file. There may be zero or more of these.  The key is a dotted string
containing zero or more wildcards that describes the hierarchy of a
given element (Project.Dep1.CVSROOT, for example).  The value is the
new value for the named key.  Wildcards can only appear in place of
module names.

=item B<--verbose>

Turn on verbose output.  This basically makes the log file useless.

=item B<--version>

Print the version number and exit.

=back

=head1 FILE FORMATS

This script handles two different types of files: basic configuration
files and configuration override files.  The former describes what
will be downloaded, and the latter provides a convenient mechanism for
overriding settings in the basic configuration.  Thus, a single
configuration file can be used by multiple users, each with their own
custom overrides.

=head2 Basic configuration

The basic configuration file format is comparable to a C/C++ struct.
It defines a hierarchical grouping of data.  The hierarchies are made
up of projects, their dependencies, dependencies of the dependencies,
and so on.  A single configuration file can contain multiple projects,
each with their own dependencies.  Information can be shared by including
external files at any point throughout the file.

Within each project, several properties may be set that define the CVS
settings needed to download the project.  These include the repository
root, the repository module name, a tag on the code, a date for the
code, and a path to place the checked out code.  Of these, none are
required.  This allows a "global" module that defines something similar
to a C++ namespace.  However, if a repository root is named, a module
within that repository must be named too.

=head2 Configuration overrides

To override settings provided through a basic configuration file, the
user may choose to use the override capabilities.  The overrides may be
specified several ways, one of which is through the use of a file.  The
format of the file is very simple: one line per override with each line
taking the form:

key = value

Here, B<key> is some dotted string that describes the module hierarchy, and
B<value> is the new value for that key.  Consider the following example
configuration:

My_Module
{
   CVSROOT: :pserver:anon@server.net:/cvsroot/mymod;
   Module: mine;

   Dep1
   {
      CVSROOT: user@cvs-server2.com:/cvsroot/DepRepos;
      Module: mathlib;
   }
};

To override the CVSROOT of Dep1, the following could be done:

My_Module.Dep1.CVSROOT = :pserver:anon@cvs-server2.com:/cvsroot/DepRepos

Several modules can have a common property overridden using the wildcard
character B<*>.  For example:

My_Module.*.CVSROOT = :pserver:anon@cvs-server2.com:/cvsroot/DepRepos

This would override the CVSROOT settings for I<all> direct dependencies
of My_Module.  (Dependencies of those dependencies will not be affected,
however).  The wildcard character may only be specified for module names.
It would not make sense to set the CVSROOT, Tag, and Date properties to
the same value.

Comments in this file begin with the B<#> (hash) character and extend to
the end of the line.  No other comment syntax is supported.

=head1 FILES

=over 8

=item B<./Gatherrc>, B<./.gatherrc>, B<$HOME/.gatherrc>

If no configuration file is specified using B<--cfg>, the script will
search the current directory for the file B<Gatherrc> and then the file
B<.gatherrc>.  If not found, the user's home directory is searched for
a file of the same name.  This is the basic configuration file that
names the module (or modules) and any dependencies.  A configuration
file must be specified using one of these three methods for the script
to do any work.

=item B<./.gatherrc-override>, B<$HOME/.gatherrc-override>

After loading the configuration file, values set in the configuration
may be overridden.  An override file may be specified using B<--override>,
or individual settings may be overridden using B<--set>.  In addition,
the current directory is searched for a file named B<.gatherrc-override>.
If not found, the user's home directory is searched for a file of the
same name.  Override settings are optional.

=back
