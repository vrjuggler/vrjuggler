package InstallWeb;

require 5.004;
require Exporter;

@ISA = qw(Exporter);
@EXPORT = qw(configure install);

# -----------------------------------------------------------------------------
# Fancy install for websites, includes variable substitution, text includes, 
# common header/footer/css style sheet, xml table-of-contents rendering methods, etc...
# -----------------------------------------------------------------------------




# Do this to include the path to the script in @INC.
use File::Basename;
use UniqueID;
BEGIN 
{
    $path = (fileparse("$0"))[1];
}
use lib("$path");

use File::Path;
use File::Copy;
use File::stat;
use InstallOps;
use Utils;
use WebFile;
use XmlFile;
use HtmlFile;

#---------------------------------
# InstallWeb.pm member vars:
#---------------------------------

# constants:
my $html_comment_begin = "<!--";
my $html_comment_end = "-->";
my $usedebugoutput = "0";

my $use_abs_paths_within_jit = "1";
my $use_http_paths_within_jit = "0";

my $use_abs_paths = "1";
my $use_http_paths = "0";

# to be set by the application
my $src_dir = "unknown";
my $dest_dir = "unknown";
my $css_filename = "unknown";
my $html_header_filename = "unknown";
my $html_footer_filename = "unknown";
my $html_install_prefix = "unknown";
my $subst_file = "unknown.pl";
my $rebuild_all = 0;
my @skip_dirs;
my($uid, $gid, $mode) = ($<, (getpwuid($<))[3], "0644");


#----------------------------------------------------------------------#
#                         Sub Routines                                 #
#----------------------------------------------------------------------#
#InstallWeb::configure( $src_dir, $dest_dir, $css_filename, 
#   $html_header_filename, $html_footer_filename,
#   $html_install_prefix, $subst_file, $rebuild_all, $skip_dirs,
#   $uid, $gid, $mode );
#
# NOTE: $skip_dirs is a comma delimited list.
#
sub configure($$$$$$$$$$$$)
{
   $src_dir = shift;
   $dest_dir = shift;
   $css_filename = shift;
   $html_header_filename = shift;
   $html_footer_filename = shift;
   $html_install_prefix = shift;
   $alias_subst{"SiteURL"} = "$html_install_prefix";
   $subst_file = shift;
   $rebuild_all = shift;
   my $skipdirslist = shift;
  
   $uid = shift;
   $gid = shift;
   $mode = shift;
   
   #####################################
   
   @skip_dirs = split /[,]/ , $skipdirslist;

   # Remove lead/trail whitespace
   foreach (@skip_dirs)
   {
       s/^\s+//;
       s/\s+$//;
   }

   ## ---- Output list of options used ---- ##
   print "---------- InstallWeb Configuration -----------\n";
   print "         Source dir: $src_dir\n";
   print "           Dest dir: $dest_dir\n";
   print "           CSS file: $css_filename\n";
   print "        HTML Header: $html_header_filename\n";
   print "        HTML footer: $html_footer_filename\n";
   print "HTML install prefix: $html_install_prefix\n";
   print "     Web Subst file: $subst_file\n";
   print "        Rebuild All: $rebuild_all\n";
   print "           Skipping: @skip_dirs\n";
   print "                uid: $uid\n";
   print "                gid: $gid\n";
   print "               mode: $mode\n";
   print "-----------------------------------------------\n";

   initOutputDebug($usedebugoutput);

   # Save this for later
   # Unless it stars with a / and ends with a /
   chop($root_inst_dir = `pwd`) unless $dest_dir =~ /^\//;
   chop($root_src_dir = `pwd`) unless $src_dir =~ /^\//;

   WebFile::init('install-web', "$html_comment_begin", "$html_comment_end",
                 $usedebugoutput, "$html_install_prefix", "$root_src_dir",
                 "$src_dir");

   # Initialize Filters
   WebFile::loadSubstFile("$subst_file");
   HtmlFile::init("$css_filename", "$html_header_filename",
                  "$html_footer_filename");

   umask(002);
   mkpath("$dest_dir", 0, 0755); # Make sure that $dest_dir exists
}


sub install()
{
   print "GO!
   recurseDir(
     $root_src_dir/$src_dir
     $root_inst_dir/$dest_dir
     @skip_dirs )\n\n\n";
   #exit(0);
   # Recurse through $src_dir and create the destination directory tree.
   # recurseAction() handles further work.
   recurseDir( "$root_src_dir/$src_dir", "$root_inst_dir/$dest_dir", @skip_dirs);
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
   print XML_FILE $html_comment_begin, "Processed by InstallWeb.pm: ", scalar(localtime()), $html_comment_end;
   close(XML_FILE) or warn "WARNING: Cannot close $filename: $!\n";
   
   return 1;
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

    # initialize built-in subst vars...
    $alias_subst{"CurrentPath"} = $full_src_path;
    $alias_subst{"CurrentFile"} = $curfile;
    $alias_subst{"CurrentFilePath"} = "$full_src_path/$curfile";
    $alias_subst{"CurrentURL"} = "$html_install_prefix/$rel_path/$curfile";

   my $UniquePageID;
   UniqueID::filePathToUniqeID( "$html_install_prefix/$rel_path/$curfile", \$UniquePageID ); 
   $alias_subst{"UniquePageID"} = $UniquePageID;

    # print "recurseAction: $curfile , $full_src_path , $full_dest_path, $rel_path\n";

    # I need the full name of the file where it is going to be installed

    # Check if the file already exists
    # && if the file does not need updating. (It is newer than the source file)
    $full_dest_file = $full_dest_path . "/" . $curfile;
    if (($rebuild_all == 0) && (-e $full_dest_file))
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
      if ( $curfile =~ /\.(txt|pdf|ppt|ps|TXT|PDF|PS)$/i ) {
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

            my $html_file = new HtmlFile("$strippedFile");
            $html_file->disableFeedback();
            $html_file->load('recurse');
            $html_file->filter();

            installContents($html_file->getBody(), "$strippedFile", $uid, $gid,
                            "$mode", "$full_dest_path", $full_src_path );
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
      if ( $curfile =~ /\.(html?)$/i ) {
         my $html_file = new HtmlFile("$curfile");
         $html_file->disableFeedback();
         $html_file->load('recurse');
         $html_file->filter();

         installContents($html_file->getBody(), "$curfile", $uid, $gid,
                         "$mode", "$full_dest_path", $full_src_path);
         
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
      if ( $curfile =~ /\.xml$/i ) {
          my $xml_file = new XmlFile("$curfile");

          $xml_file->disableFeedback();
          $xml_file->load('recurse');
          $xml_file->filter("$rel_path", $WebFile::USE_HTTP_PATHS,
                            $WebFile::USE_HTTP_PATHS_WITHIN_JIT);

          installContents($xml_file->getBody(), "$curfile", $uid, $gid,
                          "$mode", "$full_dest_path", $full_src_path);
          
          #print "$file_contents\n crap!\n================\n";
          
          last SWITCH;
      }

      # Match Java stuff: class,archive,javascript
      if ( $curfile =~ /\.(class|jar|js|CLASS|JAR|JS)$/ ) {
          installFile("$curfile", $uid, $gid, "$mode", "$full_dest_path", $full_src_path);
          last SWITCH;
      }

      # Style sheets
      if ( $curfile =~ /\.css$/i ) 
      {
         my $file = new HtmlFile("$curfile");
         $file->disableFeedback();
         $file->load('recurse');
         $file->filter();

         installContents($file->getBody(), "$curfile", $uid, $gid, "$mode",
                         "$full_dest_path", $full_src_path);

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

1;
