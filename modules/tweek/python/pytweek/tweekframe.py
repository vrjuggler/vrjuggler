# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# TweekQt is (C) Copyright 2003-2006 by Patrick Hartling
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
# Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

import qt
import pytweek.tweekframebase
import pytweek.aboutbox
import pytweek.netboxes
import pytweek.globalprefsdialog
import pytweek.net

tweekframebase    = pytweek.tweekframebase
aboutbox          = pytweek.aboutbox
netboxes          = pytweek.netboxes
globalprefsdialog = pytweek.globalprefsdialog
net               = pytweek.net


class TweekFrame(tweekframebase.TweekFrameBase):
   def __init__(self, parent = None, name = None, fl = 0):
      tweekframebase.TweekFrameBase.__init__(self, parent, name, fl)

   def fileExit(self):
      qt.QApplication.exit(0)

   def helpAbout(self):
      aboutbox.AboutBox(self, None, True).show()

   def networkConnect(self):
      orb_box = netboxes.OrbConnectBox(self, None, True)
      orb_box.hostLineEdit.setText(self.globalPrefs.corbaHost)
      orb_box.portLineEdit.setText(self.globalPrefs.corbaPort)
      orb_box.iiopVersionLineEdit.setText(self.globalPrefs.iiopVersion)
      orb_box.verifyInput()
      orb_box.subjMgrListBox.clear()

      # When the box is closed with "accept" status, we want to add the
      # selected SubjectManager reference to self.subjMgrList.
      if orb_box.exec_loop():
         self.subjMgrList.append(orb_box.subjMgr)
         self.networkDisconnectAction.setEnabled(True)

   def networkDisconnect(self):
      orb_box = netboxes.OrbDisconnectBox(self, None, True)
      orb_box.subjMgrListBox.clear()

      for subj_mgr in self.subjMgrList:
         item = net.SubjectManagerWrapper(subj_mgr)
         orb_box.subjMgrListBox.insertItem(str(item))

      if orb_box.exec_loop():
         i = orb_box.subjMgrListBox.currentItem()
         
         if -1 != i:
            subj_mgr = self.subjMgrList[i]
            print "Removing", subj_mgr
            self.subjMgrList.remove(subj_mgr)

            if len(self.subjMgrList) == 0:
               self.networkDisconnectAction.setEnabled(False)

   def editGlobalPrefs(self):
      edit_box = globalprefsdialog.GlobalPrefsDialog(self, None, True)

      edit_box.userLevelComboBox.setCurrentItem(int(self.globalPrefs.userLevel) - 1)
      edit_box.nsHostLineEdit.setText(self.globalPrefs.corbaHost)
      edit_box.nsPortLineEdit.setText(self.globalPrefs.corbaPort)
      edit_box.iiopVersionLineEdit.setText(self.globalPrefs.iiopVersion)

      if edit_box.exec_loop():
         self.globalPrefs.userLevel = str(edit_box.userLevelComboBox.currentItem() + 1)
         self.globalPrefs.viewer    = edit_box.viewerComboBox.currentText()
         self.globalPrefs.corbaHost = edit_box.nsHostLineEdit.getText()
         self.globalPrefs.corbaPort = edit_box.nsPortLineEdit.getText()
         self.globalPrefs.iiopVersion = edit_box.iiopVersionLineEdit.getText()
         self.globalPrefs.save()

