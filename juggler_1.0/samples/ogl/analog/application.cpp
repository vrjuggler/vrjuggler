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

#include <application.h>

//: Constructor
AnalogDemoApplication::AnalogDemoApplication( vjKernel* kern ) : vjGlApp( kern ), x( 0.0f )
{
}

//: destructor
AnalogDemoApplication::~AnalogDemoApplication() 
{
}

//: initialization callback (called once)
void AnalogDemoApplication::init()
{
   mAnalog0.init( std::string( "VJAnalog0" ) );
}

//: Called immediately upon opening a new OpenGL context 
//  (called once for each window)
//
// put your opengl initialization here...
void AnalogDemoApplication::contextInit()
{
   //: Initialize the cube GL state (display list)
      
      // create cube geometry displaylist
      mCubeDisplayList->id = glGenLists( 1 );
      ::glNewList( mCubeDisplayList->id, GL_COMPILE );
         geom::renderVertexArray( mCubeGeometry.data(), mCubeGeometry.size() );
      ::glEndList();
}

//: Function to "draw" the scene 
//  put your opengl draw functions here
//
//  PRE: OpenGL state has correct transformation and buffer selected
//  POST: The current scene has been drawn
void AnalogDemoApplication::draw()
{
   // save the glEnable and matrix states, so we can restore it later.
   ::glPushAttrib( GL_ENABLE_BIT );
   ::glMatrixMode( GL_MODELVIEW );
   ::glPushMatrix();
   
   // clear the screen
   ::glClearColor( 0.0f, 0.0f, 0.1f, 1.0f );
   ::glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
   ::glDisable( GL_LIGHTING );
   ::glShadeModel( GL_SMOOTH );
   ::glEnable( GL_DEPTH_TEST );

   // enable alpha colors
   ::glEnable( GL_BLEND );
   ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
   
   // move the cube to at the front of an average cave wall.
   // setting it to where the analog data specifies (0..1) * 6 == (0..6)
   ::glLoadIdentity();
   ::glTranslatef( 0.0f, mAnalog0->getData() * 6.0f, -6 ); 
      
   // spin the cube
   ::glRotatef( x, 1.0f, 0.7f, 0.1f );

   // render the cube geometry (display listed)
   ::glCallList( mCubeDisplayList->id );

   // render the floor geometry (not display listed)
   const float size = 6.0f;
   ::glLoadIdentity();
   ::glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
   ::glNormal3f( 0.0f, 1.0f, 0.0f );
   ::glBegin( GL_QUADS );
      ::glTexCoord2f( 1.0f, 1.0f );
      ::glVertex3f(  size, 0, -size );

      ::glTexCoord2f( 0.0f, 1.0f );
      ::glVertex3f( -size, 0, -size );

      ::glTexCoord2f( 0.0f, 0.0f );
      ::glVertex3f( -size, 0,  size );

      ::glTexCoord2f( 1.0f, 0.0f );
      ::glVertex3f(  size, 0,  size );
   ::glEnd();
   
   // restore the glEnable and matrix state so any 
   // simulator stuff renders correctly
   ::glPopMatrix();
   ::glPopAttrib();
}

// Function called before updating trackers but after the frame is drawn
//
// do calculations here...
void AnalogDemoApplication::postFrame()
{
   x += 5;
}
