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

#include <vrj/Draw/OpenGL/Config.h>

#include <gmtl/Vec.h>

#include <vrj/Kernel/User.h>
#include <vrj/Draw/OpenGL/DrawHeadFunctors.h>


namespace vrj
{

namespace opengl
{

DrawEllipsoidHeadFunctor::DrawEllipsoidHeadFunctor()
   : mQuadObj(NULL)
{
}

DrawEllipsoidHeadFunctor::~DrawEllipsoidHeadFunctor()
{
   if ( NULL != mQuadObj )
   {
      gluDeleteQuadric(mQuadObj);
   }
}

void DrawEllipsoidHeadFunctor::contextInit()
{
   if ( NULL == mQuadObj )
   {
      mQuadObj = gluNewQuadric();
   }
}

void DrawEllipsoidHeadFunctor::draw(vrj::UserPtr user)
{
   // All units are in meters.
   const float head_height(0.254f);      // 10 inches
   const float head_width_scale(0.7f);
   const float head_depth_scale(0.8f);
   //const float eye_vertical(0.067f);
   //const float eye_horizontal(0.21f);
   const float interocular(user->getInterocularDistance());
   const float eye_radius(0.0254f * 0.5f);      // 0.5 in

   glPushMatrix();
      // Head pos is the center of the eyes, so we need to move the head offset
      // a bit to get it positioned correctly.  We will do this by measuring
      // the distance from center of head to forehead and then translate by
      // the negative of that.
      gmtl::Vec3f forehead_offset(0.0f, head_height*0.17f,
                                  -(head_depth_scale*head_height)*0.45f);
      glTranslatef(-forehead_offset[0], -forehead_offset[1],
                   -forehead_offset[2]);

      // Scale to get an ellipsoid head.
      glScalef(head_width_scale, 1.0f, head_depth_scale);
      //glEnable(GL_BLEND);
      //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.5f, 0.75f, 0.90f, 0.67f);
      drawSphere(head_height/2.0f, 10, 10);             // Head
      //glDisable(GL_BLEND);
   glPopMatrix();

   // --- Draw the eyes --- //
   glPushMatrix();
      glColor3f(0.8f, 0.4f, 0.2f);
      //glTranslatef(0.0f, eye_vertical, -eye_horizontal);
      glPushMatrix();                     // Right eye
         glTranslatef((interocular/2.0f), 0.0f, 0.0f);
         drawSphere(eye_radius, 5, 5);
      glPopMatrix();
      glPushMatrix();                     // Left eye
         glTranslatef(-(interocular/2.0f), 0.0f, 0.0f);
         drawSphere(eye_radius, 5, 5);
      glPopMatrix();
   glPopMatrix();

   // --- Draw the axis --- //
   gmtl::Vec3f x_axis(0.15f, 0.0f, 0.0f);
   gmtl::Vec3f y_axis(0.0f, 0.15f, 0.0f);
   gmtl::Vec3f z_axis(0.0f, 0.0f, 0.15f);
   gmtl::Vec3f origin(0.0f, 0.0f, 0.0f);

   glPushAttrib(GL_LIGHTING_BIT);
   glDisable(GL_LIGHTING);
   glBegin(GL_LINES);
       glColor3f(1.0f, 0.0f, 0.0f);
       glVertex3fv(origin.mData);
       glVertex3fv(x_axis.mData);
       glColor3f(0.0f, 1.0f, 0.0f);
       glVertex3fv(origin.mData);
       glVertex3fv(y_axis.mData);
       glColor3f(0.0f, 0.0f, 1.0f);
       glVertex3fv(origin.mData);
       glVertex3fv(z_axis.mData);
   glEnd();
   glPopAttrib();
}

void DrawEllipsoidHeadFunctor::drawSphere(const float radius,
                                          const int slices, const int stacks)
{
  gluQuadricDrawStyle(mQuadObj, (GLenum) GLU_FILL);
  gluQuadricNormals(mQuadObj, (GLenum) GLU_SMOOTH);
  gluSphere(mQuadObj, radius, slices, stacks);
}

}

}
