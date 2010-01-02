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

package JugglerConfigure;

use strict 'vars';
use vars qw($DEFAULT_MODULE);

sub parseConfigFile($);

$DEFAULT_MODULE = '';
my %MODULES     = ();

sub parseConfigFile ($)
{
   my $cfg = shift;
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

         $MODULES{"$mod"} = new JugglerModule("$mod");

         while ( $deps !~ /^\s*$/ )
         {
            # Match a dependency on another module.
            if ( $deps =~ /^\s*depend\s+(\S+);/ )
            {
               $deps = $';
               my $module_name = "$1";

               die "ERROR: No such module $module_name for $mod dependency\n"
                  unless defined($MODULES{"$module_name"});

               $MODULES{"$mod"}->addDependencies($MODULES{"$module_name"}->getDependencies());
            }
            # Match a dependency on a package.  This may have environment
            # variable settings, or it may just be a path.
            elsif ( $deps =~ /\s*(\S.+?)(:\s+(.+?)|\s*);/ )
            {
               $deps = $';

               my $dep_path = "$1";
               my %vars = ();

               if ( defined($3) )
               {
                  my @var_list = split(/\s*,\s*/, "$3");

                  my $var;
                  foreach $var ( @var_list )
                  {
                     $var =~ /\s*(\w+)=(\S+)\s*/;
                     $vars{"$1"} = "$2";
                  }
               }

               $MODULES{"$mod"}->addDependency(new ModuleDependency("$dep_path",
                                                                    \%vars));
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

   return %MODULES;
}

package JugglerModule;

sub new ($$)
{
   my $class = shift;
   my $name  = shift;

   return bless
   {
      'name' => $name,          # Name of this module
      'deps' => []              # Array of ModuleDependecy objects
   }, $class;
}

sub getName ($)
{
   my $this = shift;
   return $this->{'name'};
}

sub getDependencies ($)
{
   my $this = shift;
   return @{$this->{'deps'}};
}

sub addDependency ($$)
{
   my $this = shift;
   my $dep  = shift;
   push(@{$this->{'deps'}}, $dep) unless $this->hasDependency($dep);
}

sub addDependencies ($@)
{
   my $this = shift;

   # The $MODULES entry for $module_name contains an array of hash references.
   # We just copy those references into the array for the current module
   # ($this).  Simple, no?
   my $dep;
   foreach $dep ( @_ )
   {
      $this->addDependency($dep) unless $this->hasDependency($dep);
   }
}

sub hasDependency ($$)
{
   my $this    = shift;
   my $new_dep = shift;

   my $has_dependency = 0;

   my $cur_dep;
   foreach $cur_dep ( $this->getDependencies() )
   {
      if ( $cur_dep->getPath() eq $new_dep->getPath() )
      {
         $has_dependency = 1;
         last;
      }
   }

   return $has_dependency;
}

package ModuleDependency;

sub new ($$;$)
{
   my $class = shift;
   my $path  = shift;
   my $env   = shift || {};
   
   return bless
   {
      'path' => $path,          # Path to this dependency
      'env'  => $env            # Reference to environment variable hash
   }, $class;
}

sub getPath ($)
{
   my $this = shift;
   return $this->{'path'};
}

sub getEnvironment ($)
{
   my $this = shift;
   return %{$this->{'env'}};
}

sub getEnvironmentValue ($$)
{
   my $this = shift;
   my $key  = shift;

   return ${$this->{'env'}}{"$key"};
}

1;
