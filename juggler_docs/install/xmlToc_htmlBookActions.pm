package xmlToc_htmlBookActions;

use strict qw(vars);
use vars qw(@ISA @EXPORT);

@ISA = qw(Exporter);

require 5.003;
require Exporter;

##################################################
# How to use this package:
# .i.e:
# use xmlToc;
# use xmlToc_htmlBookActions;
# xmlToc_htmlBookActions::useme();
# xmlToc::load(\$xmldata, "filename.xml");
# xmlToc::traverse(\$htmldata, $xmldata);
# print $htmldata;
##################################################

@EXPORT = qw(useme setAction pushFolder_action popFolder_action pushFont_action
             popFont_action item_action defaults_action treecontrol_action);

# Prototypes.
sub number($$$$);

my(%book) = ();

$book{'title'}      = '';
$book{'files'}      = [];
$book{'chapters'}   = [];
$book{'appendices'} = [];

my $new_page_str = '<!--NewPage-->';
my $html_comment_begin = '<!--';
my $html_comment_end   = '-->';

# Commands to ignore a block when generating the book.
my $book_ignore_b = 'book-ignore-begin';
my $book_ignore_e = 'book-ignore-end';

sub dummyAction ($) {
}

# ============================================================================
# Public interface functions.
# ============================================================================

sub useme () {
    xmlToc::setAction("folder", \&xmlToc_htmlBookActions::pushFolder_action);
    xmlToc::setAction("/folder", \&xmlToc_htmlBookActions::popFolder_action);
    xmlToc::setAction("item", \&xmlToc_htmlBookActions::item_action);
    xmlToc::setAction("font", \&xmlToc_htmlBookActions::pushFont_action);
    xmlToc::setAction("/font", \&xmlToc_htmlBookActions::popFont_action);
    xmlToc::setAction("idefault", \&xmlToc_htmlBookActions::defaults_action);
    xmlToc::setAction("treecontrol",
                      \&xmlToc_htmlBookActions::treecontrol_action);
}

my $include_processor = \&dummyAction;    # Callback for processing includes
my $tag_processor     = \&dummyAction;    # Callback for processing tags

sub setAction ($$) {
    my $action_name = shift;
    my $callback    = shift;

    my $status = 1;

    SWITCH: {
        if ( "$action_name" eq "include" ) {
            $include_processor = $callback;
            last SWITCH;
        }

        if ( "$action_name" eq "tag" ) {
            $tag_processor = $callback;
            last SWITCH;
        }

        # Unknown value in $action_name--return failure status.
        $status = 0;
    }

    return $status;
}

# ============================================================================
# XML parsing callbacks follow.
# ============================================================================

sub pushFolder_action ($$$$$) {
    my $xmlToc_data_out       = shift; # reference to the data
    my $whatAmI               = shift;
    my $myParams              = shift;
    my $any_content_inbetween = shift;
    my $indent_level          = shift;

    $myParams =~ /title[ ]*=[ ]*"(.*?)"/s;
    my $title = "$1";

    outputIndents($xmlToc_data_out, $indent_level);

    if ( $whatAmI =~ /rootfolder/ ) {
        $book{'title'}         = "$title";
        @{$book{'files'}}      = ();
        @{$book{'chapters'}}   = ();
        @{$book{'appendices'}} = ();
    }

    if ( $myParams =~ /link\s*=\s*"(.*?)"/si ) {
        push(@{$book{'files'}}, "$1");
    }

    outputIndents($xmlToc_data_out, $indent_level);
}

sub popFolder_action ($$$$$) {
    my $xmlToc_data_out       = shift; # reference to the data
    my $whatAmI               = shift;
    my $myParams              = shift;
    my $any_content_inbetween = shift;
    my $indent_level          = shift;

    outputIndents($xmlToc_data_out, $indent_level);

    if ( $whatAmI =~ /rootfolder/ ) {
        # Loop over all the HTML files and process each one.
        foreach ( @{$book{'files'}} ) {
            print "+";
            processFile("$_");
        }

        # If this book has any chapters, concatenate them.  The files will
        # already have been processed and stored in the book's chapters array
        # by processFile().
        if ( $#{$book{'chapters'}} > -1 ) {
            my $chapter_count = 1;

            foreach ( @{$book{'chapters'}} ) {
                $$xmlToc_data_out .= number(\$chapter_count, "$_", 'Chapter',
                                            'h1');
            }
        }

        # If this book has any appendices, make an Appendices section and
        # append them.  The files will already have been processed and
        # stored in the book's appendices array by processFile().
        if ( $#{$book{'appendices'}} > -1 ) {
            $$xmlToc_data_out .= "$new_page_str\n<H1>Appendices</H1>\n\n";

            my $appendix_count = 1;

            foreach ( @{$book{'appendices'}} ) {
                $$xmlToc_data_out .= number(\$appendix_count, "$_", 'Appendix',
                                            'h1');
            }
        }
    }
}

sub pushFont_action ($$$$$) {
   my $xmlToc_data_out       = shift; # reference to the data
   my $whatAmI               = shift;
   my $myParams              = shift;
   my $any_content_inbetween = shift;
   my $indent_level          = shift;

   outputIndents($xmlToc_data_out, $indent_level);
}

sub popFont_action ($$$$$) {
    my $xmlToc_data_out       = shift; # reference to the data
    my $whatAmI               = shift;
    my $myParams              = shift;
    my $any_content_inbetween = shift;
    my $indent_level          = shift;

    outputIndents($xmlToc_data_out, $indent_level);
}

sub item_action ($$$$$) {
    my $xmlToc_data_out       = shift; # reference to the data
    my $whatAmI               = shift;
    my $myParams              = shift;
    my $any_content_inbetween = shift;
    my $indent_level          = shift;

    $myParams =~ /title[ ]*=[ ]*"(.*?)"/s;
    my $title = "$1";

    my $link = '';

    if ( $myParams =~ /link[ ]*=[ ]*"(.*?)"/s ) {
        $link = "$1";
        push(@{$book{'files'}}, "$link");
    }

    if ( $myParams =~ /jit[ ]*=[ ]*"(.*?)"/s ) {
        my $jitfolder = "";
        xmlToc::load(\$jitfolder, $1);
        xmlToc::traverse($xmlToc_data_out, $jitfolder);
    }
}

sub defaults_action ($$$$$) {
}

sub treecontrol_action($$$$$) {
}

# helper func for outputting indentations
# (takes an integer for num of indentations)
sub outputIndents ($$) {
    my $output_ref   = shift; #reference to data
    my $indent_level = shift;
    my $x            = 0;

    while ( $x < $indent_level ) {
        $x = $x + 1;
        $$output_ref .= "   "; # 3 spaces == one tab
    }
}

sub processFile ($) {
    my $file = shift;

    loadHTML("$file", 0);
}

sub loadHTML ($$$) {
    my $input_file   = shift;
    my $use_feedback = shift;

    my $status = 1;

    if ( open(INPUT, "$input_file") ) {
        my $input;
        while ( <INPUT> ) {
            $input .= $_;
            print "." if $use_feedback;
        }

        close(INPUT) or warn "WARNING: Cannot close $input_file: $!\n";

        # Extract only the content between the opening and closing body tags.
        $input =~ /<\s*body.*?>(.*?)<\s*\/body\s*>/is;
        my $body = "$1";

        filterHTML(\$body);

        # Strip out tables of contents.  We will let the HTML -> PostScript
        # or HTML -> PDF converter make the TOC.
        $body =~ s/${html_comment_begin}\s+install-web\s+toc-begin\s*${html_comment_end}.*?${html_comment_begin}\s+install-web\s+toc-end\s*${html_comment_end}//ogis;

        # Strip out blocks to be ignored in book format.
        $body =~ s/${html_comment_begin}\s+install-web\s+${book_ignore_b}\s*${html_comment_end}.*?${html_comment_begin}\s+install-web\s+${book_ignore_e}\s*${html_comment_end}//ogis;

        # At this point, the HTML is suitable for appending.

        # If this file is flagged as an appendix, move its contents to the
        # book's appendices.
        if ( $body =~ /${html_comment_begin}\s+install-web\s+appendix\s*${html_comment_end}/ois ) {
            push(@{$book{'appendices'}}, "$body");
        }
        # Otherwise, put the file contents in the chapter array.
        else {
            push(@{$book{'chapters'}}, "$body");
        }
    }
    else {
        warn "WARNING: Could not open $input_file for reading: $!\n";
        $status = 0;
    }

    return $status;
}

sub filterHTML ($) {
    my $file_contents = shift;

    my $ignore_all_str = 'install-web ignore';
    my $ignore_tags_str = 'install-web no-tags';
    my $ignore_includes_str = 'install-web no-includes';
    my $ignore_css_str = 'install-web no-css';
    my $ignore_header_str = 'install-web no-header';
    my $ignore_footer_str = 'install-web no-footer';
    my $include_footer_str = 'install-web common-footer';

    ### Check for IGNORE ALL ####
    if ( $$file_contents =~ m/$ignore_all_str/is ) {
        print "ignoring file.\n";
    }
    else {
        ############### search and replace (tags and includes) ################
        #
        if ( $$file_contents =~ /${html_comment_begin}\s*?$ignore_includes_str\s*?${html_comment_end}/ois )
        {
            print "[ignoring includes...]";
        }
        else {
            &$include_processor($file_contents);
        }

        if ( $$file_contents =~ /${html_comment_begin}\s*?$ignore_tags_str\s*?${html_comment_end}/ois )
        {
            print "[ignoring tags...]";
        }
        else {
            &$tag_processor($file_contents, 0);
        }
        #
        ############### end of search and replace (tags and includes) #########
    }
}

sub number ($$$$) {
    my $count_ref    = shift;
    my $input_text   = shift;
    my $section_name = shift;
    my $section_tag  = shift;

    my $numbered_text = '';

    # Use the first letter of the section name for status output.
    $section_name =~ /^(\w)/;
    my $output_char = "$1";

    # If this section contains the text indicating the started of a new
    # section, insert "Chapter X:" before the title of the section where X is
    # the current value of $$count_ref.  This is accomplished by looping over
    # the input text and inserting numbers before each title.  The new text is
    # reconstructed in the $numbered_text variable.
    while ( $input_text =~ /(<${section_tag}.*?>)(\s*<.+?>\s*)*(.*?)(<\/${section_tag}>)/is ) {
        $numbered_text .= "$`$1$2$section_name $$count_ref: $3$4";
        $input_text = $';
        $$count_ref++;
        print "$output_char";
    }

    # Append whatever was left after the last successful match.  If there was
    # no match, this just puts the full input text in $numbered_text.
    $numbered_text .= "$input_text";

    return "$numbered_text";
}

1;
