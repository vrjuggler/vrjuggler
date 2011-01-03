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

import qt
import pytweek.orbconnectboxbase
import pytweek.orbdisconnectboxbase
import pytweek.net

connectbox    = pytweek.orbconnectboxbase
disconnectbox = pytweek.orbdisconnectboxbase


class OrbConnectBox(connectbox.OrbConnectBoxBase):
   def __init__(self, parent = None, name = None, model = 0, fl = 0):
      connectbox.OrbConnectBoxBase.__init__(self, parent, name, model, fl)

   def verifyInput(self):
      if not self.hostLineEdit.text().isEmpty() and \
         str(self.portLineEdit.text()).isdigit() and \
         not self.iiopVersionLineEdit.text().isEmpty():
         self.connectPushButton.setEnabled(True)
      else:
         self.connectPushButton.setEnabled(False)

   def tryConnect(self):
      try:
         cs = net.CorbaService(self.hostLineEdit.text(),
                               self.portLineEdit.text(),
                               self.iiopVersionLineEdit.text(),
                               self.subcontextLineEdit.text())
         cs.initialize()
               
         # Now populate the left panel of the splitter with the list elements.
         count = 0
         self.subjMgrList = cs.getSubjectManagerList()
         for m in self.subjMgrList:
            self.subjMgrListBox.insertItem("Subject Manager #" + str(count))
            count += 1
      except CORBA.SystemException, ex:
         self.subjMgrList = None
         self.subjMgrListBox.clear()
         print ex.details()
 
   def subjMgrSelected(self):
      i = self.subjMgrListBox.currentItem()
      if -1 != i:
         self.okPushButton.setEnabled(True)
         self.subjMgr = self.subjMgrList[i]
         info = self.subjMgr.getInfo()
                 
         # Clear the table of whatever was in it before.
         if self.subjMgrInfoTable.numRows() > 0:
            self.subjMgrInfoTable.removeRows(range(self.subjMgrInfoTable.numRows()))
        
         # Resize the table and insert new information.
         self.subjMgrInfoTable.setNumRows(len(info))
         labels = qt.QStringList()
        
         for row in range(len(info)):
            labels.append(info[row].key)
            self.subjMgrInfoTable.setText(row, 0, info[row].value)
        
         self.subjMgrInfoTable.setRowLabels(labels)
         self.subjMgrInfoTable.adjustColumn(0)
      else:
         self.okPushButton.setEnabled(False)
         self.subjMgr = None
                 
         # Clear the table of whatever was in it before.
         if self.subjMgrInfoTable.numRows() > 0:
            self.subjMgrInfoTable.removeRows(range(self.subjMgrInfoTable.numRows()))

class OrbDisconnectBox(disconnectbox.OrbDisconnectBoxBase):
   def __init__(self, parent = None, name = None, model = 0, fl = 0):
      disconnectbox.OrbDisconnectBoxBase.__init__(self, parent, name, model, fl)

   def subjMgrSelected(self):
      if self.subjMgrListBox.currentItem() != -1:
         self.disconnectPushButton.setEnabled(True)
      else:
         self.disconnectPushButton.setEnabled(False)

