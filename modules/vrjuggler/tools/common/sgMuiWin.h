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

#ifndef _SG_MUI_WIN_H_
#define _SG_MUI_WIN_H_

#include <iostream.h>
#include <stdio.h>
#include <GL/glut.h>

#include <mui/mui.h>

/**
 * sgMuiWin: This is a base class for general mui windows.
 *
 * NOTE: For now this is a singleton.  Only one is allowed to be created at a time.
 */

class sgMuiWin
{
protected:
	sgMuiWin()
	{
		winHandle = -1;
	}

public:
	void setWindow(int handle)
	{
		winHandle = handle;
	}

		// Creates a new Glut window and assigns it to us.
	int createWindow(int xo = 0, int yo = 0, int xs = 400, int ys = 400, char* name = NULL);

public:
		// The interface should be created in this function.
		// It is suggested, that all mui objects should have their id set
		// to allow for easier interaction.
	virtual void constructUI()
	{;}

		// This function is called when there is MUI interaction
		// It is suggested that this is simply a switch on mui id values
	virtual void interact(muiObject* obj, enum muiReturnValue val)
	{;}

		// Helper function for lazy people. (like me)
		// Sets the callback function for the given object to call the class member "interact".
	void setMuiCallback(muiObject* obj)
		{ muiSetCallback(obj, sgMuiWin::muiCallback); }

		// This is the function that is called by mui as a callback
		// It should then call the interact function of the singleton object
	static
	void muiCallback(muiObject* obj, enum muiReturnValue val)
	{
		getInstance()->interact(obj, val);
	}

public:
	int		winHandle;		// The glut window.  If -1, then has not been set.
	int		width, height;

	// ---------- SINGLETON -------- //
public:
		// NOTE: When deriving subclass, override this function to create a
		//        New get instance.
		// NOTE2: This one will be that one that muiCallback uses, but that is fine
		//        since it will never be called with a NULL _instance
	static sgMuiWin* getInstance();

protected:
	static sgMuiWin* _instance;
};



#endif
