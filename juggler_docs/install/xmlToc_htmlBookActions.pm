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

@EXPORT = qw(useme pushFolder_action popFolder_action pushFont_action
             popFont_action item_action defaults_action treecontrol_action);

my(%book) = ();

$book{'title'}      = '';
$book{'files'}      = [];
$book{'chapters'}   = [];
$book{'appendices'} = [];

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
            foreach ( @{$book{'chapters'}} ) {
                print 'c';
                $$xmlToc_data_out .= "$_";
            }
        }

        # If this book has any appendices, make an Appendices section and
        # append them.  The files will already have been processed and
        # stored in the book's appendices array by processFile().
        if ( $#{$book{'appendices'}} > -1 ) {
            $$xmlToc_data_out .= "<!--NewPage-->\n<H1>Appendices</H1>\n\n";

            foreach ( @{$book{'appendices'}} ) {
                print '@';
                $$xmlToc_data_out .= "$_";
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

        my $html_comment_begin = $InstallWeb::html_comment_begin;
        my $html_comment_end   = $InstallWeb::html_comment_end;

        # Strip out tables of contents.  We will let the HTML -> PostScript
        # or HTML -> PDF converted make the TOC.
        $body =~ s/${html_comment_begin}\s+install-web\s+toc-begin\s*${html_comment_end}.*?${html_comment_begin}\s+install-web\s+toc-end\s*${html_comment_end}//ogis;

        filterHTML(\$body);

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

    my $html_comment_begin = $InstallWeb::html_comment_begin;
    my $html_comment_end   = $InstallWeb::html_comment_end;

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
            InstallWeb::processIncludesRecursive($file_contents);
        }

        if ( $$file_contents =~ /${html_comment_begin}\s*?$ignore_tags_str\s*?${html_comment_end}/ois )
        {
            print "[ignoring tags...]";
        }
        else {
            InstallWeb::replaceTagsRecursive($file_contents, 0);
        }
        #
        ############### end of search and replace (tags and includes) #########
    }
}

1;
