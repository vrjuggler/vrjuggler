package xmlToc;
@ISA = qw(Exporter);

require 5.003;
require Exporter;


##############################################################################
# NOTE: see the "Interface to xmlToc" section below
##############################################################################




#####################################################
# Helper functions for internal use (not exported)

# output indents to STDOUT
sub outputIndent($)
{
   my $tabs = shift;
   my $x = 0;
   while ($x < $tabs)
   {
      $x = $x + 1;
      print "   "; # 3 spaces == one tab
   }
}

# current indentation level
my $indent_level=0;

# increase indent by one
sub indentLevel()
{
   #outputIndent($indent_level);
#   print "===>\n";
   $indent_level = $indent_level + 1;
}

# reduce indent by one
sub outdentLevel()
{
   #outputIndent($indent_level);
#   print "<===\n";
   $indent_level = $indent_level - 1;
}

# default function if you don't set any of your own
sub unknown_action($$$$$)
{
   my $xmlToc_data_out = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   my $tabs = shift;
   
   #outputIndent( $tabs );
   print "\n(xmlToc) unknown tag = $whatAmI\n";
   #outputIndent( $tabs );
   print "               content = $myParams\n";
}

# function pointers, initialized to unknown_action
#my $caller = caller();
#my $treecontrol_action = \&{"${caller}::treecontrol_action"};
my $pushFolder_action = \&xmlToc::unknown_action;
my $popFolder_action = \&xmlToc::unknown_action;
my $pushFont_action = \&xmlToc::unknown_action;
my $popFont_action = \&xmlToc::unknown_action;
my $item_action = \&xmlToc::unknown_action;
my $defaults_action = \&xmlToc::unknown_action;
my $treecontrol_action = \&xmlToc::unknown_action;

# Execute the action associated with the tag type
# args: data_out              (reference to string, this is what this function returns)
#       tag type              (string, will be "item, folder, font, etc...")
#       tag params            (string, you will need to parse this for any tag params you're interested in)
#       any_content_inbetween (string, content appearing after the tag if any)
sub exec_action($$$$)
{
   # pick up the 4 arguments to this function...
   my $xmlToc_data_out = shift; # reference to the data
   my $whatAmI = shift;
   my $myParams = shift;
   my $any_content_inbetween = shift;
   
   my $still_need_indent = 0;
   my $callback_action = \&xmlToc::unknown_action;
   
   # pop:
   if ($whatAmI =~ m/\/rootfolder|\/folder/i)
   {
      outdentLevel();
      $callback_action = $popFolder_action;
   }
   elsif ($whatAmI =~ m/\/item|\/jitfolder/i)
   {
      # no item pop func yet...
      #$callback_action = $item_action;
      print "WEIRD: there shouldn't be an </item> tag\n";
   }
   elsif ($whatAmI =~ m/\/font/i)
   {
      $callback_action = $popFont_action;
   }
   elsif ($whatAmI =~ m/\/idefault/i)
   {
      $callback_action = $defaults_action;
   }
   elsif ($whatAmI =~ m/\/treecontrol/i)
   {
      $callback_action = $treecontrol_action;
   }
   
   # push
   elsif ($whatAmI =~ m/item|jitfolder/i)
   {
      $callback_action = $item_action;
   }
   elsif ($whatAmI =~ m/rootfolder|folder/i)
   {
      $still_need_indent = 1;
      $callback_action = $pushFolder_action;
   }
   elsif ($whatAmI =~ m/font/i)
   {
      $callback_action = $pushFont_action;
   }
   elsif ($whatAmI =~ m/idefault/i)
   {
      $callback_action = $defaults_action;
   }
   elsif ($whatAmI =~ m/treecontrol/i)
   {
      $callback_action = $treecontrol_action;
   }

   &$callback_action( \$$xmlToc_data_out, $whatAmI, $myParams, $any_content_inbetween, $indent_level);

   if ($still_need_indent == 1)
   {
      indentLevel();
   }
}

# End of internal use helper functions
#####################################################



#######################################################
# Interface to xmlToc:
@EXPORT = qw(traverse setAction load);

# args: data_out (string reference, returns loaded data here)
#       filename (string, name of xmlToc formatted file)
sub load($$)
{
   my $xmlTocData = shift;
   my $inputFile = shift;

   if (open(INPUT, "$inputFile"))
   {
    # gather each line of the source file into one string so we can do regexs across multiple lines.
    #print "Reading: $inputFile...\n";
    while (<INPUT>)
    {
       $$xmlTocData .= $_;
    }
    close(INPUT);

    # eliminate newlines
    $$xmlTocData =~ s/[\n]//gis;
    
    # success
    return 1;
   }

   else
   {
     warn "WARNING: Could not open $inputFile for reading: $!\n";
     return -1;
   }
}

# Set your call-backs with these "set" functions
sub setAction($$)
{
   my $whatAmI = shift;
   my $action = shift;
   
   #pop (TODO: use a hash to make this more simple/generic)
   if ($whatAmI =~ m/\/rootfolder|\/folder/i)
   {
      $popFolder_action = $action;
   }
   elsif ($whatAmI =~ m/\/item/i)
   {
      #$item_action = $action;
   }
   elsif ($whatAmI =~ m/\/font/i)
   {
      $popFont_action = $action;
   }
   elsif ($whatAmI =~ m/\/idefault/i)
   {
      #$defaults_action = $action;
   }
   elsif ($whatAmI =~ m/\/treecontrol/i)
   {
      #$treecontrol_action = $action;
   }
   
   #push (TODO: use a hash to make this more simple/generic)
   elsif ($whatAmI =~ m/rootfolder|folder/i)
   {
      $pushFolder_action = $action;
   }
   elsif ($whatAmI =~ m/item/i)
   {
      $item_action = $action;
   }
   elsif ($whatAmI =~ m/font/i)
   {
      $pushFont_action = $action;
   }
   elsif ($whatAmI =~ m/idefault/i)
   {
      $defaults_action = $action;
   }
   elsif ($whatAmI =~ m/treecontrol/i)
   {
      $treecontrol_action = $action;
   }
   
   
   
   # assert if wrong args are given to this function
   else
   {
      warn "ASSERT: Invalid action type: $whatAmI\n";
      exit( 0 );
   }
}

# traverse the TOC stored in the data passed as parameter 2
# return the processed TOC in parameter 1
#
# for each entry in the toc, the appropriate callback function will be called.
# if you don't register a callback, your entries should appear as undefined 
# at STDOUT
sub traverse($$)
{
   my $data_out = shift; # reference to the data
   my $xmlTocData_in = shift;  # copy of the data


   # process the xml data
   while ($xmlTocData_in =~ m/(<.*?>)/s)
   {
      my $params = $1;
      $xmlTocData_in = $';

      # pick up any between-tag cruft, might as well pass it to the user... :)
      if ($xmlTocData_in =~ m/[^<^>]?(.*?)[^<]/s)
      {
          $any_content_inbetween = $1;
#         $xmlTocData_in = $';
      }

      if ($params =~ m/<[ ]*(\/rootfolder|\/folder|\/font|\/idefault|\/treecontrol)[ ]*>/s)
      {
         exec_action( \$$data_out, $1, "", $any_content_inbetween );
      }

      elsif ($params =~ m/<[ ]*(rootfolder|folder|item|font|idefault|treecontrol|jitfolder)(.*?)>/s)
      {
         exec_action( \$$data_out, $1, $2, $any_content_inbetween );        
      }

      elsif ($params =~ m/<(.*?)>/s)
      {
         exec_action( \$$data_out, "unknown", $1, $any_content_inbetween );
      }
   } # while
}
# end of xmlToc module interface
#######################################################################

1;
