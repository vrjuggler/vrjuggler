# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

import string
__all__     = ["vrj", "gadget", "jccl"]
__build__   = 1
__version__ = '0.8.3.%d' % __build__
__date__    = string.join(string.split('$Date$')[1:3], ' ')
__author__  = 'Patrick Hartling <patrick@vrjuggler.org>'
__doc__     = '''This is PyJuggler.  For information regarding PyJuggler see:
    http://www.vrjuggler.org/pyjuggler/

For information on VR Juggler see:
    http://www.vrjuggler.org'''
del string
