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


#include <vjConfig.h>

#ifdef VJ_OS_Darwin
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/vjProjection.h>
#include <Kernel/vjCameraProjection.h>
#include <Kernel/vjFrustum.h>
#include <Utils/vjDebug.h>
#include <Kernel/vjViewport.h>
//#include <Kernel/vjSimViewport.h>
//#include <Kernel/vjSurfaceViewport.h>
#include <Config/vjConfigChunk.h>

// This variable determines which matrix stack we put the viewing transformation
// If it is on the proj matrix, then lighting and env maps work but fog breaks.
#define USE_PROJECTION_MATRIX 1  /* Should we put the camera transforms on the
                                   Projection or modelview matrix */


int vjGlWindow::mCurMaxWinId = 0;


void vjGlWindow::config(vjDisplay* displayWindow)
{
   vjASSERT(displayWindow != NULL);      // We can't config to a NULL display
   mDisplay = displayWindow;
   mDisplay->getOriginAndSize( origin_x, origin_y, window_width, window_height);
   border = mDisplay->shouldDrawBorder();

   /// Other stuff
}


void vjGlWindow::updateViewport()
{
   glViewport(0,0, window_width, window_height);
   setDirtyViewport(false);
}

void vjGlWindow::setViewport(float xo, float yo, float xSize, float ySize)
{
   vjASSERT( ((xo+xSize) <= 1.0f) && "X viewport sizes are out of range");
   vjASSERT( ((yo+ySize) <= 1.0f) && "Y viewport sizes are out of range");

   unsigned ll_x = unsigned(xo*float(window_width));
   unsigned ll_y = unsigned(yo*float(window_height));
   unsigned x_size = unsigned(xSize*float(window_width));
   unsigned y_size = unsigned(ySize*float(window_height));

   glViewport(ll_x, ll_y, x_size, y_size);
}


void vjGlWindow::setViewBuffer(vjViewport::View view)
{
   if(!isStereo())
      glDrawBuffer(GL_BACK);
   else if(vjViewport::LEFT_EYE == view)
      glDrawBuffer(GL_BACK_LEFT);
   else if(vjViewport::RIGHT_EYE == view)
      glDrawBuffer(GL_BACK_RIGHT);
}

void vjGlWindow::setProjection(vjProjection* proj)
{
   if (!window_is_open)
      return;

   float* frust = proj->mFrustum.frust;

   vjDEBUG(vjDBG_DRAW_MGR,7)  << "---- Frustum ----\n"
               << proj->mFrustum.frust << std::endl
               << vjDEBUG_FLUSH;

   // --- Set up the projection --- //
   glMatrixMode(GL_PROJECTION);
   {
      glLoadIdentity();             // Load identity matrix
      glFrustum(frust[vjFrustum::VJ_LEFT],frust[vjFrustum::VJ_RIGHT],
                 frust[vjFrustum::VJ_BOTTOM],frust[vjFrustum::VJ_TOP],
                 frust[vjFrustum::VJ_NEAR],frust[vjFrustum::VJ_FAR]);
#ifdef USE_PROJECTION_MATRIX
         // Set camera rotation and position
      glMultMatrixf(proj->mViewMat.getFloatPtr());
#endif
   }
   glMatrixMode(GL_MODELVIEW);
#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(proj->viewMat.getFloatPtr());
#endif
}



/** Sets the projection matrix for this window to draw the camera eye frame */
void vjGlWindow::setCameraProjection(vjCameraProjection* camProj)
{
   if (!window_is_open)
      return;

   vjASSERT(camProj != NULL && "Trying to use a non-camera projection with a sim view");
   float* frust = camProj->mFrustum.frust;

   vjDEBUG(vjDBG_DRAW_MGR,7)  << "---- Camera Frustrum ----\n"
               << camProj->mFrustum.frust << std::endl
               << vjDEBUG_FLUSH;

      // --- Set to the correct buffer --- //
   glDrawBuffer(GL_BACK);

      // --- Set up the projection --- //
   glMatrixMode(GL_PROJECTION);
   {
      glLoadIdentity();             // Load identity matrix
      /*
      glFrustum(frust[vjFrustum::LEFT],frust[vjFrustum::RIGHT],
                 frust[vjFrustum::BOTTOM],frust[vjFrustum::TOP],
                 frust[vjFrustum::NEAR],frust[vjFrustum::FAR]);
      */

      gluPerspective(camProj->mVertFOV, camProj->mAspectRatio*((window_width)/float(window_height)),
                     frust[vjFrustum::VJ_NEAR], frust[vjFrustum::VJ_FAR]);
#ifdef USE_PROJECTION_MATRIX
       // Set camera rotation and position
   glMultMatrixf(camProj->mViewMat.getFloatPtr());
#endif
   }
   glMatrixMode(GL_MODELVIEW);

#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(camProj->mViewMat.getFloatPtr());
#endif
}


std::ostream& operator<<(std::ostream& out, vjGlWindow* win)
{
   vjASSERT(win != NULL);
   vjASSERT(win->mDisplay != NULL);

   //out << "-------- vjGlWindow --------" << endl;
   out << "Open: " << (win->window_is_open ? "Y" : "N") << std::endl;
   out << "Display:" << *(win->mDisplay) << std::endl;
   out << "Stereo:" << (win->in_stereo ? "Y" : "N") << std::endl;
   return out;
}
