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

use Getopt::Std;
use strict 'vars';
use vars qw(%opts);

getopts('i:o:p:t:v:x', \%opts);

die "ERROR: No input file given!\n" unless defined($opts{'i'});
die "ERROR: No tempalte file given!\n" unless defined($opts{'t'});

my $input_file    = "$opts{'i'}";
my $template_file = "$opts{'t'}";
my $version_file  = "$opts{'v'}";

if ( ! open(INPUT, "$input_file") ) {
    die "ERROR: Could not open input file $input_file: $!\n";
}
else {
    my(%vars) = ();

    my $line   = '';

    while ( $line = <INPUT> ) {
        if ( $line =~ /^s[%,]@(\w+)@[%,](.*?)[%,]/ ) {
            $vars{"$1"} = "$2";
        }
    }

    close(INPUT) or warn "WARNING: Could not close $input_file: $!\n";

    if ( $version_file && open(INPUT, "$version_file") ) {
        my $last_ver;

        if ( $last_ver = <INPUT> ) {
            if ( $last_ver =~ /^(\d+)\.(\d+)\.(\d+) .*$/ ) {
                $vars{'MAJOR_VERSION'} = $1;
                $vars{'MINOR_VERSION'} = $2;
                $vars{'MICRO_VERSION'} = $3;
            }
        }
    }

    if ( ! open(TEMPLATE, "$template_file") ) {
    }
    else {
        my $input = '';

        while ( <TEMPLATE> ) {
            $input .= "$_";
        }

        my $output_file = '';
        my $output_handle;

        if ( defined($opts{'o'}) ) {
            $output_file = "$opts{'o'}";

            open(OUTPUT, "> $output_file") or
                die "ERROR: Could not create $output_file: $!\n";
            $output_handle = OUTPUT;
        }
        else {
            $output_handle = stdout;
        }

        my $key = '';
        foreach $key ( keys(%vars) ) {
            $input =~ s/\@${key}\@/$vars{$key}/g;
        }

        # Replace the preamble block with whatever was given as an argument to
        # the -p option.  If nothing was given, the preamble will be removed.
        # This is probably what the user wants.
        $input =~ s/\%\{.*?\%\}/$opts{'p'}/s;

        print $output_handle "$input";

        close($output_handle) or warn "WARNING: Could not close output: $!\n";

        # Set the execute bit on the output file if -x was given.
        chmod(0755, "$output_file") if defined($opts{'x'});
    }
}

exit 0;
