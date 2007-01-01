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
 
#if defined(WIN32) || defined(WIN64)
#  include <windows.h>
#endif

#include <gadget/Type/Position/PositionUnitConversion.h>
#include "WallTestHelpers.h"


void drawCube()
{
   glBegin(GL_QUADS);
   
   glNormal3f(0,-1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(1,-1,-1);
   glVertex3f(1,-1,1);
   glVertex3f(-1,-1,1);

   glNormal3f(0,1,0);
   glVertex3f(-1,1,-1);
   glVertex3f(-1,1,1);
   glVertex3f(1,1,1);
   glVertex3f(1,1,-1);

   glNormal3f(-1,0,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,1);
   glVertex3f(-1,1,1);
   glVertex3f(-1,1,-1);
   
   glNormal3f(1,0,0);
   glVertex3f(1,-1,-1);
   glVertex3f(1,1,-1);
   glVertex3f(1,1,1);
   glVertex3f(1,-1,1);

   glNormal3f(0,0,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(1,-1,-1);
   glVertex3f(1,1,-1);
   glVertex3f(-1,1,-1);
   
   glNormal3f(0,0,1);
   glVertex3f(-1,-1,1);
   glVertex3f(1,-1,1);
   glVertex3f(1,1,1);
   glVertex3f(-1,1,1);
   
   glEnd();
}

void drawPyramid()
{
   glBegin(GL_TRIANGLES);
   glNormal3f(0,-1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(0,-1,1);
   glVertex3f(1,-1,-1);

   glNormal3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(0,1,0);
   glVertex3f(0,-1,1);

   glNormal3f(0,1,0);
   glVertex3f(0,-1,1);
   glVertex3f(0,1,0);
   glVertex3f(1,-1,-1);

   glNormal3f(0,1,0);
   glVertex3f(1,-1,-1);
   glVertex3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glEnd();

}

gmtl::Matrix44f calculateSurfaceRotation(gmtl::Point3f ll,gmtl::Point3f lr,gmtl::Point3f ul,gmtl::Point3f ur)
{
   // Find the base vectors for the surface axiis (in terms of the base coord system)
   // With z out, x to the right, and y up
   gmtl::Vec3f x_base(lr-ll), 
               y_base(ur-lr), 
               z_base;
   gmtl::cross( z_base, x_base, y_base);

   // They must be normalized
   gmtl::normalize(x_base); 
   gmtl::normalize(y_base); 
   gmtl::normalize(z_base);

   // Calculate the surfaceRotMat using law of cosines
   return gmtl::makeDirCos<gmtl::Matrix44f>(x_base,y_base,z_base);
}


void calcSurfaceParameters(vrj::SurfaceViewport* surf, gmtl::Matrix44f& surfRotation, gmtl::Vec3f& center, float& width, float& height)
{
   gmtl::Point3f ur,lr,ul,ll;
   surf->getCorners(ll,lr,ur,ul);

   // Convert to the units of the application (feet)
   float scale = gadget::PositionUnitConversion::ConvertToFeet;
   ll *= scale;
   lr *= scale;
   ur *= scale;
   ul *= scale;

   // Get the rotation matrix of a surface
   surfRotation = calculateSurfaceRotation(ll,lr,ul,ur);
   center = ll + lr + ul + ur;
   center /= 4.0;
   
   // Get width and height of wall
   width = gmtl::length(gmtl::Vec3f(lr-ll));
   height = gmtl::length(gmtl::Vec3f(ur-lr));   
}



