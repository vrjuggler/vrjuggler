unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= tweekframe.ui \
	aboutbox.ui \
	orbconnectbox.ui
IMAGES	= images/filenew \
	images/fileopen \
	images/filesave \
	images/print \
	images/undo \
	images/redo \
	images/editcut \
	images/editcopy \
	images/editpaste \
	images/searchfind \
	images/tweek-icon.png \
	images/tweek3d.png
TEMPLATE	=app
CONFIG	+= qt warn_on release
LANGUAGE	= C++
