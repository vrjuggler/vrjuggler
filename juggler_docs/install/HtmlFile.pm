package HtmlFile;

use strict qw(vars);
use vars qw(@ISA @EXPORT);

use Utils;

@ISA = (WebFile);

require WebFile;
require Exporter;

@EXPORT = qw(init);


# HTML tags.
my $head_tag_begin = "<head>";
my $head_tag_end   = "<\/head>";
my $html_tag_begin = "<html>";
my $body_tag_end   = "<\/body>";

my $css_header  = '';
my $html_header = '';
my $html_footer = '';


#  Initialize the HTML filter that we will use
#
# syntax:
#     initHtmlFilter($css_header_file_name, $header_file_name, $footer_file_name)
# args:
#     css_header_file_name: The name of the file with the css information to copy
#     header_file_name: The name of the file with the header information to copy
#     footer_file_name: The name of the file with the footer information to copy
#
sub init ($$$) {
    my $css_header_file_name = shift;
    my $header_file_name     = shift;
    my $footer_file_name     = shift;

    #### --- GET CSS HEADER --- ###
    if ( ! open(CSS_FILE, "$css_header_file_name") ) {
        warn "WARNING: Cannot read HTML CSS Data from $css_header_file_name: $!\n";
        print stderr "WARNING: Did you specify a file?????\n;";
        return -1;
    }

    while ( <CSS_FILE> ) {
        $css_header .= $_;
    }

    outputDebug("CSS Header: $css_header_file_name\n------- begin --------\n" .
                $css_header . "\n------- end ----------\n");

    #### --- GET HTML HEADER --- ####
    if ( ! open(HTML_HEADER, "$header_file_name") ) {
        warn "WARNING: Cannot read HTML Header info from $header_file_name: $!\n";
        print stderr "WARNING: Did you specify a file?????\n;";
        return -1;
    }

    while ( <HTML_HEADER> ) {
       $html_header .= $_;
    }

    outputDebug("HTML Header: $header_file_name\n------- begin --------\n" .
                $html_header . "\n------- end ----------\n" );

    #### --- GET HTML FOOTER --- ####
    if ( ! open(HTML_FOOTER, "$footer_file_name") ) {
        warn "WARNING: Cannot read HTML Footer info from $footer_file_name: $!\n";
        print stderr "WARNING: Did you specify a file?????\n;";
        return -1;
    }

    while ( <HTML_FOOTER> ) {
       $html_footer .= $_;
    }

    outputDebug("HTML Footer: $footer_file_name\n------- begin --------\n" .
                $html_footer . "\n------- end ----------\n");
}

# ============================================================================
# ============================================================================

sub new ($$) {
    my ($class, $filename) = @_;

    return $class->allocate("$filename");
}

sub DESTROY ($) {
    my $this = shift;

    return 1;
}

sub filter ($) {
    my $this = shift;

    outputDebug( "==> htmlFilter\n" );

    ### Check for IGNORE ALL ####
    if ( $this->{'body'} =~ m/${WebFile::cmds{'ignore-all'}}\s+${WebFile::cmt_end}/is )
    {
        print "ignoring file.\n";
    }
    else {
        # Strip out ignored blocks of text.  This is done here to remove large
        # blocks that might slow down processing time.
        $this->{'body'} =~ s/${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-block-begin'}}\s*${WebFile::cmt_end}.*?${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-block-end'}}\s*${WebFile::cmt_end}//gis;

        ############### search and replace (tags and includes) ################
        #
        if ( $this->{'body'} =~ /${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-includes'}}\s*${WebFile::cmt_end}/is )
        {
            print "[ignoring includes...]";
        }
        else {
            $this->processIncludesRecursive();
        }

        if ( $this->{'body'} =~ /${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-tags'}}\s*${WebFile::cmt_end}/is )
        {
            print "[ignoring tags...]";
        }
        else {
            $this->replaceTagsRecursive($WebFile::USE_HTTP_PATHS);
        }
        #
        ############# end of search and replace (tags and includes) ###########

        ########## begin custom html commands ###############
        #

        # ----------- HEADER INSERT ----------- #
        # Insert header
        # Look for <BODY> and put it right after that
        if ( $this->{'body'} =~ /${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-header'}}\s*${WebFile::cmt_end}/is )  ### Check for IGNORE HEADER ####
        {
            print "[ignoring header...]";
        }
        else {
            $this->{'body'} =~ s/(${head_tag_end})/\n${html_header}\n\1\n/is;
        }

        # ----------- CSS HEADER INSERT ----------- #
        # if there is </head> then insert before it
        # if not, then look for <html> and insert after it with
        # <head>...</head> around it
        if ( $this->{'body'} =~ /${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-css'}}\s*${WebFile::cmt_end}/is )  ### Check for IGNORE HEADER ####
        {
            print "[ignoring stylesheet...]";
        }
        else {
            if ( $this->{'body'} =~ /${head_tag_end}/ois ) {
                $this->{'body'} =~ s/(${head_tag_end})/\n${css_header}\n\1\n/is;
#                print "added css (<head>)...";
            }
            elsif ( $this->{'body'} =~ /${html_tag_begin}/ois ) {
                $this->{'body'} =~ s/(${html_tag_begin})/\1\n${head_tag_begin}\n${css_header}\n${head_tag_end}\n/is;
#                print "added css (<html>)...";
            }
            else {
#                print "did NOT add css...";
            }

            # ------ Edit the CSS link path ----- #
            # ASSERT: The path in the css file is relative to the
            #         root directory of the web install
            $this->{'body'} =~ s/<link(.*?)\"stylesheet\"(.*?)href(.*?)\"/$&$WebFile::install_prefix/gis;
        }

        # ----------- FOOTER INSERT ----------- #
        # Insert footer
        # Look for </BODY> and put it right before that
        if ( $this->{'body'} =~ /${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-footer'}}\s*${WebFile::cmt_end}/is )  ### Check for IGNORE FOOTER ####
        {
            print "[ignoring footer...]";
        }
        else {
            if ( $this->{'body'} =~ /${WebFile::cmt_begin}${WebFile::cmds{'include-footer'}}${WebFile::cmt_end}/is )
            {
                print "[Putting footer in custom place...]";
                $this->{'body'} =~ s/${WebFile::cmt_begin}${WebFile::cmds{'include-footer'}}${WebFile::cmt_end}/\n${html_footer}\n\1\n/is;
            }
            else {
               $this->{'body'} =~ s/(${body_tag_end})/\n${html_footer}\n\1\n/is;
            }
        }
        #
        ########## done, with custom html commands ###############


        ############### search and replace (tags and includes) ################
        #
        if ( $this->{'body'} =~ /${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-includes'}}\s*${WebFile::cmt_end}/is )
        {
            print "[ignoring includes...]";
        }
        else {
            $this->processIncludesRecursive();
        }

        if ( $this->{'body'} =~ /${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-tags'}}\s*${WebFile::cmt_end}/is )
        {
            print "[ignoring tags...]";
        }
        else {
            $this->replaceTagsRecursive($WebFile::USE_HTTP_PATHS);
        }
        #
        ############# end of search and replace (tags and includes) ###########

        # Strip out ignored blocks of text again to make sure we get those that
        # were part of included files.
        $this->{'body'} =~ s/${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-block-begin'}}\s*${WebFile::cmt_end}.*?${WebFile::cmt_begin}\s*${WebFile::cmds{'ignore-block-end'}}\s*${WebFile::cmt_end}//gis;
    }
}

1;
