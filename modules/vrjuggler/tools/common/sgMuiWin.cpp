/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <sgMuiWin.h>


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


