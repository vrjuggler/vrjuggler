/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Display.h>
#include <vrj/Draw/OpenGL/DrawManager.h>

#include <WallTestHelpers.h>
#include <CubeLineMode.h>


void CubeLineMode::draw(WallTest*)
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
            vprASSERT(surface != NULL);

            drawCubeLine(surface);
         }
      }
   }
}

void CubeLineMode::drawCubeLine(vrj::SurfaceViewportPtr surf)
{
   gmtl::Matrix44f rotate;
   gmtl::Vec3f center;
   float wall_width, wall_height;

   calcSurfaceParameters(surf, rotate, center, wall_width, wall_height);

   glPushMatrix();
   {
      // Translate to center of wall and rotate to looking down wall
      glTranslatef(center[0],center[1],center[2]);
      glMultMatrixf(rotate.getData());

      drawCubeLine();
   }
   glPopMatrix();
}

void CubeLineMode::drawCubeLine()
{
   glColor3f(0.0f, 0.0f, 1.0f);

   for ( float z = 5.0f; z >= -25.0f; z -= 2.5f )
   {
      glPushMatrix();
         glTranslatef(0.0f, 0.0f, z);
         glScalef(0.25f, 0.25f, 0.25f);
         drawCube();
      glPopMatrix();
   }
}
