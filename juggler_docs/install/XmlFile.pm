package XmlFile;

use strict qw(vars);
use vars qw(@ISA);

use Utils;

@ISA = qw(WebFile);

require WebFile;


sub new ($$) {
    my ($class, $filename) = @_;

    return $class->allocate("$filename");
}

sub DESTROY ($) {
    my $this = shift;

    return 1;
}

# syntax:
#     xmlFilter($filecontents, $rel_path)
# args:
#     $filecontents (string reference): The contents of the file to run the
#                                       filter on
#     $relpath (string): The relative path to the given file-contents from
#                        the "root" web dir
#
sub filter ($$$$) {
    my $this                           = shift;
    my $rel_path                       = shift;
    my $path_replace_method            = shift;
    my $path_replace_method_within_jit = shift;
 
    outputDebug( "==> xmlFilter\n" );

    $this->processIncludesRecursive();

    # ---- PATH REPLACE ----
    # If it is not a tag, then do a simple replace
    # linkurl = "somewhere/something.html" ==> linkurl="webprefix/rel_path/somewhere/something.html"
    # NOTE: We don't want to replace links that have TAGS in them.
    # NOTE2: This means that all other types of links (rlink|jit|link) must
    #        have paths relative to the urllink OR that use a ${TAG} format
    #        for the paths.
#    $this->{'body'} =~ s/link(.*?)=(.*?)\"(?!\$)/link=\"$WebFile::install_prefix$rel_path\//gis;
#    $this->{'body'} =~ s/(rlink|jit|link)(\s*?)=(\s*?)\"(?!\$)/$1=\"$WebFile::install_prefix\/$rel_path\//gis;
    $this->{'body'} =~ s/(linkurl)(\s*)=(\s*)\"(?!\$)/\1=\"${WebFile::install_prefix}\/${rel_path}\//gis;

    ## REPLACE TAGS
    if ( $path_replace_method_within_jit != $path_replace_method ) {
        $this->replaceTagsRecursive_withinJit($path_replace_method_within_jit);
    }

    $this->replaceTagsRecursive($path_replace_method);

    # Replace whitespace
    $this->processRemoveWhitespace();

    # causes jexplorer to go catatonic...
#    $this->processRemoveNewlines();

    outputDebug( "<== xmlFilter\n" );
}

sub replaceTagsRecursive_withinJit ($$) {
    my $this                     = shift;
    my $file_path_replace_method = shift;
   
    my $whats_left = $this->{'body'};

    while ( $whats_left =~ m/jit[ ]*?=[ ]*?"(.*?)"/gis ) {
        $whats_left = $';
        my $jitTag = $1;

        # replace any tags within the jit tag
        my $jitTagFixed = $jitTag;
        WebFile::replaceStringTagsRecursive(\$jitTagFixed,
                                            $file_path_replace_method,
                                            $WebFile::ALLOWED_RECURSION);

        # change all $ to \$ so regex doesn't think they are "endline"
        # characters...
        $jitTag =~ s/\$/\\\$/gs;

        # do a search and replace on the file contents for the old jit="" tag
        # with the new jit="" tag
        $this->{'body'} =~ s/jit[ ]*?=[ ]*?"${jitTag}"/jit="${jitTagFixed}"/gs;
    }
}

# Get rid of white space lines to make the file smaller to download
#
# This function basically only looks for blank lines and deletes them.
sub processRemoveWhitespace ($) {
    my $this = shift;

    outputDebug( "==> processRemoveWhitespace\n" );

    $this->{'body'} =~ s/(\s*)\n/\n/gis;
}

sub processRemoveNewlines ($) {
   my $this = shift;

   outputDebug( "==> processRemoveNewlines\n" );
   
   # eliminate newlines
   $this->{'body'} =~ s/[\n]//gis;
}

1;
