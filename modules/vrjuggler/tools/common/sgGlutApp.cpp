/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <sgGlutApp.h>
#include <sgGlutWin.h>

	/// --- STATICS --- //
sgGlutApp* sgGlutApp::_instance = NULL;
sgGlutWin* sgGlutApp::win[256];

sgGlutApp::sgGlutApp()
{
		// Initialize the wins to NULL
	for(int i=0; i<256; i++)
		win[i] = NULL;
}


	/// Function to create a new glut window witht he given parameters.
	/// This function will setup the given window and return it.
	/// POST: Window is created
	///		  Window is added to the win list
int sgGlutApp::createNewGlutWin(int xo, int yo, int xs, int ys, char* name, unsigned int dispMode)
{
	sgGlutWin* newWin;
	newWin = new sgGlutWin();						// Allocate a new GLUT window
	newWin->createWindow(xo, yo, xs, ys, name, dispMode);		// Create the new window
	win[newWin->winHandle] = newWin;                // Set its entry
	setGLUTCallbacks(newWin->winHandle);			// Set the callbacks for new window.
	return newWin->winHandle;
}

	// Sets the GLUT callbacks to point to sgGlutApp.
	// This should be called after creating new window that is
	// a member of sgGlutApp.
	// winHandle - Handle to window.  -1 --> none set. Use current window
	// NOTE: idle callback must be set by user code.
void sgGlutApp::setGLUTCallbacks(int winHandle)
{
	int entryWinHandle = glutGetWindow();		// Save state
	if(winHandle == -1)
		winHandle = entryWinHandle;

	glutSetWindow(winHandle);
	glutDisplayFunc(sgGlutApp::cbDraw);
	glutReshapeFunc(sgGlutApp::cbReshape);
	glutMouseFunc(sgGlutApp::cbMouseInput);
	glutMotionFunc(sgGlutApp::cbMouseMotion);
	glutPassiveMotionFunc(sgGlutApp::cbMousePassiveMotion);
	glutKeyboardFunc(sgGlutApp::cbKeyInput);
	glutKeyboardUpFunc(sgGlutApp::cbKeyInputUp);
	glutSpecialFunc(sgGlutApp::cbKeySpecialInput);
	glutSpecialUpFunc(sgGlutApp::cbKeySpecialInputUp);

	glutSetWindow(entryWinHandle);				// Set back to entry glut window
}



	// Draws all the windows.  Called in a idle callback
void sgGlutApp::idleDraw()
{
	for(int i=0; i<256; i++)
	{
		if(win[i] != NULL)
		{
			glutSetWindow(i);
			preDraw();			// Do any special pre-Draw
			win[i]->draw();
			postDraw();			// Same for post draw
			glutSwapBuffers();		// Done drawing.  Swap em
		}
	}
}

	// Calls the window app functions.
	// The windows take care of "once a frame" type updates
void sgGlutApp::callWinAppFunctions()
{
	for(int i=0; i<256; i++)
	{
		if(win[i] != NULL)
		{
			glutSetWindow(i);
			win[i]->app();
		}
	}
}


   // --- Callbacks --- //
void sgGlutApp::cbDraw()
{
	int curWin = glutGetWindow();
	if(win[curWin] != NULL)
		win[curWin]->draw();	
}

void sgGlutApp::cbIdle()
{
	sgGlutApp::getInstance()->appIdle();		// Call the idle routine
}

void sgGlutApp::cbReshape(int width, int height)
{
	int curWin = glutGetWindow();
	if(win[curWin] != NULL)
		win[curWin]->reshape(width, height);
}

void sgGlutApp::cbMouseInput(int button, int state, int x, int y)
{
	int curWin = glutGetWindow();
	if(win[curWin] != NULL)
		win[curWin]->mouseInput(button, state, x, y);
}

void sgGlutApp::cbMouseMotion(int x, int y)
{
	int curWin = glutGetWindow();
	if(win[curWin] != NULL)
		win[curWin]->mouseMotion(x, y);
}

void sgGlutApp::cbMousePassiveMotion(int x, int y)
{
	int curWin = glutGetWindow();
	if(win[curWin] != NULL)
		win[curWin]->mousePassiveMotion(x, y);
}

void sgGlutApp::cbKeyInput(unsigned char key, int x, int y)
{
    if(_instance != NULL)
	_instance->keyInput(key, x, y);
}

void sgGlutApp::cbKeyInputUp(unsigned char key, int x, int y)
{
    if(_instance != NULL)
	_instance->keyInputUp(key, x, y);
}

void sgGlutApp::cbKeySpecialInput(int key, int x, int y)
{
    if(_instance != NULL)
	_instance->keySpecialInput(key, x, y);
}
	
void sgGlutApp::cbKeySpecialInputUp(int key, int x, int y)
{
    if(_instance != NULL)
	_instance->keySpecialInputUp(key, x, y);
}	

sgGlutApp*	sgGlutApp::getInstance()
{
	if(_instance == NULL)
		_instance = new sgGlutApp;		// Allocate an object

	return _instance;
}

