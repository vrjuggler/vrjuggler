#!/usr/bin/env python

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# TweekQt is (C) Copyright 2003-2010 by Patrick Hartling
#
# This application is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This application is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this application; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

import sys
from os import getenv
from os import path
from qt import QApplication, QMimeSourceFactory, QStringList
from pytweek import tweekframe, prefs

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

# Load the user's preferences file.  If it does not exist, it will be created.
prefs = prefs.GlobalPreferences("/home/patrick/.tweekrc")
prefs.parse()

frame = tweekframe.TweekFrame()
# XXX: This is a hack because I am lame.
frame.globalPrefs = prefs
frame.subjMgrList = []
frame.show()
app.exec_loop()
