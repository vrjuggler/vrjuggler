# PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

# Import everything from __vrj into our namespace.
from __vrj import *

# Attempt to import everything from __osgapp into our namespace.  If it fails,
# then Open Scene Graph is not available for use.  For people who don't want
# to use vrj.OsgApp, this is fine.  For those who do, they have to have
# OSG and PyOSG installed.
try:
   from __osgapp import *
except:
   pass
