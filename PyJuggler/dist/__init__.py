# PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

import string
__all__     = ["vrj", "gadget", "jccl", "vpr"]
__build__   = 1
__version__ = '0.10.1.%d' % __build__
__date__    = string.join(string.split('$Date$')[1:3], ' ')
__author__  = 'Patrick Hartling <patrick@vrjuggler.org>'
__doc__     = '''This is PyJuggler.  For information regarding PyJuggler see:
    http://www.vrjuggler.org/pyjuggler/

For information on VR Juggler see:
    http://www.vrjuggler.org'''
del string
