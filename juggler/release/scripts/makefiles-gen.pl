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
#         --TEST_VJ_LIBS_BASIC=<Basic Juggler libraries needed for test code>
#         --TEST_VJ_LIBS_GL=<OpenGL API libraries needed for test code>
#         --TEST_VJ_LIBS_PF=<Performer API libraries needed for test code>
#         --TEST_LIBS_BASIC=<Basic system libraries needed for test code>
#         --TEST_EXTRA_LIBS_BASIC=<Basic extra libraries needed for test code>
#         --TEST_EXTRA_LIBS_GL=<Extra OpenGL libraries needed for test code>
#         --TEST_EXTRA_LIBS_PF=<Extra Performer libraries needed for test code>
#         --srcdir=<Location of source code>
#         --prefix=<Base directory where Makefile will go>
#         --startdir=<Directory where search begins>
#         --SUBDIRS=<Directories containing Makefile.in's>
#         --uname=<Owner's user name>
#         --gname=<Group name>
#         --mode=<Mode bits>
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
        --TEST_VJ_LIBS_BASIC=<Basic Juggler libraries needed for test code>
        --TEST_VJ_LIBS_GL=<OpenGL API libraries needed for test code>
        --TEST_VJ_LIBS_PF=<Performer API libraries needed for test code>
        --TEST_VJ_LIBS_BASIC=<Basic system libraries needed for test code>
        --TEST_EXTRA_LIBS_BASIC=<Basic extra libraries needed for test code>
        --TEST_EXTRA_LIBS_GL=<Extra OpenGL libraries needed for test code>
        --TEST_EXTRA_LIBS_PF=<Extra Performer libraries needed for test code>
        --SUBDIRS=<Directories containing Makefile.in's>
      [ --uname=<Owner's user name> --gname=<Group name> --mode=<Mode bits> ]
USAGE_EOF
}

# Parse the command-line options and store the given values in %VARS which
# is indexed by the tag name to be replaced (e.g., 'CXX').
GetOptions("CXX=s" => \$VARS{'CXX'}, "DEFS:s" => \$VARS{'DEFS'},
	   "CPPFLAGS:s" => \$VARS{'CPPFLAGS'},
	   "CXXFLAGS:s" => \$VARS{'CXXFLAGS'},
	   "INCLUDES:s" => \$VARS{'INCLUDES'},
	   "TEST_VJ_LIBS_BASIC=s" => \$VARS{'TEST_VJ_LIBS_BASIC'},
	   "TEST_VJ_LIBS_GL=s" => \$VARS{'TEST_VJ_LIBS_GL'},
	   "TEST_VJ_LIBS_PF=s" => \$VARS{'TEST_VJ_LIBS_PF'},
	   "TEST_LIBS_BASIC=s" => \$VARS{'TEST_LIBS_BASIC'},
	   "TEST_EXTRA_LIBS_BASIC:s" => \$VARS{'TEST_EXTRA_LIBS_BASIC'},
	   "TEST_EXTRA_LIBS_GL:s" => \$VARS{'TEST_EXTRA_LIBS_GL'},
	   "TEST_EXTRA_LIBS_PF:s" => \$VARS{'TEST_EXTRA_LIBS_PF'},
	   "srcdir=s" => \$VARS{'srcdir'}, "SUBDIRS=s" => \$VARS{'SUBDIRS'},
	   "prefix=s" => \$prefix, "startdir=s" => \$startdir,
	   "uname=s" => \$uname, "gname=s" => \$gname, "mode=s" => \$mode);

umask(002);
chdir("$startdir") or die "ERROR: Cannot chdir to $startdir: $!\n";

my ($basedir, $dir);
chop($basedir = `pwd`);

# Defaults.
my($uid, $gid, $mode_bits) = ($<, (getpwuid($<))[3], "0644");

if ( $uname ) {
    $uid = (getpwnam("$uname"))[2] or die "getpwnam($uname): $!\n";
}

if ( $gname ) {
    $gid = (getgrnam("$gname"))[2] or die "getgrnam($gname): $!\n";
}

$mode_bits = "$mode" if $mode;

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
    chown($uid, $gid, "$outfile") or die "chown: $!\n";
    chmod(oct($mode_bits), "$outfile") or die "chmod: $!\n";
    unlink("$workfile");
}

exit 0;
