#!/usr/local/bin/perl

# -----------------------------------------------------------------------------
# Install an entire directory from the VR Juggler development tree without
# the CVS directories.  All files in the directory tree are copied to the
# destination directory.
#
# Usage:
#     install-dir.pl -s <source directory> -t <destination directory>
#                   [-u <user name> -g <group name> -m <mode>]
#                    -i <dir ignore>,<dir ignore>
#                    -c <css header> -e <html header> -f <html footer>
#                    -h <HTML install path> -w <web subst file>
#                    -a (force all)
#                    -z generate Zscript
#
#
#     <source directory> - The source directory to be installed
#     <destination directory> - The location to install the source directory
#     <HTML install path> - A prefix for HTML info. ex: "www.vrjugger.org/"
# -----------------------------------------------------------------------------
require 5.004;

use File::Basename;
use File::Path;
use File::Copy;
use Getopt::Std;
use File::stat;
#use strict;

use xmlToc;
use xmlToc_htmlTocActions;

# Do this to include the path to the script in @INC.
BEGIN {
    $path = (fileparse("$0"))[1];
}

use lib("$path");
use InstallOps;

# Ensure that there are four command-line arguments.  If not, exit with
# error status.
if ( $#ARGV < 3 ) {
    warn "Usage: $0 -s <source directory> -t <target directory> -i<ignore dir>,<ignore dir>,.. -c <css header> -e <html header> -f <html footer> -h <HTML install path> -w <subst file> -a (force all) -z <name of script file to output>\n";
    exit 1;
}

# Get the -i and -o options and store their values in $opt_i and $opt_o
# respectively.
getopt('s:t:i:c:e:f:w:h:z:');

my $src_dir = "$opt_s";
my $dest_dir = "$opt_t";
my $css_filename = "$opt_c";
my $html_header_filename = "$opt_e";
my $html_footer_filename = "$opt_f";
my $html_install_prefix = "$opt_h";
my $subst_file = "$opt_w";
my $levels_of_recursion_allowed = 4; # you may have to change this ...

my $html_comment_begin = "<!-- ";
my $html_comment_end = " -->";

my $usedebugoutput = "0";
   
my $check_file_mtime = 1;     # should we check file modification time in recurseAction?
if ($opt_a == 1)
{
   $check_file_mtime = 0;
   print "--- FORCING ALL FILES TO BE PROCESSED ---\n";
}

## ---- Output list of options used ---- ##
print "---------- WebInstall Options -----------\n";
print "         Source dir: $src_dir\n";
print "           Dest dir: $dest_dir\n";
print "           CSS file: $css_filename\n";
print "        HTML Header: $html_header_filename\n";
print "        HTML footer: $html_footer_filename\n";
print "HTML install prefix: $html_install_prefix\n";
print "     Web Subst file: $subst_file\n\n";

create_script("$opt_z");

# List of directories to skip
my @skip_dirs = split /[,]/ , $opt_i;

# Remove lead/trail whitespace
foreach (@skip_dirs)
{
    s/^\s+//;
    s/\s+$//;
}

#
# Set defaults for the variables
#
my($uid, $gid, $mode) = ($<, (getpwuid($<))[3], "0644");

if ( $opt_u ) {
    $uname = "$opt_u" if $opt_u;
    $uid = (getpwnam("$uname"))[2] or die "getpwnam($uname): $!\n";
}

if ( $opt_g ) {
    $gname = "$opt_g" if $opt_g;
    $gid = (getgrnam("$gname"))[2] or die "getgrnam($gname): $!\n";
}

$mode = "$opt_m" if $opt_m;

umask(002);
mkpath("$dest_dir", 0, 0755); # Make sure that $dest_dir exists

# Initialize Filters
initWebSubstTags();
initHtmlFilter($css_filename, $html_header_filename, $html_footer_filename);

# Save this for later
# Unless it stars with a / and ends with a /
chop($root_inst_dir = `pwd`) unless $dest_dir =~ /^\//;
chop($root_src_dir = `pwd`) unless $src_dir =~ /^\//;

# Recurse through $src_dir and create the destination directory tree.
# recurseAction() handles further work.
recurseDir("$root_src_dir/$src_dir", "$root_inst_dir/$dest_dir", @skip_dirs);

exit 0;


#----------------------------------------------------------------------#
#                         Sub Routines                                 #
#----------------------------------------------------------------------#

$css_header="";
$html_header="";
$html_footer="";

# Fills the %path_subst hash with tags to use
# Fills the %alias_subst hash with tags to use
# for replacing tag entries in the config files.
sub initWebSubstTags()
{
   #$file = "someprog.pl";
   if ($subst_file)
   {
      unless ($return = do $subst_file) {
          warn "couldn't parse $subst_file: $@"         if $@;
          warn "couldn't do $subst_file: $!"            unless defined $return;
          warn "couldn't run $subst_file"               unless $return;
      }
   }

   print "----- path_subst hash values -------------\n";
   while ( ($k,$v) = each %path_subst ) {
       print "$k => $v\n";
   }
   print "-----------------------------------------\n";

   print "----- alias_subst hash values -------------\n";
   while ( ($k,$v) = each %alias_subst ) {
       print "$k => $v\n";
   }
   print "-----------------------------------------\n";
}

#  Initialize the HTML filter that we will use
#
# syntax:
#     initHtmlFilter($css_header_file_name, $header_file_name, $footer_file_name)
# args:
#     css_header_file_name: The name of the file with the css information to copy
#     header_file_name: The name of the file with the header information to copy
#     footer_file_name: The name of the file with the footer information to copy
#
sub initHtmlFilter($$)
{
   my $css_header_file_name = shift;
   my $header_file_name = shift;
   my $footer_file_name = shift;

   #### --- GET CSS HEADER --- ###
   if (!open(CSS_FILE, "$css_header_file_name") ) {
       warn "WARNING: Cannot read HTML CSS Data from $css_header_file_name: $!\n";
       warn "WARNING: Did you specify a file?????\n;";
       return -1;
   }

   while(<CSS_FILE>)
   {
      $css_header .= $_;
   }

   print "CSS Header: $css_header_file_name\n------- begin --------\n", $css_header, "\n------- end ----------\n";

   #### --- GET HTML HEADER --- ####
   if (!open(HTML_HEADER, "$header_file_name"))
   {
      warn "WARNING: Cannot read HTML Header info from $header_file_name: $!\n";
      warn "WARNING: Did you specify a file?????\n;";
      return -1;
   }

   while(<HTML_HEADER>)
   {
      $html_header .= $_;
   }

   print "HTML Header: $header_file_name\n------- begin --------\n", $html_header, "\n------- end ----------\n";

   #### --- GET HTML FOOTER --- ####
   if (!open(HTML_FOOTER, "$footer_file_name"))
   {
      warn "WARNING: Cannot read HTML Footer info from $footer_file_name: $!\n";
      warn "WARNING: Did you specify a file?????\n;";
      return -1;
   }

   while(<HTML_FOOTER>)
   {
      $html_footer .= $_;
   }

   print "HTML Footer: $footer_file_name\n------- begin --------\n", $html_footer, "\n------- end ----------\n";
}


#  Initialize the HTML filter that we will use
#
# syntax:
#     htmlFilter($filename)
# args:
#     file_contents: The contents of the file to run the filter on
#
#
sub htmlFilter($)
{
   outputDebug( "==> htmlFilter\n" );
   my $head_tag_begin = "<head>";
   my $head_tag_end = "<\/head>";
   my $html_tag_begin = "<html>";
   my $body_tag_end = "<\/body>";
   my $ignore_all_str = 'install-web ignore';
   my $ignore_tags_str = 'install-web no-tags';
   my $ignore_includes_str = 'install-web no-includes';
   my $ignore_css_str = 'install-web no-css';
   my $ignore_header_str = 'install-web no-header';
   my $ignore_footer_str = 'install-web no-footer';
   my $include_footer_str = 'install-web common-footer';
   
   my $file_contents = shift;
   
   ### Check for IGNORE ALL ####
   if ($$file_contents =~ m/$ignore_all_str/is)
   {
      print "ignoring file.\n";
      return 1;
   }

   
   
   ################# search and replace (tags and includes) ##################
   #
   if ($$file_contents =~ m/$ignore_includes_str/is)
   {
   }
   else
   { 
      processIncludesRecursive( $file_contents );
   }
   
   if ($$file_contents =~ m/$ignore_tags_str/is)
   {
   }
   else
   { 
      replaceTagsRecursive( $file_contents );
   }
   #
   ################# end of search and replace (tags and includes) ##################
   
   
   ########## begin custom html commands ###############
   #
   
   # ----------- HEADER INSERT ----------- #
   # Insert header
   # Look for <BODY> and put it right after that
   if ($$file_contents =~ m/$ignore_header_str/is)  ### Check for IGNORE HEADER ####
   {
      print "[ignoring header...]";
   }
   else
   {
      $$file_contents =~ s/($head_tag_end)/\n$html_header\n$1\n/is;
   }
   
   # ----------- CSS HEADER INSERT ----------- #
   # if there is </head> then insert before it
   # if not, then look for <html> and insert after it with <head>...</head> around it
   if ($$file_contents =~ m/$ignore_css_str/is)  ### Check for IGNORE HEADER ####
   {
      print "[ignoring stylesheet...]";
   }
   else
   {
      if ($$file_contents =~ s/($head_tag_end)/\n$css_header\n$1\n/is)
      {
      #print "added css (</head>)...";
      }
      elsif ($$file_contents =~ s/($html_tag_begin)/$1\n$head_tag_begin\n$css_header\n$head_tag_end\n/is)
      {
      #print "added css (<html>)...";
      }
      else
      {
      #print "did NOT add css...";
      }

      # ------ Edit the CSS link path ----- #
      # ASSERT: The path in the css file is relative to the
      #         root directory of the web install
      $$file_contents =~ s/<link(.*?)\"stylesheet\"(.*?)href(.*?)\"/$&$html_install_prefix/gis;
   }

   # ----------- FOOTER INSERT ----------- #
   # Insert footer
   # Look for </BODY> and put it right before that
   if ($$file_contents =~ m/$ignore_footer_str/is)  ### Check for IGNORE FOOTER ####
   {
      print "[ignoring footer...]";
   }
   else
   {
      if ($$file_contents =~ m/$html_comment_begin$include_footer_str$html_comment_end/is)
      {
         print "[Putting footer in custom place...]";
         $$file_contents =~ s/$html_comment_begin$include_footer_str$html_comment_end/\n$html_footer\n$1\n/is;
      }
      else
      {
         $$file_contents =~ s/($body_tag_end)/\n$html_footer\n$1\n/is;
      }
   }
   #
   ########## done, with custom html commands ###############
   
   
   ################# search and replace (tags and includes) ##################
   #
   if ($$file_contents =~ m/$ignore_includes_str/is)
   {
   }
   else
   { 
      processIncludesRecursive( $file_contents );
   }
   
   if ($$file_contents =~ m/$ignore_tags_str/is)
   {
   }
   else
   { 
      replaceTagsRecursive( $file_contents );
   }
   #
   ################# end of search and replace (tags and includes) ##################
}

sub outputDebug($)
{
   if ($usedebugoutput == 1)
   {
      my $outputdebug = shift;
      print $outputdebug;
   }
}

#  Initialize the HTML filter that we will use
#
# syntax:
#     xmlFilter($filecontents, $rel_path)
# args:
#     $filecontents (string reference): The contents of the file to run the filter on
#     $relpath (string): The relative path to the given file-contents from the "root" web dir
#
sub xmlFilter($$$)
{
   outputDebug( "==> xmlFilter\n" );
   my $file_contents = shift;
   my $rel_path = shift;
   my $nohttp = shift;
 
   processIncludesRecursive( $file_contents );

   # ---- PATH REPLACE ----
   # If it is not a tag, then do a simple replace
   # linkurl = "somewhere/something.html" ==> linkurl="webprefix/rel_path/somewhere/something.html"
   # NOTE: We don't want to replace links that have TAGS in them
   # NOTE2: This means that all other types of links (rlink|jit|link) must have paths relative to the urllink
   #       OR that use a ${TAG} format for the paths
   #$file_contents ~= s/link(.*?)=(.*?)\"(?!\$)/link=\"$html_install_prefix$rel_path\//gis;
   #$file_contents =~ s/(rlink|jit|link)(\s*?)=(\s*?)\"(?!\$)/$1=\"$html_install_prefix\/$rel_path\//gis;
   $$file_contents =~ s/(linkurl)(\s*?)=(\s*?)\"(?!\$)/$1=\"$html_install_prefix\/$rel_path\//gis;

   ## REPLACE TAGS
   replaceTagsRecursive( $file_contents, $nohttp );

   ## Replace white space
   processRemoveWhitespace( $file_contents );
   
   # causes jexplorer to go catatonic...
   #processRemoveNewlines( $file_contents );
   
   outputDebug( "<== xmlFilter\n" );
}

#  replaceTags Recursive version (see other for documentation)
#
sub replaceTagsRecursive($$)
{
   outputDebug( "==> replaceTagsRecursive\n" );
   
   my $contents_ref = shift;
   my $in_include = shift;
   
   # fix filename, if it contains a tag
   my $found_a_tag = 1;
   my $levels_of_recursion = 0;
   while( $found_a_tag == 1 )
   {
      $found_a_tag = replaceTags( $contents_ref, $in_include );
      
      # bail if too many recursions.
      $levels_of_recursion = $levels_of_recursion + 1;
      if ($levels_of_recursion > $levels_of_recursion_allowed)
      {
         ## bail!
         print "replaceTagsRecursive: too much recursion, bailing (you need to fix it or set the recursion level higher in install-web.pl...)\n";
         exit(0);
      }
   }
   
   outputDebug( "<== replaceTagsRecursive\n" );
}

# Replace the tags in the file contents passed to the subroutine
#
# Each tag in %path_subst is replaced with the tag value prefixed
# by the html_install_directory
# Each tag in %alias_subst is replace by the tag only
#
# returns whether a tag was found 0=false 1=true
sub replaceTags($$)
{
   outputDebug( "==> replaceTags\n" );
   my $contents_ref = shift;
   my $in_include = shift;
   my $found_a_tag = 0;
  
   # find subst tags and replace with <html_inst_dir>/<tag value>
   while (($tag,$tag_value) = each(%path_subst))
   {
      # see if there are any tags, if so note it, and replace them
      if ($$contents_ref =~ m/(\$)[{\(]$tag[}\)]/gis)
      {
         $found_a_tag = 1;
         
         #print "Checking web subst: $tag ==> $tag_value\n";
         # Look for ${tag} or $(tag)
         if ($in_include == 0)
         {
            $$contents_ref =~ s/(\$)[{(]$tag[})]/$html_install_prefix$tag_value/gis;
         }
         else
         {
            my $root_dir = "$root_src_dir/$src_dir/";
            $$contents_ref =~ s/(\$)[{(]$tag[})]/$root_dir$tag_value/gis;
         }
      }
   }

   # alias_subst replace
   while (($tag,$tag_value) = each(%alias_subst))
   {
      # see if there are any tags, if so note it, and replace them
      if ($$contents_ref =~ m/(\$)[{\(]$tag[}\)]/gis)
      {
         $found_a_tag = 1;
         $$contents_ref =~ s/(\$)[{(]$tag[})]/$tag_value/gis;
      }
   }
   
   outputDebug( "<== replaceTags\n" );
   return $found_a_tag;
}

# args: data_out (string reference, returns loaded data here)
#       filename (string, name of xmlToc formatted file)
sub load($$$$)
{
   outputDebug( "==> load\n" );
   my $data = shift;
   my $inputFile = shift;
   my $whocalledme = shift;
   my $use_feedback = shift;

   if (open(INPUT, "$inputFile"))
   {
      # gather each line of the source file into one string so we can do regexs across multiple lines.
      while (<INPUT>)
      {
         $$data .= $_;
         if ($use_feedback == 1)
         {
            print ".";
         }
      }
      close(INPUT) or warn "WARNING ($whocalledme): Cannot close $inputFile: $!\n";

      # success
      return 1;
   }

   else
   {
      warn "WARNING ($whocalledme): Could not open $inputFile for reading: $!\n";
      return -1;
   }
}


sub save($$)
{
   outputDebug( "==> save\n" );
   my $data = shift;
   my $filename = shift;

   # -----------  SAVE FILE ----------- #
   if ( ! open(XML_FILE, ">$filename") ) {
       warn "WARNING: Cannot create $filename: $!\n";
       return -1;
   }

   print XML_FILE $data;
   print XML_FILE $html_comment_begin, "Processed by install-web: ", scalar(localtime()), $html_comment_end;
   close(XML_FILE) or warn "WARNING: Cannot close $filename: $!\n";
   
   return 1;
}

# recursive-capable process includes (see other for documentation)
# arg: file_contents (reference to string)
sub processIncludesRecursive($)
{
   outputDebug( "==> processIncludesRecursive\n" );
   my $contents_ref = shift;
   
   ## Check includes ##
   my $found_an_include = 1;
   my $levels_of_recursion = 0;
   while( $found_an_include == 1 )
   {
      my $prev_name = $$contents_ref;
      $found_an_include = processIncludes( $contents_ref );
      #if ( $found_an_include == 1 )
      #{
      #   print "including file: changing $prev_name to $filename\n";
      #}
      
      # count number of times we recurse
      $levels_of_recursion = $levels_of_recursion + 1;
      if ($levels_of_recursion > $levels_of_recursion_allowed)
      {
         ## bail!
         print "processIncludesRecursive: too much recursion, bailing...\n";
         $found_an_include = 0;
      }
   }
}

# One process include
#
# Check for includes and do them
# returns whether or not includes did exist 0=false or 1=true
# arg: file_contents (reference to string)
sub processIncludes($)
{
   outputDebug( "==> processIncludes\n" );
   my $include_command = 'install-web include';
   my $include_toc_command = 'install-web include-toc';
   my $include_tocdocs_command = 'install-web include-tocdocs';
  
   my $contents_ref = shift;
   
   my $includes_existed = 0;
   
   # While we have some more includes to deal with
   while ($$contents_ref =~ m/($html_comment_begin[ ]*$include_command.*?\s*?(\S*?)\s*?$html_comment_end)/is)
   {
      my $include_statement = $1; # what was that match?
      my $orig_filename = $2;     # Get filename from the match
      
      # fix filename, if it contains tag(s)
      my $expandvars_filename = $orig_filename;
      replaceTagsRecursive( \$expandvars_filename, "1" );
      $expandvars_filename =~ s/"//g;
      
      # fix the $ char so the regex doesn't think its an end-line char.
      $include_statement =~ s/\$/\\\$/gs;  
      
      #print "\n\nMatched: incl: $include_statement\n";
      #print "Matched: orig: $orig_filename\n";
      #print "Matched: expd: $expandvars_filename\n\n";

      # ----------- TOC include ----------- #
      if ($include_statement =~ m/$include_toc_command/)
      {
         my $xmltoc_data = "";
         my $htmltoc_data = "";
         print "[including TOC: \"$expandvars_filename\" ";
         if (xmlToc::load( \$xmltoc_data, $expandvars_filename ))
         {
            xmlFilter( \$xmltoc_data, "", "1" );  # must filter the loaded xml file...
            xmlToc_htmlTocActions::useme();
            xmlToc::traverse( \$htmltoc_data, $xmltoc_data );
         }
         $$contents_ref =~ s/$include_statement/$htmltoc_data/gis;
         print "]";
         $includes_existed = 1;
      }
      
      # ----------- TOC-DOCS include ----------- #
      elsif ($expandvars_filename =~ m/$include_tocdocs_command/)
      {
         print "==========================\n======================\nImplementME!\n\n====================\n";
      }
      
      # ----------- text file include ----------- #
      else
      {
         my $text_data = "";
         print "[including: \"$expandvars_filename\" ";
         main::load( \$text_data, $expandvars_filename, "include text", "1" );

         $$contents_ref =~ s/$include_statement/$text_data/is;

         print "]";

         $includes_existed = 1;
      }
   }
   
   return $includes_existed;
}

# Get rid of white space lines to make the file smaller to download
#
# This function basically only looks for blank lines and deletes them.
sub processRemoveWhitespace($)
{
   outputDebug( "==> processRemoveWhitespace\n" );
   my $contents_ref = shift;

   $$contents_ref =~ s/(\s*)\n/\n/gis;
}

sub processRemoveNewlines($)
{
   outputDebug( "==> processRemoveNewlines\n" );
   my $data = shift;
   
   # eliminate newlines
   $$data =~ s/[\n]//gis;
}
# -----------------------------------------------------------------------------
# Copy the current file to the appropriate place in the destination directory
# tree.
#
# Syntax:
#     recurseAction($curfile,full_src_path,full_dest_path,relative_path);
#
# Arguments:
#     $curfile - The name of the current file in the recursion process.
# -----------------------------------------------------------------------------
sub recurseAction($) 
{
    #outputDebug( "==> recurseAction\n" );
    my $curfile = shift;
    my $full_src_path = shift;
    my $full_dest_path = shift;
    my $rel_path = shift;

    # print "recurseAction: $curfile , $full_src_path , $full_dest_path, $rel_path\n";

    # I need the full name of the file where it is going to be installed

    # Check if the file already exists
    # && if the file does not need updating. (It is newer than the source file)
    $full_dest_file = $full_dest_path . "/" . $curfile;
    if (($check_file_mtime == 1) && (-e $full_dest_file))
    {
       if (stat($full_dest_file)->mtime > stat($curfile)->mtime)  # ASSERT: Dest file is newer
       {
          #print "   Skipping $curfile\n";
          return;    # We will skip it
       }
    }

    print "   Processing file: $curfile...";
    #installFile("$curfile", $uid, $gid, "$mode", "$dest_dir");

     # This is equivalent to a C switch block.
    SWITCH: {
      # Match .txt or .TXT.
      if ( $curfile =~ /\.(txt|pdf|ps|TXT|PDF|PS)$/i ) {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          last SWITCH;
      }
      
      # Match html.placeholder
      if ( $curfile =~ /\.(html.placeholder)$/i ) 
      {
         my $strippedFile = $curfile;
         $strippedFile =~ s/\.(placeholder)$//;
         if (!(-e $strippedFile))
         {
	         print "File $strippedFile doesn't exist, using placeholder\n";
	         copy("$curfile", "$strippedFile");

            my $file_contents = "";
            main::load( \$file_contents, $strippedFile, "recurse", "0" );
            htmlFilter( \$file_contents );
            installContents( $file_contents, $strippedFile, $uid, $gid, "$mode", "$full_dest_path", $full_src_path );
         }
         last SWITCH;
      }
      
      # Match .placeholder
      if ( $curfile =~ /\.(placeholder)$/i ) 
      {
         my $strippedFile = $curfile;
         $strippedFile =~ s/\.(placeholder)$//;
         if (!(-e $strippedFile))
         {
            print "File $strippedFile doesn't exist, using placeholder\n";
            copy("$curfile", "$strippedFile");
            installFile("$strippedFile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
         }
         last SWITCH;
      }

      # Match .html,.htm
      if ( $curfile =~ /\.(htm|HTM|html|HTML)$/ ) 
      {
         my $file_contents = "";
         main::load( \$file_contents, $curfile, "recurse", "0" );
         htmlFilter( \$file_contents );
         installContents( $file_contents, $curfile, $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
         
         last SWITCH;
      }

      # Match .gif,.jpg, .png
      if ( $curfile =~ /\.(png|jpg|gif|jpeg|PNG|JPG|GIF|JPEG)$/ ) {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          last SWITCH;
      }
      
      # Match binary files
      if ( $curfile =~ /\.(scr|bin|gz|tar|zip|exe|rpm|mid|mp3|BIN|GZ|TAR|ZIP|EXE|RPM|MID|MP3|SCR)$/ )
      {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          last SWITCH;
      }

      # Match .xml
      if ( $curfile =~ /\.(xml|XML)$/ ) {
          my $file_contents = "";
          
          main::load( \$file_contents, $curfile, "recurse", "0" );
          xmlFilter( \$file_contents, "$rel_path", "0" );
          installContents( $file_contents, "$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          
          #print "$file_contents\n crap!\n================\n";
          
          last SWITCH;
      }

      # Match Java stuff: class,archive,javascript
      if ( $curfile =~ /\.(class|jar|js|CLASS|JAR|JS)$/ ) {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          last SWITCH;
      }

      # Style sheets
      if ( $curfile =~ /\.(css|CSS)$/ ) 
      {
         my $file_contents = "";
         main::load( \$file_contents, $curfile, "recurse", "0" );
         htmlFilter( \$file_contents );
         installContents($file_contents, "$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);

         last SWITCH;
      }

      # PGP files (public key ring)
      if ( $curfile =~ /\.(pkr|PKR)$/ ) 
      {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          last SWITCH;
      }
    }
    print "  done.\n";
}

####
sub create_script($)
{
   my $script_name = shift;

   if (!open(OUTFILE, ">$script_name"))
   {
      warn "WARNING: Cannot read from $filename: $!\n";
      return -1;
   }

   ## Create the script file ####
   print OUTFILE "#! perl\n";
   print OUTFILE "\# Options:  -a     recompile everything\n";
   print OUTFILE "\#           -n     don't run netscape after compile\n";
   print OUTFILE "\n";
   print OUTFILE "\n";
   print OUTFILE "require 5.004;\n";
   print OUTFILE "use Getopt::Std;\n";
   print OUTFILE "\n";
   print OUTFILE "\ngetopt('');\n";

   print OUTFILE "my \$src_dir = \"$src_dir\";\n";
   print OUTFILE "my \$dest_dir = \"$dest_dir\";\n";
   print OUTFILE "my \$css_filename = \"$css_filename\";\n";
   print OUTFILE "my \$html_header_filename = \"$html_header_filename\";\n";
   print OUTFILE "my \$html_footer_filename = \"$html_footer_filename\";\n";
   print OUTFILE "my \$html_install_prefix = \"$html_install_prefix\";\n";
   print OUTFILE "my \$subst_file = \"$subst_file\";\n";
   print OUTFILE "\n\n\n";

   print OUTFILE "my \$command = \"install-web.pl \";\n";
   print OUTFILE "\$command .= \" -s \$src_dir\";\n";
   print OUTFILE "\$command .= \" -t \$dest_dir\";\n";
   print OUTFILE "\$command .= \" -i $opt_i\";\n";
   print OUTFILE "\$command .= \" -c \$css_filename\";\n";
   print OUTFILE "\$command .= \" -f \$html_header_filename\";\n";
   print OUTFILE "\$command .= \" -f \$html_footer_filename\";\n";
   print OUTFILE "\$command .= \" -h \$html_install_prefix\";\n";
   print OUTFILE "\$command .= \" -w \$subst_file\";\n";
   print OUTFILE "\$command .= \" -z use.last.build.pl\";\n";
   print OUTFILE "if (\$opt_a == 1)\n{\n";
   print OUTFILE "    \$command .= \" -a\";\n}";
   print OUTFILE "\n\n";

   print OUTFILE "print \"\$command\\n\\n\\n\\n----------------\\n\\n\";\n";
   print OUTFILE "system(\"\$command\");\n";

   ## Netscape executing command ##
   print OUTFILE "if (\$opt_n == 0)\n";
   print OUTFILE "{\n";
   print OUTFILE "    my \$netscape_command = \"netscape \" . \$html_install_prefix . \"index.html &\";\n";
   print OUTFILE "    print \"\$netscape_command\\n\";\n";
   print OUTFILE "    system(\"\$netscape_command\");\n";
   print OUTFILE "}\n";
   close(OUTFILE) or warn "WARNING: Cannot close $filename: $!\n";
}
