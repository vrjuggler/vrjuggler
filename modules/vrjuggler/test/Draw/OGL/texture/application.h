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

#ifndef _TEXTURE_APP_
#define _TEXTURE_APP_

#include <vjConfig.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include <Kernel/GL/vjGlApp.h>
#include <Kernel/GL/vjGlContextData.h>
#include <Kernel/vjDebug.h>

#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <Kernel/vjDebug.h>

#include "renderGeometry.h"
#include "cubeGeometry.h"

#include "Image.h"
#include "Texture.h"
#include "hexImage.h"
#include "renderTexture.h"

/*
#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjAnalogInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>
#include <Kernel/vjUser.h>
*/

class textureApp : public vjGlApp
{
public:
   textureApp( vjKernel* kern ) : vjGlApp( kern ), x( 0.0f )
   {
   }

   virtual ~textureApp() 
   {
   }

   // Execute any initialization needed before the API is started
   virtual void init();

   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
   }

   // Called immediately upon opening a new OpenGL context
   virtual void contextInit();

   /** Function to draw the scene
    * PRE: OpenGL state has correct transformation and buffer selected
    * POST: The current scene has been drawn
    */
   virtual void draw()
   {
      ::glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
      ::glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
      ::glDisable( GL_LIGHTING );
      ::glShadeModel( GL_SMOOTH );
      ::glEnable( GL_DEPTH_TEST );
      
      // enable alpha colors
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
      
      ::glMatrixMode( GL_MODELVIEW );
      ::glPushMatrix();
         ::glLoadIdentity();      

         // move the cube to at the front of an average cave wall.
         ::glTranslatef( 0.0f, 6.0f, -6 ); 
         
         // spin the cube
         ::glRotatef( x, 1.0f, 0.7f, 0.1f );
         
         // render the cube texture (texture object)
         tex::render( mCubeTexture, textureApp::getTexObjID( mCubeTexture ) );

         // render the cube geometry (display listed)
         ::glCallList( mCubeDisplayList->id );
      ::glPopMatrix();
      
      // draw a floor
      float size = 20.0f;
      ::glPushMatrix();
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
      ::glPopMatrix();
   }

   // Function called before updating trackers but after the frame is drawn
   virtual void postFrame()
   {
      x += 5;
   }

private:
   float x;

   // use the type: vjGlContextData<ResourceID>
   // to store your OpenGL texture object and display list IDs
   // i.e. vjGlContextData<ResourceID>  mTexObj;
   class ResourceID
   {
   public:
      ResourceID() : id( -1 )
      {
      }
      unsigned int id;
   };
   
   // helper functions to make your code easier to read
   // i.e. hides all the static_cast and template lengthyness
   static void setTexObjID( Texture& t, vjGlContextData<ResourceID>& texObjectID )
   {
      void* data = reinterpret_cast<void*>( &texObjectID );
      t.setUserData( data );
   }   
   static unsigned int& getTexObjID( Texture& t ) 
   { 
      vjGlContextData<ResourceID>* texObject = static_cast<vjGlContextData<ResourceID>* >( t.userData() );
      return (*texObject)->id;
   }
   
   // Cube objects:
   Texture                       mCubeTexture;
   cubeGeometry                  mCubeGeometry;
   vjGlContextData<ResourceID>   mCubeTextureObj;
   vjGlContextData<ResourceID>   mCubeDisplayList;
};


#endif
