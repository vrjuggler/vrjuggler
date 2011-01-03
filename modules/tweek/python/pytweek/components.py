# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# TweekQt is (C) Copyright 2003-2011 by Patrick Hartling
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

class Viewer:
   def __init__(self):
      print "Viewer.__init__()"
      self.component = None
      self.panels    = []
      self.name      = "Unknown Viewer"

   def getName(self):
      return self.name

   def getComponent(self):
      return self.component

   def addPanel(self, panel):
      self.panels.append(panel)

   def removePanel(self, panel):
      self.panels.remove(panel)

   def getPanels(self):
      return self.panels

class Panel:
   def __init__(self):
      print "Panel.__init__()"
      self.component = None
      self.name      = "Unknown Panel"

   def getName(self):
      return self.name

   def getComponent(self):
      return self.component
