#!/usr/bin/perl -w

require 5.004;

use strict 'vars';
use vars qw($base_dir $module $CONFIG_ARGS $PATH_ARGS $HOST_ARGS $FEATURE_ARGS
            $CUSTOM_ARGS $LAST_ARG_GROUP);
use vars qw(%MODULES);

use Cwd qw(chdir getcwd);
use File::Basename;
use File::Path;
use Getopt::Long;
use Pod::Usage;

# Subroutine prototypes.
sub parseConfigFile($);
sub configureModule($);
sub generateMakefile();
sub printHelp();
sub getConfigureHelp($$);
sub parseOutput($$);

my $help        = 0;
my $cfg         = "juggler.cfg";
$module         = '';
my $script_help = 0;

$CONFIG_ARGS    = 0;
$PATH_ARGS      = 1;
$HOST_ARGS      = 2;
$FEATURE_ARGS   = 3;
$CUSTOM_ARGS    = 4;
$LAST_ARG_GROUP = 5;

my @save_argv = @ARGV;

Getopt::Long::Configure('pass_through');
GetOptions('help|?' => \$help, 'cfg=s' => \$cfg, 'module=s' => \$module,
           'script-help' => \$script_help)
   or pod2usage(2);
pod2usage(1) if $script_help;

die "ERROR: No configuration given\n" unless $cfg;

$base_dir = (fileparse("$0"))[1];

open(RECONFIG, "> reconfig");
print RECONFIG "$0 ", "@save_argv\n";
close(RECONFIG);

parseConfigFile("$cfg");

printHelp() && exit(0) if $help;

if ( $module )
{
   configureModule("$module");
}
else
{
   foreach ( keys(%MODULES) )
   {
      configureModule("$_");
   }
}

generateMakefile();

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

               my $var;
               foreach $var ( @var_list )
               {
                  $var =~ /\s*(\w+)=(\S+)\s*/;
                  $vars{"$1"} = "$2";
               }

               push(@{$MODULES{"$mod"}}, {'path' => $1, 'env' => \%vars});
            }
            else
            {
               # XXX: Not quite right...
               $deps ='';
            }
         }
      }

      $cfg_file =~ s/^\s*//;
   }
}

sub configureModule ($)
{
   my $module_name = shift;

   my $cwd = getcwd();

   die "ERROR: No module $module_name defined\n"
      unless defined($MODULES{"$module_name"});

   my $modref;
   foreach $modref ( @{$MODULES{"$module_name"}} )
   {
      my $mod_path = $$modref{'path'};

      mkpath("$mod_path", 1, 0755) unless -d "$mod_path";
      chdir("$mod_path")
         or warn "WARNING: Could not chdir to $mod_path\n";
      print "Running $cwd/$base_dir/$mod_path/configure @ARGV\n";
      system("$cwd/$base_dir/$mod_path/configure @ARGV 2>&1") == 0
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
               $ENV{"$_"} = "$cwd/instlinks";
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

sub generateMakefile ()
{
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

   if ( $module )
   {
      foreach ( @{$MODULES{"$module"}} )
      {
         $modules .= "${$_}{'path'} ";
      }
   }
   else
   {
      my $mod_name;
      foreach $mod_name ( keys(%MODULES) )
      {
         foreach ( @{$MODULES{"$module"}} )
         {
            $modules .= "${$_}{'path'} ";
         }
      }
   }

   my $cwd = getcwd();
   chdir("$base_dir");
   $input_file =~ s/\@JUGGLER_PROJECTS\@/$modules/g;
   $input_file =~ s/\@JUGGLERROOT_ABS\@/$ENV{'PWD'}/g;
   $input_file =~ s/\@topdir\@/$cwd/g;
   chdir("$cwd");

   print "Generating Makefile\n";
   open(OUTPUT, "> Makefile") or die "ERROR: Could not create Makefile: $!\n";
   print OUTPUT "$input_file";
   close(OUTPUT) or warn "WARNING: Failed to save Makefile: $!\n";
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
   else
   {
      foreach ( keys(%MODULES) )
      {
         getConfigureHelp("$_", \@help_output);
      }
   }

   print "Modules that may be built:\n";
   foreach ( keys(%MODULES) )
   {
      print "\t$_\n";
   }

   print "\n";

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

=item B<--help>

=item B<--cfg>=file

=back

=head1 DESCRIPTION

=out
