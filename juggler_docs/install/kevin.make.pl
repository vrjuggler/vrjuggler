#!/home/freeware/bin/perl
require 5.004;


use Getopt::Std;

getopt('');
my $src_dir = "../.";
my $dest_dir = "/home/users/kevn/public_html/vrj/";
my $css_filename = "html/css_header.html";
my $html_header_filename = "html/header.html";
my $html_footer_filename = "html/footer.html";
my $html_install_prefix = "http://www.vrac.iastate.edu/~kevn/vrj/";
my $web_subst_file = "web_subst.pl";



my $command = "install-web.pl ";
$command .= " -s $src_dir";
$command .= " -t $dest_dir";
$command .= " -i __deleteme,JExplorer,install,CVS";
$command .= " -c $css_filename";
$command .= " -e $html_header_filename";
$command .= " -f $html_footer_filename";
$command .= " -h $html_install_prefix";
$command .= " -w $web_subst_file";
$command .= " -z most.recent.pl";
if($opt_a == 1)
{
$command .= " -a";
}

system("$command");
my $netscape_command = "netscape " . $html_install_prefix . "index.html &";
print "$netscape_command\n";
system("$netscape_command");
