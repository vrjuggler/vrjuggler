/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#if defined(WIN32) || defined(WIN64)
#  include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>

#include <vrj/Draw/OpenGL/DrawManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/Display.h>

#include <WallTestHelpers.h>
#include <TestPatternMode.h>


void TestPatternMode::draw(WallTest*)
{
   vrj::opengl::UserData* user_data =
      vrj::opengl::DrawManager::instance()->currentUserData();
   vrj::ViewportPtr cur_vp    = user_data->getViewport();
   vrj::ProjectionPtr proj    = user_data->getProjection();

   // XXX: How does this happen?
   if ( NULL == cur_vp.get() )
   {
      return;
   }

   if ( cur_vp->isSimulator() )
   {
      vrj::DisplayManager* displayManager =
         vrj::opengl::DrawManager::instance()->getDisplayManager();
      std::vector<vrj::DisplayPtr> disps = displayManager->getAllDisplays();

      for ( unsigned int i = 0; i < disps.size(); ++i )
      {
         for ( unsigned int v = 0; v < disps[i]->getNumViewports(); ++v )
         {
            vrj::ViewportPtr viewport=disps[i]->getViewport(v);

            if ( viewport->isSurface() )
            {
               // Get a pointer to the surface
               vrj::SurfaceViewportPtr surface =
                  boost::dynamic_pointer_cast<vrj::SurfaceViewport>(viewport);
               vprASSERT(surface.get() != NULL);

               drawWallPattern(surface);
            }
         }
      }
   }
   else
   {
      vrj::SurfaceViewportPtr surface =
         boost::dynamic_pointer_cast<vrj::SurfaceViewport>(cur_vp);
      vprASSERT(surface.get() != NULL);

      drawWallPattern(surface);
   }
}

void TestPatternMode::drawWallPattern(vrj::SurfaceViewportPtr surf)
{
   gmtl::Matrix44f rotate;
   gmtl::Vec3f center;
   float wall_width, wall_height;

   calcSurfaceParameters(surf, rotate, center, wall_width, wall_height);

   glPushMatrix();
   {
      // Translate to center of wall and rotate to looking down wall
      glTranslatef(center[0], center[1], center[2]);
      glMultMatrixf(rotate.getData());

      drawGrid(wall_width, wall_height);
   }
   glPopMatrix();
}

void TestPatternMode::drawGrid(float wallWidth, float wallHeight)
{
   // Scale to size of wall and then to 10 units wide and tall
   glPushMatrix();
   {
      glScalef(wallWidth, wallHeight, 1.0f);
      glScalef(0.1f, 0.1f, 0.1f);

      glPushAttrib(GL_LIGHTING);
      glDisable(GL_LIGHTING);

      glLineWidth(2);

      glBegin(GL_LINES);

      float x;
      for(x=-5;x<=5;x+=0.5)
      {
         if(x==-2.5)      glColor3f(0,1,1);
         else if(x==-2)   glColor3f(1,1,0);
         else if(x==-1.5) glColor3f(1,0,1);
         else if(x==-1)   glColor3f(1,0,0);
         else if(x==-0.5) glColor3f(0,1,0);
         else if(x==0)    glColor3f(0,0,1);
         else if(x==0.5)  glColor3f(0,1,0);
         else if(x==1)    glColor3f(1,0,0);
         else if(x==1.5)  glColor3f(1,0,1);
         else if(x==2)    glColor3f(1,1,0);
         else if(x==2.5)  glColor3f(0,1,1);
         else             glColor3f(1,1,1);

         glVertex2f(x, -5.0f);
         glVertex2f(x, 5.0f);

         glVertex2f(-5.0f, x);
         glVertex2f(5.0f, x);
      }

      glEnd();

      glColor3f(1.0f, 1.0f, 0.0f);
      glBegin(GL_LINE_LOOP);
      for ( x = 0; x < gmtl::Math::TWO_PI; x += gmtl::Math::PI / 20.0 )
      {
         glVertex2f(sin(x)*5,cos(x)*5);
      }
      glEnd();

      glColor3f(0.0f, 1.0f, 1.0f);
      glLineWidth(5);
      glBegin(GL_LINES);
         glVertex2f(-1.0f, 0.0f);
         glVertex2f(1.0f, 0.0f);
         glVertex2f(0.0f, -1.0f);
         glVertex2f(0.0f, 1.0f);
      glEnd();
      glLineWidth(1);

      glPopAttrib();
   }
   glPopMatrix();
}
