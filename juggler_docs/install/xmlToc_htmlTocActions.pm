package xmlToc_htmlTocActions;
@ISA = qw(Exporter);

require 5.003;
require Exporter;

##################################################
# How to use this package:
# .i.e: 
# use xmlToc;
# use xmlToc_htmlTocActions;
# xmlToc_htmlTocActions::useme();
# xmlToc::load( \$xmldata, "filename.xml" );
# xmlToc::traverse( \$htmldata, $xmldata );
# print $htmldata;
##################################################

@EXPORT = qw(useme pushFolder_action popFolder_action pushFont_action popFont_action item_action defaults_action treecontrol_action);

sub useme()
{
   xmlToc::setAction( "folder", \&xmlToc_htmlTocActions::pushFolder_action );
   xmlToc::setAction( "/folder", \&xmlToc_htmlTocActions::popFolder_action );
   xmlToc::setAction( "item", \&xmlToc_htmlTocActions::item_action );
   xmlToc::setAction( "font", \&xmlToc_htmlTocActions::pushFont_action );
   xmlToc::setAction( "/font", \&xmlToc_htmlTocActions::popFont_action );
   xmlToc::setAction( "idefault", \&xmlToc_htmlTocActions::defaults_action );
   xmlToc::setAction( "treecontrol", \&xmlToc_htmlTocActions::treecontrol_action );
}

sub pushFolder_action($$$$$)
{
   my $xmlToc_data_out = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   my $indent_level = shift;
   
   $myParams =~ m/title[ ]*=[ ]*"(.*?)"/s;
   my $title = $1;


      outputIndents( $xmlToc_data_out, $indent_level);
      if ($whatAmI =~ m/rootfolder/)
      {
         $$xmlToc_data_out .= "<li><b>$title</b></li>\n";
      }
      else
      {
         $$xmlToc_data_out .= "<li>$title</li>\n";
      }
   
      outputIndents( $xmlToc_data_out, $indent_level);
      $$xmlToc_data_out .= "<ul>\n";
} 
sub popFolder_action($$$$$)
{
   my $xmlToc_data_out = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   my $indent_level = shift;

   outputIndents( $xmlToc_data_out, $indent_level);
   $$xmlToc_data_out .= "</ul>\n";
}
sub pushFont_action($$$$$)
{
   my $xmlToc_data_out = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   my $indent_level = shift;

   $myParams =~ s/size[ ]*=[ ]*".*?"//s;

   outputIndents( $xmlToc_data_out, $indent_level);
   $$xmlToc_data_out .= "<font $myParams>\n";
}
sub popFont_action($$$$$)
{
   my $xmlToc_data_out = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   my $indent_level = shift;
   outputIndents( $xmlToc_data_out, $indent_level);
   $$xmlToc_data_out .= "</font>\n";
}
sub item_action($$$$$)
{
   my $xmlToc_data_out = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   my $indent_level = shift;
   
   $myParams =~ m/title[ ]*=[ ]*"(.*?)"/s;
   my $title = $1;

   my $link = '';

   if ( $myParams =~ /link[ ]*=[ ]*"(.*?)"/s ) {
      $link = "$1";
   }

   if ($myParams =~ m/jit[ ]*=[ ]*"(.*?)"/s)
   {
      my $jitfolder = "";
      xmlToc::load( \$jitfolder, $1 );
      xmlToc::traverse( $xmlToc_data_out, $jitfolder );
   }

   else
   {
      outputIndents( $xmlToc_data_out, $indent_level);

      if ( "$link" eq "" ) {
         $$xmlToc_data_out .= "<li>$title</li>\n";
      }
      else {
         $$xmlToc_data_out .= "<li><a href=\"$link\">$title</a></li>\n";
      }
   }
}

sub defaults_action($$$$$)
{
   my $xmlToc_data_out = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   my $indent_level = shift;
}

sub treecontrol_action($$$$$)
{
   my $xmlToc_data_out = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   my $indent_level = shift;
}







# helper func for outputting indentations 
# (takes an integer for num of indentations)
sub outputIndents($$)
{
   my $output_ref = shift; #reference to data
   my $indent_level = shift;
   my $x = 0;
      
   while ($x < $indent_level)
   {
      $x = $x + 1;
      $$output_ref .= "   "; # 3 spaces == one tab
   }
}
