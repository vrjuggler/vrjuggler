#!/usr/bin/perl
use File::Basename;
use Getopt::Std;

# Do this to include the path to the script in @INC.
my $path;

BEGIN {
   $path = (fileparse("$0"))[1];
}

use lib($path);
use RecurseDir;

# get opts:
getopts('d:e:ha');

my $working_dir = "$opt_d" || ".";
my @extensions = split( /:/, "$opt_e" );

$RecurseDir::print_cur_dir = 1;
$RecurseDir::pass_rec_func_cur_file = 1;



recurseDir( $working_dir );

exit 0;

sub recurseFunc
{
   my $filename = shift;

   return unless checkName("$filename");

   if ( $filename =~ m/vj(.*?\..*)$/is)
   {
      print "mv $filename $1\n";
      system("mv $filename $1");
   }
}

sub checkExt( $ ) 
{
   my $filename = shift;

   foreach (@extensions)
   {
      if ( $filename =~ /\.$_$/ )
      {
         return 1 ;
      }
      
   }
   return 0;
}

sub checkName( $ ) 
{
   my $filename = shift;

   if (!checkExt( $filename ))
   {
      return 0 
   }

   return 1 if $filename =~ /vj.+?\..*/;
   return 0;
}
