#
# Copyright (c) 1998 Patrick L. Hartling
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
#	RecurseDir.pm,v 1.7 2000/10/31 16:20:53 patrick Exp
#

# =============================================================================
# This is a silly little module for doing directory recursion.
# =============================================================================

package RecurseDir;

require Exporter;

@ISA = qw(Exporter);
@EXPORT = qw(recurseDir setRecurseAction);

my $caller = caller();
my $rec_func = \&{"${caller}::recurseFunc"};

sub setRecurseAction ($) {
    $rec_func = shift;
}

my(@dirstack) = ();

sub recurseDir ($) {
    my $dirname = shift;

    # This if we are just starting (i.e., @dirstack is empty) and the
    # directory at which all recusrion is rooted is something other than the
    # current directory, add its name to @dirstack.
    if ( $#dirstack == -1 && "$dirname" ne "." ) {
	push(@dirstack, "$dirname");
    }

    # Don't bother chdir()'ing to the current directory and printing an
    # empty directory stack.
    if ( $#dirstack != -1 ) {
	chdir("$dirname");

	print("==> ", join("/", @dirstack), "\n") if $print_cur_dir;
    }

    opendir(DIR, ".");
    my(@files) = sort(readdir(DIR));
    closedir(DIR);

    my $file;
    foreach $file ( @files ) {
	next if $file =~ /^\.\.?$/;

	next if -l "$file" && $skip_symlinks;

	if ( -d "$file" ) {
	    push(@dirstack, "$file");
	    recurseDir("$file");
	    chdir("..");
	    print("<== ", join("/", @dirstack), "\n") if $print_cur_dir;
	    pop(@dirstack);
	} else {
	    # Pass &$rec_func only the current file name.
	    if ( $pass_rec_func_cur_file ) {
		&$rec_func("$file");
	    }
	    # Pass &$rec_func the current file name and the current directory
	    # stack.
	    elsif ( $pass_rec_func_cur_file_dir ) {
		&$rec_func("$file", join("/", @dirstack));
	    }
	    # Pass &$rec_func nothing.
	    else {
		&$rec_func();
	    }
	}
    }
}

1;
