# PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

import pydoc
import sys

import PyJuggler.vpr
import PyJuggler.jccl
import PyJuggler.snx
import PyJuggler.gadget
import PyJuggler.cluster
import PyJuggler.vrj

modules = [
   '__builtin__',
   'PyJuggler',
   'PyJuggler.vpr',
   'PyJuggler.jccl',
   'PyJuggler.snx',
   'PyJuggler.gadget',
   'PyJuggler.cluster',
   'PyJuggler.vrj',
   'PyJuggler.__vrj',
   'PyJuggler.__osgapp',
]

sys.argv.insert(1, '-w')
sys.argv.insert(2, '')

for m in modules:
   sys.argv[2] = m
   try:
      pydoc.cli()
   except:
      pass
