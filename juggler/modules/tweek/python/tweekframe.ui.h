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
   
   if orb_box.exec_loop():       
      host = orb_box.hostLineEdit.text()
      port = orb_box.portLineEdit.text()
      iiop_ver = orb_box.iiopVersionLineEdit.text()
      subcontext = orb_box.subcontextLineEdit.text()
}


void TweekFrame::networkDisconnect()
{

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
