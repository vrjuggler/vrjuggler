#include <glut/sgMuiWin.h>


sgMuiWin* sgMuiWin::_instance = NULL;


		// Creates a new Glut window and assigns it to us.
int sgMuiWin::createWindow(int xo, int yo, int xs, int ys, char* name)
{
	winHandle = glutCreateWindow(name);
	glutPositionWindow(xo, yo);
	glutReshapeWindow(xs, ys);
	width = xs; height = ys;
	this->constructUI();
	muiInit();
	return winHandle;
}


	// NOTE: When deriving subclass, override this function to create a
	//        New get instance.
	// NOTE2: This one will be that one that muiCallback uses, but that is fine
	//        since it will never be called with a NULL _instance
sgMuiWin* sgMuiWin::getInstance()
{
	if(_instance == NULL)
		_instance = new sgMuiWin;

	return _instance;
}


