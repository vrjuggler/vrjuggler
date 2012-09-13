/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/Draw/OpenGL/Config.h>

#include <vector>
#include <gmtl/Vec.h>
#include <gmtl/Output.h>
#include <gmtl/VecOps.h>

#include <vrj/Draw/OpenGL/DrawAxesFunctors.h>

#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

namespace vrj
{

namespace opengl
{

DrawAxesFunctor::DrawAxesFunctor()
{
   mOrigin.set(0.0f, 0.0f, 0.0f);
   mX_Axis.set(1.0f, 0.0f, 0.0f);
   mY_Axis.set(0.0f, 1.0f, 0.0f);
   mZ_Axis.set(0.0f, 0.0f, 1.0f);

   mScaleFactor = 1.0f;
}

void DrawAxesFunctor::contextInit()
{
  ;
}

void DrawAxesFunctor::draw(vrj::UserPtr)
{
   glPushAttrib(GL_LIGHTING_BIT);
   glDisable(GL_LIGHTING);
   glBegin(GL_LINES);
       glColor3fv(mX_Axis.mData);
       glVertex3fv(mOrigin.mData);
       glVertex3f(mScaleFactor * mX_Axis[0], mScaleFactor * mX_Axis[1],
          mScaleFactor * mX_Axis[2]);
       glColor3fv(mY_Axis.mData);
       glVertex3fv(mOrigin.mData);
       glVertex3f(mScaleFactor * mY_Axis[0], mScaleFactor * mY_Axis[1],
          mScaleFactor * mY_Axis[2]);
       glColor3fv(mZ_Axis.mData);
       glVertex3fv(mOrigin.mData);
       glVertex3f(mScaleFactor * mZ_Axis[0], mScaleFactor * mZ_Axis[1],
          mScaleFactor * mZ_Axis[2]);
   glEnd();
   glPopAttrib();
}

} // End of opengl namespace

} // End of vrj namespace
