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
   try:
      cs = CorbaService(self.hostLineEdit.text(),
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
}


void OrbConnectBox::subjMgrSelected()
{
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
      labels = QStringList()

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
}
