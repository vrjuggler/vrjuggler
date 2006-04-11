unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= tweekframe.ui \
	aboutbox.ui \
	orbconnectbox.ui \
	globalprefsdialog.ui \
	orbdisconnectbox.ui
IMAGES	= images/filenew \
	images/fileopen \
	images/filesave \
	images/undo \
	images/redo \
	images/editcut \
	images/editcopy \
	images/editpaste \
	images/tweek-icon.png \
	images/tweek3d.png
TEMPLATE	=app
CONFIG	+= qt warn_on release
LANGUAGE	= C++
