#!/usr/local/bin/perl

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

require 5.004;

use Getopt::Std;
use Time::Local;

# Require that all global variables be defined here.  Otherwise, they must be
# lexically scoped (my) variables.
use strict 'vars';
use vars qw(%opts);

my $status = 0;

getopts('f:i:Mmo:pv:', \%opts);

die "ERROR: No version number given!\n" unless $opts{'f'} || $opts{'v'};

my($version_string, $version_number) = ('0.0.0', '000000000');
my($major, $minor, $patch) = (0, 0, 0);

# If -v was given on the command line, use that for the version number.
if ( $opts{'v'} ) {
    $version_string = "$opts{'v'}";
    $version_string =~ /^(\d+)\.(\d+)\.(\d+)$/;

    $major          = $1;
    $minor          = $2;
    $patch          = $3;
    $version_number = sprintf("%03d%03d%03d", $major, $minor, $patch);
}
# Otherwise, extract the version number from the named version input file.
elsif ( $opts{'f'} ) {
    open(VER_FILE, "$opts{'f'}")
        or die "ERROR: Could not open version file $opts{'f'}: $!\n";

    my $line = <VER_FILE>;
    close(VER_FILE);

    if ( $line =~ /^\s*((\d+)\.(\d+)\.(\d+))/ ) {
        $version_string = "$1";
        $major          = $2;
        $minor          = $3;
        $patch          = $4;
        $version_number = sprintf("%03d%03d%03d", $major, $minor, $patch);
    }
    else {
        die "ERROR: No version number found in $opts{'f'}\n";
    }
}

if ( $opts{'o'} ) {
    if ( $opts{'i'} ) {
        open(TEMPLATE, "$opts{'i'}")
            or die "ERROR: Could not open $opts{'i'}: $!\n";

        # Concatenate the contents of the input template into a single scalar
        # variable.
        my($line, $input) = ('', '');
        while ( $line = <TEMPLATE> ) {
            $input .= "$line";
        }

        close(TEMPLATE);

        # Replace the key strings in the input file with the integer version
        # number and the version string.
        $input =~ s/#undef\s+__VJ_version/#define __VJ_version $version_number/;
        $input =~ s/#undef\s+VJ_VERSION/#define VJ_VERSION "$version_string"/;

        # If the output file already exists, read it and compare it with the
        # output we have now.  If they are different, overwrite the old file.
        # Otherwise, don't bother.
        if ( -r "$opts{'o'}" && open(INPUT, "$opts{'o'}") ) {
            my $old_file = '';

            # Read in the existing file except for the heading line added by
            # this script.
            while ( <INPUT> ) {
                next if m|^/\* $opts{'o'}\.  Generated.+?\*/$|o;
                $old_file .= "$_";
            }

            close(INPUT);

            # Print the contents of the input template with the above
            # substitutions performed to the new output file only if the new
            # file is different than the old one.
            writeFile("$opts{'o'}", "$input") unless "$old_file" eq "$input";
        }
        # If the output file does not exist, print the contents of the input
        # template with the above substitutions performed to the new output
        # file.
        else {
            writeFile("$opts{'o'}", "$input");
        }
    }
    else {
        die "ERROR: Must have input file to create output file!\n";
    }
}
# Return the major version number.
elsif ( $opts{'M'} ) {
    print "$major\n";
}
# Return the minor version number.
elsif ( $opts{'m'} ) {
    print "$minor\n";
}
# Return the patch level.
elsif ( $opts{'p'} ) {
    print "$patch\n";
}
else {
    die "ERROR: Must have output file to generate!\n";
}

exit 0;

sub writeFile ($$) {
    my $file = shift;
    my $data = shift;

    # Create the output file.
    open(OUTPUT, "> $file") or die "ERROR: Could not create $file: $!\n";

    print OUTPUT "/* $file.  Generated automatically by record-version.pl */\n";
    print OUTPUT "$data";

    close(OUTPUT) or warn "WARNING: Could not close $file: $!\n";
}
