package HtmlToc;

require 5.003;

use strict qw(vars);
use vars qw(@ISA);

@ISA = qw(xmlToc);


##################################################
# How to use this package:
#
# use xmlToc;
# use HtmlToc;
# $html_toc = new HtmlToc( "filename.xml" );
# $html_toc->load();
# $html_toc->traverse();
# print $html_toc->getData();
##################################################

# ============================================================================
# "Public" methods.
# ============================================================================

# ----------------------------------------------------------------------------
# Constructor.
# ----------------------------------------------------------------------------
sub new ($;$) {
   my $class    = shift;
   my $filename = shift || '';

   return $class->allocate("$filename");
}

# ----------------------------------------------------------------------------
# Destructor.
# ----------------------------------------------------------------------------
sub DESTROY ($) {
   my $this = shift;

   return 1;
}

# ============================================================================
# "Protected" methods.
# ============================================================================

sub pushFolder($$$$)
{
   my $this = shift; # reference to the object
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   
   $myParams =~ m/title[ ]*=[ ]*"(.*?)"/s;
   my $title = $1;


      $this->outputIndents();
      if ($whatAmI =~ m/rootfolder/)
      {
         $this->{'body'} .= "<li><b>$title</b></li>\n";
      }
      else
      {
         $this->{'body'} .= "<li>$title</li>\n";
      }
   
      $this->outputIndents();
      $this->{'body'} .= "<ul>\n";
} 
sub popFolder($$$$)
{
   my $this = shift; # reference to the object
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;

   $this->outputIndents();
   $this->{'body'} .= "</ul>\n";
}
sub pushFont($$$$$)
{
   my $this = shift; # reference to the object
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;

   $myParams =~ s/size[ ]*=[ ]*".*?"//s;

   $this->outputIndents();
   $this->{'body'} .= "<font $myParams>\n";
}
sub popFont($$$$)
{
   my $this = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;

   $this->outputIndents();
   $this->{'body'} .= "</font>\n";
}
sub item($$$$)
{
   my $this = shift; # reference to the object
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   
   $myParams =~ m/title[ ]*=[ ]*"(.*?)"/s;
   my $title = $1;

   my $link = '';

   if ( $myParams =~ /link[ ]*=[ ]*"(.*?)"/s ) {
      $link = "$1";
   }

   if ($myParams =~ m/jit[ ]*=[ ]*"(.*?)"/s)
   {
      my $jitfolder = new HtmlToc("$1");
      $jitfolder->load();
      $jitfolder->traverse();
      $this->{'body'} .= $jitfolder->getData();
   }

   else
   {
      $this->outputIndents();

      if ( "$link" eq "" ) {
         $this->{'body'} .= "<li>$title</li>\n";
      }
      else {
         $this->{'body'} .= "<li><a href=\"$link\">$title</a></li>\n";
      }
   }
}

sub defaults($$$$)
{
   my $this = shift; # reference to the object
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
}

sub treecontrol($$$$)
{
   my $this = shift; # reference to the object
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
}

1;
