#!/usr/bin/perl

# -----------------------------------------------------------------------------
# Fancy install for websites, includes variable substitution, text includes, 
# common header/footer/css style sheet, xml table-of-contents rendering methods, etc...
# -----------------------------------------------------------------------------
# Usage:
#     install-web.pl -s <source directory> -t <destination directory>
#                   [-u <user name> -g <group name> -m <mode>]
#                    -i <dir ignore>,<dir ignore>
#                    -c <css header> -e <html header> -f <html footer>
#                    -h <HTML install path> -w <tag subst file>
#                    -a (force all)
#                    -z generate Zscript
#
#
#     <source directory> - The source directory to be installed
#     <destination directory> - The location to install the source directory
#     <HTML install path> - A prefix for HTML info. ex: "www.vrjugger.org/"
# -----------------------------------------------------------------------------
require 5.004;

# Do this to include the path to the script in @INC.
use File::Basename;
BEGIN {
    $path = (fileparse("$0"))[1];
}
use lib("$path");
use Getopt::Std;
use InstallWeb;

$| = 1;

# Ensure that there are four command-line arguments.  If not, exit with
# error status.
if ( $#ARGV < 3 ) 
{
    warn "Usage: $0 -s <source directory> -t <target directory> -i<ignore dir>,<ignore dir>,.. -c <css header> -e <html header> -f <html footer> -h <HTML install path> -w <subst file> -a (force all) -z <name of script file to output>\n";
    exit 1;
}

# Get the options
getopt('s:t:i:c:e:f:w:h:z:');


# create a script if -z was specified
if ($opt_z)
{
   main::create_script( $opt_z );
}

# run InstallWeb

# $uid $gid $mode: permissions
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

# Configure InstallWeb...
InstallWeb::configure( 
   $opt_s, # src_dir
   $opt_t, # dest_dir
   $opt_c, # css_filename
   $opt_e, # html_header_filename
   $opt_f, # html_footer_filename
   $opt_h, # html_install_prefix
   $opt_w, # subst_file
   $opt_a, # rebuild_all
   $opt_i,  # skip_dirs
   $uid,   # UID
   $gid,   # GID
   $mode  # MODE
   );

# install the configured website
InstallWeb::install();

exit 0;




##############################################################################
# subroutines
##############################################################################
sub create_script($)
{
   my $script_name = shift;

   if (!open(OUTFILE, ">$script_name"))
   {
      warn "WARNING: Cannot read from $script_name: $!\n";
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

   print OUTFILE "my \$src_dir = \"$opt_s\";\n";
   print OUTFILE "my \$dest_dir = \"$opt_t\";\n";
   print OUTFILE "my \$css_filename = \"$opt_c\";\n";
   print OUTFILE "my \$html_header_filename = \"$opt_e\";\n";
   print OUTFILE "my \$html_footer_filename = \"$opt_f\";\n";
   print OUTFILE "my \$html_install_prefix = \"$opt_h\";\n";
   print OUTFILE "my \$subst_file = \"$opt_w\";\n";
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
   close(OUTFILE) or warn "WARNING: Cannot close $script_name: $!\n";
}
