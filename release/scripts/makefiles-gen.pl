#!/usr/local/bin/perl

# -----------------------------------------------------------------------------
# $Id$
# -----------------------------------------------------------------------------
# Generate Makefiles in the subdirectories (SUBDIRS) of the installation
# directory (prefix) from Makefile.in's found in the source directory
# (startdir).  All command-line options shown here are required.  They are
# used to replace strings of the form "@...@" in the Makefile.in files found
# in the startdir directory tree.
#
# Usage:
#     makefiles-gen.pl
#         --CXX=<C++ compiler>
#         --DEFS=<Compile-time defs>
#         --CPPFLAGS=<C preprocessor flags>
#         --CXXFLAGS=<C++ compiler flags>
#         --INCLUDES=<Extra include dirs>
#         --TEST_EXTRA_LIBS=<Libraries needed for test code>
#         --srcdir=<Location of source code>
#         --prefix=<Base directory where Makefile will go>
#         --startdir=<Directory where search begins>
#         --SUBDIRS=<Directories containing Makefile.in's>
#
# NOTE:
#    In almost every situation, the --srcdir option should be passed the
#    value "." so that the source in the installation directory will be used.
# -----------------------------------------------------------------------------

require 5.004;

use File::Basename;
use File::Copy;
use Getopt::Long;

# Do this to include the path to the script in @INC.
BEGIN {
    $path = (fileparse("$0"))[1];
}

use lib("$path");
use InstallOps;

# Turn off case insensitivity when parsing command-line options.
$Getopt::Long::ignorecase = 0;

if ( $#ARGV <= 5 ) {
    die <<USAGE_EOF;
Usage:
    $0
        --CXX=<C++ compiler>
        --DEFS=<Compile-time defs>
        --CPPFLAGS=<C preprocessor flags>
        --CXXFLAGS=<C++ compiler flags>
        --INCLUDES=<Extra include dirs>
        --srcdir=<Location of source code>
        --prefix=<Base directory where Makefile will go>
        --startdir=<Directory where search begins>
        --TEST_EXTRA_LIBS=<Libraries needed for test code>
        --SUBDIRS=<Directories containing Makefile.in's>
USAGE_EOF
}

# Parse the command-line options and store the given values in %VARS which
# is indexed by the tag name to be replaced (e.g., 'CXX').
GetOptions("CXX=s" => \$VARS{'CXX'}, "DEFS:s" => \$VARS{'DEFS'},
	   "CPPFLAGS:s" => \$VARS{'CPPFLAGS'},
	   "CXXFLAGS:s" => \$VARS{'CXXFLAGS'},
	   "INCLUDES:s" => \$VARS{'INCLUDES'},
	   "TEST_EXTRA_LIBS=s" => \$VARS{'TEST_EXTRA_LIBS'},
	   "srcdir=s" => \$VARS{'srcdir'}, "SUBDIRS=s" => \$VARS{'SUBDIRS'},
	   "prefix=s" => \$prefix, "startdir=s" => \$startdir);

chdir("$startdir") or die "ERROR: Cannot chdir to $startdir: $!\n";

my ($basedir, $dir);
chop($basedir = `pwd`);

# Loop over the directories given with --SUBDIRS and generate the Makefiles
# from the corresponding Makefile.in's.
foreach $dir ( split(/\s/, "$VARS{'SUBDIRS'}") ) {
    my $outfile = "$prefix/$dir/Makefile";

    print "Generating $outfile ...\n";

    my $infile = "$basedir/$dir/Makefile.in";
    my $workfile = "/tmp/Makefile.in";

    # Make a working copy of the input file to be safe.
    copy("$infile", "$workfile") unless "$infile" eq "$workfile";

    # Replace the tags in $workfile with the values in %VARS.
    next if replaceTags("$workfile", %VARS) < 0;

    # Move $workfile to its final destination.
    copy("$workfile", "$outfile");
    unlink("$workfile");
}

exit 0;
