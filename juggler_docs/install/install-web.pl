#!/usr/local/bin/perl

#
#  File:    $RCSfile$
#  Date modified:  $Date$
#  Version:    $Revision$
#
#
#                                 VR Juggler
#                                     by
#                               Allen Bierbaum
#                              Christopher Just
#                              Patrick Hartling
#                             Carolina Cruz-Neira
#                                Albert Baker
#
#                          Copyright - 1997,1998,1999
#                 Iowa State University Research Foundation, Inc.
#                             All Rights Reserved
#

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
use Getopt::Std;
use File::stat;
#use strict;

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

my $check_file_mtime = 1;     # should we check file modification time in recurseAction?
if($opt_a == 1)
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
   if($subst_file)
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
   if(!open(HTML_HEADER, "$header_file_name"))
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
   if(!open(HTML_FOOTER, "$footer_file_name"))
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
#     filename: The name of the file to run the filter on
#
sub htmlFilter($)
{
   my $head_tag_begin = "<head>";
   my $head_tag_end = "<\/head>";
   my $html_tag_begin = "<html>";
   my $html_comment_begin = "<!-- ";
   my $html_comment_end = " -->";
   my $body_tag_end = "<\/body>";
   my $ignore_web_filter_str = 'install-web ignore';
   my $ignore_css_str = 'install-web no-css';
   my $ignore_header_str = 'install-web no-header';
   my $ignore_footer_str = 'install-web no-footer';

   my $filename = shift;
   ##print "     |--> HTML filter: $filename ...";

   # Read in the entire file first into a single string
   my $file_contents;

   if(!open(HTML_FILE, $filename))
   {
      warn "WARNING: Cannot read from $filename: $!\n";
      return -1;
   }

   while(<HTML_FILE>)
   {
      $file_contents .= $_;
   }
   close(HTML_FILE) or warn "WARNING: Cannot close $filename: $!\n";

   ### Check for IGNORE ALL ####
   if($file_contents =~ m/$ignore_web_filter_str/is)
   {
      ##print "ignoring file.\n";
      return 1;
   }

   # ----------- HEADER INSERT ----------- #
   # Insert header
   # Look for <BODY> and put it right after that
   if($file_contents =~ m/$ignore_header_str/is)  ### Check for IGNORE HEADER ####
   {
      print "ignoring header... ";
   }
   else
   {
      $file_contents =~ s/($head_tag_end)/\n$html_header\n$1\n/is;
   }
   
   
   
   # ----------- CSS HEADER INSERT ----------- #
   # if there is </head> then insert before it
   # if not, then look for <html> and insert after it with <head>...</head> around it
   if($file_contents =~ m/$ignore_css_str/is)  ### Check for IGNORE HEADER ####
   {
      print "ignoring header... ";
   }
   else
   {
      if($file_contents =~ s/($head_tag_end)/\n$css_header\n$1\n/is)
      {
      #print "added css (</head>)...";
      }
      elsif($file_contents =~ s/($html_tag_begin)/$1\n$head_tag_begin\n$css_header\n$head_tag_end\n/is)
      {
      #print "added css (<html>)...";
      }
      else
      {
      #print "did NOT add css...";
      }
   }

   # ------ Edit the CSS link path ----- #
   # ASSERT: The path in the css file is relative to the
   #         root directory of the web install
   $file_contents =~ s/<link(.*?)stylesheet(.*?)href(.*?)\"/$&$html_install_prefix/gis;

   

   # ----------- FOOTER INSERT ----------- #
   # Insert footer
   # Look for </BODY> and put it right before that
   if($file_contents =~ m/$ignore_footer_str/is)  ### Check for IGNORE FOOTER ####
   {
      print "ignoring footer... ";
   }
   else
   {
      $file_contents =~ s/($body_tag_end)/\n$html_footer\n$1\n/is;
   }

   ## Check includes ##
   processIncludes(\$file_contents);

   ## REPLACE TAGS
   replaceTags(\$file_contents);

   # -----------  SAVE FILE ----------- #
   if ( ! open(HTML_FILE, ">$filename") ) {
       warn "WARNING: Cannot create $filename: $!\n";
       return -1;
   }

   print HTML_FILE $file_contents;
   print HTML_FILE $html_comment_begin, "Processed by install-web: ", scalar(localtime()), $html_comment_end;
   close(HTML_FILE) or warn "WARNING: Cannot close $filename: $!\n";

   #print "done.\n";
}


#  Initialize the HTML filter that we will use
#
# syntax:
#     xmlFilter("$full_dest_path/$curfile","$rel_path")
# args:
#     filename: The name of the file to run the filter on
#     $relpath: The relative path to the given file from the "root" web dir
#
sub xmlFilter($$)
{
   my $html_comment_begin = "<!-- ";
   my $html_comment_end = " -->";

   my $filename = shift;
   my $rel_path = shift;

   #print "     |--> XML filter: $filename ....";

   # Read in the entire file first into a single string
   my $file_contents;

   if(!open(XML_FILE, $filename))
   {
      warn "WARNING: Cannot read from $filename: $!\n";
      return -1;
   }

   while(<XML_FILE>)
   {
      $file_contents .= $_;
   }
   close(XML_FILE) or warn "WARNING: Cannot close $filename: $!\n";

   ## Check includes ##
   processIncludes(\$file_contents);

   # ---- PATH REPLACE ----
   # If it is not a tag, then do a simple replace
   # urllink = "somewhere/something.html" ==> urllink="webprefix/rel_path/somewhere/something.html"
   # NOTE: We don't want to replace links that have TAGS in them
   # NOTE2: This means that all other types of links (rlink|jit|link) must have paths relative to the urllink
   #       OR that use a ${TAG} format for the paths
   #$file_contents ~= s/link(.*?)=(.*?)\"(?!\$)/link=\"$html_install_prefix$rel_path\//gis;
   #$file_contents =~ s/(rlink|jit|link)(\s*?)=(\s*?)\"(?!\$)/$1=\"$html_install_prefix\/$rel_path\//gis;
   $file_contents =~ s/(linkurl)(\s*?)=(\s*?)\"(?!\$)/$1=\"$html_install_prefix\/$rel_path\//gis;

   ## REPLACE TAGS
   replaceTags(\$file_contents);

   ## Replace white space
   processRemoveWhitespace(\$file_contents);

   # -----------  SAVE FILE ----------- #
   if ( ! open(XML_FILE, ">$filename") ) {
       warn "WARNING: Cannot create $filename: $!\n";
       return -1;
   }

   print XML_FILE $file_contents;
   print XML_FILE $html_comment_begin, "Processed by install-web: ", scalar(localtime()), $html_comment_end;
   close(XML_FILE) or warn "WARNING: Cannot close $filename: $!\n";

   #print "done.\n";
}

# Replace the tags in the file contents passed to the subroutine
#
# Each tag in %path_subst is replaced with the tag value prefixed
# by the html_install_directory
# Each tag in %alias_subst is replace by the tag only
sub replaceTags($)
{
   my $contents_ref = shift;

   #print "File contents???---\n\n";
   #print $$contents_ref;

   # find and path_subst tags and replace with <html_inst_dir>/<tag value>
   while (($tag,$tag_value) = each(%path_subst))
   {
      #print "Checking web subst: $tag ==> $tag_value\n";
      # Look for ${tag} or $(tag)
      $$contents_ref =~ s/(\$)[{\(]$tag[}\)]/$html_install_prefix$tag_value/gis;
   }

   # alias_subst replace
   while (($tag,$tag_value) = each(%alias_subst))
   {
      $$contents_ref =~ s/(\$)[{\(]$tag[}\)]/$tag_value/gis;
   }
}

# Check for includes and do them
sub processIncludes($)
{
   my $contents_ref = shift;

   #print "Processing includes....";

   # While we have some more includes to deal with
   while($$contents_ref =~ m/<!--(\s*?)install-web(\s*?)include(\s*?)(\S*?)(\s*?)-->/is)
   {
      my $filename = $4;      # Get filename from the match
      my $include_contents = "";   # contents of the file

      print "including file: $filename... ";

      ###  LOAD the INCLUDE FILE ###
      if(!open(INC_FILE, $filename))
      {
         warn "WARNING: Cannot read from $filename: $!\n";
         return -1;
      }

      while(<INC_FILE>)
      { $include_contents .= $_; }
      close(INC_FILE) or warn "WARNING: Cannot close $filename: $!\n";

      $$contents_ref =~ s/<!--(\s*?)install-web(\s*?)include(\s*?)($filename)(\s*?)-->/$include_contents/is;
   }
   #print " done processing includes.\n";
}

# Get rid of white space lines to make the file smaller to download
#
# This function basically only looks for blank lines and deletes them.
sub processRemoveWhitespace (\$)
{
   my $contents_ref = shift;

   $$contents_ref =~ s/(\s*)\n/\n/gis;
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
sub recurseAction ($) {
    my $curfile = shift;
    my $full_src_path = shift;
    my $full_dest_path = shift;
    my $rel_path = shift;

    # print "recurseAction: $curfile , $full_src_path , $full_dest_path, $rel_path\n";

    # I need the full name of the file where it is going to be installed

    # Check if the file already exists
    # && if the file does not need updating. (It is newer than the source file)
    $full_dest_file = $full_dest_path . "/" . $curfile;
    if(($check_file_mtime == 1) && (-e $full_dest_file))
    {
       if(stat($full_dest_file)->mtime > stat($curfile)->mtime)  # ASSERT: Dest file is newer
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
      if ( $curfile =~ /\.(txt|pdf)$/i ) {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          last SWITCH;
      }

      # Match .html,.htm
      if ( $curfile =~ /\.(htm|html)$/ ) {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          htmlFilter("$full_dest_path/$curfile");
          last SWITCH;
      }

      # Match .gif,.jpg, .png
      if ( $curfile =~ /\.(png|jpg|gif|jpeg)$/ ) {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          last SWITCH;
      }

      # Match .xml
      if ( $curfile =~ /\.(xml|XML)$/ ) {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          xmlFilter("$full_dest_path/$curfile","$rel_path");
          last SWITCH;
      }

      # Match Java stuff: class,archive,javascript
      if ( $curfile =~ /\.(class|jar|js)$/ ) {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          last SWITCH;
      }

      # Style sheets
      if ( $curfile =~ /\.(css)$/ ) {
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

   if(!open(OUTFILE, ">$script_name"))
   {
      warn "WARNING: Cannot read from $filename: $!\n";
      return -1;
   }

   ## Create the script file ####
   print OUTFILE "#! perl\n";
   print OUTFILE "require 5.004;\n";
   print OUTFILE "\n";
   print OUTFILE "\n";
   print OUTFILE "use Getopt::Std;\n";
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
   print OUTFILE "if(\$opt_a == 1)\n{\n";
   print OUTFILE "\$command .= \" -a\";\n}";
   print OUTFILE "\n\n";

   print OUTFILE "system(\"\$command\");\n";

   ## Netscape executing command ##
   print OUTFILE "my \$netscape_command = \"netscape \" . \$html_install_prefix . \"index.html &\";\n";
   print OUTFILE "print \"\$netscape_command\\n\";\n";
   print OUTFILE "system(\"\$netscape_command\");\n";

   close(OUTFILE) or warn "WARNING: Cannot close $filename: $!\n";
}
