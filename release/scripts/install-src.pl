#!/usr/local/bin/perl

# -----------------------------------------------------------------------------
# $Id$
# -----------------------------------------------------------------------------
# Install a source (i.e., software source) directory to a specified
# destination directory.  Only pre-defined file types (determined by file
# extension) are copied.  No CVS subdirectories are installed.
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

my $dest_dir = "$opt_o";
umask(002);
mkpath("$dest_dir", 0, 0755);	# Make sure that $dest_dir exists

chop($start_dir = `pwd`) unless $dest_dir =~ /^\//;	# Save this for later

# Push the source directory onto the InstallOps module's internal directory
# stack.  This is used for recursion through the source directory.
push(@InstallOps::dirstack, "$opt_i");

# Recurse through $src_dir and create the destination directory tree.
# recurseAction() handles further work.
recurseDir("$opt_i", "$start_dir/$dest_dir");

exit 0;

# -----------------------------------------------------------------------------
# Copy files of a selected type (based on extension) to the appropriate place
# in the destination directory tree.
#
# Syntax:
#     recurseAction($curfile);
#
# Arguments:
#     $curfile - The name of the current file in the recursion process.
# -----------------------------------------------------------------------------
sub recurseAction ($) {
    my $curfile = shift;

    # This is equivalent to a C switch block.
    SWITCH: {
	# Match .txt or .TXT.
	if ( $curfile =~ /\.txt$/i ) {
	    installFile("$curfile", 0644, "$dest_dir");
	    last SWITCH;
	}

	# Match .C, .c, or .h.
	if ( $curfile =~ /\.[Cch]$/ ) {
	    installFile("$curfile", 0644, "$dest_dir");
	    last SWITCH;
	}

	# Match .cxx, .cpp, .CXX or .CPP.
	if ( $curfile =~ /\.c(xx|pp)$/i ) {
	    installFile("$curfile", 0644, "$dest_dir");
	    last SWITCH;
	}

	# Match Makefile.
	if ( "$curfile" eq "Makefile" ) {
	    installFile("$curfile", 0644, "$dest_dir");
	    last SWITCH;
	}
    }
}
