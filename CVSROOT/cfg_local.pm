# $FreeBSD: CVSROOT/cfg_local.pm,v 1.11 2001/11/30 14:26:57 joe Exp $

####################################################################
####################################################################
# This file contains local configuration for the CVSROOT perl
# scripts.  It is loaded by cfg.pm and overrides the default
# configuration in that file.
#
# It is advised that you test it with
#     'env CVSROOT=/path/to/cvsroot perl -cw cfg.pm'
# before you commit any changes.  The check is to cfg.pm which
# loads this file.
####################################################################
####################################################################

%TEMPLATE_HEADERS = (
	"Reviewed by"		=> '.*',
	"Submitted by"		=> '.*',
	"Obtained from"		=> '.*',
	"Approved by"		=> '.*',
	"SF Bug#"		=> '\d+',
);

@LOG_FILE_MAP = ();

$MAILCMD = "/usr/lib/sendmail -odb -oem";
$MAIL_BRANCH_HDR = "X-VRJuggler-CVS-Branch:";
$ADD_TO_LINE = 1;

$MAILADDRS = 'vrjuggler-checkins@lists.sourceforge.net';

@COMMIT_HOSTS = ();

$MAIL_TRANSFORM = sub {
   add_viewcvs_entry("http://cvs.sourceforge.net/cgi-bin/viewcvs.cgi/vrjuggler",
                     '', @_);
};

sub add_viewcvs_entry
{
   my $cgi_url   = shift;
   my $www_repos = shift;
   my @input     = @_;
   my @revs      = ();
   my @output    = ();

   while ( 1 )
   {
      my $line = shift(@input);
      last unless defined($line);

      push(@output, "$line");
      last if $line =~ /^\s*Revision\s*Changes\s*Path\s*$/;
   }

   foreach ( @input )
   {
      push(@output, "$_");
      push(@revs, "$_");
   }

   push(@output, '');

   my $skip = 0;
   foreach ( @revs )
   {
      # The revision block is terminated with an empty line.
      $skip = 1 if $_ =~ /^\s*$/;

      next if $skip;

      my ($rev, $add, $sub, $file, $status) = split;

      $rev =~ /(?:(.*)\.)?([^\.]+)\.([^\.]+)$/;
      my ($base, $r1, $r2) = ($1, $2, $3);
      my $prevrev = "";
      if ( $r2 == 1 )
      {
         $prevrev = $base;
      }
      else
      {
         $prevrev = "$base." if $base;
         $prevrev .= "$r1." . ($r2 - 1);
      }

      my $baseurl = "$cgi_url/$file";
      my $extra;

      if ( defined($status) )
      {
         $rev   = $prevref if $status =~ /dead/;
         $extra = "?rev=$rev&content-type=text/plain";
      }
      else
      {
         $extra = "?r1=$prevrev&r2=$rev";
      }

      push(@output, "$baseurl$extra$www_repos");
   }

   return @output;
}

1; # Perl requires all modules to return true.  Don't delete!!!!
#end
