package WebFile;

use strict qw(vars);

use vars qw(%path_subst %alias_subst %cmds);
use vars qw($install_prefix $cmt_begin $cmt_end $cmd_str $USE_HTTP_PATHS
            $USE_ABS_PATHS $USE_HTTP_PATHS_WITHIN_JIT
            $USE_ABS_PATHS_WITHIN_JIT);

use Utils;
use xmlToc;
use xmlToc_htmlTocActions;
use xmlToc_htmlBookActions;

# ============================================================================
# "Global" variables.  These are accessible to all packages but are intended
# for use by inheriting modules.
# ============================================================================
%path_subst  = ();
%alias_subst = ();

$install_prefix = 'unknown';

$cmt_begin = '<!--';
$cmt_end   = '-->';

$cmd_str = 'install-web';

# Commands.  This are used in regular expressions only and thus must be
# valid regexps.
%cmds = (
    'ignore-all'         => 'ignore',
    'ignore-tags'        => 'no-tags',
    'ignore-includes'    => 'no-includes',
    'ignore-css'         => 'no-css',
    'ignore-header'      => 'no-header',
    'ignore-footer'      => 'no-footer',
    'include-footer'     => 'common-footer',
    'ignore-block-begin' => 'block-ignore-begin',
    'ignore-block-end'   => 'block-ignore-end',
    'include'            => 'include',
    'include-toc'        => 'include-toc',
    'include-book'       => 'include-book',
);

$USE_HTTP_PATHS = 0;
$USE_ABS_PATHS  = 1;

$USE_HTTP_PATHS_WITHIN_JIT = 0;
$USE_ABS_PATHS_WITHIN_JIT  = 1;

# ============================================================================
# "Private" variables.
# ============================================================================
my $root_src_dir      = '';
my $src_dir           = '';
my $ALLOWED_RECURSION = 4;

# ============================================================================
# Public "static" methods.
# ============================================================================

sub init ($$$$$$$) {
    $cmd_str        = shift;
    $cmt_begin      = shift;
    $cmt_end        = shift;
    my $debug_val   = shift;
    $install_prefix = shift;
    $root_src_dir   = shift;
    $src_dir        = shift;

    initOutputDebug($debug_val);
    makeCommandStrings("$cmd_str");
}

sub makeCommandStrings ($) {
    my $command_str = shift;

    foreach ( keys(%cmds) ) {
        $cmds{"$_"} = "$command_str\\s+" . $cmds{"$_"};
    }
}

# loadSubstFile()
#
# Fills the %path_subst hash with tags to use
# Fills the %alias_subst hash with tags to use
# for replacing tag entries in the config files.
#
# args: filename of the subst.pl hashes
sub loadSubstFile ($) {
    my $filename = shift;

    if ( $filename ) {
        my $return;

        unless ($return = do $filename) {
            warn "couldn't parse $filename: $@" if $@;
            warn "couldn't do $filename: $!"    unless defined $return;
            warn "couldn't run $filename"       unless $return;
        }
    }

    my ($k, $v) = ('', '');

    outputDebug( "----- path_subst hash values -------------\n" );
    while ( ($k,$v) = each %path_subst ) {
        outputDebug( "$k => $v\n" );
    }
    outputDebug( "-----------------------------------------\n\n" );

    outputDebug( "----- alias_subst hash values -------------\n" );
    while ( ($k,$v) = each %alias_subst ) {
         outputDebug( "$k => $v\n" );
    }
    outputDebug( "-----------------------------------------\n" );
}

# ============================================================================
# Protected "static" methods.
# ============================================================================

# recursive-capable process includes (see other for documentation)
# arg: WebFile object
sub processStringIncludesRecursive ($;$) {
    my $string_ref        = shift;
    my $allowed_recursion = shift || $ALLOWED_RECURSION;

    outputDebug( "==> processIncludesRecursive\n" );

    ## Check includes ##
    my $found_an_include = 1;
    my $levels_of_recursion = 0;

    while ( $found_an_include == 1 ) {
#        my $prev_name = $$string_ref;
        $found_an_include = processStringIncludes($string_ref);

#        print "including file: changing $prev_name to $filename\n" if $found_an_include;

        # Count number of times we recurse.
        $levels_of_recursion = $levels_of_recursion + 1;

        if ( $levels_of_recursion > $allowed_recursion ) {
            ## bail!
            print "processStringIncludesRecursive: too much recursion " .
                  "($levels_of_recursion > $allowed_recursion)--bailing " .
                  "(you need to fix it or set the recursion level higher " .
                  "in install-web.pl...)\n";
            $found_an_include = 0;
        }
    }
}

# One process include
#
# Check for includes and do them
# returns whether or not includes did exist 0=false or 1=true
sub processStringIncludes ($) {
    my $string_ref = shift;

    outputDebug( "==> processStringIncludes\n" );

    my $includes_existed = 0;

    # While we have some more includes to deal with
    while ( $$string_ref =~ /(${cmt_begin}\s*${cmd_str}\s*include(-toc|-book)?\s*(\S*?)\s*${cmt_end})/is )
    {
        my $include_statement = $1; # what was that match?
        my $orig_filename     = $3; # Get filename from the match

        # fix filename, if it contains tag(s)
        my $expandvars_filename = "$orig_filename";
        replaceStringTagsRecursive(\$expandvars_filename, $USE_ABS_PATHS,
                                   $ALLOWED_RECURSION);
        $expandvars_filename =~ s/"//g;

        # fix the $ char so the regex doesn't think its an end-line char.
        $include_statement =~ s/\$/\\\$/gs;

#        print "\n\nMatched: incl: $include_statement\n";
#        print "Matched: orig: $orig_filename\n";
#        print "Matched: expd: $expandvars_filename\n\n";

        # ----------- TOC include ----------- #
        if ( $include_statement =~ /${cmds{'include-toc'}}/i ) {
            my $xml_file  = new XmlFile("$expandvars_filename");
            my $htmltoc_data = '';

            print "[including TOC: \"$expandvars_filename\" ";

            if ( $xml_file->load() ) {
                # "1" - jit file links are absolute because xmlToc then uses
                #       the path to  include the file
                # "0" - all other links are http, since we want the final html
                #       output
                $xml_file->filter("", $USE_HTTP_PATHS,
                                  $USE_ABS_PATHS_WITHIN_JIT);
                xmlToc_htmlTocActions::useme();
                xmlToc::traverse(\$htmltoc_data, $xml_file->getBody());
            }

            $$string_ref =~ s/${include_statement}/${htmltoc_data}/gis;
            print "]";

            $includes_existed = 1;
        }
        # ----------- BOOK include ----------- #
        elsif ( $include_statement =~ /${cmds{'include-book'}}/i ) {
            my $xml_file  = new XmlFile("$expandvars_filename");
            my $html_data = '';

            print "[including book: \"$expandvars_filename\" ";

            if ( $xml_file->load() ) {
                # "1" - jit file links are absolute because xmlToc then uses
                #       the path to include the file
                # "0" - all other links are http, since we want the final html
                #       output
                $xml_file->filter('', $USE_ABS_PATHS,
                                  $USE_ABS_PATHS_WITHIN_JIT);
                xmlToc_htmlBookActions::useme();
                xmlToc_htmlBookActions::setAction('include',
                                                  \&processStringIncludesRecursive);
                xmlToc_htmlBookActions::setAction('tag',
                                                  \&replaceStringTagsRecursive);
                xmlToc::traverse(\$html_data, $xml_file->getBody());
            }

            $$string_ref =~ s/${include_statement}/${html_data}/gis;
            print "]";

            $includes_existed = 1;
         }
         # ----------- text file include ----------- #
         else {
             my $file = new XmlFile("$expandvars_filename");
             print "[including: \"$expandvars_filename\" ";

             $file->enableFeedback();
             $file->load('include text');

             my $text = $file->getBody();

             $$string_ref =~ s/${include_statement}/${text}/is;

             print "]";

             $includes_existed = 1;
         }
    }

    return $includes_existed;
}

#  replaceStringTags Recursive version (see other for documentation)
#
sub replaceStringTagsRecursive ($$;$) {
    my $string                   = shift;
    my $file_path_replace_method = shift;
    my $allowed_recursion        = shift || $ALLOWED_RECURSION;

    outputDebug( "==> replaceStringTagsRecursive\n" );

    # fix filename, if it contains a tag
    my $found_a_tag = 1;
    my $levels_of_recursion = 0;

    while( $found_a_tag == 1 ) {
        $found_a_tag = replaceStringTags($string, $file_path_replace_method);

        # bail if too many recursions.
        $levels_of_recursion = $levels_of_recursion + 1;

        if ( $levels_of_recursion > $allowed_recursion ) {
            ## bail!
            print "replaceStringTagsRecursive: too much recursion " .
                  "($levels_of_recursion > $allowed_recursion)--bailing " .
                  "(you need to fix it or set the recursion level higher " .
                  "in install-web.pl...)\n";
            exit(0);
        }
    }

    outputDebug( "<== replaceStringTagsRecursive\n" );
}

# Replace the tags in the string contents passed to the subroutine
#
# Each tag in %path_subst is replaced with the tag value prefixed
# by the install_prefix
# Each tag in %alias_subst is replace by the tag only
#
# returns whether a tag was found 0=false 1=true
sub replaceStringTags ($$) {
    my $string                   = shift;
    my $file_path_replace_method = shift;

    outputDebug( "==> replaceStringTags\n" );

    my $found_a_tag = 0;

    my ($tag, $tag_value) = ('', '');

    # find subst tags and replace with <html_inst_dir>/<tag value>
    while ( ($tag, $tag_value) = each(%path_subst) ) {
        # see if there are any tags, if so note it, and replace them
        if ( $$string =~ /(\$)[{\(]${tag}[}\)]/gis ) {
            $found_a_tag = 1;

#            print "Checking web subst: $tag ==> $tag_value\n";

            # Look for ${tag} or $(tag)
            if ( $file_path_replace_method == $USE_HTTP_PATHS ) {
               $$string =~ s/(\$)[{(]${tag}[})]/${install_prefix}${tag_value}/gis;
            }
            else {
               my $root_dir = "$root_src_dir/$src_dir/";
               $$string =~ s/(\$)[{(]${tag}[})]/${root_dir}${tag_value}/gis;
            }
        }
    }

    # alias_subst replace
    while ( ($tag, $tag_value) = each(%alias_subst) ) {
        # see if there are any tags, if so note it, and replace them
        if ( $$string =~ /(\$)[{\(]${tag}[}\)]/gis ) {
            $found_a_tag = 1;
            $$string =~ s/(\$)[{(]${tag}[})]/${tag_value}/gis;
        }
    }

    outputDebug( "<== replaceStringTags\n" );
    return $found_a_tag;
}

# ============================================================================
# "Protected" methods.
# ============================================================================

sub allocate ($$;$) {
    my $class             = shift;
    my $filename          = shift;
    my $allowed_recursion = shift || $ALLOWED_RECURSION;

    # Create the object.
    return bless {
        'filename'          => "$filename",
        'body'              => '',
        'allowed_recursion' => $allowed_recursion,
        'feedback'          => 0
    }, $class;
}

sub load ($;$) {
    my $this   = shift;
    my $caller = shift || '';

    my $status;

    if ( open(INPUT, "$this->{'filename'}") ) {
        while ( <INPUT> ) {
            $this->{'body'} .= "$_";
            print "." if $this->{'feedback'};
        }

        close(INPUT);
        $status = 1;
    }
    else {
        warn "WARNING" . ($caller ? "($caller)" : '') .
             ": Could not open $this->{'filename'} for reading: $!\n";
        $status = 0;
    }

    return $status;
}

# recursive-capable process includes (see other for documentation)
# arg: WebFile object
sub processIncludesRecursive ($) {
    my $this = shift;

    outputDebug( "==> processIncludesRecursive\n" );

    processStringIncludesRecursive($this->getBodyRef(),
                                   $this->getAllowedRecursion());
}

#  replaceTags Recursive version (see replaceStringTags for more documentation)
#
sub replaceTagsRecursive ($$) {
    my $this                     = shift;
    my $file_path_replace_method = shift;

    outputDebug( "==> replaceTagsRecursive\n" );

    replaceStringTagsRecursive($this->getBodyRef(), $file_path_replace_method,
                               $this->getAllowedRecursion());

    outputDebug( "<== replaceTagsRecursive\n" );
}

sub enableFeedback ($) {
    my $this = shift;

    $this->{'feedback'} = 1;
}

sub disableFeedback ($) {
    my $this = shift;

    $this->{'feedback'} = 0;
}

sub getBody ($) {
    my $this = shift;

    return $this->{'body'};
}

sub getBodyRef ($) {
    my $this = shift;

    return \$this->{'body'};
}

sub getAllowedRecursion ($) {
    my $this = shift;

    return $this->{'allowed_recursion'};
}

1;
