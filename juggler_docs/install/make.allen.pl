#!/home/freeware/bin/perl
require 5.004;

use Getopt::Std;

getopt('');

my $src_dir = "../.";
my $dest_dir = "/home/users/allenb/public_html/temp_vrj/";
#my $dest_dir = "/var/tmp/web_copy";
my $css_filename = "html/css_header.html";
my $html_footer_filename = "html/footer.html";
my $html_install_prefix = "http://www.vrac.iastate.edu/~allenb/temp_vrj/";
my $subst_file = "subst.pl";



my $command = "install-web.pl ";
$command .= " -s $src_dir";
$command .= " -t $dest_dir";
$command .= " -i __deleteme,install,CVS,ver1.05";
$command .= " -c $css_filename";
$command .= " -f $html_footer_filename";
$command .= " -h $html_install_prefix";
$command .= " -w $subst_file";
if($opt_a == 1)
{
$command .= " -a";
}

system("$command");
my $netscape_command = "netscape " . $html_install_prefix . "index.html &";
print "$netscape_command\n";
system("$netscape_command");
