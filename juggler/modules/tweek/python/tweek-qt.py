#!/usr/bin/env python

# **************** <Tweek heading BEGIN do not edit this line> ****************
# Tweek
# 
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
# **************** <Tweek heading END do not edit this line> ******************

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2003 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
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
# ************** <auto-copyright.pl END do not edit this line> ****************

import sys
from os import getenv
from os import path
from qt import QApplication, QMimeSourceFactory, QStringList
from pytweek import tweekframe

class EnvError(Exception):
   def __init__(self, value):
      self.value = value
   def __str__(self):
      return repr(self.value)

tweek_base_dir = getenv("TWEEK_BASE_DIR")
if not tweek_base_dir:
   raise EnvError, "TWEEK_BASE_DIR environment variable not set"

# Construct the path to the images that will be loaded by the GUI.
image_dir = path.join(tweek_base_dir, "share", "tweek", "python", "images")

# Set up PyQt.
app = QApplication(sys.argv)
# XXX: Is there a cleaner way to define a QStringList from Python?
QMimeSourceFactory.defaultFactory().setFilePath(QStringList(image_dir))

frame = tweekframe.TweekFrame()
frame.show()
app.exec_loop()
