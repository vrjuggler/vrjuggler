/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <string>

#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>

#include <vpr/Util/Debug.h>
#include <navigation.h>
#include <ConfigApp.h>


using namespace gmtl;
using namespace jccl;

// Initializes the surface color to a white, gray, and black gradient.  This
// will be the default surface color unless we are told otherwise.
ConfigApp::ConfigApp()
   : vrj::GlApp(), mSurfaceWidth(40.0f), mSurfaceLength(40.0f),
     mStartButton(0), mStopButton(1), mFlashlightButton(2), mNavigating(false),
     mUserVelocity(0.0f)
{
   mSurfaceColor_ll[0] = 1.0f;
   mSurfaceColor_ll[1] = 1.0f;
   mSurfaceColor_ll[2] = 1.0f;

   mSurfaceColor_lr[0] = 0.7f;
   mSurfaceColor_lr[1] = 0.7f;
   mSurfaceColor_lr[2] = 0.7f;

   mSurfaceColor_ur[0] = 0.0f;
   mSurfaceColor_ur[1] = 0.0f;
   mSurfaceColor_ur[2] = 0.0f;

   mSurfaceColor_ul[0] = 0.7f;
   mSurfaceColor_ul[1] = 0.7f;
   mSurfaceColor_ul[2] = 0.7f;

   mDefaultLightPos[0] = 0.0f;
   mDefaultLightPos[1] = 10.0f;
   mDefaultLightPos[2] = 0.0f;
   mDefaultLightPos[3] = 1.0f;
}

// Determines if the given configuration element is one this application
// object can use.  This is deterined based on the type of the element.  We
// are only interested in those elements of type ConfigApp.
bool ConfigApp::configCanHandle(ConfigChunkPtr chunk)
{
   const std::string my_type("ConfigApp");

   return (my_type == chunk->getDescToken());
}

// Handles addition of a new configuration element that we can use.
bool ConfigApp::configAdd(ConfigChunkPtr chunk)
{
   const std::string ll_color("lowerLeftColor");
   const std::string lr_color("lowerRightColor");
   const std::string ur_color("upperRightColor");
   const std::string ul_color("upperLeftColor");

   // Get the color settings for the lower left corner (ll).
   for ( int i = 0; i < chunk->getNum(ll_color); ++i )
   {
      mSurfaceColor_ll[i] = chunk->getProperty<float>(ll_color, i);
   }

   // Get the color settings for the lower right corner (lr).
   for ( int i = 0; i < chunk->getNum(lr_color); ++i )
   {
      mSurfaceColor_lr[i] = chunk->getProperty<float>(lr_color, i);
   }

   // Get the color settings for the upper right corner (ur).
   for ( int i = 0; i < chunk->getNum(ur_color); ++i )
   {
      mSurfaceColor_ur[i] = chunk->getProperty<float>(ur_color, i);
   }

   // Get the color settings for the upper left corner (ul).
   for ( int i = 0; i < chunk->getNum(ul_color); ++i )
   {
      mSurfaceColor_ul[i] = chunk->getProperty<float>(ul_color, i);
   }

   return true;
}

// Initializes the device interfaces used by this application object.
void ConfigApp::init()
{
   mWand.init("VJWand");
   mButtons[0].init("VJButton0");
   mButtons[1].init("VJButton1");
   mButtons[2].init("VJButton2");
}

/// Updates navigation.
void ConfigApp::preFrame()
{
   updateNavigation();
}

// Clears the current viewport.
void ConfigApp::bufferPreDraw()
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);
}

// Renders the scene based on the current application state.  This takes
// into account configurable parameters such as the colors used for the
// "ground".
void ConfigApp::draw()
{
   glClear(GL_DEPTH_BUFFER_BIT);

   // Push a matrix to account for navigation transformation.
   glPushMatrix();

   // --- Setup for drawing --- //
   glMatrixMode(GL_MODELVIEW);

   // Initialize lights before navigation so that static lights do not move.
   initLights();

   // Transform the scene based on mNavMatrix (current navigation matrix).
   glLoadMatrixf(mNavMatrix.mData);

   // Draw the surface.
   glPushMatrix();
      const GLfloat mat_ambient[]   = { 0.7, 0.7, 0.7, 1.0 };
      const GLfloat mat_diffuse[]   = { 1.0, 0.5, 0.8, 1.0 };
      const GLfloat mat_specular[]  = { 1.0, 1.0, 1.0, 1.0 };
      const GLfloat mat_shininess[] = { 75.0 };
      const GLfloat no_mat[]        = { 0.0, 0.0, 0.0, 1.0 };

      glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_NORMALIZE);
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glEnable(GL_COLOR_MATERIAL);
      glShadeModel(GL_SMOOTH);

      // Define the corners that will be used for the surface.  These are
      // based on mSurfaceWidth (x coordinate) and mSurfaceLength (z
      // coordinate).  The surface is always centered at (0, 0, 0).
      const Vec3f ll(-(mSurfaceWidth / 2.0f), 0.0f, (mSurfaceLength / 2.0f));
      const Vec3f lr((mSurfaceWidth / 2.0f), 0.0f, (mSurfaceLength / 2.0f));
      const Vec3f ur((mSurfaceWidth / 2.0f), 0.0f, -(mSurfaceLength / 2.0f));
      const Vec3f ul(-(mSurfaceWidth / 2.0f), 0.0f, -(mSurfaceLength / 2.0f));

      const Vec3f up_normal(0.0f, 1.0f, 0.0f);

      glEnable(GL_BLEND);
      glBegin(GL_TRIANGLES);
         // Triangle #1 -------------------------------------------------------
         // Lower left corner.
         glColor3fv(mSurfaceColor_ll);
         glNormal3fv(up_normal.mData);
         glVertex3fv(ll.mData);

         // Lower right corner.
         glColor3fv(mSurfaceColor_lr);
         glNormal3fv(up_normal.mData);
         glVertex3fv(lr.mData);

         // Upper right corner.
         glColor3fv(mSurfaceColor_ur);
         glNormal3fv(up_normal.mData);
         glVertex3fv(ur.mData);
         // ------------------------------------------------------- Triangle #1

         // Triangle #2 -------------------------------------------------------
         // Upper right corner.
         glColor3fv(mSurfaceColor_ur);
         glNormal3fv(up_normal.mData);
         glVertex3fv(ur.mData);

         // Upper left corner.
         glColor3fv(mSurfaceColor_ul);
         glNormal3fv(up_normal.mData);
         glVertex3fv(ul.mData);

         // Lower left corner.
         glColor3fv(mSurfaceColor_ll);
         glNormal3fv(up_normal.mData);
         glVertex3fv(ll.mData);
         // ------------------------------------------------------- Triangle #2
      glEnd();
      glDisable(GL_BLEND);
   glPopMatrix();

   // Pop scene matrix used for navigation.
   glPopMatrix();
}

// Configures the light(s) used in the scene.  This includes the
// "flashlight" effect that may be enabled through the use of a wand button.
// The flashlight button state is tested to determine whether to use the
// flashlight or the overhead light.
void ConfigApp::initLights()
{
   const GLfloat light0_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
   const GLfloat light0_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   const GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

   glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

   // If the flashlight button is pressed, we turn the wand into a spotlight
   // source for GL_LIGHT0.
   if ( mButtons[mFlashlightButton]->getData() )
   {
      Matrix44f wand_mat;
      Vec3f wand_trans;
      Matrix44f inv_nav;
      invert(inv_nav, mNavMatrix);

      wand_mat = mWand->getData();
      setTrans(wand_trans, wand_mat);
      xform(wand_trans, inv_nav, wand_trans);

      // Get the wand position and use it to set the position of GL_LIGHT0.
      glLightfv(GL_LIGHT0, GL_POSITION, wand_trans.mData);

      // Get the wand forward vector and use it to set the direction of
      // GL_LIGHT0.
      Matrix44f wand_M_world = inv_nav * wand_mat;
      Vec3f wand_forw;
      xform(wand_forw, wand_M_world, Vec3f(0.0f, 0.0f, -1.0f));
      glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, wand_forw.mData);
      glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f);
   }
   // Otherwise, we have a light positioned at mDefaultLightPos that shines
   // downward.
   else
   {
      const GLfloat light0_direction[] = { 0.0f, -1.0f, 0.0f };
      glLightfv(GL_LIGHT0, GL_POSITION, mDefaultLightPos);
      glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
      glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
   }
}

void ConfigApp::updateNavigation()
{
   UserInfo    user_info;
   TrackedInfo wand_info;

   if ( mButtons[mStopButton]->getData() )
   {
      mNavigating = false;
   }
   else if ( mButtons[mStartButton]->getData() || mNavigating )
   {
      mUserVelocity += 0.005f;
      mNavigating = true;
   }

   user_info.setVelocity(mUserVelocity);
   user_info.setAngularVelocity(0.01f);
   wand_info.updateWithMatrix(mWand->getData());
   user_info.update(wand_info, Vec3f(0.0f, 0.0f, 0.0f));
   user_info.getSceneTransform(mNavMatrix);
}
