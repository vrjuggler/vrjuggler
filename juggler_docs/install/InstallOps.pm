#
#  File:    $RCSfile$
#  Date modified:  $Date$
#  Version:    $Revision$
#
#
#                                 VR Juggler
#                                     by
#                               Allen Bierbaum
#                              Christopher Just
#                              Patrick Hartling
#                             Carolina Cruz-Neira
#                                Albert Baker
#
#                          Copyright - 1997,1998,1999
#                 Iowa State University Research Foundation, Inc.
#                             All Rights Reserved
#

# -----------------------------------------------------------------------------
# Installation operations Perl 5 module.  This module defines the following
# subroutines for use in the Perl code that uses this.  The subroutines are
# (in alphabetical order):
#
#     - recurseDir(): Recurse through the given directory tree.
#     - newDir(): Create a new directory in a given directory tree.
#     - installFile(): Install a given file with specified permissions to a
#       destination directory.
#     - replaceTags(): Replace tags of the form "@...@" in the given file
#       with known replacment values.
# -----------------------------------------------------------------------------

package InstallOps;

require 5.003;

use File::Basename;
use File::Copy;
use File::Path;

#use strict;

require Exporter;

@ISA = qw(Exporter);
@EXPORT = qw(recurseDir newDir installFile replaceTags);

# List of directories that we have recursed into so far along the way
# NOTE: The program must push on the source directory to start from
# so that recurse directory has somethere to start from
@dirstack = ();


my $caller = caller();

# -----------------------------------------------------------------------------
# Recurse through the given directory tree starting at $start_dir.  If a file
# is encountered, run &main::recurseAction() on that file.  The new directory
# tree that will be created is rooted at $base_dest_dir.  This routine
# requires that the calling package define a subroutine called
# "recurseAction()" that defines what actions to take when a normal file is
# encountered during the recursion process.
#
# recurseAction()
#     syntax:
#        recurseAction($curfile, $full_src_path, $full_dest_path, $rel_path)
#
# arguments:
#        rel_path: The path from the root of the recursion to the directory the file is in
#
#
# Syntax:
#     recurseDir("$start_dir", "$base_inst_dir");
#
# Arguments:
#         $start_dir: The name of the directory from which the recursion
#                     begins.
#     $base_dest_dir: The base directory to which the existing directory
#                     tree will be installed. NOTE: The same for all recursive calls
# -----------------------------------------------------------------------------
sub recurseDir ($$@) {
    my $start_dir = shift;
    my $base_dest_dir = shift;
    my @skip_dirs = @_;

    # If the dir stack does not have any entries, then
    # it is the first call, so wave $start_dir as the current recursion
    # Push the source directory onto the InstallOps module's internal directory
    if (scalar(@dirstack) == 0)
    { push(@dirstack, "$start_dir"); }

    # Save the current directory.
    my $prevdir;
    chop($prevdir = `pwd`);

    # get into the new start directory to start up
    chdir("$start_dir") or die "ERROR: Cannot chdir to $start_dir: $!\n";

    opendir(SRCDIR, ".");
      my(@files) = readdir(SRCDIR);
    closedir(SRCDIR);

    my $curfile;

    foreach $curfile ( @files )
    {
      next if $curfile =~ /^\.\.?$/;      # Skip . and ..

      # $curfile is a directory.
      if ( -d "$curfile" )
      {
          ## next if "$curfile" eq "CVS"; # SKip CVS directories
          if (grep(/^$curfile/,@skip_dirs) > 0)   # Skip directories in skip dir
          {
             print "SKIPPING DIR: ", $curfile, "\n";
             next;
          }

          print "Processing DIRECTORY: $curfile \n";

          #newDir("$base_inst_dir", "$curfile");      ### Doesn't work beceuase base_inst_dir is never set
          push(@dirstack, "$curfile");
          recurseDir("$curfile",$base_dest_dir,@skip_dirs);               # go into the directory
          pop(@dirstack);
      }
      # $curfile is something other than a directory (most likely a normal
      # file).
      else
      {
          # Need inst_path
          # src_file w/path
          my $root = $dirstack[0];
          $dirstack[0] = ".";                               # Get rid of the root path for now
          my $rel_src_path = join('/', @dirstack);          # Path to the src file area
          my $full_dest_path = "$base_dest_dir/$rel_src_path";        # Path to the destination install area
          $dirstack[0] = "$root";
          my $full_src_path = join('/', @dirstack);         # get Full path to the src file

         &{"${caller}::recurseAction"}("$curfile", $full_src_path, $full_dest_path, $rel_src_path );
      }
    }

    # Go back to the previous directory so as not to intrude upon the actions
    # of the caller.
    chdir("$prevdir");
}

# -----------------------------------------------------------------------------
# Add a new directory ($newdir) to the directory tree rooted at $base_dir.
#
# Syntax:
#     newDir("$base_dir", "$newdir");
#
# Arguments:
#     $base_dir: The base of the directory tree to which the new directory
#                will be added.
#       $newdir: The new directory to add.
# -----------------------------------------------------------------------------
sub newDir ($$) {
    my $base_dir = shift;
    my $newdir = shift;

    # Save the current directory.
    my $prevdir;
    chop($prevdir = `pwd`);

    # As long as $newdir does not exist, use mkpath() to create it.
    if ( ! -d "$newdir" )
    {
      chdir("$base_dir")
         or die "newDir(): WARNING: Could not chdir to $base_dir: $!\n";

      umask(002);
      mkpath("$newdir", 0, 0755)
         or warn "newDir(): WARNING: Could not make $newdir: $!\n";
    }

    # Go back to the previous directory so as not to intrude upon the actions
    # of the caller.
    chdir("$prevdir");
}

# -----------------------------------------------------------------------------
# Install the given file with the specified permissions to the destination
# directory.
#
# NOTE: Assumes that the given filename is relative the current path.
#
# Syntax:
#     $filename: The file to install.
#          $uid: The ID of the user who will own the file.
#          $gid: The ID of the group that will own the file.
#         $mode: The mode bits for the file.
#     $dest_dir: The destination directory for the file.
#     $ src_dir: The src directory for the file.
# -----------------------------------------------------------------------------
sub installFile ($$$$$$) {
    my $filename = shift;
    my $uid = shift;
    my $gid = shift;
    my $mode = shift;
    my $dest_dir = shift;
    my $src_dir = shift;

    # get relative path just for kicks to show in print out
    $dirstack[0] = ".";                               # Get rid of the root path for now
    my $rel_src_path = join('/', @dirstack);          # Path to the src file area
    $dirstack[0] = "$root";

    # Give some info
    ##print "$rel_src_path/$filename -> $dest_dir/$filename\n";

    umask(002);
    mkpath("$dest_dir", 0, 0755) or "mkpath: $!\n";
    copy("$filename", "$dest_dir") or "copy: $!\n";
    chown($uid, $gid, "$dest_dir/$filename") or die "DIE: chown: $dest_dir/$filename: $!\n";
    chmod(oct($mode), "$dest_dir/$filename") or die "DIE: chmod: $dest_dir/$filename: $!\n";
}

# -----------------------------------------------------------------------------
# Replace tags of the form "@...@" found in $infile with known values that
# will be written to $outfile.  The values for the tags are given in the
# %VARS hash which is indexed by the tag name (without the surrounding @'s).
#
# Syntax:
#     $count = replaceTags("$infile", %VARS);
#
# Aguments:
#      $infile: The input file to be read.
#        %VARS: The hash of tags with replacement values.
#
# Returns:
#     The count of tags replaced in the input file (>= 0) on success.
#     -1 on error.
# -----------------------------------------------------------------------------
sub replaceTags ($%) {
    my $infile = shift;
    my(%VARS) = @_;

    my $count = 0;
    my $tag;

    my $progname = (fileparse("$0"))[0];
    my $outfile = "/tmp/$progname.$$";

    foreach $tag ( keys(%VARS) ) {
   if ( ! open(INPUT, "$infile") ) {
       warn "WARNING: Cannot read from $infile: $!\n";
       return -1;
   }

   if ( ! open(OUTPUT, "> $outfile") ) {
       warn "WARNING: Cannot create $outfile: $!\n";
       close(INPUT) or warn "WARNING: Cannot close $infile: $!\n";
       return -1;
   }

   my $line;

   while ( $line = <INPUT> ) {
       $count++ if $line =~ /\@$tag\@/;
       $line =~ s/\@$tag\@/$VARS{"$tag"}/g;
       print OUTPUT "$line";
   }

   close(OUTPUT) or warn "WARNING: Cannot save $outfile: $!\n";
   close(INPUT) or warn "WARNING: Cannot close $infile: $!\n";

   copy("$outfile", "$infile");
    }

    unlink("$outfile");

    return $count;
}

1;
