package HtmlBook; 

require 5.003;

use strict qw(vars);
use vars qw(@ISA);

@ISA = qw(xmlToc);

# XXX: This module knows too much about install-web.  That really, really
# needs to be changed.

##################################################
# How to use this package:
# .i.e:
# use xmlToc;
# use HtmlBook;
# $html_book = new HtmlBook("filename.xml");
# $html_book->load();
# $html_book->traverse();
# print $html_book->getData();
##################################################

my $new_page_str = '<!--NewPage-->';
my $html_comment_begin = '<!--';
my $html_comment_end   = '-->';

# Commands to ignore a block when generating the book.
my $book_ignore_b = 'book-ignore-begin';
my $book_ignore_e = 'book-ignore-end';

# Prototypes for "private" methods.
sub processFile($$);
sub loadHTML($$$$);
sub filterHTML($$);
sub number($$$$);

# ============================================================================
# "Public" methods.
# ============================================================================

# ----------------------------------------------------------------------------
# Constructor.
#
# Arguments:
#     $class    - The name of the class for the new object (passed
#                 automatically).
#     $filename - The name of the XML table of contents file to be parsed and
#                 written out in book format.  This is optional and defaults
#                 to the empty string.
# ----------------------------------------------------------------------------
sub new ($;$) {
    my $class    = shift;
    my $filename = shift || '';

    my $object = $class->allocate("$filename");

    # Extend the object to include a 'book' hash table.  This hash has the
    # book title, all the files that are processed to construct it, and the
    # individual chapters and appendices that result from the processing.
    $object->{'book'} = {
        'title'      => '',
        'files'      => [],
        'chapters'   => [],
        'appendices' => []
    };

    return $object;
}

# ----------------------------------------------------------------------------
# Destructor.
# ----------------------------------------------------------------------------
sub DESTROY ($) {
    my $this = shift;

    return 1;
}

# ============================================================================
# "Public" static methods..
# ============================================================================

# ----------------------------------------------------------------------------
# Simple dummy action.  This does nothing more than serve as a placeholder.
# ----------------------------------------------------------------------------
sub dummyAction ($) {
}

# XXX: These need to be replaced with something better...
my $include_processor = \&dummyAction;    # Callback for processing includes
my $tag_processor     = \&dummyAction;    # Callback for processing tags

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
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
# "Protected" XML parsing callbacks follow.
# ============================================================================

# ----------------------------------------------------------------------------
# Action for an opening folder tag.
# ----------------------------------------------------------------------------
sub pushFolder ($$$$) {
    my $this                  = shift; # reference to the object
    my $whatAmI               = shift;
    my $myParams              = shift;
    my $any_content_inbetween = shift;

    $myParams =~ /title[ ]*=[ ]*"(.*?)"/s;
    my $title = "$1";

    $this->outputIndents();

    if ( $whatAmI =~ /rootfolder/ ) {
        $this->{'book'}{'title'}         = "$title";
        @{$this->{'book'}{'files'}}      = ();
        @{$this->{'book'}{'chapters'}}   = ();
        @{$this->{'book'}{'appendices'}} = ();
    }

    if ( $myParams =~ /link\s*=\s*"(.*?)"/si ) {
        push(@{$this->{'book'}{'files'}}, "$1");
    }

    $this->outputIndents();
}

# ----------------------------------------------------------------------------
# Action for a closing folder tag.
# ----------------------------------------------------------------------------
sub popFolder ($$$$) {
    my $this                  = shift; # reference to the object
    my $whatAmI               = shift;
    my $myParams              = shift;
    my $any_content_inbetween = shift;

    $this->outputIndents();

    if ( $whatAmI =~ /rootfolder/ ) {
        # Loop over all the HTML files and process each one.
        foreach ( @{$this->{'book'}{'files'}} ) {
            print "+" if $this->processFile("$_");
        }

        # If this book has any chapters, concatenate them.  The files will
        # already have been processed and stored in the book's chapters array
        # by processFile().
        if ( $#{$this->{'book'}{'chapters'}} > -1 ) {
            my $chapter_count = 1;

            foreach ( @{$this->{'book'}{'chapters'}} ) {
                $this->{'body'} .= number(\$chapter_count, "$_", 'Chapter',
                                          'h1');
            }
        }

        # If this book has any appendices, make an Appendices section and
        # append them.  The files will already have been processed and
        # stored in the book's appendices array by processFile().
        if ( $#{$this->{'book'}{'appendices'}} > -1 ) {
            $this->{'body'} .= "$new_page_str\n<H1>Appendices</H1>\n\n";

            my $appendix_count = 1;

            foreach ( @{$this->{'book'}{'appendices'}} ) {
                $this->{'body'} .= number(\$appendix_count, "$_", 'Appendix',
                                          'h1');
            }
        }
    }
}

# ----------------------------------------------------------------------------
# Action for handling an opening font tag.
# ----------------------------------------------------------------------------
sub pushFont ($$$$) {
    my $this                  = shift; # reference to the object
    my $whatAmI               = shift;
    my $myParams              = shift;
    my $any_content_inbetween = shift;

    $this->outputIndents();
    $this->{'body'} .= "<font $myParams>\n";
}

# ----------------------------------------------------------------------------
# Action for handling a closing font tag.
# ----------------------------------------------------------------------------
sub popFont ($$$$) {
    my $this                  = shift; # reference to the object
    my $whatAmI               = shift;
    my $myParams              = shift;
    my $any_content_inbetween = shift;

    $this->outputIndents();
    $this->{'body'} .= "</font>\n";
}

# ----------------------------------------------------------------------------
# Action for handling an item in the table of contents.
# ----------------------------------------------------------------------------
sub item ($$$$) {
    my $this                  = shift; # reference to the object
    my $whatAmI               = shift;
    my $myParams              = shift;
    my $any_content_inbetween = shift;

    $myParams =~ /title[ ]*=[ ]*"(.*?)"/s;
    my $title = "$1";

    my $link = '';

    if ( $myParams =~ /link[ ]*=[ ]*"(.*?)"/s ) {
        $link = "$1";
        push(@{$this->{'book'}{'files'}}, "$link");
    }

    if ( $myParams =~ /jit[ ]*=[ ]*"(.*?)"/s ) {
        my $jitfolder = new HtmlBook("$1");
        $jitfolder->load();
        $jitfolder->traverse();
        $this->{'body'} .= $jitfolder->getData();
    }
}

# ----------------------------------------------------------------------------
# Action for handling defaults setting in the table of contents.
# ----------------------------------------------------------------------------
sub defaults ($$$$) {
    my $this = shift;
}

# ----------------------------------------------------------------------------
# Action for handling a treecontrol tag in the table of contents.
# ----------------------------------------------------------------------------
sub treecontrol ($$$$) {
    my $this = shift;
}

# ============================================================================
# "Private" static methods.
# ============================================================================

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
sub processFile ($$) {
    my $this = shift;
    my $file = shift;

    return $this->loadHTML("$file", 0);
}

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
sub loadHTML ($$$$) {
    my $this         = shift;
    my $input_file   = shift;
    my $use_feedback = shift;

    my $status = 1;

    if ( $input_file =~ /\.html?#\w+/ ) {
        # Ignore this type of file for the time being.
        ;
    }
    elsif ( open(INPUT, "$input_file") ) {
        my $input;
        while ( <INPUT> ) {
            $input .= $_;
            print "." if $use_feedback;
        }

        close(INPUT) or warn "WARNING: Cannot close $input_file: $!\n";

        # Extract only the content between the opening and closing body tags.
        $input =~ /<\s*body.*?>(.*?)<\s*\/body\s*>/is;
        my $body = "$1";

        # Strip out blocks to be ignored in book format.  This is done here
        # to speed things up a little bit in the cases where the included
        # text would make a big difference in processing time.
        $body =~ s/${html_comment_begin}\s+install-web\s+${book_ignore_b}\s*${html_comment_end}.*?${html_comment_begin}\s+install-web\s+${book_ignore_e}\s*${html_comment_end}//ogis;

        $this->filterHTML(\$body);

        # Strip out tables of contents.  We will let the HTML -> PostScript
        # or HTML -> PDF converter make the TOC.
        $body =~ s/${html_comment_begin}\s+install-web\s+toc-begin\s*${html_comment_end}.*?${html_comment_begin}\s+install-web\s+toc-end\s*${html_comment_end}//ogis;

        # Strip out blocks to be ignored in book format again to catch
        # the blocks that were part of included files.
        $body =~ s/${html_comment_begin}\s+install-web\s+${book_ignore_b}\s*${html_comment_end}.*?${html_comment_begin}\s+install-web\s+${book_ignore_e}\s*${html_comment_end}//ogis;

        # At this point, the HTML is suitable for appending.

        # If this file is flagged as an appendix, move its contents to the
        # book's appendices.
        if ( $body =~ /${html_comment_begin}\s+install-web\s+appendix\s*${html_comment_end}/ois ) {
            push(@{$this->{'book'}{'appendices'}}, "$body");
        }
        # Otherwise, put the file contents in the chapter array.
        else {
            push(@{$this->{'book'}{'chapters'}}, "$body");
        }
    }
    else {
        warn "WARNING: Could not open $input_file for reading: $!\n";
        $status = 0;
    }

    return $status;
}

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
sub filterHTML ($$) {
    my $this          = shift;
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

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
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
    while ( $input_text =~ /(<${section_tag}.*?>)(\s*<img.+?>\s*)*(.*?)(<\/${section_tag}>)/is )
    {
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
