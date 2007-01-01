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

#include <GL/glut.h>
#include <stdio.h>

#include "trackerConfigApp.h"
#include "trackerConfigMenu.h"

/////////////////////////////////////////////////////////////////
//            The GLUT Application                             //
/////////////////////////////////////////////////////////////////
trackerConfigApp* theApp;		// This must be set to the application

void setApplication()
{
	theApp = trackerConfigApp::getInstance();
}

void initAppWindows()
{
	theApp->initWindows();	
}

void reshape(int w, int h)
{
  /* Because Gil specified "screen coordinates" (presumably with an
     upper-left origin), this short bit of code sets up the coordinate
     system to correspond to actual window coodrinates.  This code
     wouldn't be required if you chose a (more typical in 3D) abstract
     coordinate system. */

  glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */
  glMatrixMode(GL_PROJECTION);  /* Start modifying the projection matrix. */
  glLoadIdentity();             /* Reset project matrix. */
  glOrtho(0, w, 0, h, -1, 1);   /* Map abstract coords directly to window coords. */
  glScalef(1, -1, 1);           /* Invert Y axis so increasing Y goes down. */
  glTranslatef(0, -h, 0);       /* Shift origin up to upper-left corner. */
}


int main(int argc, char **argv)
{
	setApplication();

	//glutInitWindowSize(400,400);		// Set window size
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);		// Setup framebuffer
	glutInit(&argc, argv);
	
		// ---- Create windows ---- //
	initAppWindows();
	
	// For standalone application
   glutIdleFunc(sgGlutApp::cbIdle);

   // If have windows
   trackerConfigMenu::getInstance()->registerGlutIdle(sgGlutApp::cbIdle);
	trackerConfigMenu::getInstance()->setApp(theApp);

	//glutReshapeFunc(reshape);
	
	glutMainLoop();						// Will NEVER return.  just LOOPS
	return 0;							// ANSI C requires main to return int
}

