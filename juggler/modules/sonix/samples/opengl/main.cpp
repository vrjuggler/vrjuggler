
/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * Juggler Juggler
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/

// Kevin Meinert
// simple glut skeleton application: look for the !!!TODO!!!s, 
//  and fill in your code 
//                          there as needed
//

#ifdef WIN32
	#include <windows.h>  // make the app win32 friendly. :)
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "snx/sonix.h"    // interface

#include <iostream.h>
#include <stdlib.h>

// a place to store application data...
class AppWindow
{
public:
   static int width, height;
   static int mainWin_contextID;
};
int AppWindow::width = 0, AppWindow::height = 0;
int AppWindow::mainWin_contextID = -1;

void drawGrid()
{
   glBegin( GL_LINES );
      for ( int x = -1000; x < 1000; ++x)
      {
         glVertex3f( -1000, 0, x );
         glVertex3f(  1000, 0, x );
         glVertex3f( x, 0, -1000 );
         glVertex3f( x, 0,  1000 );
      }
   glEnd();
}

//////////////////////////////////////////////////
// This is called on a glutPostRedisplay
//////////////////////////////////////////////////
static void OnRedisplay()
{ 
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
   glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
	
   
	// set up the projection matrix
	glMatrixMode( GL_PROJECTION );
        glLoadIdentity();                     
        gluPerspective( 80.0f, AppWindow::width / AppWindow::height, 0.01f, 1000.0f );
                           
   // initialize your matrix stack used for transforming your models
    glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();      

   // !!!TODO!!!: replace the following with your own opengl commands!
   glTranslatef( 0, -10, 0 );
	drawGrid();
   // !!!TODO!!!: ////////////////////////////////////////
   
   // swaps the front and back frame buffers.
   // hint: you've been drawing on the back, offscreen, buffer.  
   // This command then brings that framebuffer onscreen.
	glutSwapBuffers();
}

//////////////////////////////////////////////////
// This is called repeatedly, as fast as possible
//////////////////////////////////////////////////
#include "unistd.h"
static void OnIdle()
{
   // According to the GLUT specification, the current window is
   // undefined during an idle callback.  So we need to explicitly change
   // it if necessary
   if ( glutGetWindow() != AppWindow::mainWin_contextID )
           glutSetWindow( AppWindow::mainWin_contextID );

   // tell glut to call redisplay (which then calls OnRedisplay)
   glutPostRedisplay();
   
   usleep( 10000 );
   sonix::instance().step( 0.1 );
}

/////////////////////////////////////////////
// This is called on a Resize of the glut window
/////////////////////////////////////////////
static void OnReshape( int width, int height ) 
{
   // save these params in case your app needs them
   AppWindow::width = width;
   AppWindow::height = height;
   
   // set your viewport to the extents of the window
   glViewport( 0, 0, width, height );
   
	// let the app run idle, while resizing, 
   // glut does not do this for us automatically, so call OnIdle explicitly.
	OnIdle();
}

////////////////////////////////
// This is called on a Down Keypress        
////////////////////////////////
static void OnKeyboardDown( unsigned char k, int x, int y )
{ 
	switch (k)
	{
   // If user pressed 'q' or 'ESC', then exit the app.
   // this is really ungraceful, but necessary since GLUT does a while(1)
   // as it's control loop.  There is no GLUT method to exit, unfortunately.
	case 'q':
   case 27:
		exit( 0 );
		break;
   case '1':
{
   sonix::instance().changeAPI( "Stub" );
   snx::SoundInfo si;
   si.filename = "../../../data/sample.wav";
   si.datasource = snx::SoundInfo::FILESYSTEM;
sonix::instance().configure( "kevin", si );

sonix::instance().changeAPI( "OpenAL" );
}
break;
   case '2':
{
   sonix::instance().changeAPI( "Stub" );
   snx::SoundInfo si;
   si.filename = "../../../data/suck1.aiff";
   si.datasource = snx::SoundInfo::FILESYSTEM;
   sonix::instance().configure( "kevin", si );

sonix::instance().changeAPI( "JugglerWorks" );
}
break;
   case '3':
{
sonix::instance().changeAPI( "Stub" );
}
break;
   case 'a':
{
snx::SoundInfo si;
   si.filename = "../../../data/sample.wav";
   si.datasource = snx::SoundInfo::FILESYSTEM;
sonix::instance().configure( "kevin", si );
}
break;
   case 'b':
{
snx::SoundInfo si;
   si.filename = "../../../data/sample-drumsolo-2bars.wav";
   si.datasource = snx::SoundInfo::FILESYSTEM;
sonix::instance().configure( "kevin", si );
}
break;
      case 't':
{
sonix::instance().trigger( "kevin" );
}
break;
case 'p':
{
sonix::instance().pause( "kevin" );
}
break;
case 's':
{
sonix::instance().stop( "kevin" );
}
break;

case ',':
{
sonix::instance().setPosition( "kevin", -60, 0, 0 );
}
break;

case '.':
{
sonix::instance().setPosition( "kevin", 60, 0, 0 );
}
break;

	default:
		// do nothing if no key is pressed
		break;
	}
}
////////////////////////////////
// This is called on a Up Keypress        
////////////////////////////////
static void OnKeyboardUp( unsigned char k, int x, int y ) 
{
	switch (k)
	{
	case 'a':
		// !!!TODO!!!: add handler for when UP is released
      break;

	case 'z':
		// !!!TODO!!!: add handler for when DOWN is released
      break;
	
	default:
		// do nothing if no key is pressed
		break;
	}
}

////////////////////////////////
// This is called on a Down Keypress 
// of a "special" key such as the grey arrows.
////////////////////////////////
static void OnSpecialKeyboardDown(int k, int x, int y) 
{
	switch (k)
	{
	case GLUT_KEY_UP:
		// !!!TODO!!!: add handler for when UP is pressed
      break;
	case GLUT_KEY_DOWN:
		// !!!TODO!!!: add handler for when DOWN is pressed
      break;
	default:
		// do nothing if no special key pressed
		break;
	}
}

////////////////////////////////
// This is called on a Up Keypress        
////////////////////////////////
static void OnSpecialKeyboardUp( int k, int x, int y ) 
{
	switch (k)
	{
	case GLUT_KEY_UP:
		// !!!TODO!!!: add handler for when UP is released
      break;
	case GLUT_KEY_DOWN:
		// !!!TODO!!!: add handler for when DOWN is released
      break;
	default:
		// do nothing if no special key pressed
		break;
	}
}

////////////////////////////////
// This is called when mouse changes position
// x and y are the screen position 
// in your 2D window's coordinate frame
////////////////////////////////
static void OnMousePos( int x, int y ) 
{
   // !!!TODO!!!: do something based on mouse position
}

////////////////////////////////
// This is called when mouse clicks
////////////////////////////////
static void OnMouseClick( int a, int b, int c, int d )
{
   // !!!TODO!!!: Need mouse interaction?
   //             read the glut docs/manpage to find out how to query 
   //             which button was pressed...
   //             you may have to get this from the glut website 
   //             (use www.google.com to search for it)
}


// Initialize the application
// initialize the state of your app here if needed...
static void OnApplicationInit()
{
   // Don't put open GL code here, this func may be called at anytime
   // even before the API is initialized 
   // (like before a graphics context is obtained)
   
   
   // !!!TODO!!!: put your initialization code here.
}


void main(int argc, char* argv[])
{
    // Initialize the application
    // initialize the state of your app here if needed...
    OnApplicationInit();
   
	 // Set the window's initial size
    ::glutInitWindowSize( 640, 480 );
    ::glutInit( &argc, argv );
  
    // Set to double buffer to reduce flicker
    ::glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    
    // Set the window title
    AppWindow::mainWin_contextID = ::glutCreateWindow( "GLUT application" );
    
    cout<<"\n"<<flush;
    cout<<"YourApp - by YourName - you@your.address.com\n"<<flush;
    cout<<"       usage:  some params\n"<<flush;
    cout<<"               some more params\n"<<flush;
    cout<<"\n"<<flush;
    
	// display callbacks.
	::glutReshapeFunc( OnReshape );
	::glutIdleFunc( OnIdle );
	::glutDisplayFunc( OnRedisplay );

   // tell glut to not call the keyboard callback repeatedly 
   // when holding down a key. (uses edge triggering, like the mouse)
	::glutIgnoreKeyRepeat( 1 );

	// keyboard callback functions.
	::glutKeyboardFunc( OnKeyboardDown );
	::glutKeyboardUpFunc( OnKeyboardUp );
	::glutSpecialFunc( OnSpecialKeyboardDown );
	::glutSpecialUpFunc( OnSpecialKeyboardUp );

	// mouse callback functions...
	::glutMouseFunc( OnMouseClick );
	::glutMotionFunc( OnMousePos );
	::glutPassiveMotionFunc( OnMousePos );

    // start the application loop, your callbacks will now be called
    // time for glut to sit and spin.
    ::glutMainLoop();
}

