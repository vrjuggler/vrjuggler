# Substitutions!


# PATH SUBSTITUTIONS
#-----------------------
# File that defines the substitution hash used by install-web.pl
#
# It is a perl script that gets executed by install-web.pl.
# This is so that the developer has full perl abilities to
# perform any arbitrary logic that is desired
#
# The substitutions are defined in the hash: path_subst
#
# New substitutions can be added like this:
#
# $path_subst{"DEV_REF"} = '../here/there/everywhere'
#
# NOTE: The paths here are relative the "root" of the web
#       where the recursion starts
#
#

# directories within the ~/  directory
$path_subst{"ROOT"} = '/';
$path_subst{"HTML"} = 'html/';
$path_subst{"IMAGES"} = 'images/';
$path_subst{"CLASS_PATH"} = 'java.class/ver1.07/';
$path_subst{"JEXPLORER_DOCS"} = 'JExplorer/JEXPLORE.HTM';
## put all binaries, tarballs, and pdf's here...
$path_subst{"PUB"} = 'pub/';


# directories within the ~/html directory
$path_subst{"BUGS"} = 'html/bugs';
$path_subst{"CONTACT"} = 'html/contact.info';
$path_subst{"TEAMGUIDE"} = 'html/juggler.team.guide/';
$path_subst{"DOWNLOAD"} = 'html/download';
$path_subst{"LAYOUT"} = 'html/layout';
$path_subst{"LEGAL"} = 'html/legal';
$path_subst{"MISC"} = 'html/misc';
$path_subst{"NEWS"} = 'html/news';
$path_subst{"PUBLICATIONS"} = 'html/publications';
$path_subst{"PROGRAMMERGUIDE"} = 'html/programmer.guide';
$path_subst{"PROGRAMMERREF"} = 'html/programmer.reference/';
$path_subst{"VJCONTROL"} = 'html/vjcontrol';
$path_subst{"DOCUMENTATION"} = 'html/documentation';


# html files
$path_subst{"JUGGLER_USER_COMMUNITY"} = 'html/contact.info/usercommunity.html';



# ALIASES
#-----------------------
# An alias is a verbatim substitution.
# The AllenScript doesn't modify the variable at all.

# the href address to mail the Juggler list.
$alias_subst{"JUGGLER_MAILING_LIST"} = 'mailto:vrjuggler-contact@vrjuggler.org';
$alias_subst{"JUGGLER_DEV_MAILING_LIST"} = 'mailto:vrjuggler-contact@vrjuggler.org';

# An alias for a New Window.
# when you click, should bring content up in a completely new window.
$alias_subst{"NEW_WINDOW"} = 'NewWindow';

# Juggler bug submission program
$alias_subst{"JUGGLERBUGS"} = 'http://www.icemt.iastate.edu/juggler-bugs/';

# Contact information
$alias_subst{"ALLENB"} = 'http://www.vrac.iastate.edu/~allenb';
$alias_subst{"CAROLINA"} = 'http://www.ee.iastate.edu/~cruz';
$alias_subst{"KEVIN"} = 'http://www.vrac.iastate.edu/~kevn';
$alias_subst{"PATRICK"} = 'mailto:patrick@vrac.iastate.edu';
$alias_subst{"CHRISJUST"} = 'mailto:cjust@vrac.iastate.edu';
$alias_subst{"COATS"} = 'mailto:jwcoats@vrac.iastate.edu';
$alias_subst{"VRAC"} = 'http://www.vrac.iastate.edu';
$alias_subst{"IASTATE"} = 'http://www.iastate.edu';

# font size for the tree control (JEXPLORER)
$alias_subst{"TREE_FONTSIZE"} = '11';

# links to documentation
$alias_subst{"JEXPLORER"} = 'http://www.jexplorer.com';
$alias_subst{"AUTOCONF"} = 'http://www.gnu.org/software/autoconf/autoconf.html';
$alias_subst{"MAKE"} = 'http://www.gnu.org/software/make/make.html';
$alias_subst{"PERL"} = 'http://www.perl.com/';
$alias_subst{"PERCEPS"} = 'http://starship.python.net/crew/tbryan/PERCEPS/';
$alias_subst{"CVS"} = 'http://www.sourcegear.com/CVS';
$alias_subst{"OPENGL"} = 'http://www.opengl.org';
$alias_subst{"PERFORMER"} = 'http://www.vrac.iastate.edu';
$alias_subst{"PATTERNS"} = 'http://hillside.net/patterns/patterns.html';
# links to books and manuals
$alias_subst{"DESIGNPATTERNSBOOK"} = 'http://www.amazon.com/exec/obidos/ASIN/0201633612/o/qid=947544224/sr=2-1/002-9163340-6788217';
$alias_subst{"OPENGLREDBOOK"} = 'http://www.amazon.com/exec/obidos/ASIN/0201604582/o/qid=947802795/sr=2-1/002-9163340-6788217';
$alias_subst{"PERFORMERBOOK"} = 'http://techpubs.sgi.com';

# Miscellaneous
$alias_subst{'CONFIGURE_REV'} = '1.182';
