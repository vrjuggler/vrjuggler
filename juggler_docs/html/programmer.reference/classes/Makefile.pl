#!perl

require 5.003;

use File::Basename;
use File::Copy;
use File::Path;

# restore the default behavior if the classes aren't really installed.
if (!open(A_TEST_FILE, "classes.mcf"))
{
	print "*** Error: Class documentation not installed\n";
	print "    Copying defaults\n";
	copy("default__class_nav.xml", "class_nav.xml");
	copy("default__index.html", "index.html");
}

# restore the file if missing
if (!open(CLASSNAV_FILE, "class_nav.xml"))
{
	print "*** Error: class_nav.xml is missing\n";
	copy("default__class_nav.xml", "class_nav.xml");
}

if (!open(INDEX_FILE, "index.html")) 
{
	print "*** Error: index.html is missing\n";
	copy("default__index.html", "index.html");
}
