#!/usr/local/bin/perl

#
# VRJuggler
#   Copyright (C) 1997,1998,1999,2000
#   Iowa State University Research Foundation, Inc.
#   All Rights Reserved
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

require 5.004;

use Getopt::Std;
use Time::Local;

# Require that all global variables be defined here.  Otherwise, they must be
# lexically scoped (my) variables.
use strict 'vars';
use vars qw(%opts);

my $status = 0;

getopts('i:o:v:', \%opts);

die "ERROR: No version number given!\n" unless $opts{'v'};

my $version_string = "$opts{'v'}";
$version_string =~ /^(\d+)\.(\d+)\.(\d+)$/;
my $version_number = sprintf("%03d%03d%03d", $1, $2, $3);

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

        # Create the output file.
        open(OUTPUT, "> $opts{'o'}")
            or die "ERROR: Could not create $opts{'o'}: $!\n";

        print OUTPUT "/* $opts{'o'}.  Generated automatically by " .
                     "record-version.pl */\n";
        # Print the contents of the input template with the above
        # substitutions performed to the new output file.
        print OUTPUT "$input";
        close(OUTPUT) or warn "WARNING: Could not close $opts{'o'}: $!\n";
    }
    else {
        die "ERROR: Must have input file to create output file!\n";
    }
}
else {
    die "ERROR: Must have output file to generate!\n";
}

exit 0;
