#!/usr/local/bin/perl

#
# Copyright (c) 2000 Patrick L. Hartling
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The names of its contributors may not be used to endorse or promote
#    products derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
# NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#	$Id$
#

require 5.004;

use Getopt::Std;
use Text::Wrap;

# Require that all global variables be defined here.  Otherwise, they must be
# lexically scoped (my) variables.
use strict 'vars';
use vars qw(%opts %DEFAULT %SOUNDS);

# Subroutine prototypes.
sub readInput(@);
sub writeOutput();
sub readBlock($);

# =============================================================================
# Main program.
# =============================================================================

$Text::Wrap::columns = ($ENV{'COLUMNS'} ? $ENV{'COLUMNS'} : 80);

getopts('o:', \%opts);

my(@adf_files) = ();

# If there are no command line arguments left after calling getopts(), read
# the .adf files in the current directory.
if ( $#ARGV == -1 ) {
    opendir(CUR, ".") or die "ERROR: Could not open current directory: $!\n";
    @adf_files = grep(/\.adf$/, readdir(CUR));
    closedir(CUR);
}
# Otherwise, use the @ARGV array as the list of .adf files.
else {
    @adf_files = @ARGV;
}

print "And please, God, give me BEEG pectoral muscles...\n";

my $errors = readInput(@adf_files);

# If there were no errors, create the output file and say a happy prayer.
if ( $errors == 0 ) {
    my $pec_size = writeOutput();
    print "Happy happy joy joy! ($pec_size nano-meters)\n";
}
# Otherwise, complain about sloppy .adf files.
else {
    printf("Holy cow!  Your files have %d error%s!  You're sloppy.\n",
	   $errors, ($errors > 1 ? "s" : ""));
}

exit $errors;

# =============================================================================
# Subroutines follow.
# =============================================================================

# -----------------------------------------------------------------------------
# Read the contents of the input files storing them appropriately in %DEFAULT
# or %SOUNDS.
#
# PRE: The list of files are all .adf files.
#
# Arguments:
#     @files - The list of .adf files to read.
#
# Returns:
#     An integer giving the number of errors (i.e., conflicts) found in the
#     given files.
# -----------------------------------------------------------------------------
sub readInput (@) {
    my(@files) = @_;

    my $errors = 0;
    my $file;

    # Loop over each of the given input files.
    foreach $file ( @files ) {
	# If we could not open the input file, issue a warning.
	if ( ! open(INPUT, "$file") ) {
	    warn "WARNING: Could not open $file for reading: $!\n";
	    next;
	}
	# Otherwise, read and parse its input.
	else {
	    my $line;

	    while ( $line = <INPUT> ) {
		# -------------------------------------------------------------
		# This first set of matches is for settings that are
		# considered to be defaults for all .adf files.  They are
		# stored in the %DEFAULT hash.
		# -------------------------------------------------------------

		# Match a "system" block.
		if ( $line =~ /^\s*system\s+{\s*$/  &&
		     ! exists($DEFAULT{'system'}) )
		{
		    $DEFAULT{'system'} = readBlock(INPUT);
		}
		# Match a "path" block.
		elsif ( $line =~ /^\s*path\s+{\s*$/ ) {
		    $DEFAULT{'path'}{"$file"} = readBlock(INPUT);
		}
		# Match an "awEngine Default" block.
		elsif ( $line =~ /^\s*awEngine\s+Default\s+{\s*$/ &&
			! exists($DEFAULT{'awEngine_Default'}) )
		{
		    $DEFAULT{'awEngine_Default'} = readBlock(INPUT);
		}
		# Match an "awChannel Default" block.
		elsif ( $line =~ /^\s*awChannel\s+Default\s+{\s*$/ &&
			! exists($DEFAULT{'awChannel_Default'}) )
		{
		    $DEFAULT{'awChannel_Default'} = readBlock(INPUT);
		}
		# Match an "observer you" block.
		elsif ( $line =~ /^\s*observer\s+you\s+{\s*$/ &&
			! exists($DEFAULT{'observer_you'}) )
		{
		    $DEFAULT{'observer_you'} = readBlock(INPUT);
		}
		# Match an "env Default" block.
		elsif ( $line =~ /^\s*env\s+Default\s+{\s*$/ &&
			! exists($DEFAULT{'env_Default'}) )
		{
		    $DEFAULT{'env_Default'} = readBlock(INPUT);
		}
		# Match an "adfstate" block.
		elsif ( $line =~ /^\s*adfstate\s+{\s*$/ &&
			! exists($DEFAULT{'adfstate'}) )
		{
		    $DEFAULT{'adfstate'} = readBlock(INPUT);
		}

		# -------------------------------------------------------------
		# The remainder of the matches are for specific types of
		# sounds (player and awSound blocks for each sound name).
		# These blocks are stored in the %SOUNDS hash.
		# -------------------------------------------------------------

		# Match a "player <name>" block.
		elsif ( $line =~ /^\s*player\s+(\S+)\s+{\s*$/ ) {
		    my $sound = "$1";

		    # If this sound is already in %SOUNDS and already has a
		    # "player" definition, complain.
		    if ( exists($SOUNDS{"$sound"}) &&
			 exists($SOUNDS{"$sound"}{'player'}) )
		    {
			my $err_str = "Stimpy, you EEDEEOT!  You have a " .
				      "conflict in $file with awSound $sound!";
			print wrap("", "", "$err_str"), "\n";
			print "First defined in $SOUNDS{$sound}{'file'}\n\n";
			$errors++;
		    }
		    # Otherwise, store the block read in the 'player' key and
		    # record the file from which the block was read.
		    else {
			$SOUNDS{"$sound"}{'player'} = readBlock(INPUT);
			$SOUNDS{"$sound"}{'file'}   = "$file";
		    }
		}
		# Match an "awSound <name>" block.
		elsif ( $line =~ /^\s*awSound\s+(\S+)\s+{\s*$/ ) {
		    my $sound = "$1";

		    # If this sound is already in %SOUNDS and already has an
		    # "awSound" definition, complain.
		    if ( exists($SOUNDS{"$sound"}) &&
			 exists($SOUNDS{"$sound"}{'awSound'}) )
		    {
			my $err_str = "Stimpy, you EEDEEOT!  You have a " .
				      "conflict in $file with awSound $sound!";
			print wrap("", "", "$err_str"), "\n";
			print "First defined in $SOUNDS{$sound}{'file'}\n\n";
			$errors++;
		    }
		    # Otherwise, store the block read in the 'awSound' key and
		    # record the file from which the block was read.
		    else {
			$SOUNDS{"$sound"}{'awSound'} = readBlock(INPUT);
			$SOUNDS{"$sound"}{'file'}   = "$file";
		    }
		}
	    }

	    close(INPUT) or warn "WARNING: Could not close $file: $!\n";
	}
    }

    return $errors;
}

# -----------------------------------------------------------------------------
# Write the output file (either the file name given with the -o option or
# output.adf) using what is in %DEFAULT and %SOUNDS.
# -----------------------------------------------------------------------------
sub writeOutput () {
    my($key, $output_file, $line);

    # If '-o <filename>' was given on the command line, use <filename> as the
    # output file.
    if ( $opts{'o'} ) {
	$output_file = "$opts{o}";
    }
    # Otherwise, default to output.adf.
    else {
	$output_file = "output.adf";
    }

    # Create the output file or die trying.
    open(OUTPUT, "> $output_file")
	or die "ERROR: Could not create $output_file for writing: $!\n";

    # Loop over all the default settings and write those to OUTPUT first.
    foreach $key ( keys(%DEFAULT) ) {
	if ( "$key" =~ /^(\w+?)_(\w+)$/ ) {
	    print OUTPUT "$1 $2 {\n";
	}
	else {
	    print OUTPUT "$key {\n";
	}

	if ( "$key" eq "path" ) {
	    my $file;

	    foreach $file ( keys(%{$DEFAULT{"$key"}}) ) {
		foreach $line ( @{$DEFAULT{"$key"}{"$file"}} ) {
		    print OUTPUT "$line\n";
		}
	    }
	}
	else {
	    foreach $line ( @{$DEFAULT{"$key"}} ) {
		print OUTPUT "$line\n";
	    }
	}

	print OUTPUT "}\n\n";
    }

    # Loop over all the sounds in %SOUNDS and write those to the "scene main"
    # block.
    print OUTPUT "scene main {\n";

    foreach $key ( keys(%SOUNDS) ) {
	print OUTPUT "\tsound $key;\n";
    }

    print OUTPUT "}\n\n";

    # Loop over all the sounds in %SOUNDS again and write their "player" and
    # "awSound" blocks.
    foreach $key ( keys(%SOUNDS) ) {
	# Write the "player $key" block for the current sound..
	print OUTPUT "player $key {\n";
	
	foreach $line ( @{$SOUNDS{"$key"}{'player'}} ) {
	    print OUTPUT "$line\n";
	}

	print OUTPUT "}\n\n";

	# Write the "awSound $key" block for the current sound..
	print OUTPUT "awSound $key {\n";

	foreach $line ( @{$SOUNDS{"$key"}{'awSound'}} ) {
	    print OUTPUT "$line\n";
	}

	print OUTPUT "}\n\n";
    }

    close(OUTPUT) or warn "WARNING: Could not save $output_file: $!\n";

    return (stat("$output_file"))[7];
}

# -----------------------------------------------------------------------------
# Read a block of text ending with a closing curly brace alone on a line.  The
# text read is returned in an array reference.
#
# Arguments:
#     $handle - The file handle from which input is read.
# -----------------------------------------------------------------------------
sub readBlock ($) {
    my $handle = shift;

    my(@block) = ();
    my $line;

    while ( $line = <$handle> ) {
	if ( $line !~ /^\s*}\s*$/ ) {
	    chomp($line);

	    # Replace any environment variables in the .adf file with the
	    # expanded value of the environment variable.
	    if ( $line =~ /\${(\w+)}/ ) {
		my $env_var = "$1";
		$line =~ s/\${\w+}/$ENV{$env_var}/;
	    }

	    push(@block, "$line");
	}
	else {
	    last;
	}
    }

    return \@block;
}
