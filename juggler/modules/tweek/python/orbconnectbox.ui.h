/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/





void OrbConnectBox::verifyInput()
{
   if not self.hostLineEdit.text().isEmpty() and str(self.portLineEdit.text()).isdigit() and not self.iiopVersionLineEdit.text().isEmpty():
      self.connectPushButton.setEnabled(True)
   else:
      self.connectPushButton.setEnabled(False)
}







void OrbConnectBox::tryConnect()
{
   cs = CorbaService(self.hostLineEdit.text(),
                     self.portLineEdit.text(),
                     self.iiopVersionLineEdit.text(),
                     self.subcontextLineEdit.text())
   cs.initialize()
   mgrs = cs.getSubjectManagerList()
          
   # Now populate the left panel of the splitter with the list elements.
}
