#!/usr/bin/perl -w

# Doozer++ is (C) Copyright 2000-2009 by Iowa State University
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)
#
# Original Author:
#   Patrick Hartling

use 5.004;

use Cwd;
use File::Basename;
use File::Copy;
use File::Path;
use Getopt::Std;

use strict 'vars';
use vars qw(%opts);

sub installFile($$);
sub installFiles($$);

my $Win32 = 1 if exists($ENV{'OS'}) && $ENV{'OS'} =~ /Windows/;

# -b: Make backup before overwriting
# -B suffix: Use suffix as the backup suffix if -b is given.
# -C: Copy the file plus some magic
# -c: Copy the file--backwards compatibility only.
# -d: Create directories if parent directories are missing.
# -f: Specify the target's file flags (chflags behavior).
# -g: Specify a group.
# -L: Preserve symlinks when installing.
# -l: Use symlinks to the source file(s) instead of copy(ies).
# -M: Disable all use of mmap(2).
# -m: Specify an alternate mode (default is 0755).
# -o: Specify an owner.
# -p: Preserve the modification time.
# -S: Use "safe copy" where a temporary file is created and then renamed.
# -s: Strip binaries.
# -v: Be verbose.
getopts('bB:Ccdf:g:LlMm:o:pSsv', \%opts);

my $backup_suffix = (exists($opts{'B'}) ? "$opts{'B'}" : '.old');
my $file_mode     = (exists($opts{'m'}) ? $opts{'m'} : '0755');

warn "WARNING: -f option is not portable" if $opts{'f'};

my ($OWNER, $GROUP) = (0, 0);

if ( ! $Win32 )
{
   my $uname       = getpwuid($<);
   my (@user_info) = getpwnam("$uname");

   if ( $opts{'o'} )
   {
      if ( $opts{'o'} =~ /\w/ )
      {
         $OWNER = getpwnam("$opts{'o'}");
      }
      else
      {
         $OWNER = "$opts{'o'}";
      }
   }
   else
   {
      $OWNER = $user_info[2];
   }

   if ( $opts{'g'} )
   {
      if ( $opts{'g'} =~ /\w/ )
      {
         $GROUP = getgrnam("$opts{'g'}");
      }
      else
      {
         $GROUP = "$opts{'g'}";
      }
   }
   else
   {
      $GROUP = $user_info[3];
   }
}

# If we are using the two-file format, we have a source file and a destination
# file.
if ( $#ARGV == 1 && ! -d "$ARGV[1]" )
{
   installFile("$ARGV[0]", "$ARGV[1]");
}
else
{
   my @files    = @ARGV;
   my $dest_dir = pop(@files);

   installFiles(\@files, $dest_dir);
}

exit(0);

sub installFile($$)
{
   my $source = shift;
   my $dest   = shift;

   my $dest_dir = dirname("$dest");

   # Make the parent directories if -d was given and $dest_dir does not exist.
   if ( $opts{'d'} && ! -d "$dest_dir" )
   {
      mkpath("$dest_dir", 0, 0755)
         or warn "WARNING: Could not create directory tree $dest_dir: $!\n";
   }

   # Make the backup if necessary.
   if ( $opts{'b'} && -e "$dest" )
   {
      rename("$dest", "$dest$backup_suffix")
         or warn "WARNING: Could not make backup of $dest: $!\n";
   }

   if ( $opts{'l'} && ! $Win32 )
   {
      if ( $source !~ /^\// )
      {
         my $cur_dir = getcwd();
         $source     = "$cur_dir/$source";
      }

      unlink("$dest") if -e "$dest" or -l "$dest";
      symlink("$source", "$dest")
         or warn "WARNING: Failed to make symlink from $source to $dest: $!\n";
   }
   else
   {
      my $dest_name = (exists($opts{'S'}) ? "$dest.$$" : "$dest");

      if ( ! $Win32 && $opts{'L'} && -l "$source" )
      {
         my $pointed_file = basename(readlink("$source"));
         symlink("$pointed_file", "$dest_name");
      }
      elsif ( copy("$source", "$dest_name") )
      {
         # Finish the safe copy.
         if ( $opts{'S'} )
         {
            unlink("$dest") if -e "$dest";

            if ( !  rename("$dest_name", "$dest") )
            {
               warn "WARNING: Failed to rename $dest_name to $dest: $!\n";
               unlink("$dest_name");
               return;
            }
         }

         chmod(oct($file_mode), "$dest")
            or warn "WARNING: Failed to set mode on $dest: $!\n";

         if ( ! $Win32 )
         {
            chown($OWNER, $GROUP, "$dest")
               or warn "WARNING: Failed to set ownership on $dest: $!\n";
         }
      }
      else
      {
         warn "WARNING: Failed to copy $source to $dest: $!\n";
      }
   }
}

sub installFiles($$)
{
   my $files_ref = shift;
   my $dest_dir  = shift;

   foreach my $full_file ( @$files_ref )
   {
      my $file_name = basename("$full_file");
      print "$full_file --> $dest_dir/$file_name\n" if $opts{'v'};
      installFile("$full_file", "$dest_dir/$file_name");
   }
}
