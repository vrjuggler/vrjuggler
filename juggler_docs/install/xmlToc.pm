package xmlToc;

require 5.003;

use strict qw(vars);


##############################################################################
# NOTE: see the "Interface to xmlToc" section below
##############################################################################



# ============================================================================
# "Private" methods.
# ============================================================================

#####################################################
# Helper functions for internal use (not exported)

# output indents to STDOUT
sub outputIndent($)
{
   my $tabs = shift;
   my $x;
   for ( $x = 0; $x < $tabs; $x++ )
   {
      print "   "; # 3 spaces == one tab
   }
}

# increase indent by one
sub indentLevel($)
{
   my $this = shift;

#   outputIndent($this->{'indent_level'});
#   print "===>\n";
   $this->{'indent_level'}++;
}

# reduce indent by one
sub outdentLevel($)
{
   my $this = shift;

#   outputIndent($this->{'indent_level'});
#   print "<===\n";
   $this->{'indent_level'}--;
}

# default function if you don't set any of your own
sub unknown_action($$$$$)
{
   my $this = shift; # reference to the object
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;

#   $this->outputIndent();
   print "\n(xmlToc) unknown tag = $whatAmI\n";
#   $this->outputIndent();
   print "               content = $myParams\n";
}

# Execute the action associated with the tag type
# args: data_out              (reference to string, this is what this function returns)
#       tag type              (string, will be "item, folder, font, etc...")
#       tag params            (string, you will need to parse this for any tag params you're interested in)
#       any_content_inbetween (string, content appearing after the tag if any)
sub exec_action($$$$)
{
   # pick up the 4 arguments to this function...
   my $this = shift; # reference to the object
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;

   my $still_need_indent = 0;

   # pop:
   if ($whatAmI =~ m/\/rootfolder|\/folder/i)
   {
      $this->outdentLevel();
      $this->popFolder($whatAmI, $myParams, $any_content_inbetween);
   }
   elsif ($whatAmI =~ m/\/item|\/jitfolder/i)
   {
      # no item pop func yet...
#      $this->item($whatAmI, $myParams, $any_content_inbetween);
      print "WEIRD: there shouldn't be an </item> tag\n";
   }
   elsif ($whatAmI =~ m/\/font/i)
   {
      $this->popFont($whatAmI, $myParams, $any_content_inbetween);
   }
   elsif ($whatAmI =~ m/\/idefault/i)
   {
      $this->defaults($whatAmI, $myParams, $any_content_inbetween);
   }
   elsif ($whatAmI =~ m/\/treecontrol/i)
   {
      $this->treecontrol($whatAmI, $myParams, $any_content_inbetween);
   }
   
   # push
   elsif ($whatAmI =~ m/item|jitfolder/i)
   {
      $this->item($whatAmI, $myParams, $any_content_inbetween);
   }
   elsif ($whatAmI =~ m/rootfolder|folder/i)
   {
      $still_need_indent = 1;
      $this->pushFolder($whatAmI, $myParams, $any_content_inbetween);
   }
   elsif ($whatAmI =~ m/font/i)
   {
      $this->pushFont($whatAmI, $myParams, $any_content_inbetween);
   }
   elsif ($whatAmI =~ m/idefault/i)
   {
      $this->defaults($whatAmI, $myParams, $any_content_inbetween);
   }
   elsif ($whatAmI =~ m/treecontrol/i)
   {
      $this->treecontrol($whatAmI, $myParams, $any_content_inbetween);
   }
   else
   {
      $this->unknown_action($whatAmI, $myParams, $any_content_inbetween);
   }

   if ($still_need_indent == 1)
   {
      $this->indentLevel();
   }
}

# End of internal use helper functions
#####################################################


# ============================================================================
# "Public" methods.
# ============================================================================

#######################################################
# Interface to xmlToc:

# args: this (object reference, returns loaded data here)
#       filename (string, name of xmlToc formatted file) [optional]
sub load($;$)
{
   my $this = shift;

   if ( @_ ) {
       $this->{'filename'} = shift;
   }

   if (open(INPUT, "$this->{'filename'}"))
   {
    # gather each line of the source file into one string so we can do regexs across multiple lines.
    #print "Reading: $inputFile...\n";
    while (<INPUT>)
    {
       $this->{'content'} .= $_;
    }
    close(INPUT);

    # eliminate newlines
    $this->{'content'} =~ s/[\n]//gis;
    
    # success
    return 1;
   }

   else
   {
     warn "WARNING: Could not open $this->{'filename'} for reading: $!\n";
     return -1;
   }
}

sub setContent ($$)
{
   my $this = shift;

   $this->{'content'} = shift;
}

# traverse the TOC stored in the object passed in parameter 1
#
# for each entry in the toc, the appropriate callback function will be called.
# if you don't register a callback, your entries should appear as undefined 
# at STDOUT
sub traverse($)
{
   my $this = shift;

   my $any_content_inbetween = '';

   # Process the xml data in the 'content' field of the object.  The resulting
   # output is stored in the 'body' field.
   while ($this->{'content'} =~ m/(<.*?>)/s)
   {
      my $params = $1;
      $this->{'content'} = $';

      # pick up any between-tag cruft, might as well pass it to the user... :)
      if ($this->{'content'} =~ m/[^<^>]?(.*?)[^<]/s)
      {
          $any_content_inbetween = $1;
#         $this->{'content'} = $';
      }

      if ($params =~ m/<[ ]*(\/rootfolder|\/folder|\/font|\/idefault|\/treecontrol)[ ]*>/s)
      {
         $this->exec_action( $1, "", $any_content_inbetween );
      }

      elsif ($params =~ m/<[ ]*(rootfolder|folder|item|font|idefault|treecontrol|jitfolder)(.*?)>/s)
      {
         $this->exec_action( $1, $2, $any_content_inbetween );        
      }

      elsif ($params =~ m/<(.*?)>/s)
      {
         $this->exec_action( "unknown", $1, $any_content_inbetween );
      }
   } # while
}

sub getData ($)
{
   my $this = shift;

   return $this->{'body'};
}

# end of xmlToc module interface
#######################################################################

# ============================================================================
# "Protected" methods.
# ============================================================================

sub allocate ($;$)
{
   my $class    = shift;
   my $filename = shift || '';

   return bless {
      'filename'     => "$filename",    # XML table of contents file
      'content'      => '',             # Original content of XML TOC file
      'body'         => '',             # Result of processing content
      'indent_level' => 0               # Current indentation level
   }, $class;
}

# "Pure virutal" methods.
sub pushFolder($$$$);
sub popFolder($$$$);
sub pushFont($$$$);
sub popFont($$$$);
sub item($$$$);
sub defaults($$$$);
sub treecontrol($$$$);

# helper func for outputting indentations
sub outputIndents ($)
{
   my $this = shift; #reference to object

   my $x;
   for ( $x = 0; $x < $this->{'indent_level'}; $x++ ) {
      $this->{'body'} .= "   "; # 3 spaces == one tab
   }
}

1;
