# WEB SUBSTITUTIONS
#-----------------------
# File that defines the substitution hash used by web-install
#
# It is a perl script that gets executed by web-install.
# This is so that the developer has full perl abilities to
# perform any arbitrary logic that is desired
#
# The substitutions are defined in the hash: web_subst
#
# New substitutions can be added like this:
#
# $web_subst{"DEV_REF"} = '../here/there/everywhere'
#
# NOTE: The paths here are relative the "root" of the web
#       where the recursion starts
#
#

# directories within the ~/  directory
$web_subst{"HTML"} = 'html/';
$web_subst{"IMAGES"} = 'images/';
$web_subst{"CLASS_PATH"} = 'java.class/';

# directories within the ~/html directory
$web_subst{"BUGS"} = 'html/bugs';
$web_subst{"CONTACT"} = 'html/contact.info';
$web_subst{"DEVGUIDE"} = 'html/developer.guide/';
$web_subst{"DEVREF"} = 'html/developer.reference/';
$web_subst{"DOWNLOAD"} = 'html/download';
$web_subst{"LAYOUT"} = 'html/layout';
$web_subst{"LEGAL"} = 'html/legal';
$web_subst{"MISC"} = 'html/misc';
$web_subst{"NEWS"} = 'html/news';
$web_subst{"PUBLICATIONS"} = 'html/publications';
$web_subst{"USERGUIDE"} = 'html/user.guide';
$web_subst{"VJCONTROL"} = 'html/vjcontrol';

# html files
$web_subst{"JUGGLER_USER_COMMUNITY"} = 'html/contact.info/usercommunity.html';



# ALIASES
#-----------------------
# An alias is a verbatim substitution.
# The AllenScript doesn't modify the variable at all.

# the href address to mail the Juggler list.
$alias_subst{"JUGGLER_MAILING_LIST"} = 'mailto:vrjuggler-contact@vrjuggler.org';

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

