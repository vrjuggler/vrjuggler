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

#include <vrj/vjConfig.h>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gloveApp.h>

void gloveApp::renderLightsAndMaterials()
{
    GLfloat light0_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
    GLfloat light0_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
    GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
    GLfloat light0_position[] = {0.0f, 0.75f, 0.75f, 0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

    GLfloat mat_ambient[] = { 0.0f, 0.0f,  0.0f,  1.0f };
    GLfloat mat_diffuse[] = { 0.7f,  0.7f,  0.7f,  1.0f };
    GLfloat mat_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f };
    GLfloat mat_shininess[] = { 30.0f };
    GLfloat no_emm[] = { 0.0f,  0.0f,  0.0f,  1.0f };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
    glMaterialfv( GL_FRONT,  GL_EMISSION, no_emm);
    glEnable(GL_COLOR_MATERIAL);
}

void gloveApp::initGlState()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
}

void gloveApp::myDraw()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->initGlState();
    this->renderLightsAndMaterials();

    glPushMatrix();
    
   glMatrixMode(GL_MODELVIEW);
   //glLoadIdentity();
   glLoadMatrixf( mNavigation.getFloatPtr() );
   
   // draw the floor
   glPushMatrix();
       glScalef( 3.0f, 1.0f, 3.0f );
       mScene->drawFloor();
   glPopMatrix();

   // draw cube.
   glPushMatrix();
       glColor3f( 0.4f, 0.1f, 0.2f );
       glTranslatef( mCubePos[0], mCubePos[1], mCubePos[2] );
       glEnable(GL_TEXTURE_2D);
       mScene->renderRainbowTexture();
       mScene->drawCube( 1.0f, 1.0f, 1.0f, mCubeSelected );
       glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   // draw cone.
   glPushMatrix();
       glColor3f( 0.6f, 0.2f, 0.6f );
       glTranslatef( mConePos[0], mConePos[1], mConePos[2] );
       mScene->drawCone( 1.0f, 1.0f, 1.0f, mConeSelected );
   glPopMatrix();

   // draw Sphere.
   glPushMatrix();
       glColor3f( 0.8f, 0.8f, 0.2f );
       glTranslatef( mSpherePos[0], mSpherePos[1], mSpherePos[2] );
       mScene->drawSphere( 1.0f, 1.0f, 1.0f, mSphereSelected );
   glPopMatrix();

   // draw table.
   glPushMatrix();
       glTranslatef( 0.0f, 0.0f, -20.0f );
       glEnable(GL_TEXTURE_2D);
       mScene->renderWoodTexture();
       mScene->drawTable();
       glDisable(GL_TEXTURE_2D);
   glPopMatrix();
   
   glPopMatrix();
}

bool gloveApp::LeftPointing( )
{
   if (mPinchLeftThumb->getData() == 0 &&
      mPinchLeftIndex->getData() == 0 &&
      mPinchLeftMiddle->getData() == 1 &&
      mPinchLeftRing->getData() == 1 &&
      mPinchLeftPinky->getData() == 1)
   {
      return true;
   }
   else 
      return false;
}

bool gloveApp::LeftOpen()
{
   if (mPinchLeftThumb->getData() == 0 &&
      mPinchLeftIndex->getData() == 0 &&
      mPinchLeftMiddle->getData() == 0 &&
      mPinchLeftRing->getData() == 0 &&
      mPinchLeftPinky->getData() == 0)
   {
      return true;
   }
   else 
      return false;
}

bool gloveApp::RightPointing( )
{
   if (mPinchRightThumb->getData() == 0 &&
      mPinchRightIndex->getData()  == 0 &&
      mPinchRightMiddle->getData()  == 1 &&
      mPinchRightRing->getData()  == 1 &&
      mPinchRightPinky->getData() == 1)
   {
      return true;
   }
   else 
      return false;
}

bool gloveApp::RightOpen()
{
   if (mPinchRightThumb->getData() == 0 &&
      mPinchRightIndex->getData()  == 0 &&
      mPinchRightMiddle->getData()  == 0 &&
      mPinchRightRing->getData()  == 0 &&
      mPinchRightPinky->getData() == 0)
   {
      return true;
   }
   else 
      return false;
}

bool gloveApp::LeftFist()
{
   if (mPinchLeftIndex->getData() == 1 ||
         mPinchLeftThumb->getData() == 1)
   {
      return true;
   }
   else 
      return false;
}

bool gloveApp::RightFist()
{
   if (mPinchRightIndex->getData() == 1 ||
         mPinchRightThumb->getData() == 1 )
   {
      return true;
   }
   else 
      return false;
}

// Function called after tracker update but before start of drawing.  Do
// calculations and state modifications here.
//  In the glove application, this function does the logic for picking the
//  objects.
void gloveApp::postFrame()
{
   vrj::GlApp::postFrame();
   
   //: we need to keep track of the wand, and the user.
    UserInfo    userInfo;
    TrackedInfo wandInfo;
    TrackedInfo headInfo;

    vrj::Vec3 glovePos;
    vrj::Matrix finger_matrix;
    vrj::Matrix invNav;
    invNav.invert(mNavigation);
    
   /////////////////////////////////////////////////////////
   //: Debug stuff
   std::cout<<mPinchLeftThumb->getData()
            <<mPinchLeftIndex->getData()
            <<mPinchLeftMiddle->getData()
            <<mPinchLeftRing->getData()
            <<mPinchLeftPinky->getData()
            <<mPinchRightThumb->getData()
            <<mPinchRightIndex->getData()
            <<mPinchRightMiddle->getData()
            <<mPinchRightRing->getData()
            <<mPinchRightPinky->getData()<<"\n"<<std::flush;
         
   if (LeftPointing() == true)
   {
      std::cout<<"Left Pointing"<<std::flush;
   } 
   
   else if (LeftOpen() == true)
   {
      std::cout<<"Left Open"<<std::flush;
   }
   
   else if (LeftFist() == true)
   {
      std::cout<<"Left Fist"<<std::flush;
   }
   
   if (RightPointing() == true)
   {
      std::cout<<", Right Pointing"<<std::flush;
   } 
   
   else if (RightOpen() == true)
   {
      std::cout<<", Right Open"<<std::flush;
   }
   
   else if (RightFist() == true)
   {
      std::cout<<", Right Fist"<<std::flush;
   }
   
   std::cout<<"\n"<<std::flush;
   
   /////////////////////////////////////////////////////////
   //: Handle navigation
   //mNavigation.accelerate( LeftPointing() == true );
   //mNavigation.rotate( LeftPointing() == false && LeftOpen() == false );
   //mNavigation.setMatrix( mGlove->getPos(vrj::GloveData::INDEX) );
   //mNavigation.update( time );

   
   //: Get the position of the index finger:
    finger_matrix = mGlove->getPos(vrj::GloveData::INDEX);
    finger_matrix.getTrans( glovePos[0], glovePos[1], glovePos[2] );
    glovePos.xformVec( invNav, glovePos );
    
    ////////////////////////
    // NAVIGATION         //
    ////////////////////////////////////////////////////////
    static float userVelocity = 0;

    if (LeftPointing() == true)
    {
      userVelocity += 0.0001f;
    } 
    else if (LeftFist() == true)
    {
      userVelocity = 0.0f;
    }
    userInfo.setVelocity( userVelocity );
    userInfo.setAngularVelocity( 0.01f );
    vrj::Matrix tttt = mGlove->getPos(vrj::GloveData::INDEX);
    wandInfo.updateWithMatrix( tttt );
    userInfo.update( wandInfo, vrj::Vec3(0.0f, 0.0f, 0.0f) );
    userInfo.getSceneTransform( mNavigation );
    ////////////////////////////////////////////////////////
    
    
   ////////////////////////////////////////////////////////
   //: pick up the object if you're grabbing.
   //  set the object position equal to the glove position.

   if ( this->RightFist() == true )
   {
      if (mConeSelected)
          mConePos = glovePos;
      else if (mSphereSelected)
          mSpherePos = glovePos;
      else if (mCubeSelected)
          mCubePos = glovePos;
   }

   float cubeDistance   = (glovePos - mCubePos).length();
   float sphereDistance = (glovePos - mSpherePos).length();
   float coneDistance   = (glovePos - mConePos).length();
   float min = nMin( cubeDistance, sphereDistance, coneDistance);

   //: If the distance between hand and object is too far
   //  don't highlight any of them.
   if (min > 1.0f)
   {
      mCubeSelected = false;
      mSphereSelected = false;
      mConeSelected = false;
   }

   // ...otherwise,
   //   If glove is not grabbing, or
   //   we don't already have a selected one, then...
   else if ( this->RightOpen() == true ||
      (mCubeSelected   == false &&
       mSphereSelected == false &&
       mConeSelected   == false)   )
   {
      // ... highlight the closest one to the glove.
      if (min == coneDistance)
      {

          mCubeSelected = false;
          mSphereSelected = false;
          mConeSelected = true;
      } 

      else if (min == sphereDistance)
      {
          mCubeSelected = false;
          mSphereSelected = true;
          mConeSelected = false;
      } 

      else if (min == cubeDistance)
      {
          mCubeSelected = true;
          mSphereSelected = false;
          mConeSelected = false;
      }
   }
}
