/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _SG_GLUT_APP_H_
#define _SG_GLUT_APP_H_

#include <iostream.h>
#include <stdio.h>
#include <GL/glut.h>

#include <sgGlutWin.h>

/***
 * SINGLETON
 *
 * This class defines a generic interfaace between the sceneGraph library
 * and GLUT.  User applications should be derived from this class.
 *
 * This class will manage input only for now.
 * NOTE: This class assumes only one window, and no muli-channel stuff.
 */
class sgGlutApp
{
protected:
		/// Ctor
		// POST: wins are NULL
	sgGlutApp();

public:
   // Function to create a new glut window with the given parameters.
   // This function will setup the given window and return it.
   // POST: Window is created
   //		  Window is added to the win list
	int createNewGlutWin(int xo = 0, int yo = 0, int xs = 400, int ys = 400,
		                 char* name = NULL, unsigned int dispMode = 0);

   // Sets the GLUT callbacks to point to sgGLUTApp.
   // This should be called after creating new window that is
   // a member of sgGLUTApp.
   // winHandle - Handle to window.  -1 --> none set. Use current window
   // NOTE: idle callback must be set by user code.
	void setGLUTCallbacks(int winHandle = -1);

public:
	//: This is where user code for application processing goes
   // This is called before drawing occurs for the given frame
   // Imediately after this, the preFrame gets called
   virtual void appFunc()
	{ ;}

	
	// Called immediately before drawing the next frame during idle time
   virtual void preIdleFrame()
	{;}

	// Called immediately after drawing a frame.  During idle time.
   virtual void postIdleFrame()
	{;}

	// For each glut window, called before draw
   // Has glContext set correctly to draw
   virtual void preDraw()
	{;}

	// For each glut window, called after draw
   // Has glContext set correctly to draw
   virtual void postDraw()
	{;}

public:	    // ---- Application level callbacks --- //
	    // These functions get called by the glut keyborad callbacks. //
	
	    /// This function is called when the user presses a key in ANY window
	    /// key - ASCII rep of key
	    /// x,y - Location of mouse in the window
	virtual void keyInput(unsigned char key, int x, int y)
	{;}
	
	    /// This function is called when the user releases a key in ANY window
	    /// key - ASCII rep of key
	    /// x,y - Location of mouse in the window
	virtual void keyInputUp(unsigned char key, int x, int y)
	{;}
	
	    /// This function is called when the user presses a key in ANY window
	    /// key - GLUT_KEY_* identifier for the key.  Pg 24 of GLUT manual
	    /// x,y - Location of mouse in the window
	virtual void keySpecialInput(int key, int x, int y)
	{;}
	
	    /// This function is called when the user releases a key in ANY window
	    /// key - GLUT_KEY_* identifier for the key.  Pg 24 of GLUT manual
	    /// x,y - Location of mouse in the window
	virtual void keySpecialInputUp(int key, int x, int y)
	{;}


   //  ------------ INTERNAL CLASS CODE ------------- //
protected:
   // Most applications will not override or call directly
   // Don't over ride these functions except for special app base classes
		
   // Called by the glut idle loop
   //! NOTE: This CONTROLS everything
	virtual void appIdle()
	{
		appFunc();			// Do updates.  (Animate, math, DCS's, etc.)
		
		preIdleFrame();
		idleDraw();
		postIdleFrame();

		callWinAppFunctions();	// NOTE: End of frame stuff. Updates button state.
								// Next time around will contain any events processed
								// between now aand then
	}
		

   // Draws all the windows. (Also calls pre and post draw)
   // Called in a idle callback
	virtual void idleDraw();

   // Calls the window app functions.
	// The windows take care of "once a frame" type updates
	virtual void callWinAppFunctions();



public:		// Callback functions
	static void cbDraw();
	static void cbIdle();
	static void cbReshape(int width, int height);
	static void cbMouseInput(int button, int state, int x, int y);
	static void cbMouseMotion(int x, int y);
	static void cbMousePassiveMotion(int x, int y);
	static void cbKeyInput(unsigned char key, int x, int y);
	static void cbKeyInputUp(unsigned char key, int x, int y);
	static void cbKeySpecialInput(int key, int x, int y);
	static void cbKeySpecialInputUp(int key, int x, int y);
	
public:
	static sgGlutWin*	win[256];		// A list of the windows. Index by int so can use glut win id.

public:
	static sgGlutApp*	getInstance();
protected:
	static sgGlutApp*	_instance;
};

#endif
