#!/usr/local/bin/perl

# -----------------------------------------------------------------------------
# $Id$
# -----------------------------------------------------------------------------
# Install an entire directory from the VR Juggler development tree without
# the CVS directories.  All files in the directory tree are copied to the
# destination directory.
#
# Usage:
#     install-dir.pl -i <source directory> -o <destination directory>
#
#     <source directory> - The source directory to be installed
#     <destination directory> - The location to install the source directory
# -----------------------------------------------------------------------------

require 5.004;

use File::Basename;
use File::Path;
use Getopt::Std;

# Do this to include the path to the script in @INC.
BEGIN {
    $path = (fileparse("$0"))[1];
}

use lib("$path");
use InstallOps;

# Ensure that there are four command-line arguments.  If not, exit with
# error status.
if ( $#ARGV != 3 ) {
    warn "Usage: $0 -i <source directory> -o <destination directory>\n";
    exit 1;
}

# Get the -i and -o options and store their values in $opt_i and $opt_o
# respectively.
getopt('i:o:');

my $src_dir = "$opt_i";
my $dest_dir = "$opt_o";
mkpath("$dest_dir", 0, 0755);	# Make sure that $dest_dir exists

chop($start_dir = `pwd`) unless $dest_dir =~ /^\//;	# Save this for later

# Push the source directory onto the InstallOps module's internal directory
# stack.  This is used for recursion through the source directory.
push(@InstallOps::dirstack, "$src_dir");

# Recurse through $src_dir and create the destination directory tree.
# recurseAction() handles further work.
recurseDir("$src_dir", "$start_dir/$dest_dir");

exit 0;

# -----------------------------------------------------------------------------
# Copy the current file to the appropriate place in the destination directory
# tree.
#
# Syntax:
#     recurseAction($curfile);
#
# Arguments:
#     $curfile - The name of the current file in the recursion process.
# -----------------------------------------------------------------------------
sub recurseAction ($) {
    my $curfile = shift;

    installFile("$curfile", 0644, "$dest_dir");
}
