#!/usr/local/bin/perl
#
# $Id$

#
#
# Expected format:
# 
# COMMAND
# username@email.com, username2@email.com
# <the rest is commands>

require 5.003;	# to be sure.  log_accum needs perl5

use File::Find ();
use strict qw(vars);
use vars qw($cwd);

BEGIN {
    require Cwd;
    $cwd = Cwd::cwd();
}

# for the convenience of &wanted calls, including -eval statements:
use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;

my $CVSROOT = $ENV{'CVSROOT'} || "/cvsroot/vrjuggler";

sub print_exit_message {
    print "** NOTE: Add entry to ChangeLog for major changes **\n";
}

sub cleanup_lockfiles {
    print "Cleaing up garbage CVS locks in $CVSROOT...\n";
    File::Find::find({wanted => \&wanted}, "$CVSROOT");
}

sub wanted {
    my ($dev,$ino,$mode,$nlink,$uid,$gid);

    /^#cvs.*\z/s &&
    (
        (($dev,$ino,$mode,$nlink,$uid,$gid) = lstat($_)) &&
        -f _
        ||
        -d _
    ) &&
    ($uid == $<) &&
    handle();
}

sub handle {
    if ( -d "$name" ) {
        doexec(0, 'rmdir', '{}');
    }
    else {
        unlink("$name") || warn "$name: $!\n";
    }
}

sub doexec {
    my $ok = shift;
    my(@cmd) = @_;
    for my $word (@cmd)
        { $word =~ s#{}#$name#g }
    if ($ok) {
        my $old = select(STDOUT);
        $| = 1;
        print "@cmd";
        select($old);
        return 0 unless <STDIN> =~ /^y/;
    }
    chdir $cwd; #sigh
    print "@cmd\n";
    system @cmd;
    chdir $File::Find::dir;
    return !$?;
}

my $MAILCMD = "/usr/lib/sendmail -odb -oem -t";

open(CMDFILE, "$ARGV[0]") or die "Could not read $ARGV[0]: $!\n";

my $junk = <CMDFILE>;

chomp $junk;

if ( $junk eq "COMMAND" )
{
   print "We have a command in cmd.verify.pl\n";

   my $addrs = <CMDFILE>;

   open(MAIL, "| $MAILCMD") or die "Please check $MAILCMD.\n";
   print MAIL "To: $addrs\n";
   print MAIL "Subject: COMMANDS\n";

   my(@cmds) = <CMDFILE>;

   print MAIL "-" x 15, " Commands ", "-" x 15, "\n";

   foreach ( @cmds )
   {
      print MAIL "$_\n";
   }

   print MAIL "-" x 15, " Commands ", "-" x 15, "\n";

   my $cmd;
   foreach $cmd ( @cmds )
   {
      print MAIL `$cmd`;
   }

   close(MAIL);

   cleanup_lockfiles();

   exit 1;
}
else
{
   exit 0;
}
