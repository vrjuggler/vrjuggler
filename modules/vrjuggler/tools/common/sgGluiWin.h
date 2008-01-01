/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _SG_GLUI_WIN_H_
#define _SG_GLUI_WIN_H_

#include <iostream.h>
#include <stdio.h>
#include <GL/glut.h>
#include <glui.h>


//: This is a base class for general glui windows.
//
// NOTE: For now this is a singleton.  Only one is allowed to be created at a time.
//
class sgGluiWin
{
protected:
	sgGluiWin()
	{ mGluiWin = NULL; }

public:
	//: Register our idle function with glui
	void registerGlutIdle(void (*func)(void))
	{ GLUI_Master.set_glutIdleFunc(func); }

	//: Create new window
	//! ARGS: win_name - Name of window
	//! ARGS: flags - Unused
	//! ARGS: x,y - Initial x,y location
	void createWindow(char* win_name, int flags=0, int x=10, int y=10)
	{
		mGluiWin = GLUI_Master.create_glui(win_name, flags, x, y);
		this->constructUI();
	}

public:
		// The interface should be created in this function.
		// All glui interface items should be created in this class
	virtual void constructUI()
	{;}

	//: This function is called by the callback to interact with widgets
	virtual void interact(int wId)
	{;}

	// This function is used as the callback function to the glui items
	static void gluiCallback(int id)
	{ getInstance()->interact(id); }

	GLUI*		gluiWin()
	{ return mGluiWin; }

private:
	GLUI*		mGluiWin;		//: The glui window to use
	
	// ---------- SINGLETON -------- //
public:
		// NOTE: When deriving subclass, override this function to create a
		//        New get instance.
		// NOTE2: This one will be that one that muiCallback uses, but that is fine
		//        since it will never be called with a NULL _instance
	static sgGluiWin* getInstance()
	{
      // if(_instance == NULL)
		//	_instance = new sgGluiWin;
      if(_instance == NULL)
         cerr << "Called sgGluiWin::instance: While instance == NULL!!!\n";
		return _instance;
	}

protected:
	static sgGluiWin* _instance;
};

sgGluiWin* sgGluiWin::_instance = NULL;

#endif
