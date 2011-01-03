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

from os import path
import xml.dom
from xml.dom.minidom import parse
from xml.sax._exceptions import SAXParseException

viewerTag = 'viewer'
userTag   = 'user'
corbaTag  = 'corba'
windowTag = 'windowsize'

def getText(nodelist):
   rc = ""
   for node in nodelist:
      if node.nodeType == node.TEXT_NODE:
         rc = rc + node.data
   return rc

class GlobalPreferences:
   domTree = None

   viewer = None
   userLevel = 1
   corbaHost = ''
   corbaPort = '2809'
   iiopVersion = '1.2'
   windowWidth = '800'
   windowHeight = '600'

   def __init__(self, file):
      self.prefsFile = file

   def parse(self):
      if not path.exists(self.prefsFile):
         self.domTree = xml.dom.minidom.Document()
         self.domTree.appendChild(self.domTree.createElement("tweekrc"))
         self.save()
      else:
         try:
            self.domTree = parse(self.prefsFile)

            top_element = self.domTree.documentElement
            assert top_element.tagName == "tweekrc"

            for child in top_element.childNodes:
               if child.nodeType == xml.dom.Node.ELEMENT_NODE:
                  if child.tagName == viewerTag:
                     self.viewer = child.getAttribute("name")
                  elif child.tagName == userTag:
                     self.userLevel = child.getAttribute("level")
                  elif child.tagName == corbaTag:
                     self.corbaHost   = child.getAttribute("host")
                     self.corbaPort   = child.getAttribute("port")
                     self.iiopVersion = child.getAttribute("iiop")
                  elif child.tagName == windowTag:
                     self.windowWidth  = child.getAttribute("width")
                     self.windowHeight = child.getAttribute("height")
         except SAXParseException, ex:
            print "Crap", ex

   def save(self):
      # First, sync up the DOM tree with the current state.
      self.syncPrefsWithDOM()

      # Then truncate the existing preferences file and write the DOM tree
      # back out to it.
      new_file = open(self.prefsFile, 'w')
      self.domTree.writexml(new_file)
      new_file.close()

   def syncPrefsWithDOM(self):
      assert self.domTree != None
      top_element = self.domTree.documentElement

      # <viewer/>
      list = top_element.getElementsByTagName(viewerTag)
      if len(list) == 1:
         list[0].setAttribute("name", str(self.viewer))
      else:
         new_viewer = self.domTree.createElement(viewerTag)
         new_viewer.setAttribute("name", str(self.viewer))
         top_element.appendChild(new_viewer)

      # <user/>
      list = top_element.getElementsByTagName(userTag)
      if len(list) == 1:
         list[0].setAttribute("level", str(self.userLevel))
      else:
         new_user = self.domTree.createElement(userTag)
         new_user.setAttribute("level", str(self.userLevel))
         top_element.appendChild(new_user)

      # <corba/>
      list = top_element.getElementsByTagName(corbaTag)
      if len(list) == 1:
         list[0].setAttribute("host", str(self.corbaHost))
         list[0].setAttribute("port", str(self.corbaPort))
         list[0].setAttribute("iiop", str(self.iiopVersion))
      else:
         new_corba = self.domTree.createElement(corbaTag)
         new_corba.setAttribute("host", str(self.corbaHost))
         new_corba.setAttribute("port", str(self.corbaPort))
         new_corba.setAttribute("iiop", str(self.iiopVersion))
         top_element.appendChild(new_corba)

      # <windowsize/>
      list = top_element.getElementsByTagName(windowTag)
      if len(list) == 1:
         list[0].setAttribute("width", str(self.windowWidth))
         list[0].setAttribute("height", str(self.windowHeight))
      else:
         new_win = self.domTree.createElement(windowTag)
         new_win.setAttribute("width", str(self.windowWidth))
         new_win.setAttribute("height", str(self.windowHeight))
         top_element.appendChild(new_win)

#prefs = GlobalPreferences(path.join(path.expanduser("~patrick"), ".tweekrc"))
#prefs.parse()
#prefs.save()
