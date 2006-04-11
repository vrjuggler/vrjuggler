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

#include <vrj/vrjConfig.h>

#include <vpr/vpr.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include <vrj/Display/Projection.h>
#include <vrj/Display/CameraProjection.h>
#include <vrj/Display/Frustum.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Viewport.h>
//#include <vrj/Display/SimViewport.h>
//#include <vrj/Display/SurfaceViewport.h>
#include <jccl/Config/ConfigChunk.h>
#include <vrj/Draw/OGL/GlWindow.h>

// This variable determines which matrix stack we put the viewing transformation
// If it is on the proj matrix, then lighting and env maps work but fog breaks.
#define USE_PROJECTION_MATRIX 1  /* Should we put the camera transforms on the
                                   Projection or modelview matrix */



namespace vrj
{

int GlWindow::mCurMaxWinId = 0;
vpr::Mutex GlWindow::mWinIdMutex;


void GlWindow::config(vrj::Display* displayWindow)
{
   vprASSERT(displayWindow != NULL);      // We can't config to a NULL display
   mDisplay = displayWindow;
   mDisplay->getOriginAndSize( origin_x, origin_y, window_width, window_height);
   border = mDisplay->shouldDrawBorder();

   /// Other stuff
}


void GlWindow::updateViewport()
{
   glViewport(0,0, window_width, window_height);
   setDirtyViewport(false);
}

void GlWindow::setViewport(float xo, float yo, float xSize, float ySize)
{
   vprASSERT( ((xo+xSize) <= 1.0f) && "X viewport sizes are out of range");
   vprASSERT( ((yo+ySize) <= 1.0f) && "Y viewport sizes are out of range");

   unsigned ll_x = unsigned(xo*float(window_width));
   unsigned ll_y = unsigned(yo*float(window_height));
   unsigned x_size = unsigned(xSize*float(window_width));
   unsigned y_size = unsigned(ySize*float(window_height));

   glViewport(ll_x, ll_y, x_size, y_size);
}


void GlWindow::setViewBuffer(vrj::Viewport::View view)
{
   if(!isStereo())
      glDrawBuffer(GL_BACK);
   else if(Viewport::LEFT_EYE == view)
      glDrawBuffer(GL_BACK_LEFT);
   else if(Viewport::RIGHT_EYE == view)
      glDrawBuffer(GL_BACK_RIGHT);
}

void GlWindow::setProjection(vrj::Projection* proj)
{
   if (!window_is_open)
      return;

   const float* frust = proj->getFrustum().frust;

   vprDEBUG(vrjDBG_DRAW_MGR,7)  << "---- Frustum ----\n"
               << proj->getFrustum().frust << std::endl
               << vprDEBUG_FLUSH;

   // --- Set up the projection --- //
   glMatrixMode(GL_PROJECTION);
   {
      glLoadIdentity();             // Load identity matrix
      glFrustum(frust[Frustum::VJ_LEFT],frust[Frustum::VJ_RIGHT],
                 frust[Frustum::VJ_BOTTOM],frust[Frustum::VJ_TOP],
                 frust[Frustum::VJ_NEAR],frust[Frustum::VJ_FAR]);
#ifdef USE_PROJECTION_MATRIX
         // Set camera rotation and position
      glMultMatrixf(proj->getViewMatrix().mData);
#endif
   }
   glMatrixMode(GL_MODELVIEW);
#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(proj->getViewMat().mData);
#endif
}


int GlWindow::getNextWindowId()
{
vpr::Guard<vpr::Mutex> guard(mWinIdMutex);      // Protect the id
   return mCurMaxWinId++;
}

std::ostream& operator<<(std::ostream& out, GlWindow* win)
{
   vprASSERT(win != NULL);
   vprASSERT(win->mDisplay != NULL);

   //out << "-------- GlWindow --------" << endl;
   out << "Open: " << (win->window_is_open ? "Yes" : "No") << std::endl;
   out << "Display Info:\n" << *(win->mDisplay) << std::endl;
   out << "Stereo: " << (win->in_stereo ? "Yes" : "No") << std::endl;
   return out;
}

} // end namespace
