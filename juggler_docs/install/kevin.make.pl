#!/usr/freeware/bin/perl
# Options:  -a     recompile everything
#           -n     don't run netscape after compile

require 5.004;


use Getopt::Std;

getopt('');
my $src_dir = "../.";
my $dest_dir = "/home/users/kevn/public_html/vrj/";
my $css_filename = "html/css_header.html";
my $html_header_filename = "html/header.html";
my $html_footer_filename = "html/footer.html";
my $html_install_prefix = "http://www.vrac.iastate.edu/~kevn/vrj/";
my $subst_file = "subst.pl";



my $command = "install-web.pl ";
$command .= " -s $src_dir";
$command .= " -t $dest_dir";
$command .= " -i cgi-bin,__deleteme,install,CVS,ver1.05,ver1.08,bak";
$command .= " -c $css_filename";
$command .= " -e $html_header_filename";
$command .= " -f $html_footer_filename";
$command .= " -h $html_install_prefix";
$command .= " -w $subst_file";
$command .= " -z most.recent.pl";
if($opt_a == 1)
{
$command .= " -a";
}

system( "CreateTree.pl -i ../html/layout/website.toc.xml -o ./webtc -s subst.pl" );

print "$command\n\n\n\n-------------\n\n";
system( "$command" );

if($opt_n == 0)
{
  my $netscape_command = "netscape " . $html_install_prefix . "index.html &";
  print "$netscape_command\n";
  system("$netscape_command");
}
