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
   orb_box.show()
   if orb_box.result() == QDialog.Accepted:
      host = orb_box.hostLineEdit.text()
      port = orb_box.portLineEdit.text()
      iiop_ver = orb_box.iiopVersionLineEdit.text()
      subcontext = orb_box.subcontextLineEdit.text()
}


void TweekFrame::networkDisconnect()
{

}
