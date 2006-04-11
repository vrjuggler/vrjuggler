/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

void TweekFrame::fileNew()
{

}


void TweekFrame::fileOpen()
{

}


void TweekFrame::fileSave()
{

}


void TweekFrame::fileSaveAs()
{

}


void TweekFrame::fileExit()
{
   QApplication.exit(0)
}


void TweekFrame::helpIndex()
{

}


void TweekFrame::helpContents()
{

}


void TweekFrame::helpAbout()
{
   AboutBox(self, None, True).show()
}


void TweekFrame::networkConnect()
{
   orb_box = OrbConnectBox(self, None, True)
   orb_box.hostLineEdit.setText(self.globalPrefs.corbaHost)
   orb_box.portLineEdit.setText(self.globalPrefs.corbaPort)
   orb_box.iiopVersionLineEdit.setText(self.globalPrefs.iiopVersion)
   orb_box.verifyInput()
   orb_box.subjMgrListBox.clear()

   # When the box is closed with "accept" status, we want to add the selected
   # SubjectManager reference to self.subjMgrList.
   if orb_box.exec_loop():
      self.subjMgrList.append(orb_box.subjMgr)
      self.networkDisconnectAction.setEnabled(True)
}


void TweekFrame::networkDisconnect()
{
   orb_box = OrbDisconnectBox(self, None, True)
   orb_box.subjMgrListBox.clear()

   for subj_mgr in self.subjMgrList:
      item = SubjectManagerWrapper(subj_mgr)
      orb_box.subjMgrListBox.insertItem(str(item))

   if orb_box.exec_loop():
      i = orb_box.subjMgrListBox.currentItem()
      
      if -1 != i:
         subj_mgr = self.subjMgrList[i]
         print "Removing", subj_mgr
         self.subjMgrList.remove(subj_mgr)

         if len(self.subjMgrList) == 0:
            self.networkDisconnectAction.setEnabled(False)
}


void TweekFrame::editGlobalPrefs()
{
   edit_box = GlobalPrefsDialog(self, None, True)

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
}


void TweekFrame::editModulePrefs()
{

}
